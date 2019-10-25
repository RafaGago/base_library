#ifndef __BL_TIME_EXTRAS_H__
#define __BL_TIME_EXTRAS_H__

#include <bl/time_extras/libexport.h>

#include <bl/base/error.h>
#include <bl/base/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
  A library for more uncommon time-related functions.
  This library is stateful but thread safe.
  ---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Initializes this library. "bl_time_extras_init" has to be called at least
   once. Only the first call will do actual work.
*/
extern BL_TIME_EXTRAS_EXPORT bl_err bl_time_extras_init (void);
/*----------------------------------------------------------------------------*/
/* Destructs this library. If multiple calls were made to" bl_extras_init", as
many calls need to be done to "bl_time_extras_destroy" and the destruction will
be done by the last caller.
*/
extern BL_TIME_EXTRAS_EXPORT void bl_time_extras_destroy (void);
/*----------------------------------------------------------------------------*/
/* Returns the difference in nanoseconds between the "bl_timestamp" monotonic
   clock vs the calendar clock "bl_sysclock". This is useful to convert
   monotonic to calendar time. */
extern BL_TIME_EXTRAS_EXPORT toffset bl_tstamp_to_sysclock_diff_ns (void);
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /*__BL_TIME_EXTRAS_LIBEXPORT_H__*/
