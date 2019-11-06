/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/{HDR_PREFIX}/{CLOCKNAME}_funcs_arbitrary_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_{CLOCKNAME_UPPER}_FNS_ARBITRARY_BASE_H__
#define __BL_{CLOCKNAME_UPPER}_FNS_ARBITRARY_BASE_H__

/* Common time conversions for the {CLOCKNAME} clock*/

#include <bl/{INCLUDE_HDR}>

#define BL_{CLOCKNAME_UPPER}_BASE BL_ARBITRARY_BASE
#define BL_{CLOCKNAME_UPPER}_BITS {BITS}
/*---------------------------------------------------------------------------*/
extern {EXPORT} bl_timeoft{BITS}
  bl_{CLOCKNAME}_to_time_private (bl_timept{BITS} ts, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
extern {EXPORT} bl_timeoft{BITS}
  bl_{CLOCKNAME}_to_time_ceil_private (bl_timept{BITS} amt, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
extern {EXPORT} bl_timept{BITS}
  bl_time_to_{CLOCKNAME}_private (bl_timeoft{BITS} amt, bl_u{BITS} factor);
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_sec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_time_private (ts, 1);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_msec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_time_private (ts, bl_msec_in_sec);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_usec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_time_private (ts, bl_usec_in_sec);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_nsec (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_time_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_sec_ceil (bl_timept{BITS} ts)
{
   return bl_{CLOCKNAME}_to_time_ceil_private (ts, 1);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_msec_ceil (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_time_ceil_private (ts, bl_msec_in_sec);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_usec_ceil (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_time_ceil_private (ts, bl_usec_in_sec);
}

static inline bl_timeoft{BITS} bl_{CLOCKNAME}_to_nsec_ceil (bl_timept{BITS} ts)
{
  return bl_{CLOCKNAME}_to_time_ceil_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft{BITS} bl_sec_to_{CLOCKNAME}_max (void)
{
  return bl_{CLOCKNAME}_to_sec (bl_timept{BITS}_max_safe_add_sub);
}

static inline bl_timeoft{BITS} bl_msec_to_{CLOCKNAME}_max (void)
{
  return bl_{CLOCKNAME}_to_msec (bl_timept{BITS}_max_safe_add_sub);
}

static inline bl_timeoft{BITS} bl_usec_to_{CLOCKNAME}_max (void)
{
  return bl_{CLOCKNAME}_to_usec (bl_timept{BITS}_max_safe_add_sub);
}

static inline bl_timeoft{BITS} bl_nsec_to_{CLOCKNAME}_max (void)
{
  return bl_{CLOCKNAME}_to_nsec (bl_timept{BITS}_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept{BITS} bl_sec_to_{CLOCKNAME} (bl_timeoft{BITS} sec)
{
  bl_assert (sec <= bl_sec_to_{CLOCKNAME}_max ());
  return bl_time_to_{CLOCKNAME}_private (sec, 1);
}

static inline bl_timept{BITS} bl_msec_to_{CLOCKNAME} (bl_timeoft{BITS} msec)
{
  bl_assert (msec <= bl_msec_to_{CLOCKNAME}_max ());
  return bl_time_to_{CLOCKNAME}_private (msec, bl_msec_in_sec);
}

static inline bl_timept{BITS} bl_usec_to_{CLOCKNAME} (bl_timeoft{BITS} usec)
{
  bl_assert (usec <= bl_usec_to_{CLOCKNAME}_max ());
  return bl_time_to_{CLOCKNAME}_private (usec, bl_usec_in_sec);
}
static inline bl_timept{BITS} bl_nsec_to_{CLOCKNAME} (bl_timeoft{BITS} nsec)
{
  bl_assert (nsec <= bl_nsec_to_{CLOCKNAME}_max ());
  return bl_time_to_{CLOCKNAME}_private (nsec, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
#endif /* #ifndef __BL_{CLOCKNAME_UPPER}_FNS_ARBITRARY_BASE_H__*/

/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: src/bl/{SRC_PREFIX}/{CLOCKNAME}_funcs_arbitrary_base.c */
/*---------------------------------------------------------------------------*/
#include <bl/{INCLUDE_SRC}>
#if BL_{CLOCKNAME_UPPER}_BASE == BL_ARBITRARY_BASE
/*---------------------------------------------------------------------------*/
{EXPORT} bl_timeoft{BITS}
  bl_{CLOCKNAME}_to_time_private (bl_timept{BITS} ts, bl_u{BITS} factor)
{
  bl_u64 f   = bl_{CLOCKNAME}_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_timeoft{BITS}) (sec * factor) + ((rem * factor) / f);
}
/*---------------------------------------------------------------------------*/
{EXPORT} bl_timeoft{BITS}
  bl_{CLOCKNAME}_to_time_ceil_private (bl_timept{BITS} ts, bl_u{BITS} factor)
{
  bl_u64 f   = bl_{CLOCKNAME}_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_timeoft{BITS}) (sec * factor) + bl_div_ceil (rem * factor, f);
}
/*---------------------------------------------------------------------------*/
{EXPORT} bl_timept{BITS}
  bl_time_to_{CLOCKNAME}_private (bl_timeoft{BITS} amt, bl_u{BITS} factor)
{
  bl_u64     f   = bl_{CLOCKNAME}_get_freq();
  bl_timeoft sec = amt / factor;
  bl_timeoft rem = amt % factor;
  return (bl_timept{BITS}) (sec * f) + bl_div_ceil (rem * f, factor);
}
/*---------------------------------------------------------------------------*/
#endif /* #if BL_{CLOCKNAME_UPPER}_BASE == BL_ARBITRARY_BASE */
