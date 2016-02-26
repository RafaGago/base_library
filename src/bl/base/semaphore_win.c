#include <bl/base/platform.h>

#ifdef BL_WINDOWS

#include <bl/base/semaphore.h>
#include <bl/base/time.h>
#include <bl/base/assert.h>
#include <bl/base/static_integer_math.h>
#include <bl/base/processor_pause.h>

/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
bl_err BL_EXPORT bl_tm_sem_wait (bl_sem* s, u32 usec)
{
  DWORD         ms;
  u64           f;
  LARGE_INTEGER deadline;
  
  if (usec != bl_tm_sem_infinity) {
    ms = div_ceil (usec, usec_in_msec);
    f  = qpc_get_freq();
    bl_assert_side_effect (QueryPerformanceCounter (&deadline) != 0);
    deadline.QuadPart += div_ceil (f * usec, usec_in_sec);
  }
  else {
    /*"INFINITE" is defined as 0xffffffff so "ms" can never be "INFINITE" from
    the other branch because it is divided by 1000*/
    ms = INFINITE;
  }
  
  switch (WaitForSingleObject (*s, ms)) {
  case 0: return bl_ok;
  case WAIT_TIMEOUT: {
    LARGE_INTEGER now;
    while (true) {
      bl_assert_side_effect (QueryPerformanceCounter (&now) != 0);
      if (now.QuadPart - deadline.QuadPart >= 0) {
        return bl_timeout;
      }
      /*an assert (false) has triggered here on release builds. It did it very
      infrequently but it unveiled  that "WaitForSingleObject" might have the 
      same behavior than "Sleep"; might return earlier than expected:
      
      https://msdn.microsoft.com/en-us/library/windows/desktop/ms686298(v=vs.85).aspx
      
      It well documented that the ms passed to "Sleep" is rounded to the floor
      of some internal resolution. I didn't think that this was the case with
      WaitForSingleObject, as MSDN doesn't document it:
      
      https://msdn.microsoft.com/en-us/library/windows/desktop/ms687032(v=vs.85).aspx
      
      It seems reasonable to think so given the assertion triggering and that
      both use the same task scheduler. It can be some issue with the QPC
      (QueryPerformanceCounter) instead, I don't know.
      
      From a practical point of view this busy-loop code is needed. The
      code calling a semaphore might have behavior that is dependent on this
      call not timing out early (from the QPC point of view).

      I find ugly to need to call QPC at least twice (might be two extra 
      Kernel calls depending on how is implemented).
      */
      for (uword i = 0; i < 16; ++i) {
        processor_pause();
        processor_pause();
        processor_pause();
        processor_pause();
      }
    }    
  } 
  default: return bl_error;    
  }
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* BL_WINDOWS */