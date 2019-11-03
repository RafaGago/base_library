/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/base/impl/generated/timepoint/timepoint{BITS}_funcs_arbitrary_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_TIMEPOINT{BITS}_FNS_ARBITRARY_BASE_H__
#define __BL_TIMEPOINT{BITS}_FNS_ARBITRARY_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/libexport.h>

#define BL_TIMEPOINT{BITS}_HAS_ARBITRARY_BASE 1
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_timeoft{BITS} bl_timept{BITS}_to_private(bl_timept{BITS} ts, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_timeoft{BITS}
  bl_timept{BITS}_to_ceil_private (bl_timept{BITS} ts, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_timept{BITS}
  bl_to_timept{BITS}_private (bl_timeoft{BITS} amt, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_timept{BITS}_to_sec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_to_private (ts, 1);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_to_msec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_to_private (ts, bl_msec_in_sec);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_to_usec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_to_private (ts, bl_usec_in_sec);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_to_nsec (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_to_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_timept{BITS}_to_sec_ceil (bl_timept{BITS} ts)
{
   return bl_timept{BITS}_to_ceil_private (ts, 1);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_to_msec_ceil (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_to_ceil_private (ts, bl_msec_in_sec);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_to_usec_ceil (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_to_ceil_private (ts, bl_usec_in_sec);
}

static inline bl_timeoft{BITS} bl_timept{BITS}_to_nsec_ceil (bl_timept{BITS} ts)
{
  return bl_timept{BITS}_to_ceil_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_sec_to_timept{BITS}_max (void)
{
  return bl_timept{BITS}_to_sec (bl_timept{BITS}_max_safe_add_sub);
}

static inline bl_timeoft{BITS} bl_msec_to_timept{BITS}_max (void)
{
  return bl_timept{BITS}_to_msec (bl_timept{BITS}_max_safe_add_sub);
}

static inline bl_timeoft{BITS} bl_usec_to_timept{BITS}_max (void)
{
  return bl_timept{BITS}_to_usec (bl_timept{BITS}_max_safe_add_sub);
}

static inline bl_timeoft{BITS} bl_nsec_to_timept{BITS}_max (void)
{
  return bl_timept{BITS}_to_nsec (bl_timept{BITS}_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept{BITS} bl_sec_to_timept{BITS} (bl_timeoft{BITS} sec)
{
  bl_assert (sec <= bl_sec_to_timept{BITS}_max ());
  return bl_to_timept{BITS}_private (sec, 1);
}

static inline bl_timept{BITS} bl_msec_to_timept{BITS} (bl_timeoft{BITS} msec)
{
  bl_assert (msec <= bl_msec_to_timept{BITS}_max ());
  return bl_to_timept{BITS}_private (msec, bl_msec_in_sec);
}

static inline bl_timept{BITS} bl_usec_to_timept{BITS} (bl_timeoft{BITS} usec)
{
  bl_assert (usec <= bl_usec_to_timept{BITS}_max ());
  return bl_to_timept{BITS}_private (usec, bl_usec_in_sec);
}

static inline bl_timept{BITS} bl_nsec_to_timept{BITS} (bl_timeoft{BITS} nsec)
{
  bl_assert (nsec <= bl_nsec_to_timept{BITS}_max ());
  return bl_to_timept{BITS}_private (nsec, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
#endif /* __TIMEPOINT{BITS}_FNS_ARBITRARY_BASE_H__ */
