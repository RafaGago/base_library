/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/base/impl/generated/timepoint/timepoint{BITS}_sysclock_funcs_second_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_TIMEPOINT{BITS}_SYSCLOCK_FNS_SECOND_BASE_H__
#define __BL_TIMEPOINT{BITS}_SYSCLOCK_FNS_SECOND_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define BL_TIMEPOINT{BITS}_SYSCLOCK_HAS_SECOND_BASE 1
/*---------------------------------------------------------------------------*/
#define bl_timept{BITS}_sysclock_to_sec_priv(ts)\
  ((bl_timeoft{BITS}) ts)

#define bl_timept{BITS}_sysclock_to_msec_priv(ts)\
  (((bl_timeoft{BITS}) ts) * bl_msec_in_sec)

#define bl_timept{BITS}_sysclock_to_usec_priv(ts)\
  (((bl_timeoft{BITS}) ts) * bl_usec_in_sec)

#define bl_timept{BITS}_sysclock_to_nsec_priv(ts)\
  (((bl_timeoft{BITS}) ts) * bl_nsec_in_sec)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_timept{BITS}_sysclock_max()\
  bl_timept{BITS}_sysclock_to_sec_priv (bl_timept{BITS}_max_safe_add_sub)

#define bl_msec_to_timept{BITS}_sysclock_max()\
  bl_timept{BITS}_sysclock_to_msec_priv (bl_timept{BITS}_max_safe_add_sub)

#define bl_nsec_to_timept{BITS}_sysclock_max()\
  bl_timept{BITS}_sysclock_to_nsec_priv (bl_timept{BITS}_max_safe_add_sub)

#define bl_usec_to_timept{BITS}_sysclock_max()\
  bl_timept{BITS}_sysclock_to_usec_priv (bl_timept{BITS}_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_sec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_sysclock_to_sec_priv (ts);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_msec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_sysclock_to_msec_priv (ts);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_usec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_sysclock_to_usec_priv (ts);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_nsec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_sysclock_to_nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_sec_ceil (bl_timept{BITS} ts)
{
  return (bl_timeoft{BITS}) ts;
}

static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_msec_ceil (bl_timept{BITS} ts)
{
  return (bl_timeoft{BITS}) (ts * bl_msec_in_sec);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_usec_ceil (bl_timept{BITS} ts)
{
  return (bl_timeoft{BITS}) (ts * bl_usec_in_sec);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_sysclock_to_nsec_ceil (bl_timept{BITS} ts)
{
  return (bl_timeoft{BITS}) (ts * bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept{BITS} bl_sec_to_timept{BITS}_sysclock (bl_timeoft{BITS} sec)
{
  bl_assert (sec <= bl_sec_to_timept{BITS}_sysclock_max());
  return ((bl_timept{BITS}) sec);
}

static inline bl_timept{BITS} bl_msec_to_timept{BITS}_sysclock (bl_timeoft{BITS} msec)
{
  bl_assert (msec <= bl_msec_to_timept{BITS}_sysclock_max());
  return bl_div_ceil (((bl_timept{BITS}) msec), bl_msec_in_sec);
}

static inline bl_timept{BITS} bl_usec_to_timept{BITS}_sysclock (bl_timeoft{BITS} usec)
{
  bl_assert (usec <= bl_usec_to_timept{BITS}_sysclock_max());
  return bl_div_ceil (((bl_timept{BITS}) usec), bl_usec_in_sec);
}

static inline bl_timept{BITS} bl_nsec_to_timept{BITS}_sysclock (bl_timeoft{BITS} nsec)
{
  bl_assert (nsec <= bl_nsec_to_timept{BITS}_sysclock_max());
  return bl_div_ceil (((bl_timept{BITS}) nsec), bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/

#endif /*__TIMEPOINT{BITS}_FNS_MICROSECOND_BASE_H__*/
