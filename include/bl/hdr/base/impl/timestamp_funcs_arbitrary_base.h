#ifndef __BL_TIMESTAMP_FNS_ARBITRARY_BASE_H__
#define __BL_TIMESTAMP_FNS_ARBITRARY_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/hdr/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#if !defined (BL_TIMESTAMP_64BIT)
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec (tstamp ts)
{
  return (toffset) (((u64) ts) / bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_msec (tstamp ts)
{
  return (toffset) (((u64) ts * msec_in_sec) / bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_usec (tstamp ts)
{
  return (toffset) (((u64) ts * usec_in_sec) / bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_nsec (tstamp ts)
{
  return (toffset) (((u64) ts * nsec_in_sec) / bl_tstamp_get_freq());
}
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec_ceil (tstamp ts)
{
  return (toffset) div_ceil (((u64) ts), bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_msec_ceil (tstamp ts)
{
  return (toffset) div_ceil (((u64) ts * msec_in_sec), bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_usec_ceil (tstamp ts)
{
  return (toffset) div_ceil (((u64) ts * usec_in_sec), bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_nsec_ceil (tstamp ts)
{
  return (toffset) div_ceil (((u64) ts * nsec_in_sec), bl_tstamp_get_freq());
}
/*---------------------------------------------------------------------------*/
#else /*BL_TIMESTAMP_64BIT*/
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec (tstamp ts)
{
  return (toffset) (((u64) ts) / bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_msec (tstamp ts)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f;    
  return (toffset) (sec * msec_in_sec) + ((rem * msec_in_sec) / f);
}

static inline toffset bl_tstamp_to_usec (tstamp ts)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f;      
  return (toffset) (sec * usec_in_sec) + ((rem * usec_in_sec) / f);
}

static inline toffset bl_tstamp_to_nsec (tstamp ts)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f;  
  return (toffset) (sec * nsec_in_sec) + ((rem * nsec_in_sec) / f);
}
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_to_sec_ceil (tstamp ts)
{
  return (toffset) div_ceil (((u64) ts), bl_tstamp_get_freq());
}

static inline toffset bl_tstamp_to_msec_ceil (tstamp ts)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f;     
  return (toffset) (sec * msec_in_sec) + div_ceil (rem * msec_in_sec, f);  
}

static inline toffset bl_tstamp_to_usec_ceil (tstamp ts)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f;     
  return (toffset) (sec * usec_in_sec) + div_ceil (rem * usec_in_sec, f);
}

static inline toffset bl_tstamp_to_nsec_ceil (tstamp ts)
{
  u64 f   = bl_tstamp_get_freq();
  u64 sec = ts / f;
  u64 rem = ts % f; 
  return (toffset) (sec * nsec_in_sec) + div_ceil (rem * nsec_in_sec, f);
}
/*---------------------------------------------------------------------------*/
#endif /*BL_64BIT_TIMESTAMP*/
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
#if !defined (BL_TIMESTAMP_64BIT)
/*---------------------------------------------------------------------------*/
static inline tstamp bl_sec_to_tstamp (toffset sec)
{
  bl_assert (sec <= bl_sec_to_tstamp_max ());
  return (tstamp) (((tstamp) sec) * bl_tstamp_get_freq());
}

static inline tstamp bl_msec_to_tstamp (toffset msec)
{
  bl_assert (msec <= bl_msec_to_tstamp_max ());
  return (tstamp) div_ceil ((u64) msec * bl_tstamp_get_freq(), msec_in_sec);
}

static inline tstamp bl_usec_to_tstamp (toffset usec)
{
  bl_assert (usec <= bl_usec_to_tstamp_max ());
  return (tstamp) div_ceil ((u64) usec * bl_tstamp_get_freq(), usec_in_sec);
}

static inline tstamp bl_nsec_to_tstamp (toffset nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp_max ());
  return (tstamp) div_ceil ((u64) nsec * bl_tstamp_get_freq(), nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
#else /*BL_64BIT_TIMESTAMP*/
/*---------------------------------------------------------------------------*/
static inline tstamp bl_sec_to_tstamp (toffset sec)
{
  bl_assert (sec <= bl_sec_to_tstamp_max ());
  return (tstamp) (((tstamp) sec) * bl_tstamp_get_freq());
}

static inline tstamp bl_msec_to_tstamp (toffset msec)
{
  bl_assert (msec <= bl_msec_to_tstamp_max ());
  u64      f        = bl_tstamp_get_freq();
  toffset  sec      = msec / msec_in_sec;
  toffset  msec_rem = msec % msec_in_sec;
  return (f * sec) + div_ceil (msec_rem * f, msec_in_sec);
}

static inline tstamp bl_usec_to_tstamp (toffset usec)
{
  bl_assert (usec <= bl_usec_to_tstamp_max ());
  u64      f        = bl_tstamp_get_freq();
  toffset  sec      = usec / usec_in_sec;
  toffset  usec_rem = usec % usec_in_sec;
  return (f * sec) + div_ceil (usec_rem * f, usec_in_sec);
}

static inline tstamp bl_nsec_to_tstamp (toffset nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp_max ());
  u64      f        = bl_tstamp_get_freq();
  toffset  sec      = nsec / nsec_in_sec;
  toffset  nsec_rem = nsec % nsec_in_sec;
  return (f * sec) + div_ceil (nsec_rem * f, usec_in_sec);
}
/*---------------------------------------------------------------------------*/
#endif /*BL_64BIT_TIMESTAMP*/
#endif /* __TIMESTAMP_FNS_ARBITRARY_BASE_H__ */
