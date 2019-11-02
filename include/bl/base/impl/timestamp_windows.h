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
static inline bl_u64 bl_tstamp_get_freq (void)
{
  return bl_qpc_get_freq();
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_get_tstamp (void)
{
  LARGE_INTEGER t;
  bl_assert_always (QueryPerformanceCounter (&t) != 0);
  return (bl_tstamp) t.QuadPart;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_tstamp_sysclock_get (void)
{
  FILETIME t;
  bl_u64 f = bl_qpc_get_freq();
  GetSystemTimeAsFileTime (&t);
  bl_u64 res = (bl_u64) t.dwLowDateTime | (((bl_u64) t.dwHighDateTime) << 32);
#if defined (BL_TIMESTAMP_64BIT)
  res *= 100; /*to nsec*/
#else
  res /= bl_usec_in_sec * 10; /*to sec*/
#endif
  return (bl_tstamp) res;
}
/*---------------------------------------------------------------------------*/
#if defined (BL_TIMESTAMP_64BIT)

#define bl_tstamp_sysclock_get_freq() bl_nsec_in_sec

static inline bl_tstamp bl_tstamp_sysclock_to_epoch (bl_tstamp t)
{
  return t - (116444736000000000LL * 100);
}

#include <bl/base/impl/timestamp_sysclock_funcs_nanosecond_base.h>

#else

static inline bl_tstamp bl_tstamp_sysclock_to_epoch (bl_tstamp t)
{
  return t - (116444736000000000LL / (bl_usec_in_sec * 10));
}

#define bl_tstamp_sysclock_get_freq() 1

#include <bl/base/impl/timestamp_sysclock_funcs_second_base.h>

#endif
/*---------------------------------------------------------------------------*/

#endif /* __TIMESTAMP_WINDOWS_H__ */

