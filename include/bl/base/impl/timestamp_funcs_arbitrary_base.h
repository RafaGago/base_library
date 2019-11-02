#ifndef __BL_TIMESTAMP_FNS_ARBITRARY_BASE_H__
#define __BL_TIMESTAMP_FNS_ARBITRARY_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/libexport.h>

#define BL_TIMESTAMP_HAS_ARBITRARY_BASE 1
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_toffset bl_tstamp_to_private(bl_tstamp ts, bl_u32 factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_toffset
  bl_tstamp_to_ceil_private (bl_tstamp ts, bl_u32 factor);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_tstamp
  bl_to_tstamp_private (bl_toffset amt, bl_u32 factor);
/*---------------------------------------------------------------------------*/
static inline bl_toffset bl_tstamp_to_sec (bl_tstamp ts)
{
  return bl_tstamp_to_private (ts, 1);
}

static inline bl_toffset bl_tstamp_to_msec (bl_tstamp ts)
{
  return bl_tstamp_to_private (ts, bl_msec_in_sec);
}

static inline bl_toffset bl_tstamp_to_usec (bl_tstamp ts)
{
  return bl_tstamp_to_private (ts, bl_usec_in_sec);
}

static inline bl_toffset bl_tstamp_to_nsec (bl_tstamp ts)
{
  return bl_tstamp_to_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_toffset bl_tstamp_to_sec_ceil (bl_tstamp ts)
{
   return bl_tstamp_to_ceil_private (ts, 1);
}

static inline bl_toffset bl_tstamp_to_msec_ceil (bl_tstamp ts)
{
  return bl_tstamp_to_ceil_private (ts, bl_msec_in_sec);
}

static inline bl_toffset bl_tstamp_to_usec_ceil (bl_tstamp ts)
{
  return bl_tstamp_to_ceil_private (ts, bl_usec_in_sec);
}

static inline bl_toffset bl_tstamp_to_nsec_ceil (bl_tstamp ts)
{
  return bl_tstamp_to_ceil_private (ts, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_toffset bl_sec_to_tstamp_max (void)
{
  return bl_tstamp_to_sec (bl_tstamp_max_safe_add_sub);
}

static inline bl_toffset bl_msec_to_tstamp_max (void)
{
  return bl_tstamp_to_msec (bl_tstamp_max_safe_add_sub);
}

static inline bl_toffset bl_usec_to_tstamp_max (void)
{
  return bl_tstamp_to_usec (bl_tstamp_max_safe_add_sub);
}

static inline bl_toffset bl_nsec_to_tstamp_max (void)
{
  return bl_tstamp_to_nsec (bl_tstamp_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_sec_to_tstamp (bl_toffset sec)
{
  bl_assert (sec <= bl_sec_to_tstamp_max ());
  return bl_to_tstamp_private (sec, 1);
}

static inline bl_tstamp bl_msec_to_tstamp (bl_toffset msec)
{
  bl_assert (msec <= bl_msec_to_tstamp_max ());
  return bl_to_tstamp_private (msec, bl_msec_in_sec);
}

static inline bl_tstamp bl_usec_to_tstamp (bl_toffset usec)
{
  bl_assert (usec <= bl_usec_to_tstamp_max ());
  return bl_to_tstamp_private (usec, bl_usec_in_sec);
}

static inline bl_tstamp bl_nsec_to_tstamp (bl_toffset nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp_max ());
  return bl_to_tstamp_private (nsec, bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
#endif /* __TIMESTAMP_FNS_ARBITRARY_BASE_H__ */
