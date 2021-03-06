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
  static LARGE_INTEGER freq = {0, 0};
  if (freq.QuadPart != 0) {
    return freq.QuadPart;
  }
  freq = bl_qpc_get_freq_priv();
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
#include <bl/base/impl/generated/time/timept64_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_sysclock64_get (void)
{
  FILETIME t;
  GetSystemTimeAsFileTime (&t);
  bl_u64 res = ((bl_u64) t.dwLowDateTime) | (((bl_u64) t.dwHighDateTime) << 32);
  res *= 100; /*to nsec*/
  return (bl_timept64) res;
}
/*---------------------------------------------------------------------------*/
#define bl_sysclock64_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_sysclock64_to_epoch (bl_timept64 t)
{
  return t - 11644473600000000000LL;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/generated/time/sysclock64_funcs_nanosecond_base.h>
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
  return (bl_timept32) bl_timept64_get();
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/generated/time/timept32_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_sysclock32_get (void)
{
  FILETIME t;
  GetSystemTimeAsFileTime (&t);
  bl_u64 res = ((bl_u64) t.dwLowDateTime) | (((bl_u64) t.dwHighDateTime) << 32);
  res /= bl_usec_in_sec * 10; /*to sec*/
  return (bl_timept32) res;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_sysclock32_to_epoch (bl_timept32 t)
{
  return (bl_timept32)(t - (116444736000000000LL / (bl_usec_in_sec * 10)));
}
/*---------------------------------------------------------------------------*/
#define bl_sysclock32_get_freq() 1
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/generated/time/sysclock32_funcs_second_base.h>
/*----------------------------------------------------------------------------*/
#endif /* __TIMEPOINT_WINDOWS_H__ */
