#ifndef __POSIX_THREAD_H__
#define __POSIX_THREAD_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <bl/base/thread.h> instead"
#endif

#include <pthread.h>

#include <errno.h>
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/error.h>

typedef pthread_t bl_thread;
typedef int (*bl_thread_func) (void* context);
/*---------------------------------------------------------------------------*/
static inline bl_err bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  )
{
  bl_assert (t);
  return pthread_create (t, 0, (void* (*) (void*)) f, context) == 0 ?
    bl_ok : bl_error;
}
/*---------------------------------------------------------------------------*/
static inline void bl_thread_yield (void)
{
  sched_yield();
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_thread_join (bl_thread* t)
{
  bl_assert (t);
  return pthread_join (*t, nullptr) == 0 ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
static inline bl_thread bl_thread_native_handle (bl_thread t)
{
  return t;
}
/*----------------------------------------------------------------------------*/

#include <bl/base/impl/tss_posix.h>

#endif /* __POSIX_THREAD_H__ */

