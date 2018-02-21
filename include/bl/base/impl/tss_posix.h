
#ifndef __BL_TSS_POSIX_H__
#define __BL_TSS_POSIX_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <bl/base/thread.h> instead"
#endif

#include <pthread.h>
#include <errno.h>
#include <bl/base/error.h>

#if defined (__cplusplus)
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
#define bl_thread_local __thread
#define bl_tss_dtor_callconv
typedef pthread_key_t bl_tss;
typedef void (bl_tss_dtor_callconv *bl_tss_cleanup_func)(void*);
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tss_init (bl_tss* key, bl_tss_cleanup_func cleanup)
{
  switch (pthread_key_create (key, cleanup)) {
  case 0:      return bl_mkok();
  case EAGAIN: return bl_mkerr (bl_busy);
  case ENOMEM: return bl_mkerr (bl_alloc);
  default:     return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/
static inline void bl_tss_destroy (bl_tss key)
{
   (void) pthread_key_delete (key);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tss_set (bl_tss key, void const* val)
{
  switch (pthread_setspecific (key, val)) {
  case 0:      return bl_mkok();
  case ENOMEM: return bl_mkerr (bl_alloc);
  case EINVAL: return bl_mkerr (bl_invalid);
  default:     return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/
static inline void* bl_tss_get (bl_tss key)
{
  return pthread_getspecific (key);
}
/*----------------------------------------------------------------------------*/

#if defined (__cplusplus)
} /* extern "C" { */
#endif

#endif /* __BL_TSS_POSIX_H__ */