#ifndef __POSIX_THREAD_H__
#define __POSIX_THREAD_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <hdr/thread.h> instead"
#endif

#include <assert.h>
#include <pthread.h>
#include <sched.h>
#include <bl/hdr/base/platform.h>

typedef pthread_t bl_thread;
typedef int (*bl_thread_func) (void* context);
/*---------------------------------------------------------------------------*/
#define bl_thread_success 0
#define bl_thread_nomem   1
#define bl_thread_busy    2
#define bl_thread_error   3
/*---------------------------------------------------------------------------*/
static inline int bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  )
{
  assert (t);
  return pthread_create (t, 0, (void* (*) (void*)) f, context) == 0 ? 
    bl_thread_success : bl_thread_error;
}
/*---------------------------------------------------------------------------*/
static inline void bl_thread_yield (void)
{
  sched_yield();
}
/*---------------------------------------------------------------------------*/
static inline int bl_thread_join (bl_thread* t)
{
  assert (t);
  return pthread_join (*t, nullptr) == 0 ? bl_thread_success : bl_thread_error;
}
/*---------------------------------------------------------------------------*/

#endif /* __POSIX_THREAD_H__ */

