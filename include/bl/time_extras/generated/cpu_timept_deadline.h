/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/deadline.gen.h -r BITS=32,CLOCKNAME=timept32,INCLUDE_HDR=base/time.h,HDR_PREFIX=base/impl/generated/time -r BITS=64,CLOCKNAME=timept64,INCLUDE_HDR=base/time.h,HDR_PREFIX=base/impl/generated/time -r BITS=32,CLOCKNAME=sysclock32,INCLUDE_HDR=base/time.h,HDR_PREFIX=base/impl/generated/time -r BITS=64,CLOCKNAME=sysclock64,INCLUDE_HDR=base/time.h,HDR_PREFIX=base/impl/generated/time -r BITS=64,CLOCKNAME=cpu_timept,INCLUDE_HDR=time_extras/time_extras.h,HDR_PREFIX=time_extras/generated

Don't edit.
*/
/* AUTOGENERATE: include/bl/time_extras/generated/cpu_timept_deadline.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_DEADLINE_CPU_TIMEPT_H__
#define __BL_DEADLINE_CPU_TIMEPT_H__

/* deadline/timeout usage pattern coded in a correct way */

#include <bl/time_extras/time_extras.h>
#include <bl/base/assert.h>
#include <bl/base/error.h>

/*---------------------------------------------------------------------------*/
/* deadline cpu_timept */
/*---------------------------------------------------------------------------*/
#if BL_CPU_TIMEPT_BASE <= BL_NANOSECOND_BASE

static inline bl_err bl_cpu_timept_deadline_init_nsec_explicit(
  bl_timept64* d, bl_timept64 now, bl_timeoft64 nsec
  )
{
  bl_assert (d);
  bl_assert (nsec <= bl_nsec_to_cpu_timept_max());
  if (nsec <= bl_nsec_to_cpu_timept_max()) {
    *d = now + bl_nsec_to_cpu_timept (nsec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
static inline bl_err
  bl_cpu_timept_deadline_init_nsec (bl_timept64* d, bl_timeoft64 nsec)
{
  return bl_cpu_timept_deadline_init_nsec_explicit(
    d, bl_cpu_timept_get(), nsec
    );
}
#endif
/*---------------------------------------------------------------------------*/
#if BL_CPU_TIMEPT_BASE <= BL_MICROSECOND_BASE

static inline bl_err bl_cpu_timept_deadline_init_usec_explicit(
  bl_timept64* d, bl_timept64 now, bl_timeoft64 usec
  )
{
  bl_assert (d);
  bl_assert (usec <= bl_nsec_to_cpu_timept_max());
  if (usec <= bl_usec_to_cpu_timept_max()) {
    *d = now + bl_usec_to_cpu_timept (usec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}

static inline bl_err
  bl_cpu_timept_deadline_init_usec (bl_timept64* d, bl_timeoft64 usec)
{
  return bl_cpu_timept_deadline_init_usec_explicit(
    d, bl_cpu_timept_get(), usec
    );
}
#endif
/*---------------------------------------------------------------------------*/
#if BL_CPU_TIMEPT_BASE <= BL_MILLISECOND_BASE

static inline bl_err bl_cpu_timept_deadline_init_msec_explicit(
  bl_timept64* d, bl_timept64 now, bl_timeoft64 msec
  )
{
  bl_assert (d);
  bl_assert (msec <= bl_msec_to_cpu_timept_max());
  if (msec <= bl_msec_to_cpu_timept_max()) {
    *d = now + bl_msec_to_cpu_timept (msec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
static inline bl_err
  bl_cpu_timept_deadline_init_msec (bl_timept64* d, bl_timeoft64 msec)
{
  return bl_cpu_timept_deadline_init_msec_explicit(
    d, bl_cpu_timept_get(), msec
    );
}
#endif
/*---------------------------------------------------------------------------*/
#if BL_CPU_TIMEPT_BASE <= BL_SECOND_BASE

static inline bl_err bl_cpu_timept_deadline_init_sec_explicit(
  bl_timept64* d, bl_timept64 now, bl_timeoft64 sec
  )
{
  bl_assert (d);
  bl_assert (sec <= bl_sec_to_cpu_timept_max());
  if (sec <= bl_sec_to_cpu_timept_max()) {
    *d = now + bl_sec_to_cpu_timept (sec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}

static inline bl_err
  bl_cpu_timept_deadline_init_sec (bl_timept64* d, bl_timeoft64 sec)
{
  return bl_cpu_timept_deadline_init_sec_explicit(
    d, bl_cpu_timept_get(), sec
    );
}
#endif
/*---------------------------------------------------------------------------*/
static inline bl_timept64diff bl_cpu_timept_deadline_compare(
  bl_timept64 a, bl_timept64 b
  )
{
  return bl_timept64_get_diff (a, b);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64
  bl_cpu_timept_deadline_min (bl_timept64 a, bl_timept64 b)
{
  return bl_cpu_timept_deadline_compare (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64
  bl_cpu_timept_deadline_max (bl_timept64 a, bl_timept64 b)
{
  return bl_cpu_timept_deadline_compare (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_cpu_timept_deadline_expired_explicit(
  bl_timept64 d, bl_timept64 now
  )
{
  return bl_cpu_timept_deadline_compare (d, now) <= 0;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_cpu_timept_deadline_expired (bl_timept64 d)
{
  return bl_cpu_timept_deadline_expired_explicit (d, bl_cpu_timept_get());
}
/*---------------------------------------------------------------------------*/

#endif /* #ifndef __BL_DEADLINE_CPU_TIMEPT_H__ */