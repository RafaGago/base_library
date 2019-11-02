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
#define bl_tstamp_get_freq() bl_usec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (bl_tstamp)
   ((((bl_u64) t.tv_sec) * bl_usec_in_sec) + (t.tv_nsec / bl_nsec_in_usec));
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp_sysclock_get_freq() 1
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_tstamp_sysclock_get (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (bl_tstamp) t.tv_sec;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp_funcs_microsecond_base.h>
#include <bl/base/impl/timestamp_sysclock_funcs_second_base.h>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
#define bl_tstamp_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_get_tstamp (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (((bl_u64) t.tv_sec) * bl_nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp_sysclock_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_tstamp_sysclock_get (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (((bl_u64) t.tv_sec) * bl_nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/timestamp_funcs_nanosecond_base.h>
#include <bl/base/impl/timestamp_sysclock_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
#endif /* #if !defined (BL_TIMESTAMP_64BIT) */
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_tstamp_sysclock_to_epoch (bl_tstamp t)
{
  return t;
}
/*---------------------------------------------------------------------------*/
/* private. for internal use */
/*---------------------------------------------------------------------------*/
static void bl_timespec_normalize (struct timespec* t)
{
  t->tv_sec += (t->tv_nsec / bl_nsec_in_sec);
  t->tv_nsec = (t->tv_nsec % bl_nsec_in_sec);
}
/* private. for internal use */
/*---------------------------------------------------------------------------*/
static inline struct timespec bl_timespec_us_from_now (bl_u32 usec, int clock)
{
  struct timespec t;
  clock_gettime (clock, &t);
  t.tv_sec  += usec / bl_usec_in_sec;
  t.tv_nsec += (usec % bl_usec_in_sec) * bl_nsec_in_usec;
  bl_timespec_normalize (&t);
  return t;
}
/*----------------------------------------------------------------------------*/

#endif /* __BL_TIMESTAMP_POSIX_H__ */
