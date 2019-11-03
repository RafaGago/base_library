#ifndef __BL_NONBLOCK_BACKOFF_H__
#define __BL_NONBLOCK_BACKOFF_H__

/*----------------------------------------------------------------------------*/
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/time.h>

#include <bl/nonblock/libexport.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
typedef struct bl_nonblock_backoff {
  bl_uword   spin;
  bl_uword   spin_max;
  bl_uword   yield;
  bl_uword   yield_max;
  bl_timeoft sleep_ns;
  bl_timeoft sleep_mul;
  bl_timeoft sleep_div;
  bl_timeoft sleep_ns_max;
}
bl_nonblock_backoff;
/*------------------------------------------------------------------------------
spin_max:  number of spinning loops
yield_max: number of yield loops
sleep: sleep uses a simple incrementing polynomial:

  sleep_now = sleep_now + ((sleep_now * sleep_mul) / sleep_div)

Where "sleep_ns_init" is the intitial value of "sleep_now" and "sleep_us_max"
is the maximum value that sleep_now can take.

If one wishes to have constant delays "sleep_mul" has to be set to 0.

------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_nonblock_backoff_init(
    bl_nonblock_backoff* nb,
    bl_uword             spin_max,
    bl_uword             yield_max,
    bl_timeoft           sleep_us_init,
    bl_timeoft           sleep_mul,
    bl_timeoft           sleep_div,
    bl_timeoft           sleep_us_max
    );
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_nonblock_backoff_init_default(
    bl_nonblock_backoff* nb, bl_timeoft sleep_us_max
    );
/*------------------------------------------------------------------------------
Can be used to determine if a thread has reached the idle state
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_timeoft bl_nonblock_backoff_next_sleep_us (bl_nonblock_backoff* nb);
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_nonblock_backoff_run (bl_nonblock_backoff* nb);
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCK_BACKOFF_H__ */

