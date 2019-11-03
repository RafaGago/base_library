/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/base/impl/generated/timestamp/timestamp{BITS}_funcs_nanosecond_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_TIMESTAMP{BITS}_FNS_NANOSECOND_BASE_H__
#define __BL_TIMESTAMP{BITS}_FNS_NANOSECOND_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define BL_TIMESTAMP{BITS}_HAS_NANOSECOND_BASE 1
/*---------------------------------------------------------------------------*/
#define bl_tstamp{BITS}_to_sec_priv(ts)\
  ((bl_toffset{BITS}) (ts / bl_nsec_in_sec))

#define bl_tstamp{BITS}_to_msec_priv(ts)\
  ((bl_toffset{BITS}) (ts / bl_nsec_in_msec))

#define bl_tstamp{BITS}_to_usec_priv(ts)\
  ((bl_toffset{BITS}) (ts / bl_nsec_in_usec))

#define bl_tstamp{BITS}_to_nsec_priv(ts)\
  ((bl_toffset{BITS}) ts)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_tstamp{BITS}_max()\
  bl_tstamp{BITS}_to_sec_priv (bl_tstamp{BITS}_max_safe_add_sub)

#define bl_msec_to_tstamp{BITS}_max()\
  bl_tstamp{BITS}_to_msec_priv (bl_tstamp{BITS}_max_safe_add_sub)

#define bl_nsec_to_tstamp{BITS}_max()\
  bl_tstamp{BITS}_to_nsec_priv (bl_tstamp{BITS}_max_safe_add_sub)

#define bl_usec_to_tstamp{BITS}_max()\
  bl_tstamp{BITS}_to_usec_priv (bl_tstamp{BITS}_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline bl_toffset{BITS} bl_tstamp{BITS}_to_sec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_sec_priv (ts);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_msec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_msec_priv (ts);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_usec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_usec_priv (ts);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_nsec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_toffset{BITS} bl_tstamp{BITS}_to_sec_ceil (bl_tstamp{BITS} ts)
{
  return (bl_toffset{BITS}) bl_div_ceil (ts, bl_nsec_in_sec);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_msec_ceil (bl_tstamp{BITS} ts)
{
  return (bl_toffset{BITS}) bl_div_ceil (ts, bl_nsec_in_msec);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_usec_ceil (bl_tstamp{BITS} ts)
{
  return (bl_toffset{BITS}) bl_div_ceil (ts, bl_nsec_in_usec);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_nsec_ceil (bl_tstamp{BITS} ts)
{
  return (bl_toffset{BITS}) (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp{BITS} bl_sec_to_tstamp{BITS} (bl_toffset{BITS} sec)
{
  bl_assert (sec <= bl_sec_to_tstamp{BITS}_max());
  return ((bl_tstamp{BITS}) sec) * bl_nsec_in_sec;
}

static inline bl_tstamp{BITS} bl_msec_to_tstamp{BITS} (bl_toffset{BITS} msec)
{
  bl_assert (msec <= bl_msec_to_tstamp{BITS}_max());
  return ((bl_tstamp{BITS}) msec) * bl_nsec_in_msec;
}

static inline bl_tstamp{BITS} bl_usec_to_tstamp{BITS} (bl_toffset{BITS} usec)
{
  bl_assert (usec <= bl_usec_to_tstamp{BITS}_max());
  return ((bl_tstamp{BITS}) usec) * bl_nsec_in_usec;
}

static inline bl_tstamp{BITS} bl_nsec_to_tstamp{BITS} (bl_toffset{BITS} nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp{BITS}_max());
  return (bl_tstamp{BITS}) nsec;
}
/*---------------------------------------------------------------------------*/

#endif /*__TIMESTAMP{BITS}_FNS_NANOSECOND_BASE_H__*/
