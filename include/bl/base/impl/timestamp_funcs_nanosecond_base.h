#ifndef __BL_TIMESTAMP_FNS_NANOSECOND_BASE_H__
#define __BL_TIMESTAMP_FNS_NANOSECOND_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define BL_TIMESTAMP_HAS_NANOSECOND_BASE 1
/*---------------------------------------------------------------------------*/
#define bl_tstamp2sec_priv(ts)\
  ((toffset) (ts / nsec_in_sec))
 
#define bl_tstamp2msec_priv(ts)\
  ((toffset) (ts / nsec_in_msec))
 
#define bl_tstamp2usec_priv(ts)\
  ((toffset) (ts / nsec_in_usec))
 
#define bl_tstamp2nsec_priv(ts)\
  ((toffset) ts) 
/*---------------------------------------------------------------------------*/
#define bl_sec_to_tstamp_max()\
  bl_tstamp2sec_priv (tstamp_max_safe_add_sub)

#define bl_msec_to_tstamp_max()\
  bl_tstamp2msec_priv (tstamp_max_safe_add_sub)
  
#define bl_nsec_to_tstamp_max()\
  bl_tstamp2nsec_priv (tstamp_max_safe_add_sub)
  
#define bl_usec_to_tstamp_max()\
  bl_tstamp2usec_priv (tstamp_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec (tstamp ts)
{
  return bl_tstamp2sec_priv (ts);
}

static inline toffset bl_tstamp_to_msec (tstamp ts)
{
  return bl_tstamp2msec_priv (ts);
}

static inline toffset bl_tstamp_to_usec (tstamp ts)
{
  return bl_tstamp2usec_priv (ts);
}

static inline toffset bl_tstamp_to_nsec (tstamp ts)
{
  return bl_tstamp2nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec_ceil (tstamp ts)
{
  return (toffset) div_ceil (ts, nsec_in_sec);
}

static inline toffset bl_tstamp_to_msec_ceil (tstamp ts)
{
  return (toffset) div_ceil (ts, nsec_in_msec);
}

static inline toffset bl_tstamp_to_usec_ceil (tstamp ts)
{
  return (toffset) div_ceil (ts, nsec_in_usec);
}

static inline toffset bl_tstamp_to_nsec_ceil (tstamp ts)
{
  return (toffset) (ts);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_sec_to_tstamp (toffset sec)
{
  bl_assert (sec <= bl_sec_to_tstamp_max());
  return ((tstamp) sec) * nsec_in_sec;
}

static inline tstamp bl_msec_to_tstamp (toffset msec)
{
  bl_assert (msec <= bl_msec_to_tstamp_max());
  return ((tstamp) msec) * nsec_in_msec;
}

static inline tstamp bl_usec_to_tstamp (toffset usec)
{
  bl_assert (usec <= bl_usec_to_tstamp_max());
  return ((tstamp) usec) * nsec_in_usec;
}

static inline tstamp bl_nsec_to_tstamp (toffset nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp_max());
  return (tstamp) nsec;
}
/*---------------------------------------------------------------------------*/

#endif /*__TIMESTAMP_FNS_NANOSECOND_BASE_H__*/
