#ifndef __BL_SEMAPHORE_WIN_H__
#define __BL_SEMAPHORE_WIN_H__

#include <base_library/hdr/platform.h>
#include <base_library/hdr/integer.h>
#include <base_library/hdr/error.h>
#include <base_library/hdr/deadline.h>
#include <base_library/hdr/include_windows.h>
#include <base_library/lib/semaphore.h>
/*----------------------------------------------------------------------------*/
typedef HANDLE bl_sem;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_init (bl_sem* s)
{
  *s = CreateSemaphore (nullptr, 0, MAXLONG, nullptr);
  return *s ? bl_ok : bl_err;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_destroy (bl_sem* s)
{
  return CloseHandle (*s) ? bl_ok : bl_err;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_wait (bl_sem* s)
{
  return WaitForSingleObject (*s, INFINITE) ? bl_err : bl_ok;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_signal (bl_sem* s)
{
  return ReleaseSemaphore (*s, 1, NULL) ? bl_ok : bl_err;
}
/*----------------------------------------------------------------------------*/
typedef bl_sem bl_tm_sem;
/*----------------------------------------------------------------------------*/
#define bl_tm_sem_resolution_us 2000 /*TODO just an informed guess */
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tm_sem_init (bl_tm_sem* s)
{
  return bl_sem_create (s);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tm_sem_destroy (bl_tm_sem* s)
{
  return bl_sem_destroy (s);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tm_sem_wait (bl_sem* s, u32 usec)
{
  if (INFINITE != bl_tm_sem_infinity) {
    /*this is true always, leaving it as documentation*/
    if (unlikely (usec == bl_tm_sem_infinity)) {
    	usec = INFINITE;
    }
    else if (unlikely (usec == INFINITE)) {
      static_assert_ns (INFINITE - 1 != 0);
      --usec;
    }
  }
  switch (WaitForSingleObject (*s, usec / 1000)) {
  case 0:            return bl_ok;
  case WAIT_TIMEOUT: return bl_timeout;
  default:           return bl_err;    
  }
}
/*----------------------------------------------------------------------------*/

#endif /* __BL_SEMAPHORE_WIN_H__ */

