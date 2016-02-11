#ifndef __BL_TIMESTAMP_POSIX_H__
#define __BL_TIMESTAMP_POSIX_H__

/* this file is made to be included from <bl/hdr/base/time.h>, it is just
   to avoid clutter on that file, hence the lack of some includes to make it
   fail in case of the user triying to include it directly */

#include <time.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
/*---------------------------------------------------------------------------*/
#ifdef BL_USE_CLOCK_MONOTONIC_RAW
  #define BL_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW
#else
  #define BL_CLOCK_MONOTONIC CLOCK_MONOTONIC /*TODO this isn't posix now...*/
#endif
/*---------------------------------------------------------------------------*/
#if !defined (BL_TIMESTAMP_64BIT)
static inline tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (tstamp)
   ((((u64) t.tv_sec) * usec_in_sec) + (t.tv_nsec / nsec_in_usec));
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp_get_freq() usec_in_sec
/*---------------------------------------------------------------------------*/
#include <bl/hdr/base/impl/timestamp_funcs_microsecond_base.h>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (((u64) t.tv_sec) * nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp_get_freq() nsec_in_sec
/*---------------------------------------------------------------------------*/
#include <bl/hdr/base/impl/timestamp_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
#endif /* #if !defined (BL_TIMESTAMP_64BIT) */

/* private. for internal use */
/*---------------------------------------------------------------------------*/
static void timespec_normalize (struct timespec* t)
{
  /*on most cpus this is a div, an addition and a substraction*/
  t->tv_sec += (t->tv_nsec / nsec_in_sec);
  t->tv_nsec = (t->tv_nsec % nsec_in_sec);
}
/* private. for internal use */
/*---------------------------------------------------------------------------*/
static struct timespec timespec_us_from_now (u32 usec, int clock)
{
  struct timespec t;
  clock_gettime (clock, &t);
  t.tv_sec  += usec / usec_in_sec;
  t.tv_nsec += (usec % usec_in_sec) * nsec_in_usec;
  timespec_normalize (&t);
  return t;
}
/*----------------------------------------------------------------------------*/

#endif /* __BL_TIMESTAMP_POSIX_H__ */
