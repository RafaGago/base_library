#ifndef __BL_SEMAPHORE_WIN_H__
#define __BL_SEMAPHORE_WIN_H__

#include <bl/base/platform.h>
#include <bl/base/libexport.h>
#include <bl/base/integer.h>
#include <bl/base/error.h>
#include <bl/base/include_windows.h>
#include <bl/base/semaphore.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
typedef HANDLE bl_sem;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_init (bl_sem* s)
{
  *s = CreateSemaphore (nullptr, 0, MAXLONG, nullptr);
  return *s ? bl_mkok() : bl_mkerr_sys (bl_error, errno);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_destroy (bl_sem* s)
{
  return CloseHandle (*s) ? bl_mkok() : bl_mkerr_sys (bl_error, errno);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_wait (bl_sem* s)
{
  return WaitForSingleObject (*s, INFINITE) ?
    bl_mkok() : bl_mkerr_sys (bl_error, errno);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_signal (bl_sem* s)
{
  return ReleaseSemaphore (*s, 1, NULL) ?
    bl_mkok() : bl_mkerr_sys (bl_error, errno);
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
extern BL_EXPORT bl_err bl_tm_sem_wait (bl_sem* s, u32 usec);
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tm_sem_signal (bl_sem* s)
{
  return bl_sem_signal (s);
}
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_SEMAPHORE_WIN_H__ */

