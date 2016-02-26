#ifndef __BL_DEADLINE_H__
#define __BL_DEADLINE_H__

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/error.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/time.h>

/* some convenience functions for tstamp */
/*---------------------------------------------------------------------------*/
static inline bl_err deadline_init (tstamp* d, toffset usec)
{
  bl_assert (d);
  if (usec <= bl_usec_to_tstamp_max ()) {
    *d = bl_get_tstamp() + bl_usec_to_tstamp (usec);
    return bl_ok;
  }
  else {
    return bl_invalid;
  }
}
/*---------------------------------------------------------------------------*/
static inline tstampdiff deadline_compare (tstamp a, tstamp b)
{
  return tstamp_get_diff (a, b);
}
/*---------------------------------------------------------------------------*/
static inline tstamp deadline_min (tstamp a, tstamp b)
{
  return deadline_compare (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline tstamp deadline_max (tstamp a, tstamp b)
{
  return deadline_compare (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bool deadline_expired_explicit (tstamp d, tstamp now)
{
  return deadline_compare (d, now) <= 0;
}
/*---------------------------------------------------------------------------*/
static inline bool deadline_expired (tstamp d)
{
  return deadline_expired_explicit (d, bl_get_tstamp());
}
/*---------------------------------------------------------------------------*/

#endif /* __BL_DEADLINE_H__ */
