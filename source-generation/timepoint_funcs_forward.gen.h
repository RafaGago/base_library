/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor to color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/{HDR_PREFIX}/{DSTCLK}_funcs_from_{SRCCLK}.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_{DSTCLK_UPPER}_FUNC_FROM_{SRCCLK_UPPER}_
#define __BL_{DSTCLK_UPPER}_FUNC_FROM_{SRCCLK_UPPER}_

/*---------------------------------------------------------------------------*/
#if defined (BL_{SRCCLK_UPPER}_HAS_NANOSECOND_BASE)
  #define BL_{DSTCLK_UPPER}_HAS_NANOSECOND_BASE 1
#elif defined (BL_{SRCCLK_UPPER}_HAS_MICROSECOND_BASE)
  #define BL_{DSTCLK_UPPER}_HAS_MICROSECOND_BASE 1
#elif defined (BL_{SRCCLK_UPPER}_HAS_MILLISECOND_BASE)
  #define BL_{DSTCLK_UPPER}_HAS_MILLISECOND_BASE 1
#elif defined (BL_{SRCCLK_UPPER}_HAS_SECOND_BASE)
  #define BL_{DSTCLK_UPPER}_HAS_SECOND_BASE 1
#elif defined (BL_{SRCCLK_UPPER}_HAS_ARBITRARY_BASE)
  #define BL_{DSTCLK_UPPER}_HAS_ARBITRARY_BASE 1
#else
  #error "Couldn't determine time base of BL_{SRCCLK_UPPER}"
#endif
#define BL_{DSTCLK_UPPER}_BITS BL_{SRCCLK_UPPER}_BITS
/*---------------------------------------------------------------------------*/
#define bl_sec_to_{DSTCLK}_max()  bl_sec_to_{SRCCLK}_max()
#define bl_msec_to_{DSTCLK}_max() bl_msec_to_{SRCCLK}_max()
#define bl_nsec_to_{DSTCLK}_max() bl_nsec_to_{SRCCLK}_max()
#define bl_usec_to_{DSTCLK}_max() bl_usec_to_{SRCCLK}_max()
/*---------------------------------------------------------------------------*/
#define bl_{DSTCLK}_to_sec(x)  bl_{SRCCLK}_to_sec (x)
#define bl_{DSTCLK}_to_msec(x) bl_{SRCCLK}_to_msec (x)
#define bl_{DSTCLK}_to_usec(x) bl_{SRCCLK}_to_usec (x)
#define bl_{DSTCLK}_to_nsec(x) bl_{SRCCLK}_to_nsec (x)
/*---------------------------------------------------------------------------*/
#define bl_{DSTCLK}_to_sec_ceil(x)  bl_{SRCCLK}_to_sec_ceil( x)
#define bl_{DSTCLK}_to_msec_ceil(x) bl_{SRCCLK}_to_msec_ceil (x)
#define bl_{DSTCLK}_to_usec_ceil(x) bl_{SRCCLK}_to_usec_ceil (x)
#define bl_{DSTCLK}_to_nsec_ceil(x) bl_{SRCCLK}_to_nsec_ceil (x)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_{DSTCLK}(x)  bl_sec_to_{SRCCLK} (x)
#define bl_msec_to_{DSTCLK}(x) bl_msec_to_{SRCCLK} (x)
#define bl_usec_to_{DSTCLK}(x) bl_usec_to_{SRCCLK} (x)
#define bl_nsec_to_{DSTCLK}(x) bl_nsec_to_{SRCCLK} (x)
/*---------------------------------------------------------------------------*/

#endif /*__BL_{DSTCLK_UPPER}_FUNC_FROM_{SRCCLK_UPPER}_*/
