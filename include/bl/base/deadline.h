#ifndef __BL_DEADLINE_H__
#define __BL_DEADLINE_H__

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/error.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/time.h>

/* some convenience functions for bl_timept */

/*---------------------------------------------------------------------------*/
/* 32 bit */
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline32_init_explicit(
  bl_timept32* d, bl_timept32 now, bl_timeoft32 usec
  )
{
  bl_assert (d);
  if (usec <= bl_usec_to_timept32_max()) {
    *d = now + bl_usec_to_timept32 (usec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline32_init (bl_timept32* d, bl_timeoft32 usec)
{
  return bl_deadline32_init_explicit (d, bl_timept32_get(), usec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32diff bl_deadline32_compare(
  bl_timept32 a, bl_timept32 b
  )
{
  return bl_timept32_get_diff (a, b);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_deadline32_min (bl_timept32 a, bl_timept32 b)
{
  return bl_deadline32_compare (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_deadline32_max (bl_timept32 a, bl_timept32 b)
{
  return bl_deadline32_compare (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_deadline32_expired_explicit(
  bl_timept32 d, bl_timept32 now
  )
{
  return bl_deadline32_compare (d, now) <= 0;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_deadline32_expired (bl_timept32 d)
{
  return bl_deadline32_expired_explicit (d, bl_timept32_get());
}
/*---------------------------------------------------------------------------*/
/* 64 bit */
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline64_init_explicit(
  bl_timept64* d, bl_timept64 now, bl_timeoft64 usec
  )
{
  bl_assert (d);
  if (usec <= bl_usec_to_timept64_max()) {
    *d = now + bl_usec_to_timept64 (usec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline64_init_ns_explicit(
  bl_timept64* d, bl_timept64 now, bl_timeoft64 nsec
  )
{
  bl_assert (d);
  if (nsec <= bl_nsec_to_timept64_max ()) {
    *d = now + bl_nsec_to_timept64 (nsec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline64_init (bl_timept64* d, bl_timeoft64 usec)
{
  return bl_deadline64_init_explicit (d, bl_timept64_get(), usec);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline64_init_ns (bl_timept64* d, bl_timeoft64 nsec)
{
  return bl_deadline64_init_ns_explicit (d, bl_timept64_get(), nsec);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64diff bl_deadline64_compare(
  bl_timept64 a, bl_timept64 b
  )
{
  return bl_timept64_get_diff (a, b);
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_deadline64_min (bl_timept64 a, bl_timept64 b)
{
  return bl_deadline64_compare (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_deadline64_max (bl_timept64 a, bl_timept64 b)
{
  return bl_deadline64_compare (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_deadline64_expired_explicit(
  bl_timept64 d, bl_timept64 now
  )
{
  return bl_deadline64_compare (d, now) <= 0;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_deadline64_expired (bl_timept64 d)
{
  return bl_deadline64_expired_explicit (d, bl_timept64_get());
}
/*---------------------------------------------------------------------------*/
/* default */
/*---------------------------------------------------------------------------*/
#if BL_TIMEPOINT_BITS == 32

#define bl_deadline_init_explicit(x, y, z) bl_deadline32_init_explicit (x, y, z)
#define bl_deadline_init(x, y)             bl_deadline32_init (x, y)
#define bl_deadline_compare(x, y)          bl_deadline32_compare (x, y)
#define bl_deadline_min(x, y)              bl_deadline32_min (x, y)
#define bl_deadline_max(x, y)              bl_deadline32_max (x, y)
#define bl_deadline_expired_explicit(x, y) bl_deadline32_expired_explicit (x, y)
#define bl_deadline_expired(x)             bl_deadline32_expired (x)

#else

#define bl_deadline_init_explicit(x, y, z) bl_deadline64_init_explicit (x, y, z)
#define bl_deadline_init(x, y)             bl_deadline64_init (x, y)
#define bl_deadline_compare(x, y)          bl_deadline64_compare (x, y)
#define bl_deadline_min(x, y)              bl_deadline64_min (x, y)
#define bl_deadline_max(x, y)              bl_deadline64_max (x, y)
#define bl_deadline_expired_explicit(x, y) bl_deadline64_expired_explicit (x, y)
#define bl_deadline_expired(x)             bl_deadline64_expired (x)

#endif

#endif /* __BL_DEADLINE_H__ */
