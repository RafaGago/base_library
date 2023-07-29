#ifndef __BL_TIME_EXTRAS_H__
#define __BL_TIME_EXTRAS_H__

#include <bl/time_extras/libexport.h>

#include <bl/base/error.h>
#include <bl/base/time.h>
#include <bl/base/platform.h>
/*----------------------------------------------------------------------------*/
#define BL_CPU_TIMEPT_UNAVAILABLE 0
#define BL_CPU_TIMEPT_RDTSC       1
/*----------------------------------------------------------------------------*/
#ifndef BL_NO_CPU_TIMEPT
  #if BL_ARCH_IS_X64_X86_IA64
    #define BL_CPU_TIMEPT BL_CPU_TIMEPT_RDTSC
  #else
    #define BL_CPU_TIMEPT BL_CPU_TIMEPT_UNAVAILABLE
  #endif
#else
  #define BL_CPU_TIMEPT BL_CPU_TIMEPT_UNAVAILABLE
#endif
/*----------------------------------------------------------------------------*/
#define BL_CPU_TIMEPT_IS(impl) (BL_CPU_TIMEPT == BL_CPU_TIMEPT_##impl)
#define BL_HAS_CPU_TIMEPT (!BL_CPU_TIMEPT_IS (UNAVAILABLE))
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
  A library for more uncommon time-related functions.
  This library is stateful but thread safe.
  ---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Initializes this library. If multiple calls were made to" bl_extras_init", as
many calls need to be done to "bl_time_extras_destroy" and the destruction will
be done by the last caller. This function can fail, as it tries to get some data
about the clocks
*/
extern BL_TIME_EXTRAS_EXPORT bl_err bl_time_extras_init (void);
/*----------------------------------------------------------------------------*/
/* Destructs this library. If multiple calls were made to" bl_extras_init", as
many calls need to be done to "bl_time_extras_destroy" and the destruction will
be done by the last caller.
*/
extern BL_TIME_EXTRAS_EXPORT void bl_time_extras_destroy (void);
/*----------------------------------------------------------------------------*/
/* Returns the difference in nanoseconds between the "bl_timept" monotonic
   clock vs the calendar clock "bl_sysclock". This is useful to convert
   monotonic to calendar time. */
extern BL_TIME_EXTRAS_EXPORT
  bl_timeoft64 bl_timept64_to_sysclock64_diff_ns (void);
/*----------------------------------------------------------------------------*/
#if BL_CPU_TIMEPT_IS (RDTSC)
#if BL_COMPILER_IS (MICROSOFT_VC)
  #include <intrin.h>
#else
  #include <x86intrin.h>
#endif
/*----------------------------------------------------------------------------*/
static inline bl_timept64 bl_cpu_timept_get (void)
{
  _mm_mfence();
  return __rdtsc();
}
/* the fast version is suitable for timestamping only, not for measurements, as
it allows instruction reordering by the compiler */
/*----------------------------------------------------------------------------*/
static inline bl_timept64 bl_cpu_timept_get_fast (void)
{
  return __rdtsc();
}
/*----------------------------------------------------------------------------*/
#endif /* #if BL_CPU_TIMEPT_IS (RDTSC) */
/*----------------------------------------------------------------------------*/
#if BL_HAS_CPU_TIMEPT
extern BL_TIME_EXTRAS_EXPORT bl_u64 bl_cpu_timept_get_freq (void);
/*----------------------------------------------------------------------------*/
/* Returns the difference in nanoseconds between the "bl_cpu_timept" monotonic
   clock vs the calendar clock "bl_sysclock". This is useful to convert
   monotonic to calendar time. This is done by measuring and interpreting the
   data in a not-completely-naive way.
   */
extern BL_TIME_EXTRAS_EXPORT bl_timeoft64
  bl_cpu_timept_to_sysclock64_diff_ns (void);
/*----------------------------------------------------------------------------*/
/*fast_timept represents the fastest monotonic clock available on the platform*/

#define bl_fast_timept_get()                   bl_cpu_timept_get()
#define bl_fast_timept_get_fast()              bl_cpu_timept_get_fast()
#define bl_fast_timept_get_freq()              bl_cpu_timept_get_freq()
#define bl_fast_timept_to_sysclock64_diff_ns() \
  bl_cpu_timept_to_sysclock64_diff_ns()

#include <bl/time_extras/generated/cpu_timept_funcs_arbitrary_base.h>
#include <bl/time_extras/generated/fast_timept_funcs_from_cpu_timept.h>

#else /* BL_HAS_CPU_TIMEPT */

#define bl_fast_timept_get()                 bl_timept64_get()
#define bl_fast_timept_get_fast()            bl_timept64_get()
#define bl_fast_timept_get_freq()            bl_timept64_get_freq()
#define bl_fast_timept_to_sysclock_diff_ns() bl_timept64_to_sysclock64_diff_ns()

#include <bl/time_extras/generated/fast_timept_funcs_from_timept64.h>

#endif /* else BL_HAS_CPU_TIMEPT */

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /*__BL_TIME_EXTRAS_LIBEXPORT_H__*/
