/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/{HDR_PREFIX}/{CLOCKNAME}_deadline.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_DEADLINE_{CLOCKNAME_UPPER}_H__
#define __BL_DEADLINE_{CLOCKNAME_UPPER}_H__

/* deadline/timeout usage pattern coded in a correct way */

#include <bl/{INCLUDE_HDR}>
#include <bl/base/assert.h>
#include <bl/base/error.h>

/*---------------------------------------------------------------------------*/
/* deadline {CLOCKNAME} */
/*---------------------------------------------------------------------------*/
#if BL_{CLOCKNAME_UPPER}_BASE <= BL_NANOSECOND_BASE

static inline bl_err bl_{CLOCKNAME}_deadline_init_nsec_explicit(
  bl_timept{BITS}* d, bl_timept{BITS} now, bl_timeoft{BITS} nsec
  )
{
  bl_assert (d);
  bl_assert (nsec <= bl_nsec_to_{CLOCKNAME}_max());
  if (nsec <= bl_nsec_to_{CLOCKNAME}_max()) {
    *d = now + bl_nsec_to_{CLOCKNAME} (nsec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
static inline bl_err
  bl_{CLOCKNAME}_deadline_init_nsec (bl_timept{BITS}* d, bl_timeoft{BITS} nsec)
{
  return bl_{CLOCKNAME}_deadline_init_nsec_explicit(
    d, bl_{CLOCKNAME}_get(), nsec
    );
}
#endif
/*---------------------------------------------------------------------------*/
#if BL_{CLOCKNAME_UPPER}_BASE <= BL_MICROSECOND_BASE

static inline bl_err bl_{CLOCKNAME}_deadline_init_usec_explicit(
  bl_timept{BITS}* d, bl_timept{BITS} now, bl_timeoft{BITS} usec
  )
{
  bl_assert (d);
  bl_assert (usec <= bl_nsec_to_{CLOCKNAME}_max());
  if (usec <= bl_usec_to_{CLOCKNAME}_max()) {
    *d = now + bl_usec_to_{CLOCKNAME} (usec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}

static inline bl_err
  bl_{CLOCKNAME}_deadline_init_usec (bl_timept{BITS}* d, bl_timeoft{BITS} usec)
{
  return bl_{CLOCKNAME}_deadline_init_usec_explicit(
    d, bl_{CLOCKNAME}_get(), usec
    );
}
#endif
/*---------------------------------------------------------------------------*/
#if BL_{CLOCKNAME_UPPER}_BASE <= BL_MILLISECOND_BASE

static inline bl_err bl_{CLOCKNAME}_deadline_init_msec_explicit(
  bl_timept{BITS}* d, bl_timept{BITS} now, bl_timeoft{BITS} msec
  )
{
  bl_assert (d);
  bl_assert (msec <= bl_msec_to_{CLOCKNAME}_max());
  if (msec <= bl_msec_to_{CLOCKNAME}_max()) {
    *d = now + bl_msec_to_{CLOCKNAME} (msec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
static inline bl_err
  bl_{CLOCKNAME}_deadline_init_msec (bl_timept{BITS}* d, bl_timeoft{BITS} msec)
{
  return bl_{CLOCKNAME}_deadline_init_msec_explicit(
    d, bl_{CLOCKNAME}_get(), msec
    );
}
#endif
/*---------------------------------------------------------------------------*/
#if BL_{CLOCKNAME_UPPER}_BASE <= BL_SECOND_BASE

static inline bl_err bl_{CLOCKNAME}_deadline_init_sec_explicit(
  bl_timept{BITS}* d, bl_timept{BITS} now, bl_timeoft{BITS} sec
  )
{
  bl_assert (d);
  bl_assert (sec <= bl_sec_to_{CLOCKNAME}_max());
  if (sec <= bl_sec_to_{CLOCKNAME}_max()) {
    *d = now + bl_sec_to_{CLOCKNAME} (sec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}

static inline bl_err
  bl_{CLOCKNAME}_deadline_init_sec (bl_timept{BITS}* d, bl_timeoft{BITS} sec)
{
  return bl_{CLOCKNAME}_deadline_init_sec_explicit(
    d, bl_{CLOCKNAME}_get(), sec
    );
}
#endif
/*---------------------------------------------------------------------------*/
static inline bl_timept{BITS}diff bl_{CLOCKNAME}_deadline_compare(
  bl_timept{BITS} a, bl_timept{BITS} b
  )
{
  return bl_timept{BITS}_get_diff (a, b);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept{BITS}
  bl_{CLOCKNAME}_deadline_min (bl_timept{BITS} a, bl_timept{BITS} b)
{
  return bl_{CLOCKNAME}_deadline_compare (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept{BITS}
  bl_{CLOCKNAME}_deadline_max (bl_timept{BITS} a, bl_timept{BITS} b)
{
  return bl_{CLOCKNAME}_deadline_compare (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_{CLOCKNAME}_deadline_expired_explicit(
  bl_timept{BITS} d, bl_timept{BITS} now
  )
{
  return bl_{CLOCKNAME}_deadline_compare (d, now) <= 0;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_{CLOCKNAME}_deadline_expired (bl_timept{BITS} d)
{
  return bl_{CLOCKNAME}_deadline_expired_explicit (d, bl_{CLOCKNAME}_get());
}
/*---------------------------------------------------------------------------*/

#endif /* #ifndef __BL_DEADLINE_{CLOCKNAME_UPPER}_H__ */
