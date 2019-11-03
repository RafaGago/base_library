/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/timepoint_funcs_microsecond_base.gen.h -r BITS=32 -r BITS=

Don't edit.
*/
/* AUTOGENERATE: include/bl/base/impl/generated/timepoint/timepoint_funcs_microsecond_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_TIMEPOINT_FNS_MICROSECOND_BASE_H__
#define __BL_TIMEPOINT_FNS_MICROSECOND_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define BL_TIMEPOINT_HAS_MICROSECOND_BASE 1
/*---------------------------------------------------------------------------*/
#define bl_timept_to_sec_priv(ts)\
  ((bl_timeoft) (ts / bl_usec_in_sec))

#define bl_timept_to_msec_priv(ts)\
  ((bl_timeoft) (ts / bl_usec_in_msec))

#define bl_timept_to_usec_priv(ts)\
  ((bl_timeoft) ts)

#define bl_timept_to_nsec_priv(ts)\
  (((bl_timeoft) ts) * bl_nsec_in_usec)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_timept_max()\
  bl_timept_to_sec_priv (bl_timept_max_safe_add_sub)

#define bl_msec_to_timept_max()\
  bl_timept_to_msec_priv (bl_timept_max_safe_add_sub)

#define bl_nsec_to_timept_max()\
  bl_timept_to_nsec_priv (bl_timept_max_safe_add_sub)

#define bl_usec_to_timept_max()\
  bl_timept_to_usec_priv (bl_timept_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline bl_timeoft bl_timept_to_sec (bl_timept ts)
{
  return bl_timept_to_sec_priv (ts);
}

static inline bl_timeoft bl_timept_to_msec (bl_timept ts)
{
  return bl_timept_to_msec_priv (ts);
}

static inline bl_timeoft bl_timept_to_usec (bl_timept ts)
{
  return bl_timept_to_usec_priv (ts);
}

static inline bl_timeoft bl_timept_to_nsec (bl_timept ts)
{
  return bl_timept_to_nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft bl_timept_to_sec_ceil (bl_timept ts)
{
  return (bl_timeoft) bl_div_ceil (ts, bl_usec_in_sec);
}

static inline bl_timeoft bl_timept_to_msec_ceil (bl_timept ts)
{
  return (bl_timeoft) bl_div_ceil (ts, bl_usec_in_msec);
}

static inline bl_timeoft bl_timept_to_usec_ceil (bl_timept ts)
{
  return ((bl_timeoft) ts);
}

static inline bl_timeoft bl_timept_to_nsec_ceil (bl_timept ts)
{
  return (bl_timeoft) (ts * bl_nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept bl_sec_to_timept (bl_timeoft sec)
{
  bl_assert (sec <= bl_sec_to_timept_max());
  return ((bl_timept) sec) * bl_usec_in_sec;
}

static inline bl_timept bl_msec_to_timept (bl_timeoft msec)
{
  bl_assert (msec <= bl_msec_to_timept_max());
  return ((bl_timept) msec) * bl_usec_in_msec;
}

static inline bl_timept bl_usec_to_timept (bl_timeoft usec)
{
  bl_assert (usec <= bl_usec_to_timept_max());
  return ((bl_timept) usec);
}

static inline bl_timept bl_nsec_to_timept (bl_timeoft nsec)
{
  bl_assert (nsec <= bl_nsec_to_timept_max());
  return bl_div_ceil (((bl_timept) nsec), bl_nsec_in_usec);
}
/*---------------------------------------------------------------------------*/

#endif /*__TIMEPOINT_FNS_MICROSECOND_BASE_H__*/
