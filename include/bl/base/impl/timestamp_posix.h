#ifndef __BL_TIMESTAMP_POSIX_H__
#define __BL_TIMESTAMP_POSIX_H__

/* this file is made to be included from <bl/base/time.h>, it is just
   to avoid clutter on that file, hence the lack of some includes to make it
   fail in case of the user triying to include it directly */

#include <time.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
/*---------------------------------------------------------------------------*/
#ifdef BL_USE_CLOCK_MONOTONIC_RAW
  #define BL_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW
#else
  #define BL_CLOCK_MONOTONIC CLOCK_MONOTONIC /*TODO this isn't posix now...*/
#endif
/*---------------------------------------------------------------------------*/
/* FIXME: this is made inline just to remove warnings*/
/*---------------------------------------------------------------------------*/
#if !defined (BL_TIMESTAMP_64BIT)
#define bl_tstamp_get_freq() usec_in_sec
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (tstamp)
   ((((u64) t.tv_sec) * usec_in_sec) + (t.tv_nsec / nsec_in_usec));
}
/*---------------------------------------------------------------------------*/
#define bl_sysclock_tstamp_get_freq() 1
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_sysclock_tstamp (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (tstamp) t.tv_sec;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp_funcs_microsecond_base.h>
#include <bl/base/impl/timestamp_sysclock_funcs_second_base.h>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
#define bl_tstamp_get_freq() nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (((u64) t.tv_sec) * nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
#define bl_sysclock_tstamp_get_freq() nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline tstamp bl_get_sysclock_tstamp (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (((u64) t.tv_sec) * nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp_funcs_nanosecond_base.h>
#include <bl/base/impl/timestamp_sysclock_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
#endif /* #if !defined (BL_TIMESTAMP_64BIT) */
/*---------------------------------------------------------------------------*/
static inline tstamp bl_sysclock_tstamp_to_epoch (tstamp t)
{
  return t;
}
/*---------------------------------------------------------------------------*/
/* private. for internal use */
/*---------------------------------------------------------------------------*/
static void timespec_normalize (struct timespec* t)
{
  t->tv_sec += (t->tv_nsec / nsec_in_sec);
  t->tv_nsec = (t->tv_nsec % nsec_in_sec);
}
/* private. for internal use */
/*---------------------------------------------------------------------------*/
static inline struct timespec timespec_us_from_now (u32 usec, int clock)
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
