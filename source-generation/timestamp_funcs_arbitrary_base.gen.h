/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/base/impl/generated/timestamp/timestamp{BITS}_funcs_arbitrary_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_TIMESTAMP{BITS}_FNS_ARBITRARY_BASE_H__
#define __BL_TIMESTAMP{BITS}_FNS_ARBITRARY_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/libexport.h>

#define BL_TIMESTAMP{BITS}_HAS_ARBITRARY_BASE 1
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_toffset{BITS} bl_tstamp{BITS}_to_private(bl_tstamp{BITS} ts, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_toffset{BITS}
  bl_tstamp{BITS}_to_ceil_private (bl_tstamp{BITS} ts, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_tstamp{BITS}
  bl_to_tstamp{BITS}_private (bl_toffset{BITS} amt, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
static inline bl_toffset{BITS} bl_tstamp{BITS}_to_sec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_private (ts, 1);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_msec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_private (ts, bl_msec_in_sec);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_usec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_private (ts, bl_usec_in_sec);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_nsec (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_toffset{BITS} bl_tstamp{BITS}_to_sec_ceil (bl_tstamp{BITS} ts)
{
   return bl_tstamp{BITS}_to_ceil_private (ts, 1);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_msec_ceil (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_ceil_private (ts, bl_msec_in_sec);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_usec_ceil (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_ceil_private (ts, bl_usec_in_sec);
}

static inline bl_toffset{BITS} bl_tstamp{BITS}_to_nsec_ceil (bl_tstamp{BITS} ts)
{
  return bl_tstamp{BITS}_to_ceil_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_toffset{BITS} bl_sec_to_tstamp{BITS}_max (void)
{
  return bl_tstamp{BITS}_to_sec (bl_tstamp{BITS}_max_safe_add_sub);
}

static inline bl_toffset{BITS} bl_msec_to_tstamp{BITS}_max (void)
{
  return bl_tstamp{BITS}_to_msec (bl_tstamp{BITS}_max_safe_add_sub);
}

static inline bl_toffset{BITS} bl_usec_to_tstamp{BITS}_max (void)
{
  return bl_tstamp{BITS}_to_usec (bl_tstamp{BITS}_max_safe_add_sub);
}

static inline bl_toffset{BITS} bl_nsec_to_tstamp{BITS}_max (void)
{
  return bl_tstamp{BITS}_to_nsec (bl_tstamp{BITS}_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp{BITS} bl_sec_to_tstamp{BITS} (bl_toffset{BITS} sec)
{
  bl_assert (sec <= bl_sec_to_tstamp{BITS}_max ());
  return bl_to_tstamp{BITS}_private (sec, 1);
}

static inline bl_tstamp{BITS} bl_msec_to_tstamp{BITS} (bl_toffset{BITS} msec)
{
  bl_assert (msec <= bl_msec_to_tstamp{BITS}_max ());
  return bl_to_tstamp{BITS}_private (msec, bl_msec_in_sec);
}

static inline bl_tstamp{BITS} bl_usec_to_tstamp{BITS} (bl_toffset{BITS} usec)
{
  bl_assert (usec <= bl_usec_to_tstamp{BITS}_max ());
  return bl_to_tstamp{BITS}_private (usec, bl_usec_in_sec);
}

static inline bl_tstamp{BITS} bl_nsec_to_tstamp{BITS} (bl_toffset{BITS} nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp{BITS}_max ());
  return bl_to_tstamp{BITS}_private (nsec, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
#endif /* __TIMESTAMP{BITS}_FNS_ARBITRARY_BASE_H__ */
