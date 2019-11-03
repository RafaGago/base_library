#ifndef __BL_TIMEPOINT_POSIX_H__
#define __BL_TIMEPOINT_POSIX_H__

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
#define bl_timept32_get_freq() bl_usec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_get (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (bl_timept32)
   ((((bl_u64) t.tv_sec) * bl_usec_in_sec) + (t.tv_nsec / bl_nsec_in_usec));
}
/*---------------------------------------------------------------------------*/
#define bl_timept32_sysclock_get_freq() 1
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_sysclock_get (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (bl_timept32) t.tv_sec;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_sysclock_to_epoch (bl_timept32 t)
{
  return t;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/generated/timepoint/timepoint32_funcs_microsecond_base.h>
#include <bl/base/impl/generated/timepoint/timepoint32_sysclock_funcs_second_base.h>
/*---------------------------------------------------------------------------*/
/* 64 bit */
/*---------------------------------------------------------------------------*/
#define bl_timept64_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_get (void)
{
  struct timespec t;
  clock_gettime (BL_CLOCK_MONOTONIC, &t);
  return (((bl_u64) t.tv_sec) * bl_nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
#define bl_timept64_sysclock_get_freq() bl_nsec_in_sec
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_sysclock_get (void)
{
  struct timespec t;
  clock_gettime (CLOCK_REALTIME, &t);
  return (((bl_u64) t.tv_sec) * bl_nsec_in_sec) + t.tv_nsec;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_sysclock_to_epoch (bl_timept64 t)
{
  return t;
}
/*---------------------------------------------------------------------------*/
#include <bl/base/impl/generated/timepoint/timepoint64_funcs_nanosecond_base.h>
#include <bl/base/impl/generated/timepoint/timepoint64_sysclock_funcs_nanosecond_base.h>
/*----------------------------------------------------------------------------*/
/* timepoint bases. */
/*----------------------------------------------------------------------------*/
#if BL_TIMEPOINT_BITS == 32
  #include <bl/base/impl/generated/timepoint/timepoint_funcs_microsecond_base.h>
  #include <bl/base/impl/generated/timepoint/timepoint_sysclock_funcs_second_base.h>
#else
  #include <bl/base/impl/generated/timepoint/timepoint_funcs_nanosecond_base.h>
  #include <bl/base/impl/generated/timepoint/timepoint_sysclock_funcs_nanosecond_base.h>
#endif
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

#endif /* __BL_TIMEPOINT_POSIX_H__ */
