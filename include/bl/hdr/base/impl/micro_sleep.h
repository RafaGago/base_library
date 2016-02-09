#ifndef __BL_MICRO_SLEEP_IMPL_H__
#define __BL_MICRO_SLEEP_IMPL_H__

#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/time.h>
#include <bl/hdr/base/deadline.h>

/*----------------------------------------------------------------------------*/
#if defined (__cplusplus)
#include <chrono>
#include <thread>
extern "C" {
static inline void bl_thread_usleep (toffset us)
{
  if (us <= 0) { return; }
  std::this_thread::sleep_for (std::chrono::microseconds (us));
}
} /*extern "C" { */
/*----------------------------------------------------------------------------*/
#elif defined (BL_POSIX)
#include <errno.h>
/*TODO: proper detection, not all versions of POSIX support this*/
static void bl_thread_usleep (toffset us)
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
/*
elif defined (BL_WINDOWS)
#include <bl/hdr/base/include_windows.h>
static void bl_thread_usleep (toffset us)
{
  if (us < 0) { return; }
  bl_assert (us > 0);
  tstamp deadline;
  side_effect_assert (deadline_init (&deadline, us) == bl_ok);
  uword millis = us / usec_in_msec;
  if (millis) {
    Sleep (millis);
  }
  while (!deadline_expired (deadline)) {
    Sleep (0);
  }
}
*/
/*----------------------------------------------------------------------------*/
#else
 #error "bl_nano_sleep_impl unimplemented"
#endif
/*----------------------------------------------------------------------------*/
#endif /* __BL_NANO_SLEEP_IMPL_H__ */

