#ifndef __BL_C11_THREAD_H__
#define __BL_C11_THREAD_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <hdr/thread.h> instead"
#endif

#include <threads.h>
#include <bl/base/integer.h>
#include <bl/base/assert.h>
#include <bl/base/platform.h>

typedef thrd_t       bl_thread;
typedef thrd_start_t bl_thread_func;
/*----------------------------------------------------------------------------*/
#define bl_thread_success thrd_success
#define bl_thread_nomem   thrd_timedout
#define bl_thread_busy    thrd_busy
#define bl_thread_error   thrd_error
/*----------------------------------------------------------------------------*/
static inline int bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  )
{
  bl_assert (t);
  return thrd_create (t, f, context);
}
/*----------------------------------------------------------------------------*/
static inline void bl_thread_yield (void)
{
  return thrd_yield();
}
/*----------------------------------------------------------------------------*/
static inline int bl_thread_join (bl_thread* t)
{
  bl_assert (t);
  return thrd_join (t, nullptr);
}
/*----------------------------------------------------------------------------*/
#define bl_thread_local thread_local
typedef tss_t      bl_tss;
typedef tss_dtor_t bl_tss_dtor;
#define            bl_tss_dtor_callconv
/*----------------------------------------------------------------------------*/
static inline int bl_tss_init (bl_tss* key, bl_tss_cleanup_func cleanup)
{
  return tss_create (key, cleanup);
}
/*----------------------------------------------------------------------------*/
static inline void bl_tss_destroy (bl_tss key)
{
  tss_delete (key);
}
/*----------------------------------------------------------------------------*/
static inline int bl_tss_set (bl_tss key, void* val)
{
  return tss_set (key, val);
}
/*----------------------------------------------------------------------------*/
static inline void* bl_tss_get (bl_tss key)
{
  return tss_get (key);
}
/*----------------------------------------------------------------------------*/


#endif /* __BL_CC_THREAD_H__ */

