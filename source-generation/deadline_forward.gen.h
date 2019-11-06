/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/{HDR_PREFIX}/{DSTCLK}_deadline_from_{SRCCLK}.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_{DSTCLK_UPPER}_DEADLINE_FROM_{SRCCLK_UPPER}_
#define __BL_{DSTCLK_UPPER}_DEADLINE_FROM_{SRCCLK_UPPER}_
/*---------------------------------------------------------------------------*/
#if BL_{DSTCLK_UPPER}_BASE <= BL_NANOSECOND_BASE

#define bl_{DSTCLK}_deadline_init_nsec_explicit(d, now, nsec) \
  bl_{SRCCLK}_deadline_init_nsec_explicit (d, now, nsec)

#define bl_{DSTCLK}_deadline_init_nsec(d, nsec) \
  bl_{SRCCLK}_deadline_init_nsec (d, nsec)

#endif
/*---------------------------------------------------------------------------*/
#if BL_{DSTCLK_UPPER}_BASE <= BL_MICROSECOND_BASE

#define bl_{DSTCLK}_deadline_init_usec_explicit(d, now, usec) \
  bl_{SRCCLK}_deadline_init_usec_explicit (d, now, usec)

#define bl_{DSTCLK}_deadline_init_usec(d, usec) \
  bl_{SRCCLK}_deadline_init_usec (d, usec)

#endif
/*---------------------------------------------------------------------------*/
#if BL_{DSTCLK_UPPER}_BASE <= BL_MILLISECOND_BASE

#define bl_{DSTCLK}_deadline_init_msec_explicit(d, now, msec) \
  bl_{SRCCLK}_deadline_init_msec_explicit (d, now, msec)

#define bl_{DSTCLK}_deadline_init_msec(d, msec) \
  bl_{SRCCLK}_deadline_init_msec (d, msec)

#endif
/*---------------------------------------------------------------------------*/
  #if BL_{DSTCLK_UPPER}_BASE <= BL_SECOND_BASE

#define bl_{DSTCLK}_deadline_init_sec_explicit(d, now, sec) \
  bl_{SRCCLK}_deadline_init_sec_explicit (d, now, sec)

#define bl_{DSTCLK}_deadline_init_sec(d, sec) \
  bl_{SRCCLK}_deadline_init_sec (d, sec)

#endif
/*---------------------------------------------------------------------------*/
#define bl_{DSTCLK}_deadline_compare(a, b) \
  bl_{SRCCLK}_deadline_compare (a, b)

#define bl_{DSTCLK}_deadline_min(a, b) \
  bl_{SRCCLK}_deadline_min (a, b)

#define bl_{DSTCLK}_deadline_max(a, b) \
  bl_{SRCCLK}_deadline_max (a, b)

#define bl_{DSTCLK}_deadline_expired_explicit(d, now) \
  bl_{SRCCLK}_deadline_expired_explicit (d, now)

#define bl_{DSTCLK}_deadline_expired(d) \
  bl_{SRCCLK}_deadline_expired (d)
/*---------------------------------------------------------------------------*/
#endif /*__BL_{DSTCLK_UPPER}_DEADLINE_FROM_{SRCCLK_UPPER}_*/
