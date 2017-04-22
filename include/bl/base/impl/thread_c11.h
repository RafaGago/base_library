#ifndef __BL_C11_THREAD_H__
#define __BL_C11_THREAD_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <bl/base/thread.h> instead"
#endif

#include <threads.h>
#include <bl/base/integer.h>
#include <bl/base/assert.h>
#include <bl/base/error.h>
#include <bl/base/platform.h>

typedef thrd_t       bl_thread;
typedef thrd_start_t bl_thread_func;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_thread_error_convert (int err)
{
  switch (err) {
  case thrd_success:    return bl_ok;
  case thrd_timedout:   return bl_timeout;
  case thrd_busy:       return bl_busy;
  case thrd_nomem:      return bl_alloc;
  default:              return bl_error;
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  )
{
  bl_assert (t);
  return bl_thread_error_convert (thrd_create (t, f, context));
}
/*----------------------------------------------------------------------------*/
static inline void bl_thread_yield (void)
{
  thrd_yield();
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_thread_join (bl_thread* t)
{
  bl_assert (t);
  return bl_thread_error_convert (thrd_join (t, nullptr));
}
/*----------------------------------------------------------------------------*/
#define bl_thread_local thread_local
typedef tss_t      bl_tss;
typedef tss_dtor_t bl_tss_dtor;
#define            bl_tss_dtor_callconv
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tss_init (bl_tss* key, bl_tss_cleanup_func cleanup)
{
  return bl_thread_error_convert (tss_create (key, cleanup));
}
/*----------------------------------------------------------------------------*/
static inline void bl_tss_destroy (bl_tss key)
{
  tss_delete (key);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_tss_set (bl_tss key, void* val)
{
  return bl_thread_error_convert (tss_set (key, val));
}
/*----------------------------------------------------------------------------*/
static inline void* bl_tss_get (bl_tss key)
{
  return tss_get (key);
}
/*----------------------------------------------------------------------------*/


#endif /* __BL_CC_THREAD_H__ */

