#ifndef __BL_SEMAPHORE_WIN_H__
#define __BL_SEMAPHORE_WIN_H__

#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/error.h>
#include <bl/hdr/base/deadline.h>
#include <bl/hdr/base/include_windows.h>
#include <bl/lib/base/semaphore.h>
/*----------------------------------------------------------------------------*/
typedef HANDLE bl_sem;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_init (bl_sem* s)
{
  *s = CreateSemaphore (nullptr, 0, MAXLONG, nullptr);
  return *s ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_destroy (bl_sem* s)
{
  return CloseHandle (*s) ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_wait (bl_sem* s)
{
  return WaitForSingleObject (*s, INFINITE) ? bl_error : bl_ok;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_signal (bl_sem* s)
{
  return ReleaseSemaphore (*s, 1, NULL) ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
typedef bl_sem bl_tm_sem;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tm_sem_init (bl_tm_sem* s)
{
  return bl_sem_init (s);
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
  default:           return bl_error;    
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tm_sem_signal (bl_sem* s)
{
  return bl_sem_signal (s);
}
/*----------------------------------------------------------------------------*/

#endif /* __BL_SEMAPHORE_WIN_H__ */

