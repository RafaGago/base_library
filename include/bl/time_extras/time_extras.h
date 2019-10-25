#ifndef __BL_TIME_EXTRAS_H__
#define __BL_TIME_EXTRAS_H__

#include <bl/time_extras/libexport.h>

#include <bl/base/error.h>
#include <bl/base/time.h>
#include <bl/base/platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
  A library for more uncommon time-related functions.
  This library is stateful but thread safe.
  ---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Initializes this library. This library is stateful. "bl_time_extras_init"
   has to be called at least once to use any of the functions included on this
   header. Only the first call will do some work.

   This function tries to empirically acquire information about the clocks by
   doing measurements. It can rarely fail if the scheduler repeatedly preeempts
   some critical points inside.
*/
extern BL_TIME_EXTRAS_EXPORT bl_err bl_time_extras_init (void);
/*----------------------------------------------------------------------------*/
/* Destructs this library. If multiple calls were made to" bl_extras_init", as
many calls need to be done to "bl_time_extras_destroy" and the destruction will
be done by the last caller.
*/
extern BL_TIME_EXTRAS_EXPORT void bl_time_extras_destroy (void);
/*----------------------------------------------------------------------------*/
/* Returns the difference in nanoseconds between the "bl_tstamp" monotonic
   clock vs the calendar clock "bl_sysclock". This is useful to convert
   monotonic to calendar time. */
extern BL_TIME_EXTRAS_EXPORT toffset bl_tstamp_to_sysclock_diff_ns (void);
/*----------------------------------------------------------------------------*/
#if defined (BL_INTEL_AMD_PC)
/*----------------------------------------------------------------------------*/
#if BL_COMPILER == BL_MSC
  #include <intrin.h>
#else
  #include <x86intrin.h>
#endif

#include <bl/base/atomic.h>

#define BL_HAS_CPU_TSTAMP 1
/*----------------------------------------------------------------------------*/
static inline tstamp bl_get_cputstamp (void)
{
  _mm_mfence();
  return __rdtsc();
}
/* the fast version is suitable for timestamping only, not for measurements, as
it allows instruction reordering by the compiler */
/*----------------------------------------------------------------------------*/
static inline tstamp bl_get_cputstamp_fast (void)
{
  return __rdtsc();
}
/*----------------------------------------------------------------------------*/
extern BL_TIME_EXTRAS_EXPORT toffset bl_cputstamp_to_nsec (tstamp t);
/*----------------------------------------------------------------------------*/
extern BL_TIME_EXTRAS_EXPORT u64 bl_cputstamp_get_freq (void);
/*----------------------------------------------------------------------------*/
/* Returns the difference in nanoseconds between the "bl_cputstamp" monotonic
   clock vs the calendar clock "bl_sysclock". This is useful to convert
   monotonic to calendar time.*/
extern BL_TIME_EXTRAS_EXPORT toffset bl_cputstamp_to_sysclock_diff_ns (void);
/*----------------------------------------------------------------------------*/
#else /*  defined (BL_INTEL_AMD_PC) */
#define BL_HAS_CPU_TSTAMP 0
#endif /* else  defined (BL_INTEL_AMD_PC) */
/*----------------------------------------------------------------------------*/
/*ftstamp represents the fastest monotonic clock available on the platform  */

#if BL_HAS_CPU_TSTAMP == 1

#define bl_get_ftstamp()                 bl_get_cputstamp()
#define bl_get_ftstamp_fast()            bl_get_cputstamp_fast()
#define bl_fstamp_to_nsec(t)             bl_cputstamp_to_nsec (t)
#define bl_ftstamp_get_freq()            bl_cputstamp_get_freq()
#define bl_ftstamp_to_sysclock_diff_ns() bl_cputstamp_to_sysclock_diff_ns()

#else /* BL_HAS_CPU_TSTAMP == 1 */

#define bl_get_ftstamp()                 bl_get_tstamp()
#define bl_get_ftstamp_fast()            bl_get_tstamp()
#define bl_fstamp_to_nsec(t)             bl_stamp_to_nsec (t)
#define bl_ftstamp_get_freq()            bl_stamp_get_freq()
#define bl_ftstamp_to_sysclock_diff_ns() bl_stamp_to_sysclock_diff_ns()

#endif /* else BL_HAS_CPU_TSTAMP == 1 */

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /*__BL_TIME_EXTRAS_LIBEXPORT_H__*/
