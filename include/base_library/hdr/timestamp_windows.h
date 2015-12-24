#ifndef __TIMESTAMP_WINDOWS_H__
#define __TIMESTAMP_WINDOWS_H__

/* this file is made to be included from <base_library/hdr/time.h>, it is just
   to avoid clutter on that file, hence the lack of some includes to make it
   fail in case of the user triying to include it directly */

#include <assert.h>
#include <base_library/hdr/platform.h>
#include <base_library/hdr/integer.h>
#include <base_library/hdr/utility.h>
#include <base_library/hdr/static_integer_math.h>
#include <base_library/hdr/include_windows.h>
/*---------------------------------------------------------------------------*/
static inline u64 qpc_get_freq_priv (void)
{
  LARGE_INTEGER freq;
  side_effect_assert (QueryPerformanceFrequency (&freq) != 0);
  return freq;
}
/*---------------------------------------------------------------------------*/
static u64 qpc_get_freq (void)
{
  static LARGE_INTEGER freq = qpc_get_freq_priv();
  return freq.QuadPart;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_tstamp (void)
{
  LARGE_INTEGER t;
  side_effect_assert (QueryPerformanceCounter (&t) != 0);
  return (tstamp) t.QuadPart;
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_sec (tstamp ts)
{
  return (uword) (((u64) ts) / qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_msec (tstamp ts)
{
  return (uword) (((u64) ts * msec_in_sec) / qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_usec (tstamp ts)
{
  return (uword) (((u64) ts * usec_in_sec) / qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_nsec (tstamp ts)
{
  return (uword) (((u64) ts * nsec_in_sec) / qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_sec_ceil (tstamp ts)
{
  return (uword) div_ceil (((u64) ts), qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_msec_ceil (tstamp ts)
{
  return (uword) div_ceil (((u64) ts * msec_in_sec), qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_usec_ceil (tstamp ts)
{
  return (uword) div_ceil (((u64) ts * usec_in_sec), qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_nsec_ceil (tstamp ts)
{
  return (uword) div_ceil (((u64) ts * nsec_in_sec), qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_sec (tstamp ts, i32 sec)
{
  return ts + (sec * qpc_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_msec (tstamp ts, i32 msec)
{
  assert (qpc_get_freq() <= utype_max (u32));
  return ts + div_ceil ((i64) msec * qpc_get_freq(), msec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_usec (tstamp ts, i32 usec)
{
  assert (qpc_get_freq() <= utype_max (u32));
  return ts + div_ceil ((i64) usec * qpc_get_freq(), usec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_nsec (tstamp ts, i32 nsec)
{
  assert (qpc_get_freq() <= utype_max (u32));
  return ts + div_ceil ((i64) nsec * qpc_get_freq(), nsec_in_sec);
}
/*---------------------------------------------------------------------------*/

#endif /* __TIMESTAMP_POSIX_H__ */

