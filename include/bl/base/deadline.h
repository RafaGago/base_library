#ifndef __BL_DEADLINE_H__
#define __BL_DEADLINE_H__

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/error.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/time.h>

/* some convenience functions for bl_tstamp */
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline_init_explicit(
  bl_tstamp* d, bl_tstamp now, bl_toffset usec
  )
{
  bl_assert (d);
  if (usec <= bl_usec_to_bl_tstamp_max ()) {
    *d = now + bl_usec_to_bl_tstamp (usec);
    return bl_mkok();
  }
  else {
    *d = 0;
    return bl_mkerr (bl_invalid);
  }
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_deadline_init (bl_tstamp* d, bl_toffset usec)
{
  return bl_deadline_init_explicit (d, bl_get_tstamp(), usec);
}
/*---------------------------------------------------------------------------*/
static inline bl_tstampdiff bl_deadline_compare (bl_tstamp a, bl_tstamp b)
{
  return bl_tstamp_get_diff (a, b);
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_deadline_min (bl_tstamp a, bl_tstamp b)
{
  return bl_deadline_compare (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_deadline_max (bl_tstamp a, bl_tstamp b)
{
  return bl_deadline_compare (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_deadline_expired_explicit (bl_tstamp d, bl_tstamp now)
{
  return bl_deadline_compare (d, now) <= 0;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_deadline_expired (bl_tstamp d)
{
  return bl_deadline_expired_explicit (d, bl_get_tstamp());
}
/*---------------------------------------------------------------------------*/

#endif /* __BL_DEADLINE_H__ */
