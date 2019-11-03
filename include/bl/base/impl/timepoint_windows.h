#ifndef __TIMEPOINT_WINDOWS_H__
#define __TIMEPOINT_WINDOWS_H__

/* this file is made to be included from <bl/base/time.h>, it is just
   to avoid clutter on that file, hence the lack of some includes to make it
   fail in case of the user triying to include it directly */

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/include_windows.h>
/*---------------------------------------------------------------------------*/
static inline LARGE_INTEGER bl_qpc_get_freq_priv(void)
{
  LARGE_INTEGER freq;
  bl_assert_always (QueryPerformanceFrequency (&freq) != 0);
  return freq;
}
/*---------------------------------------------------------------------------*/
/*this function is used outside this header but it's not exposed*/
static inline bl_u64 bl_qpc_get_freq (void)
{
  static LARGE_INTEGER freq = bl_qpc_get_freq_priv();
  return freq.QuadPart;
}
/*---------------------------------------------------------------------------*/
/* 64 bit */
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_get_freq (void)
{
  return bl_qpc_get_freq();
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_get (void)
{
  LARGE_INTEGER t;
  bl_assert_always (QueryPerformanceCounter (&t) != 0);
  return (bl_timept64) t.QuadPart;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timepoint64_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_sysclock_get (void)
{
  FILETIME t;
  bl_u64 f = bl_qpc_get_freq();
  GetSystemTimeAsFileTime (&t);
  bl_u64 res = (bl_u64) t.dwLowDateTime | (((bl_u64) t.dwHighDateTime) << 32);
  res *= 100; /*to nsec*/
  return (bl_timept64) res;
}
/*---------------------------------------------------------------------------*/
#define bl_timept64_sysclock_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_sysclock_to_epoch (bl_timept t)
{
  return t - (116444736000000000LL * 100);
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timepoint64_sysclock_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
/* 32 bit */
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_get_freq (void)
{
  return (bl_timept32) bl_timept64_get_freq();
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_get (void)
{
  return (bl_timept32) bl_timept32_get();
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timepoint32_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_sysclock_get (void)
{
  FILETIME t;
  bl_u64 f = bl_qpc_get_freq();
  GetSystemTimeAsFileTime (&t);
  bl_u64 res = (bl_u64) t.dwLowDateTime | (((bl_u64) t.dwHighDateTime) << 32);
  res /= bl_usec_in_sec * 10; /*to sec*/
  return (bl_timept32) res;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_sysclock_to_epoch (bl_timept32 t)
{
  return (bl_timept32)(t - (116444736000000000LL / (bl_usec_in_sec * 10)));
}
/*---------------------------------------------------------------------------*/
#define bl_timept32_sysclock_get_freq() 1
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timepoint32_sysclock_funcs_second_base.h>
/*----------------------------------------------------------------------------*/
/* timepoint bases. */
/*----------------------------------------------------------------------------*/
#include <bl/base/impl/timepoint_funcs_arbitrary_base.h>
#if BL_TIMEPOINT_BITS == 32
  #include <bl/base/impl/generated/timepoint/timepoint_sysclock_funcs_second_base.h>
#else
  #include <bl/base/impl/generated/timepoint/timepoint_funcs_nanosecond_base.h>
#endif /* #if BL_TIMEPOINT_BITS == 32 */
/*----------------------------------------------------------------------------*/
#endif /* __TIMEPOINT_WINDOWS_H__ */
