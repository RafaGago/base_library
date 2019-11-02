#include <bl/base/time.h>

/*this file will be included from other files that have the missing defines*/
#if defined BL_TIMESTAMP_HAS_ARBITRARY_BASE

#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#if !defined (BL_TIMESTAMP_64BIT)
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_toffset bl_tstamp_to_private (bl_tstamp ts, bl_u32 factor)
{
  bl_u64 f = bl_tstamp_get_freq();
  return (bl_toffset) ((((bl_u64) ts) * factor) / f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_toffset bl_tstamp_to_ceil_private (bl_tstamp ts, bl_u32 factor)
{
  bl_u64 f = bl_tstamp_get_freq();
  return (bl_toffset) bl_div_ceil (((bl_u64) ts * factor), f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_tstamp bl_to_tstamp_private (bl_toffset amt, bl_u32 factor)
{
  return (bl_tstamp) bl_div_ceil ((bl_u64) amt * bl_tstamp_get_freq(), factor);
}
/*---------------------------------------------------------------------------*/
#else /*BL_TIMESTAMP_64BIT*/
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_toffset bl_tstamp_to_private (bl_tstamp ts, bl_u32 factor)
{
  bl_u64 f   = bl_tstamp_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_toffset) (sec * factor) + ((rem * factor) / f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_toffset bl_tstamp_to_ceil_private (bl_tstamp ts, bl_u32 factor)
{
  bl_u64 f   = bl_tstamp_get_freq();
  bl_u64 sec = ts / f;
  bl_u64 rem = ts % f;
  return (bl_toffset) (sec * factor) + bl_div_ceil (rem * factor, f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_tstamp bl_to_tstamp_private (bl_toffset amt, bl_u32 factor)
{
  bl_u64      f   = bl_tstamp_get_freq();
  bl_toffset  sec = amt / factor;
  bl_toffset  rem = amt % factor;
  return (sec * f) + bl_div_ceil (rem * f, factor);
}
/*---------------------------------------------------------------------------*/
#endif /*BL_64BIT_TIMESTAMP*/
#endif /*BL_TIMESTAMP_HAS_ARBITRARY_BASE*/
