/*----------------------------------------------------------------------------*/
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/thread.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/processor_pause.h>

#include <bl/lib/nonblock/backoff.h>
/*----------------------------------------------------------------------------*/
void nonblock_backoff_init(
  nonblock_backoff* nb, 
  uword             spin_max,
  uword             yield_max, 
  i32               sleep_us_init,
  i32               sleep_us_mul,
  i32               sleep_us_div,
  i32               sleep_us_max
  )
{
  bl_assert (sleep_us_init > 0);
  bl_assert (sleep_us_mul >= 0);
  bl_assert (sleep_us_div >= 0);

  nb->spin = nb->yield = 0;

  nb->sleep_us     = sleep_us_init;
  nb->spin_max     = spin_max;
  nb->yield_max    = yield_max;
  nb->sleep_us_mul = sleep_us_mul;
  nb->sleep_us_div = sleep_us_div;
  nb->sleep_us_max = sleep_us_max;
}
/*----------------------------------------------------------------------------*/
void nonblock_backoff_init_default (nonblock_backoff* nb, i32 sleep_us_max)
{
   nonblock_backoff_init (nb, 40, 20, BL_SCHED_TMIN_US, 1, 3, sleep_us_max);
}
/*----------------------------------------------------------------------------*/
void nonblock_backoff_run (nonblock_backoff* nb)
{
  if (nb->spin < nb->spin_max) {
    ++nb->spin;
    for (uword i = 0; i < 8; ++i) {
      processor_pause();
    }
  }
  else if (nb->yield < nb->yield_max || nb->sleep_us_max < BL_SCHED_TMIN_US) {
    ++nb->yield;
    bl_thread_yield();
  }
  else {
    bl_thread_usleep (nb->sleep_us);
    i32 usleep   = nb->sleep_us;
    i64 add      = nb->sleep_us;
    add         *= nb->sleep_us_mul;
    add         /= nb->sleep_us_div;
    usleep      += (i32) add;
    usleep       = bl_max (nb->sleep_us, usleep);
    usleep       = bl_min (nb->sleep_us_max, usleep);
    nb->sleep_us = usleep;
  }
}
/*----------------------------------------------------------------------------*/

