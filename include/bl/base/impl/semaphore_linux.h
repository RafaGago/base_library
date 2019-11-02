#ifndef __BL_SEMAPHORE_LINUX_H__
#define __BL_SEMAPHORE_LINUX_H__

#include <bl/base/platform.h>
/*----------------------------------------------------------------------------*/
#ifndef BL_LINUX
  #error "this file shouldn't be included on this platform"
#endif
/*----------------------------------------------------------------------------*/
#include <semaphore.h>
#include <errno.h>
#include <bl/base/libexport.h>
#include <bl/base/atomic.h>
#include <bl/base/error.h>
#include <bl/base/integer.h>
#include <bl/base/time.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
typedef sem_t bl_sem;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_init (bl_sem* s)
{
  return sem_init (s, 0, 1) == 0 ? bl_mkok() : bl_mkerr_sys (bl_error, errno);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_destroy (bl_sem* s)
{
  return sem_destroy (s) == 0 ? bl_mkok() : bl_mkerr_sys (bl_error, errno);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_wait (bl_sem* s)
{
  if (sem_wait (s) == 0) {
    return bl_mkok();
  }
  else {
    return bl_mkerr_sys (errno == EINTR ? bl_interrupted : bl_error, errno);
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_signal (bl_sem* s)
{
  return sem_post (s) == 0 ? bl_mkok() : bl_mkerr_sys (bl_error, errno);
}
/*----------------------------------------------------------------------------*/
typedef struct bl_tm_sem {
  bl_atomic_u32 sem;
}
bl_tm_sem;
/*----------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_tm_sem_signal (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_tm_sem_wait (bl_tm_sem* s, bl_u32 usec);
/*----------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_tm_sem_init (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_tm_sem_destroy (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_SEMAPHORE_LINUX_H__ */

