#ifndef __BL_TIMESTAMP_POSIX_H__
#define __BL_TIMESTAMP_POSIX_H__

/* this file is made to be included from <bl/hdr/base/time.h>, it is just
   to avoid clutter on that file, hence the lack of some includes to make it
   fail in case of the user triying to include it directly */

#include <time.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/static_integer_math.h>
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
static inline tstamp bl_tstamp_offset_sec (toffset sec)
{
  return ((tstamp) sec) * nsec_in_sec;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_offset_msec (toffset msec)
{
  return ((tstamp) msec) * nsec_in_msec;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_offset_usec (toffset usec)
{
  return ((tstamp) usec) * nsec_in_usec;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_offset_nsec (toffset nsec)
{
  return (tstamp) nsec;
}
/*---------------------------------------------------------------------------*/
#else /*#ifdef BL64BIT_TIMESTAMP*/
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
static inline tstamp bl_tstamp_offset_sec (toffset sec)
{
  return ((tstamp) sec) * usec_in_sec;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_offset_msec (toffset msec)
{
  return ((tstamp) msec) * usec_in_msec;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_offset_usec (toffset usec)
{
  return ((tstamp) usec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_offset_nsec (toffset nsec)
{
  return div_ceil (((tstamp) nsec), nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
#endif /*#ifdef BL64BIT_TIMESTAMP, #else*/

#endif /* __BL_TIMESTAMP_POSIX_H__ */

