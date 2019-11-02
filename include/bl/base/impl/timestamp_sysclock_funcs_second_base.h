#ifndef __BL_TIMESTAMP_SYSCLOCK_FNS_SECOND_BASE_H__
#define __BL_TIMESTAMP_SYSCLOCK_FNS_SECOND_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define BL_TIMESTAMP_SYSCLOCK_HAS_SECOND_BASE 1
/*---------------------------------------------------------------------------*/
#define bl_tstamp_sysclock2sec_priv(ts)\
  ((bl_toffset) ts)

#define bl_tstamp_sysclock2msec_priv(ts)\
  (((bl_toffset) ts) * bl_msec_in_sec)

#define bl_tstamp_sysclock2usec_priv(ts)\
  (((bl_toffset) ts) * bl_usec_in_sec)

#define bl_tstamp_sysclock2nsec_priv(ts)\
  (((bl_toffset) ts) * bl_nsec_in_sec)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_bl_tstamp_sysclock_max()\
  bl_tstamp_sysclock2sec_priv (bl_tstamp_max_safe_add_sub)

#define bl_msec_to_bl_tstamp_sysclock_max()\
  bl_tstamp_sysclock2msec_priv (bl_tstamp_max_safe_add_sub)

#define bl_nsec_to_bl_tstamp_sysclock_max()\
  bl_tstamp_sysclock2nsec_priv (bl_tstamp_max_safe_add_sub)

#define bl_usec_to_bl_tstamp_sysclock_max()\
  bl_tstamp_sysclock2usec_priv (bl_tstamp_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline bl_toffset bl_tstamp_sysclock_to_sec (bl_tstamp ts)
{
  return bl_tstamp_sysclock2sec_priv (ts);
}

static inline bl_toffset bl_tstamp_sysclock_to_msec (bl_tstamp ts)
{
  return bl_tstamp_sysclock2msec_priv (ts);
}

static inline bl_toffset bl_tstamp_sysclock_to_usec (bl_tstamp ts)
{
  return bl_tstamp_sysclock2usec_priv (ts);
}

static inline bl_toffset bl_tstamp_sysclock_to_nsec (bl_tstamp ts)
{
  return bl_tstamp_sysclock2nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_toffset bl_tstamp_sysclock_to_sec_ceil (bl_tstamp ts)
{
  return (bl_toffset) ts;
}

static inline bl_toffset bl_tstamp_sysclock_to_msec_ceil (bl_tstamp ts)
{
  return (bl_toffset) (ts * bl_msec_in_sec);
}

static inline bl_toffset bl_tstamp_sysclock_to_usec_ceil (bl_tstamp ts)
{
  return (bl_toffset) (ts * bl_usec_in_sec);
}

static inline bl_toffset bl_tstamp_sysclock_to_nsec_ceil (bl_tstamp ts)
{
  return (bl_toffset) (ts * bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_sec_to_bl_tstamp_sysclock (bl_toffset sec)
{
  bl_assert (sec <= bl_sec_to_bl_tstamp_sysclock_max());
  return ((bl_tstamp) sec);
}

static inline bl_tstamp bl_msec_to_bl_tstamp_sysclock (bl_toffset msec)
{
  bl_assert (msec <= bl_msec_to_bl_tstamp_sysclock_max());
  return bl_div_ceil (((bl_tstamp) msec), bl_msec_in_sec);
}

static inline bl_tstamp bl_usec_to_bl_tstamp_sysclock (bl_toffset usec)
{
  bl_assert (usec <= bl_usec_to_bl_tstamp_sysclock_max());
  return bl_div_ceil (((bl_tstamp) usec), bl_usec_in_sec);
}

static inline bl_tstamp bl_nsec_to_bl_tstamp_sysclock (bl_toffset nsec)
{
  bl_assert (nsec <= bl_nsec_to_bl_tstamp_sysclock_max());
  return bl_div_ceil (((bl_tstamp) nsec), bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/

#endif /*__TIMESTAMP_FNS_MICROSECOND_BASE_H__*/
