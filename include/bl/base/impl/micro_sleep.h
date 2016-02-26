#ifndef __BL_MICRO_SLEEP_IMPL_H__
#define __BL_MICRO_SLEEP_IMPL_H__

#include <bl/base/integer.h>
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/time.h>
#include <bl/base/deadline.h>

/*----------------------------------------------------------------------------*/
#if defined (BL_WINDOWS)

#include <bl/base/static_integer_math.h>
#include <bl/base/include_windows.h>
#include <bl/base/processor_pause.h>
#include <TimeAPI.h>

#if defined (__cplusplus)
extern "C" {
#endif

static inline uword bl_thread_min_sleep_us (void)
{
  TIMECAPS tc;
  bl_assert_always (timeGetDevCaps (&tc, sizeof tc) == TIMERR_NOERROR);
  return (uword) (tc.wPeriodMin * usec_in_msec);
}

static void bl_thread_usleep (u32 us)
{
  if (us <= 0) { return; }
  uword minsleep_us = bl_thread_min_sleep_us();
  uword steps       = div_ceil (us, minsleep_us);
  
  u64 ticks_sec = qpc_get_freq();
  bl_assert_always (ticks_sec < pow2_ubig ((u64) 32));  
  u64 ticks = (ticks_sec * us) / usec_in_sec;
  
  LARGE_INTEGER start;
  bl_assert_side_effect (QueryPerformanceCounter (&start) != 0);
      
  Sleep ((minsleep_us * steps) / usec_in_msec);
  
  while (1) {
    LARGE_INTEGER now;
    bl_assert_side_effect (QueryPerformanceCounter (&now) != 0);
      
    u64 elapsed_ticks = now.QuadPart - start.QuadPart;      
       
    if (elapsed_ticks >= ticks) {
      break;
    }
    u64 remaining_us = ticks - elapsed_ticks;
    remaining_us     = div_ceil (remaining_us, ticks_sec);
    if (remaining_us <= BL_SCHED_TMIN_US) {
      for (uword i = 0; i < 16; ++i) {
        processor_pause();
      }
    }
    else {
      Sleep (0);
    }
  }
}

#if defined (__cplusplus)
} /*extern "C" { */
#endif
/*----------------------------------------------------------------------------*/
#elif defined (BL_POSIX)
#include <errno.h>

static inline uword bl_thread_min_sleep_us (void)
{
  return BL_SCHED_TMIN_US;
}

/*TODO: proper detection, not all versions of POSIX support this*/
static void bl_thread_usleep (u32 us)
{
  if (us <= 0) { return; }
  struct timespec t, remainder;
  int err;  
  t.tv_sec  = us / usec_in_sec;
  t.tv_nsec = (us - (t.tv_sec * usec_in_sec)) * nsec_in_usec;
  do {
    err = clock_nanosleep (CLOCK_MONOTONIC, 0, &t, &remainder);
    t   = remainder;
  }
  while (err == EINTR);
}
/*----------------------------------------------------------------------------*/
#else
 #error "bl_nano_sleep_impl unimplemented"
#endif
/*----------------------------------------------------------------------------*/
#endif /* __BL_NANO_SLEEP_IMPL_H__ */

