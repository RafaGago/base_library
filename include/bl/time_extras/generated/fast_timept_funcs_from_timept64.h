/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/timepoint_funcs_forward.gen.h -r SRCCLK=timept32,DSTCLK=timept,HDR_PREFIX=base/impl/generated/time -r SRCCLK=timept64,DSTCLK=timept,HDR_PREFIX=base/impl/generated/time -r SRCCLK=sysclock32,DSTCLK=sysclock,HDR_PREFIX=base/impl/generated/time -r SRCCLK=sysclock64,DSTCLK=sysclock,HDR_PREFIX=base/impl/generated/time -r SRCCLK=cpu_timept,DSTCLK=fast_timept,HDR_PREFIX=time_extras/generated -r SRCCLK=timept64,DSTCLK=fast_timept,HDR_PREFIX=time_extras/generated

Don't edit.
*/
/* AUTOGENERATE: include/bl/time_extras/generated/fast_timept_funcs_from_timept64.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_FAST_TIMEPT_FUNC_FROM_TIMEPT64_
#define __BL_FAST_TIMEPT_FUNC_FROM_TIMEPT64_

/*---------------------------------------------------------------------------*/
#if defined (BL_TIMEPT64_HAS_NANOSECOND_BASE)
  #define BL_FAST_TIMEPT_HAS_NANOSECOND_BASE 1
#elif defined (BL_TIMEPT64_HAS_MICROSECOND_BASE)
  #define BL_FAST_TIMEPT_HAS_MICROSECOND_BASE 1
#elif defined (BL_TIMEPT64_HAS_MILLISECOND_BASE)
  #define BL_FAST_TIMEPT_HAS_MILLISECOND_BASE 1
#elif defined (BL_TIMEPT64_HAS_SECOND_BASE)
  #define BL_FAST_TIMEPT_HAS_SECOND_BASE 1
#elif defined (BL_TIMEPT64_HAS_ARBITRARY_BASE)
  #define BL_FAST_TIMEPT_HAS_ARBITRARY_BASE 1
#else
  #error "Couldn't determine time base of BL_TIMEPT64"
#endif
#define BL_FAST_TIMEPT_BITS BL_TIMEPT64_BITS
/*---------------------------------------------------------------------------*/
#define bl_sec_to_fast_timept_max()  bl_sec_to_timept64_max()
#define bl_msec_to_fast_timept_max() bl_msec_to_timept64_max()
#define bl_nsec_to_fast_timept_max() bl_nsec_to_timept64_max()
#define bl_usec_to_fast_timept_max() bl_usec_to_timept64_max()
/*---------------------------------------------------------------------------*/
#define bl_fast_timept_to_sec(x)  bl_timept64_to_sec (x)
#define bl_fast_timept_to_msec(x) bl_timept64_to_msec (x)
#define bl_fast_timept_to_usec(x) bl_timept64_to_usec (x)
#define bl_fast_timept_to_nsec(x) bl_timept64_to_nsec (x)
/*---------------------------------------------------------------------------*/
#define bl_fast_timept_to_sec_ceil(x)  bl_timept64_to_sec_ceil( x)
#define bl_fast_timept_to_msec_ceil(x) bl_timept64_to_msec_ceil (x)
#define bl_fast_timept_to_usec_ceil(x) bl_timept64_to_usec_ceil (x)
#define bl_fast_timept_to_nsec_ceil(x) bl_timept64_to_nsec_ceil (x)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_fast_timept(x)  bl_sec_to_timept64 (x)
#define bl_msec_to_fast_timept(x) bl_msec_to_timept64 (x)
#define bl_usec_to_fast_timept(x) bl_usec_to_timept64 (x)
#define bl_nsec_to_fast_timept(x) bl_nsec_to_timept64 (x)
/*---------------------------------------------------------------------------*/

#endif /*__BL_FAST_TIMEPT_FUNC_FROM_TIMEPT64_*/
