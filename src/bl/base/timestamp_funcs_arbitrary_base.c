#include <bl/base/time.h>

/*this file will be included from other files that have the missing defines*/
#if defined BL_TIMESTAMP_HAS_ARBITRARY_BASE

#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#if !defined (BL_TIMESTAMP_64BIT)
/*---------------------------------------------------------------------------*/
BL_EXPORT toffset bl_tstamp_to_private (tstamp ts, u32 factor)
{
  u64 f = bl_tstamp_get_freq();
  return (toffset) ((((u64) ts) * factor) / f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT toffset bl_tstamp_to_ceil_private (tstamp ts, u32 factor)
{
  u64 f = bl_tstamp_get_freq();
  return (toffset) div_ceil (((u64) ts * factor), f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT tstamp bl_to_tstamp_private (toffset amt, u32 factor)
{
  return (tstamp) div_ceil ((u64) amt * bl_tstamp_get_freq(), factor);
}
/*---------------------------------------------------------------------------*/
#else /*BL_TIMESTAMP_64BIT*/
/*---------------------------------------------------------------------------*/
BL_EXPORT toffset bl_tstamp_to_private (tstamp ts, u32 factor)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f;
  return (toffset) (sec * factor) + ((rem * factor) / f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT toffset bl_tstamp_to_ceil_private (tstamp ts, u32 factor)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f;
  return (toffset) (sec * factor) + div_ceil (rem * factor, f);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT tstamp bl_to_tstamp_private (toffset amt, u32 factor)
{
  u64      f   = bl_tstamp_get_freq();
  toffset  sec = amt / factor;
  toffset  rem = amt % factor;
  return (sec * f) + div_ceil (rem * f, factor);
}
/*---------------------------------------------------------------------------*/
#endif /*BL_64BIT_TIMESTAMP*/
#endif /*BL_TIMESTAMP_HAS_ARBITRARY_BASE*/
