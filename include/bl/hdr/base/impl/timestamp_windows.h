#ifndef __TIMESTAMP_WINDOWS_H__
#define __TIMESTAMP_WINDOWS_H__

/* this file is made to be included from <bl/hdr/base/time.h>, it is just
   to avoid clutter on that file, hence the lack of some includes to make it
   fail in case of the user triying to include it directly */

#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/static_integer_math.h>
#include <bl/hdr/base/include_windows.h>

/*---------------------------------------------------------------------------*/
static inline LARGE_INTEGER qpc_get_freq_priv(void)
{
  LARGE_INTEGER freq;
  bl_assert_always (QueryPerformanceFrequency (&freq) != 0);
  return freq;
}
/*---------------------------------------------------------------------------*/
/*this function is used outside this header but it's not exposed*/
static inline u64 qpc_get_freq (void) 
{
  static LARGE_INTEGER freq = qpc_get_freq_priv();
  return freq.QuadPart;
}
/*---------------------------------------------------------------------------*/
static inline u64 bl_tstamp_get_freq (void) 
{
  return qpc_get_freq();
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_tstamp (void)
{
  LARGE_INTEGER t;
  bl_assert_always (QueryPerformanceCounter (&t) != 0);
  return (tstamp) t.QuadPart;
}
/*---------------------------------------------------------------------------*/
#include <bl/hdr/base/impl/timestamp_funcs_arbitrary_base.h>
/*---------------------------------------------------------------------------*/

#endif /* __TIMESTAMP_WINDOWS_H__ */

