/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/timepoint_funcs_nanosecond_base.gen.h -r BITS=64,CLOCKNAME=timept64 -r BITS=64,CLOCKNAME=sysclock64

Don't edit.
*/
/* AUTOGENERATE: include/bl/base/impl/generated/time/timept64_funcs_nanosecond_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_TIMEPT64_FNS_NANOSECOND_BASE_H__
#define __BL_TIMEPT64_FNS_NANOSECOND_BASE_H__

/* Common time conversions for the timept64 clock*/

#include <bl/base/static_integer_math.h>
#include <bl/base/assert.h>
/*---------------------------------------------------------------------------*/
#define BL_TIMEPT64_BASE BL_NANOSECOND_BASE
#define BL_TIMEPT64_BITS 64
/*---------------------------------------------------------------------------*/
#define bl_timept64_to_sec_priv(ts)\
  ((bl_timeoft64) (ts / bl_nsec_in_sec))

#define bl_timept64_to_msec_priv(ts)\
  ((bl_timeoft64) (ts / bl_nsec_in_msec))

#define bl_timept64_to_usec_priv(ts)\
  ((bl_timeoft64) (ts / bl_nsec_in_usec))

#define bl_timept64_to_nsec_priv(ts)\
  ((bl_timeoft64) ts)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_timept64_max()\
  bl_timept64_to_sec_priv (bl_timept64_max_safe_add_sub)

#define bl_msec_to_timept64_max()\
  bl_timept64_to_msec_priv (bl_timept64_max_safe_add_sub)

#define bl_nsec_to_timept64_max()\
  bl_timept64_to_nsec_priv (bl_timept64_max_safe_add_sub)

#define bl_usec_to_timept64_max()\
  bl_timept64_to_usec_priv (bl_timept64_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline bl_timeoft64 bl_timept64_to_sec (bl_timept64 ts)
{
  return bl_timept64_to_sec_priv (ts);
}

static inline bl_timeoft64 bl_timept64_to_msec (bl_timept64 ts)
{
  return bl_timept64_to_msec_priv (ts);
}

static inline bl_timeoft64 bl_timept64_to_usec (bl_timept64 ts)
{
  return bl_timept64_to_usec_priv (ts);
}

static inline bl_timeoft64 bl_timept64_to_nsec (bl_timept64 ts)
{
  return bl_timept64_to_nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft64 bl_timept64_to_sec_ceil (bl_timept64 ts)
{
  return (bl_timeoft64) bl_div_ceil (ts, bl_nsec_in_sec);
}

static inline bl_timeoft64 bl_timept64_to_msec_ceil (bl_timept64 ts)
{
  return (bl_timeoft64) bl_div_ceil (ts, bl_nsec_in_msec);
}

static inline bl_timeoft64 bl_timept64_to_usec_ceil (bl_timept64 ts)
{
  return (bl_timeoft64) bl_div_ceil (ts, bl_nsec_in_usec);
}

static inline bl_timeoft64 bl_timept64_to_nsec_ceil (bl_timept64 ts)
{
  return (bl_timeoft64) (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_sec_to_timept64 (bl_timeoft64 sec)
{
  bl_assert (sec <= bl_sec_to_timept64_max());
  return ((bl_timept64) sec) * bl_nsec_in_sec;
}

static inline bl_timept64 bl_msec_to_timept64 (bl_timeoft64 msec)
{
  bl_assert (msec <= bl_msec_to_timept64_max());
  return ((bl_timept64) msec) * bl_nsec_in_msec;
}

static inline bl_timept64 bl_usec_to_timept64 (bl_timeoft64 usec)
{
  bl_assert (usec <= bl_usec_to_timept64_max());
  return ((bl_timept64) usec) * bl_nsec_in_usec;
}

static inline bl_timept64 bl_nsec_to_timept64 (bl_timeoft64 nsec)
{
  bl_assert (nsec <= bl_nsec_to_timept64_max());
  return (bl_timept64) nsec;
}
/*---------------------------------------------------------------------------*/

#endif /*__BL_TIMEPT64_FNS_NANOSECOND_BASE_H__*/
