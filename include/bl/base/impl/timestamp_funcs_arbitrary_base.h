#ifndef __BL_TIMESTAMP_FNS_ARBITRARY_BASE_H__
#define __BL_TIMESTAMP_FNS_ARBITRARY_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/libexport.h>

#define BL_TIMESTAMP_HAS_ARBITRARY_BASE 1
/*---------------------------------------------------------------------------*/
extern BL_EXPORT toffset bl_tstamp_to_private (tstamp ts, u32 factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT toffset bl_tstamp_to_ceil_private (tstamp ts, u32 factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT tstamp bl_to_tstamp_private (toffset amt, u32 factor)
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec (tstamp ts)
{
  return bl_tstamp_to_private (ts, 1);
}

static inline toffset bl_tstamp_to_msec (tstamp ts)
{
  return bl_tstamp_to_private (ts, msec_in_sec);
}

static inline toffset bl_tstamp_to_usec (tstamp ts)
{
  return bl_tstamp_to_private (ts, usec_in_sec);
}

static inline toffset bl_tstamp_to_nsec (tstamp ts)
{
  return bl_tstamp_to_private (ts, nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec_ceil (tstamp ts)
{
   return bl_tstamp_to_ceil_private (ts, 1);
}

static inline toffset bl_tstamp_to_msec_ceil (tstamp ts)
{
  return bl_tstamp_to_ceil_private (ts, msec_in_sec);
}

static inline toffset bl_tstamp_to_usec_ceil (tstamp ts)
{
  return bl_tstamp_to_ceil_private (ts, usec_in_sec);
}

static inline toffset bl_tstamp_to_nsec_ceil (tstamp ts)
{
  return bl_tstamp_to_ceil_private (ts, nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline toffset bl_sec_to_tstamp_max (void)
{
  return bl_tstamp_to_sec (tstamp_max_safe_add_sub);
}

static inline toffset bl_msec_to_tstamp_max (void)
{
  return bl_tstamp_to_msec (tstamp_max_safe_add_sub);
}

static inline toffset bl_usec_to_tstamp_max (void)
{
  return bl_tstamp_to_usec (tstamp_max_safe_add_sub);
}

static inline toffset bl_nsec_to_tstamp_max (void)
{
  return bl_tstamp_to_nsec (tstamp_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_sec_to_tstamp (toffset sec)
{
  bl_assert (sec <= bl_sec_to_tstamp_max ());
  return bl_to_tstamp_private (sec, 1);
}

static inline tstamp bl_msec_to_tstamp (toffset msec)
{
  bl_assert (msec <= bl_msec_to_tstamp_max ());
  return bl_to_tstamp_private (msec, msec_in_sec);
}

static inline tstamp bl_usec_to_tstamp (toffset usec)
{
  bl_assert (usec <= bl_usec_to_tstamp_max ());
  return bl_to_tstamp_private (usec, usec_in_sec);
}

static inline tstamp bl_nsec_to_tstamp (toffset nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp_max ());
  return bl_to_tstamp_private (nsec, nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
#endif /* __TIMESTAMP_FNS_ARBITRARY_BASE_H__ */
