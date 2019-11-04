/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/base/impl/generated/time/{CLOCKNAME}_funcs_microsecond_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_{CLOCKNAME_UPPER}_FNS_MICROSECOND_BASE_H__
#define __BL_{CLOCKNAME_UPPER}_FNS_MICROSECOND_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/static_integer_math.h>
#include <bl/base/assert.h>
/*---------------------------------------------------------------------------*/
#define BL_{CLOCKNAME_UPPER}_HAS_MICROSECOND_BASE 1
#define BL_{CLOCKNAME_UPPER}_BITS (sizeof (bl_timept{BITS}) * 8)
/*---------------------------------------------------------------------------*/
#define bl_{CLOCKNAME}_to_sec_priv(ts)\
  ((bl_timeoft{BITS}) (ts / bl_usec_in_sec))

#define bl_{CLOCKNAME}_to_msec_priv(ts)\
  ((bl_timeoft{BITS}) (ts / bl_usec_in_msec))

#define bl_{CLOCKNAME}_to_usec_priv(ts)\
  ((bl_timeoft{BITS}) ts)

#define bl_{CLOCKNAME}_to_nsec_priv(ts)\
  (((bl_timeoft{BITS}) ts) * bl_nsec_in_usec)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_{CLOCKNAME}_max()\
  bl_{CLOCKNAME}_to_sec_priv (bl_{CLOCKNAME}_max_safe_add_sub)

#define bl_msec_to_{CLOCKNAME}_max()\
  bl_{CLOCKNAME}_to_msec_priv (bl_{CLOCKNAME}_max_safe_add_sub)

#define bl_nsec_to_{CLOCKNAME}_max()\
  bl_{CLOCKNAME}_to_nsec_priv (bl_{CLOCKNAME}_max_safe_add_sub)

#define bl_usec_to_{CLOCKNAME}_max()\
  bl_{CLOCKNAME}_to_usec_priv (bl_{CLOCKNAME}_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_sec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_sec_priv (ts);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_msec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_msec_priv (ts);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_usec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_usec_priv (ts);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_nsec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_sec_ceil (bl_timept{BITS} ts)
{
  return (bl_timeoft{BITS}) bl_div_ceil (ts, bl_usec_in_sec);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_msec_ceil (bl_timept{BITS} ts)
{
  return (bl_timeoft{BITS}) bl_div_ceil (ts, bl_usec_in_msec);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_usec_ceil (bl_timept{BITS} ts)
{
  return ((bl_timeoft{BITS}) ts);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_nsec_ceil (bl_timept{BITS} ts)
{
  return (bl_timeoft{BITS}) (ts * bl_nsec_in_usec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept{BITS} bl_sec_to_{CLOCKNAME} (bl_timeoft{BITS} sec)
{
  bl_assert (sec <= bl_sec_to_{CLOCKNAME}_max());
  return ((bl_timept{BITS}) sec) * bl_usec_in_sec;
}

static inline bl_timept{BITS} bl_msec_to_{CLOCKNAME} (bl_timeoft{BITS} msec)
{
  bl_assert (msec <= bl_msec_to_{CLOCKNAME}_max());
  return ((bl_timept{BITS}) msec) * bl_usec_in_msec;
}

static inline bl_timept{BITS} bl_usec_to_{CLOCKNAME} (bl_timeoft{BITS} usec)
{
  bl_assert (usec <= bl_usec_to_{CLOCKNAME}_max());
  return ((bl_timept{BITS}) usec);
}

static inline bl_timept{BITS} bl_nsec_to_{CLOCKNAME} (bl_timeoft{BITS} nsec)
{
  bl_assert (nsec <= bl_nsec_to_{CLOCKNAME}_max());
  return bl_div_ceil (((bl_timept{BITS}) nsec), bl_nsec_in_usec);
}
/*---------------------------------------------------------------------------*/

#endif /*__BL_{CLOCKNAME_UPPER}_FNS_MICROSECOND_BASE_H__*/
