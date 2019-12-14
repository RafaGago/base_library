#include <bl/base/integer.h>
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/time.h>
#include <bl/base/deadline.h>
#include <bl/base/libexport.h>

/*----------------------------------------------------------------------------*/
#if BL_OS_IS (WINDOWS)

#include <bl/base/static_integer_math.h>
#include <bl/base/include_windows.h>
#include <bl/base/processor_pause.h>
#include <TimeAPI.h>

#if defined (__cplusplus)
extern "C" {
#endif

BL_EXPORT bl_uword bl_thread_min_sleep_us (void)
{
  TIMECAPS tc;
  bl_assert_always (timeGetDevCaps (&tc, sizeof tc) == TIMERR_NOERROR);
  return (bl_uword) (tc.wPeriodMin * bl_usec_in_msec);
}

BL_EXPORT void bl_thread_usleep (bl_u32 us)
{
  if (us <= 0) { return; }
  bl_uword minsleep_us = bl_thread_min_sleep_us();
  bl_uword steps       = bl_div_ceil (us, minsleep_us);

  bl_u64 ticks_sec = bl_qpc_get_freq();
  bl_assert_always (ticks_sec < bl_pow2_ubig ((bl_u64) 32));
  bl_u64 ticks = (ticks_sec * us) / bl_usec_in_sec;

  LARGE_INTEGER start;
  bl_assert_side_effect (QueryPerformanceCounter (&start) != 0);

  Sleep ((minsleep_us * steps) / bl_usec_in_msec);

  while (1) {
    LARGE_INTEGER now;
    bl_assert_side_effect (QueryPerformanceCounter (&now) != 0);

    bl_u64 elapsed_ticks = now.QuadPart - start.QuadPart;

    if (elapsed_ticks >= ticks) {
      break;
    }
    bl_u64 remaining_us = ticks - elapsed_ticks;
    remaining_us     = bl_div_ceil (remaining_us, ticks_sec);
    if (remaining_us <= BL_SCHED_TMIN_US) {
      for (bl_uword i = 0; i < 16; ++i) {
        bl_processor_pause();
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
#elif BL_OS_IS_MOSTLY_POSIX
#include <errno.h>

#if defined (__cplusplus)
extern "C" {
#endif

BL_EXPORT bl_uword bl_thread_min_sleep_us (void)
{
  return BL_SCHED_TMIN_US;
}

/*TODO: proper detection, not all versions of POSIX support this*/
BL_EXPORT void bl_thread_usleep (bl_u32 us)
{
  if (us <= 0) { return; }
  struct timespec t, remainder;
  int err;
  t.tv_sec  = us / bl_usec_in_sec;
  t.tv_nsec = (us - (t.tv_sec * bl_usec_in_sec)) * bl_nsec_in_usec;
  do {
    err = clock_nanosleep (CLOCK_MONOTONIC, 0, &t, &remainder);
    t   = remainder;
  }
  while (err == EINTR);
}

#if defined (__cplusplus)
} /*extern "C" { */
#endif

/*----------------------------------------------------------------------------*/
#else
 #error "bl_nano_sleep_impl unimplemented"
#endif
/*----------------------------------------------------------------------------*/

