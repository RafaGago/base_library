#ifndef __TIMESTAMP_WINDOWS_H__
#define __TIMESTAMP_WINDOWS_H__

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
static inline bl_tstamp64 bl_tstamp64_get_freq (void)
{
  return bl_qpc_get_freq();
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_get (void)
{
  LARGE_INTEGER t;
  bl_assert_always (QueryPerformanceCounter (&t) != 0);
  return (bl_tstamp64) t.QuadPart;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp64_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_sysclock_get (void)
{
  FILETIME t;
  bl_u64 f = bl_qpc_get_freq();
  GetSystemTimeAsFileTime (&t);
  bl_u64 res = (bl_u64) t.dwLowDateTime | (((bl_u64) t.dwHighDateTime) << 32);
  res *= 100; /*to nsec*/
  return (bl_tstamp64) res;
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp64_sysclock_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_sysclock_to_epoch (bl_tstamp t)
{
  return t - (116444736000000000LL * 100);
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp64_sysclock_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
/* 32 bit */
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_get_freq (void)
{
  return (bl_tstamp32) bl_tstamp64_get_freq();
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_get (void)
{
  return (bl_tstamp32) bl_tstamp32_get();
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp32_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_sysclock_get (void)
{
  FILETIME t;
  bl_u64 f = bl_qpc_get_freq();
  GetSystemTimeAsFileTime (&t);
  bl_u64 res = (bl_u64) t.dwLowDateTime | (((bl_u64) t.dwHighDateTime) << 32);
  res /= bl_usec_in_sec * 10; /*to sec*/
  return (bl_tstamp32) res;
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_sysclock_to_epoch (bl_tstamp32 t)
{
  return (bl_tstamp32)(t - (116444736000000000LL / (bl_usec_in_sec * 10)));
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp32_sysclock_get_freq() 1
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp32_sysclock_funcs_second_base.h>
/*----------------------------------------------------------------------------*/
/* timestamp bases. */
/*----------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp_funcs_arbitrary_base.h>
#if !defined BL_TIMESTAMP_64BIT
  #include <bl/base/impl/generated/timestamp/timestamp_sysclock_funcs_second_base.h>
#else
  #include <bl/base/impl/generated/timestamp/timestamp_funcs_nanosecond_base.h>
#endif /* #if !defined (BL_TIMESTAMP_64BIT) */
/*----------------------------------------------------------------------------*/
#endif /* __TIMESTAMP_WINDOWS_H__ */
