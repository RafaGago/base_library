#ifndef __BL_TIMESTAMP_SYSCLOCK_FNS_SECOND_BASE_H__
#define __BL_TIMESTAMP_SYSCLOCK_FNS_SECOND_BASE_H__

/*this file will be included from other files that have the missing defines*/
#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define BL_TIMESTAMP_SYSCLOCK_HAS_SECOND_BASE 1
/*---------------------------------------------------------------------------*/
#define bl_tstamp_sysclock2sec_priv(ts)\
  ((toffset) ts)

#define bl_tstamp_sysclock2msec_priv(ts)\
  (((toffset) ts) * msec_in_sec)

#define bl_tstamp_sysclock2usec_priv(ts)\
  (((toffset) ts) * usec_in_sec)

#define bl_tstamp_sysclock2nsec_priv(ts)\
  (((toffset) ts) * nsec_in_sec)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_tstamp_sysclock_max()\
  bl_tstamp_sysclock2sec_priv (tstamp_max_safe_add_sub)

#define bl_msec_to_tstamp_sysclock_max()\
  bl_tstamp_sysclock2msec_priv (tstamp_max_safe_add_sub)

#define bl_nsec_to_tstamp_sysclock_max()\
  bl_tstamp_sysclock2nsec_priv (tstamp_max_safe_add_sub)

#define bl_usec_to_tstamp_sysclock_max()\
  bl_tstamp_sysclock2usec_priv (tstamp_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_sysclock_to_sec (tstamp ts)
{
  return bl_tstamp_sysclock2sec_priv (ts);
}

static inline toffset bl_tstamp_sysclock_to_msec (tstamp ts)
{
  return bl_tstamp_sysclock2msec_priv (ts);
}

static inline toffset bl_tstamp_sysclock_to_usec (tstamp ts)
{
  return bl_tstamp_sysclock2usec_priv (ts);
}

static inline toffset bl_tstamp_sysclock_to_nsec (tstamp ts)
{
  return bl_tstamp_sysclock2nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline toffset bl_tstamp_sysclock_to_sec_ceil (tstamp ts)
{
  return (toffset) ts;
}

static inline toffset bl_tstamp_sysclock_to_msec_ceil (tstamp ts)
{
  return (toffset) (ts * msec_in_sec);
}

static inline toffset bl_tstamp_sysclock_to_usec_ceil (tstamp ts)
{
  return (toffset) (ts * usec_in_sec);
}

static inline toffset bl_tstamp_sysclock_to_nsec_ceil (tstamp ts)
{
  return (toffset) (ts * nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_sec_to_tstamp_sysclock (toffset sec)
{
  bl_assert (sec <= bl_sec_to_tstamp_sysclock_max());
  return ((tstamp) sec);
}

static inline tstamp bl_msec_to_tstamp_sysclock (toffset msec)
{
  bl_assert (msec <= bl_msec_to_tstamp_sysclock_max());
  return div_ceil (((tstamp) msec), msec_in_sec);
}

static inline tstamp bl_usec_to_tstamp_sysclock (toffset usec)
{
  bl_assert (usec <= bl_usec_to_tstamp_sysclock_max());
  return div_ceil (((tstamp) usec), usec_in_sec);
}

static inline tstamp bl_nsec_to_tstamp_sysclock (toffset nsec)
{
  bl_assert (nsec <= bl_nsec_to_tstamp_sysclock_max());
  return div_ceil (((tstamp) nsec), nsec_in_sec);
}
/*---------------------------------------------------------------------------*/

#endif /*__TIMESTAMP_FNS_MICROSECOND_BASE_H__*/
