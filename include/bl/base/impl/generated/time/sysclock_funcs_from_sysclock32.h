/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/timepoint_funcs_forward.gen.h -r SRCCLK=timept32,DSTCLK=timept,HDR_PREFIX=base/impl/generated/time -r SRCCLK=timept64,DSTCLK=timept,HDR_PREFIX=base/impl/generated/time -r SRCCLK=sysclock32,DSTCLK=sysclock,HDR_PREFIX=base/impl/generated/time -r SRCCLK=sysclock64,DSTCLK=sysclock,HDR_PREFIX=base/impl/generated/time -r SRCCLK=cpu_timept,DSTCLK=fast_timept,HDR_PREFIX=time_extras/generated -r SRCCLK=timept64,DSTCLK=fast_timept,HDR_PREFIX=time_extras/generated

Don't edit.
*/
/* AUTOGENERATE: include/bl/base/impl/generated/time/sysclock_funcs_from_sysclock32.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_SYSCLOCK_FUNC_FROM_SYSCLOCK32_
#define __BL_SYSCLOCK_FUNC_FROM_SYSCLOCK32_

/*---------------------------------------------------------------------------*/
#if defined (BL_SYSCLOCK32_HAS_NANOSECOND_BASE)
  #define BL_SYSCLOCK_HAS_NANOSECOND_BASE 1
#elif defined (BL_SYSCLOCK32_HAS_MICROSECOND_BASE)
  #define BL_SYSCLOCK_HAS_MICROSECOND_BASE 1
#elif defined (BL_SYSCLOCK32_HAS_MILLISECOND_BASE)
  #define BL_SYSCLOCK_HAS_MILLISECOND_BASE 1
#elif defined (BL_SYSCLOCK32_HAS_SECOND_BASE)
  #define BL_SYSCLOCK_HAS_SECOND_BASE 1
#elif defined (BL_SYSCLOCK32_HAS_ARBITRARY_BASE)
  #define BL_SYSCLOCK_HAS_ARBITRARY_BASE 1
#else
  #error "Couldn't determine time base of BL_SYSCLOCK32"
#endif
#define BL_SYSCLOCK_BITS BL_SYSCLOCK32_BITS
/*---------------------------------------------------------------------------*/
#define bl_sec_to_sysclock_max()  bl_sec_to_sysclock32_max()
#define bl_msec_to_sysclock_max() bl_msec_to_sysclock32_max()
#define bl_nsec_to_sysclock_max() bl_nsec_to_sysclock32_max()
#define bl_usec_to_sysclock_max() bl_usec_to_sysclock32_max()
/*---------------------------------------------------------------------------*/
#define bl_sysclock_to_sec(x)  bl_sysclock32_to_sec (x)
#define bl_sysclock_to_msec(x) bl_sysclock32_to_msec (x)
#define bl_sysclock_to_usec(x) bl_sysclock32_to_usec (x)
#define bl_sysclock_to_nsec(x) bl_sysclock32_to_nsec (x)
/*---------------------------------------------------------------------------*/
#define bl_sysclock_to_sec_ceil(x)  bl_sysclock32_to_sec_ceil( x)
#define bl_sysclock_to_msec_ceil(x) bl_sysclock32_to_msec_ceil (x)
#define bl_sysclock_to_usec_ceil(x) bl_sysclock32_to_usec_ceil (x)
#define bl_sysclock_to_nsec_ceil(x) bl_sysclock32_to_nsec_ceil (x)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_sysclock(x)  bl_sec_to_sysclock32 (x)
#define bl_msec_to_sysclock(x) bl_msec_to_sysclock32 (x)
#define bl_usec_to_sysclock(x) bl_usec_to_sysclock32 (x)
#define bl_nsec_to_sysclock(x) bl_nsec_to_sysclock32 (x)
/*---------------------------------------------------------------------------*/

#endif /*__BL_SYSCLOCK_FUNC_FROM_SYSCLOCK32_*/
