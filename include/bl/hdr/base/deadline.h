#ifndef __BL_DEADLINE_H__
#define __BL_DEADLINE_H__

#include <assert.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/error.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/integer_math.h>
#include <bl/hdr/base/time.h>

/* some convenience functions for tstamp */
/*---------------------------------------------------------------------------*/
static inline bl_err deadline_init (tstamp* d, u32 usec)
{
  assert (d);
  assert (usec <= tstamp_safe_add_sub_max);
  *d = bl_get_tstamp();
  *d = bl_tstamp_add_usec (*d, usec);
  return (usec <= tstamp_safe_add_sub_max) ? bl_ok : bl_invalid;
}
/*---------------------------------------------------------------------------*/
static inline ststamp deadline_compare (tstamp a, tstamp b)
{
  return tstamp_compare (a, b);
}
/*---------------------------------------------------------------------------*/
static inline bool deadline_expired_explicit (tstamp d, tstamp now)
{
  return deadline_compare (now, d) >= 0;
}
/*---------------------------------------------------------------------------*/
static inline bool deadline_expired (tstamp d)
{
  return deadline_expired_explicit (d, bl_get_tstamp());
}
/*---------------------------------------------------------------------------*/

#endif /* __BL_DEADLINE_H__ */
