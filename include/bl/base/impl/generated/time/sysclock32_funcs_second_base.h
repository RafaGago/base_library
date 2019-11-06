/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/timepoint_funcs_second_base.gen.h -r BITS=32,CLOCKNAME=sysclock32

Don't edit.
*/
/* AUTOGENERATE: include/bl/base/impl/generated/time/sysclock32_funcs_second_base.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_SYSCLOCK32_FNS_SECOND_BASE_H__
#define __BL_SYSCLOCK32_FNS_SECOND_BASE_H__

/* Common time conversions for the sysclock32 clock*/

#include <bl/base/static_integer_math.h>
#include <bl/base/assert.h>
/*---------------------------------------------------------------------------*/
#define BL_SYSCLOCK32_BASE BL_SECOND_BASE
#define BL_SYSCLOCK32_BITS 32
/*---------------------------------------------------------------------------*/
#define bl_sysclock32_to_sec_priv(ts)\
  ((bl_timeoft32) ts)

#define bl_sysclock32_to_msec_priv(ts)\
  (((bl_timeoft32) ts) * bl_msec_in_sec)

#define bl_sysclock32_to_usec_priv(ts)\
  (((bl_timeoft32) ts) * bl_usec_in_sec)

#define bl_sysclock32_to_nsec_priv(ts)\
  (((bl_timeoft32) ts) * bl_nsec_in_sec)
/*---------------------------------------------------------------------------*/
#define bl_sec_to_sysclock32_max()\
  bl_sysclock32_to_sec_priv (bl_timept32_max_safe_add_sub)

#define bl_msec_to_sysclock32_max()\
  bl_sysclock32_to_msec_priv (bl_timept32_max_safe_add_sub)

#define bl_nsec_to_sysclock32_max()\
  bl_sysclock32_to_nsec_priv (bl_timept32_max_safe_add_sub)

#define bl_usec_to_sysclock32_max()\
  bl_sysclock32_to_usec_priv (bl_timept32_max_safe_add_sub)
/*---------------------------------------------------------------------------*/
static inline bl_timeoft32 bl_sysclock32_to_sec (bl_timept32 ts)
{
  return bl_sysclock32_to_sec_priv (ts);
}

static inline bl_timeoft32 bl_sysclock32_to_msec (bl_timept32 ts)
{
  return bl_sysclock32_to_msec_priv (ts);
}

static inline bl_timeoft32 bl_sysclock32_to_usec (bl_timept32 ts)
{
  return bl_sysclock32_to_usec_priv (ts);
}

static inline bl_timeoft32 bl_sysclock32_to_nsec (bl_timept32 ts)
{
  return bl_sysclock32_to_nsec_priv (ts);
}
/*---------------------------------------------------------------------------*/
static inline bl_timeoft32 bl_sysclock32_to_sec_ceil (bl_timept32 ts)
{
  return (bl_timeoft32) ts;
}

static inline bl_timeoft32 bl_sysclock32_to_msec_ceil (bl_timept32 ts)
{
  return (bl_timeoft32) (ts * bl_msec_in_sec);
}

static inline bl_timeoft32 bl_sysclock32_to_usec_ceil (bl_timept32 ts)
{
  return (bl_timeoft32) (ts * bl_usec_in_sec);
}

static inline bl_timeoft32 bl_sysclock32_to_nsec_ceil (bl_timept32 ts)
{
  return (bl_timeoft32) (ts * bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_sec_to_sysclock32 (bl_timeoft32 sec)
{
  bl_assert (sec <= bl_sec_to_sysclock32_max());
  return ((bl_timept32) sec);
}

static inline bl_timept32 bl_msec_to_sysclock32 (bl_timeoft32 msec)
{
  bl_assert (msec <= bl_msec_to_sysclock32_max());
  return bl_div_ceil (((bl_timept32) msec), bl_msec_in_sec);
}

static inline bl_timept32 bl_usec_to_sysclock32 (bl_timeoft32 usec)
{
  bl_assert (usec <= bl_usec_to_sysclock32_max());
  return bl_div_ceil (((bl_timept32) usec), bl_usec_in_sec);
}

static inline bl_timept32 bl_nsec_to_sysclock32 (bl_timeoft32 nsec)
{
  bl_assert (nsec <= bl_nsec_to_sysclock32_max());
  return bl_div_ceil (((bl_timept32) nsec), bl_nsec_in_sec);
}
/*---------------------------------------------------------------------------*/

#endif /*#ifndef __BL_SYSCLOCK32_FNS_SECOND_BASE_H__*/
