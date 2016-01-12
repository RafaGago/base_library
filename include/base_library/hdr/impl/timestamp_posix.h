#ifndef __TIMESTAMP_POSIX_H__
#define __TIMESTAMP_POSIX_H__

/* this file is made to be included from <base_library/hdr/time.h>, it is just
   to avoid clutter on that file, hence the lack of some includes to make it
   fail in case of the user triying to include it directly */

#include <time.h>
#include <base_library/hdr/platform.h>
#include <base_library/hdr/integer.h>
#include <base_library/hdr/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#ifdef BL_USE_CLOCK_MONOTONIC_RAW
  #define BL_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW
#else
  #define BL_CLOCK_MONOTONIC CLOCK_MONOTONIC /*TODO this isn't posix now...*/
#endif
/*---------------------------------------------------------------------------*/
#ifdef BL64BIT_TIMESTAMP
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (((u64) t.tv_sec) * nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_sec (tstamp ts)
{
  return (uword) (ts / nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_msec (tstamp ts)
{
  return (uword) (ts / nsec_in_msec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_usec (tstamp ts)
{
  return (uword) (ts / nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_nsec (tstamp ts)
{
  return (uword) (ts);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_sec_ceil (tstamp ts)
{
  return (uword) div_ceil (ts, nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_msec_ceil (tstamp ts)
{
  return (uword) div_ceil (ts, nsec_in_msec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_usec_ceil (tstamp ts)
{
  return (uword) div_ceil (ts, nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_nsec_ceil (tstamp ts)
{
  return (uword) (ts);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_sec (tstamp ts, i32 sec)
{
  return ts + (sec * nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_msec (tstamp ts, i32 msec)
{
  return ts + (msec * nsec_in_msec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_usec (tstamp ts, i32 usec)
{
  return ts + (usec * nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_nsec (tstamp ts, i32 nsec)
{
  return ts + nsec;
}
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (tstamp)
   ((((u64) t.tv_sec) * usec_in_sec) + (t.tv_nsec / nsec_in_usec));
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_sec (tstamp ts)
{
  return (uword) (ts / usec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_msec (tstamp ts)
{
  return (uword) (ts / usec_in_msec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_usec (tstamp ts)
{
  return (uword) ts;
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_nsec (tstamp ts)
{
  return  ((uword) ts) * nsec_in_usec;
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_sec_ceil (tstamp ts)
{
  return (uword) div_ceil (ts, usec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_msec_ceil (tstamp ts)
{
  return (uword) div_ceil (ts, usec_in_msec);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_usec_ceil (tstamp ts)
{
  return ((uword) ts);
}
/*---------------------------------------------------------------------------*/
static inline uword bl_tstamp_nsec_ceil (tstamp ts)
{
  return (uword) (ts * nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_sec (tstamp ts, i32 sec)
{
  return ts + (sec * usec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_msec (tstamp ts, i32 msec)
{
  return ts + (msec * usec_in_msec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_usec (tstamp ts, i32 usec)
{
  return ts + usec;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_add_nsec (tstamp ts, i32 nsec)
{
  return ts + div_ceil (nsec, nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
#endif

#endif /* __TIMESTAMP_POSIX_H__ */

