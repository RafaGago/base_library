#include <bl/base/time.h>

#if defined BL_TIMEPOINT_HAS_ARBITRARY_BASE

#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timeoft32 bl_timept32_to_private (bl_timept32 ts, bl_u32 factor)
{
  bl_u64 f = bl_timept_get_freq();
  return (bl_timeoft32) ((((bl_u64) ts) * factor) / f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timeoft32 bl_timept32_to_ceil_private(
  bl_timept32 ts, bl_u32 factor
  )
{
  bl_u64 f = bl_timept_get_freq();
  return (bl_timeoft32) bl_div_ceil (((bl_u64) ts * factor), f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timept32 bl_to_timept32_private (bl_timeoft32 amt, bl_u32 factor)
{
  return (bl_timept32) bl_div_ceil(
    (bl_u64) amt * bl_timept32_get_freq(), factor
    );
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timeoft64 bl_timept64_to_private (bl_timept64 ts, bl_u32 factor)
{
  bl_u64 f   = bl_timept64_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_timeoft64) (sec * factor) + ((rem * factor) / f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timeoft64 bl_timept64_to_ceil_private(
  bl_timept64 ts, bl_u32 factor
  )
{
  bl_u64 f   = bl_timept64_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_timeoft) (sec * factor) + bl_div_ceil (rem * factor, f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_timept64 bl_to_timept64_private (bl_timeoft64 amt, bl_u32 factor)
{
  bl_u64      f   = bl_timept64_get_freq();
  bl_timeoft  sec = amt / factor;
  bl_timeoft  rem = amt % factor;
  return (sec * f) + bl_div_ceil (rem * f, factor);
}
/*---------------------------------------------------------------------------*/
#endif /*BL_TIMEPOINT_HAS_ARBITRARY_BASE*/
