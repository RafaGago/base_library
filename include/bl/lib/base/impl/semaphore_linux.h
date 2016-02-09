#ifndef __BL_SEMAPHORE_LINUX_H__
#define __BL_SEMAPHORE_LINUX_H__

#include <bl/hdr/base/platform.h>
/*----------------------------------------------------------------------------*/
#ifndef BL_LINUX
  #error "this file shouldn't be included on this platform"
#endif
/*----------------------------------------------------------------------------*/
#include <semaphore.h>
#include <errno.h>
#include <bl/hdr/base/libexport.h>
#include <bl/hdr/base/atomic.h>
#include <bl/hdr/base/error.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/time.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
typedef sem_t bl_sem;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_init (bl_sem* s)
{
  return sem_init (s, 0, 1) == 0 ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_destroy (bl_sem* s)
{
  return sem_destroy (s) == 0 ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_wait (bl_sem* s)
{
  if (sem_wait (s) == 0) {
    return bl_ok;
  }  
  else {
    errno == EINTR ? bl_interrupted : bl_error;
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_sem_signal (bl_sem* s)
{
  return sem_post (s) == 0 ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
typedef struct bl_tm_sem {
  atomic_u32 sem;
}
bl_tm_sem;
/*----------------------------------------------------------------------------*/
extern bl_err BL_EXPORT bl_tm_sem_signal (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/
extern bl_err BL_EXPORT bl_tm_sem_wait (bl_tm_sem* s, u32 usec);
/*----------------------------------------------------------------------------*/
extern bl_err BL_EXPORT bl_tm_sem_init (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/
extern bl_err BL_EXPORT bl_tm_sem_destroy (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_SEMAPHORE_LINUX_H__ */

