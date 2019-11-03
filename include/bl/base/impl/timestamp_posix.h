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
/* 32 bit */
/*---------------------------------------------------------------------------*/
#define bl_tstamp32_get_freq() bl_usec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_get (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (bl_tstamp32)
   ((((bl_u64) t.tv_sec) * bl_usec_in_sec) + (t.tv_nsec / bl_nsec_in_usec));
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp32_sysclock_get_freq() 1
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_sysclock_get (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (bl_tstamp32) t.tv_sec;
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_sysclock_to_epoch (bl_tstamp32 t)
{
  return t;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/generated/timestamp/timestamp32_funcs_microsecond_base.h>
#include <bl/base/impl/generated/timestamp/timestamp32_sysclock_funcs_second_base.h>
/*---------------------------------------------------------------------------*/
/* 64 bit */
/*---------------------------------------------------------------------------*/
#define bl_tstamp64_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_get (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (((bl_u64) t.tv_sec) * bl_nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp64_sysclock_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_sysclock_get (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (((bl_u64) t.tv_sec) * bl_nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_sysclock_to_epoch (bl_tstamp64 t)
{
  return t;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/generated/timestamp/timestamp64_funcs_nanosecond_base.h>
#include <bl/base/impl/generated/timestamp/timestamp64_sysclock_funcs_nanosecond_base.h>
/*----------------------------------------------------------------------------*/
/* timestamp bases. */
/*----------------------------------------------------------------------------*/
#if !defined BL_TIMESTAMP_64BIT
  #include <bl/base/impl/generated/timestamp/timestamp_funcs_microsecond_base.h>
  #include <bl/base/impl/generated/timestamp/timestamp_sysclock_funcs_second_base.h>
#else
  #include <bl/base/impl/generated/timestamp/timestamp_funcs_nanosecond_base.h>
  #include <bl/base/impl/generated/timestamp/timestamp_sysclock_funcs_nanosecond_base.h>
#endif /* #if !defined (BL_TIMESTAMP_64BIT) */
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

#endif /* __BL_TIMESTAMP_POSIX_H__ */
