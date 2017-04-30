/*----------------------------------------------------------------------------*/
#include <bl/base/platform.h>
#include <bl/base/thread.h>
#include <bl/base/utility.h>
#include <bl/base/processor_pause.h>

#include <bl/nonblock/backoff.h>

#ifdef BL_LINUX
#include <time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
void nonblock_backoff_init(
  nonblock_backoff* nb,
  uword             spin_max,
  uword             yield_max,
  toffset           sleep_us_init,
  toffset           sleep_mul,
  toffset           sleep_div,
  toffset           sleep_us_max
  )
{
  bl_assert (sleep_us_init > 0);
  bl_assert (sleep_div >= 0);

  nb->spin = nb->yield = 0;

  nb->sleep_us     = sleep_us_init;
  nb->spin_max     = spin_max;
  nb->yield_max    = yield_max;
  nb->sleep_mul    = sleep_mul;
  nb->sleep_div    = sleep_div;
  nb->sleep_us_max = sleep_us_max;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
void nonblock_backoff_init_default (nonblock_backoff* nb, toffset sleep_us_max)
{
   nonblock_backoff_init (nb, 40, 20, BL_SCHED_TMIN_US, 1, 3, sleep_us_max);
}
/*----------------------------------------------------------------------------*/
static toffset nonblock_get_next_sleep (nonblock_backoff* nb)
{
  toffset usleep = nb->sleep_us;
  i64 add        = nb->sleep_us * 2048;
  add           *= nb->sleep_mul;
  add           /= nb->sleep_div;
  usleep        += (toffset) add / 2048;
  usleep         = bl_max (nb->sleep_us, usleep);
  usleep         = bl_min (nb->sleep_us_max, usleep);
  return usleep;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
toffset nonblock_backoff_next_sleep_us (nonblock_backoff* nb)
{
  if (nb->spin < nb->spin_max || nb->yield < nb->yield_max) {
    return 0;
  }
  return nonblock_get_next_sleep (nb);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
void nonblock_backoff_run (nonblock_backoff* nb)
{
  if (nb->spin < nb->spin_max) {
    if (nb->spin < 6) {
      processor_pause();
    }
    else if (nb->spin < 12) {
      processor_pause();
      processor_pause();
    }
    else {
      processor_pause();
      processor_pause();
      processor_pause();
      processor_pause();
    }
    ++nb->spin;
    return;
  }
  if (nb->yield < nb->yield_max || nb->sleep_us_max / 1000 < BL_SCHED_TMIN_US) {
    ++nb->yield;
#ifdef BL_LINUX
    /*Short sleep to avoid a potential sched_yield call to return immediately*/
    struct timespec t, rem;
    t.tv_sec  = 0;
    t.tv_nsec = 300;
    (void) nanosleep (&t, &rem);
#else
    /*On other OS's just yield. If yielding returns immediatelly just wait for
      the full sleep section*/
    bl_thread_yield();
#endif
    return;
  }
  bl_thread_usleep (nb->sleep_us);
  nb->sleep_us = nonblock_get_next_sleep (nb);
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
