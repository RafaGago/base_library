/*----------------------------------------------------------------------------*/
#include <bl/base/platform.h>
#include <bl/base/thread.h>
#include <bl/base/utility.h>
#include <bl/base/processor_pause.h>

#include <bl/nonblock/backoff.h>

#if BL_OS_IS (LINUX)
#include <time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
void bl_nonblock_backoff_init(
  bl_nonblock_backoff* nb,
  bl_uword             spin_max,
  bl_uword             yield_max,
  bl_timeoft           sleep_us_init,
  bl_timeoft           sleep_mul,
  bl_timeoft           sleep_div,
  bl_timeoft           sleep_us_max
  )
{
  bl_assert (sleep_us_init > 0);
  bl_assert (sleep_div >= 0);

  nb->spin = nb->yield = 0;

  nb->sleep_ns     = sleep_us_init * 1000;
  nb->spin_max     = spin_max;
  nb->yield_max    = yield_max;
  nb->sleep_mul    = sleep_mul;
  nb->sleep_div    = sleep_div;
  nb->sleep_ns_max = sleep_us_max * 1000;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
void bl_nonblock_backoff_init_default (bl_nonblock_backoff* nb, bl_timeoft sleep_us_max)
{
   bl_nonblock_backoff_init (nb, 40, 20, BL_SCHED_TMIN_US, 1, 3, sleep_us_max);
}
/*----------------------------------------------------------------------------*/
static bl_timeoft nonblock_get_next_sleep_ns (bl_nonblock_backoff* nb)
{
  bl_timeoft nsleep = nb->sleep_ns;
  bl_i64 add        = nsleep * 2048;
  add           *= nb->sleep_mul;
  add           /= nb->sleep_div;
  nsleep        += (bl_timeoft) add / 2048;
  nsleep         = bl_max (nb->sleep_ns, nsleep);
  nsleep         = bl_min (nb->sleep_ns_max, nsleep);
  return nsleep;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
bl_timeoft bl_nonblock_backoff_next_sleep_us (bl_nonblock_backoff* nb)
{
  if (nb->spin < nb->spin_max || nb->yield < nb->yield_max) {
    return 0;
  }
  return nonblock_get_next_sleep_ns (nb) / 1000;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT
void bl_nonblock_backoff_run (bl_nonblock_backoff* nb)
{
  if (nb->spin < nb->spin_max) {
    if (nb->spin < 6) {
      bl_processor_pause();
    }
    else if (nb->spin < 12) {
      bl_processor_pause();
      bl_processor_pause();
    }
    else {
      bl_processor_pause();
      bl_processor_pause();
      bl_processor_pause();
      bl_processor_pause();
    }
    ++nb->spin;
    return;
  }
  if (nb->yield < nb->yield_max || nb->sleep_ns_max / 1000 < BL_SCHED_TMIN_US) {
    ++nb->yield;
#if BL_OS_IS (LINUX)
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
  bl_thread_usleep (nb->sleep_ns / 1000);
  nb->sleep_ns = nonblock_get_next_sleep_ns (nb);
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
