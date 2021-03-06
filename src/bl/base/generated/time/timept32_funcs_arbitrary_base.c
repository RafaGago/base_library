/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/timepoint_funcs_arbitrary_base.gen.h -r BITS=64,CLOCKNAME=timept64,EXPORT=BL_EXPORT,INCLUDE_HDR=base/libexport.h,INCLUDE_SRC=base/time.h,HDR_PREFIX=base/impl/generated/time,SRC_PREFIX=base/generated/time -r BITS=32,CLOCKNAME=timept32,EXPORT=BL_EXPORT,INCLUDE_HDR=base/libexport.h,INCLUDE_SRC=base/time.h,HDR_PREFIX=base/impl/generated/time,SRC_PREFIX=base/generated/time -r BITS=64,CLOCKNAME=cpu_timept,EXPORT=BL_TIME_EXTRAS_EXPORT,INCLUDE_HDR=time_extras/libexport.h,INCLUDE_SRC=time_extras/time_extras.h,HDR_PREFIX=time_extras/generated,SRC_PREFIX=time_extras/generated

Don't edit.
*/
/* AUTOGENERATE: src/bl/base/generated/time/timept32_funcs_arbitrary_base.c */
/*---------------------------------------------------------------------------*/
#include <bl/base/time.h>
#if BL_TIMEPT32_BASE == BL_ARBITRARY_BASE
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timeoft32
  bl_timept32_to_time_private (bl_timept32 ts, bl_u32 factor)
{
  bl_u64 f   = bl_timept32_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_timeoft32) (sec * factor) + ((rem * factor) / f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timeoft32
  bl_timept32_to_time_ceil_private (bl_timept32 ts, bl_u32 factor)
{
  bl_u64 f   = bl_timept32_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_timeoft32) (sec * factor) + bl_div_ceil (rem * factor, f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timept32
  bl_time_to_timept32_private (bl_timeoft32 amt, bl_u32 factor)
{
  bl_u64     f   = bl_timept32_get_freq();
  bl_timeoft sec = amt / factor;
  bl_timeoft rem = amt % factor;
  return (bl_timept32) (sec * f) + bl_div_ceil (rem * f, factor);
}
/*---------------------------------------------------------------------------*/
#endif /* #if BL_TIMEPT32_BASE == BL_ARBITRARY_BASE */
