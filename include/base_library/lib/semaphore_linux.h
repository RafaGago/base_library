#ifndef __BL_SEMAPHORE_LINUX_H__
#define __BL_SEMAPHORE_LINUX_H__

#include <base_library/hdr/platform.h>
/*----------------------------------------------------------------------------*/
#ifndef BL_LINUX
  #error "this file shouldn't be included on this platform"
#endif
/*----------------------------------------------------------------------------*/
#include <semaphore.h>
#include <errno.h>
#include <base_library/lib/atomic.h>
#include <base_library/hdr/error.h>
#include <base_library/hdr/integer.h>
#include <base_library/hdr/time.h>
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
#define bl_tm_sem_resolution_us 500 /*TODO just an informed guess */
/*----------------------------------------------------------------------------*/
typedef struct bl_tm_sem {
  atomic_u32 sem;
}
bl_tm_sem;
/*----------------------------------------------------------------------------*/
extern bl_err bl_tm_sem_signal (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/
extern bl_err bl_tm_sem_wait (bl_tm_sem* s, u32 usec);
/*----------------------------------------------------------------------------*/
extern bl_err bl_tm_sem_init (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/
extern bl_err bl_tm_sem_destroy (bl_tm_sem* s);
/*----------------------------------------------------------------------------*/
#endif /* __BL_SEMAPHORE_LINUX_H__ */

