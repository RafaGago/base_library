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
    bl_mkok() : bl_mkerr (bl_error);
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
  return pthread_join (*t, nullptr) == 0 ? bl_mkok() : bl_mkerr (bl_error);
}
/*----------------------------------------------------------------------------*/
static inline bl_thread bl_thread_native_handle (bl_thread t)
{
  return t;
}
/*----------------------------------------------------------------------------*/
typedef pthread_mutex_t bl_mutex;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_init (bl_mutex* m)
{
  bl_assert (m);
  int err = pthread_mutex_init (m, nullptr);
  return !err ? bl_mkok() : bl_mkerr_sys (bl_error, err);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_destroy (bl_mutex* m)
{
  bl_assert (m);
  int err = pthread_mutex_destroy (m);
  return !err ? bl_mkok() : bl_mkerr_sys (bl_error, err);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_lock (bl_mutex* m)
{
  bl_assert (m);
  int err = pthread_mutex_lock (m);
  return !err ? bl_mkok() : bl_mkerr_sys (bl_error, err);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_trylock (bl_mutex* m)
{
  bl_assert (m);
  int err = pthread_mutex_trylock (m);
  if (!err) {
    return bl_mkok();
  }
  return bl_mkerr_sys (err == EBUSY ? bl_busy : bl_error, err);
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_unlock (bl_mutex* m)
{
  bl_assert (m);
  int err = pthread_mutex_unlock (m);
  return !err ? bl_mkok() : bl_mkerr_sys (bl_error, err);
}
/*----------------------------------------------------------------------------*/

#include <bl/base/impl/tss_posix.h>

#endif /* __POSIX_THREAD_H__ */
