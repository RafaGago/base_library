#ifndef __BL_CPP_THREAD_H__
#define __BL_CPP_THREAD_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <bl/base/thread.h> instead"
#endif

#ifndef __cplusplus
  #error "this file is being used by a non-C++ compiler"
#endif

#include <thread>
#include <mutex>
#include <type_traits>

#include <bl/base/assert.h>
#include <bl/base/error.h>
#include <bl/base/integer.h>
#include <bl/base/platform.h>

extern "C" {
/*---------------------------------------------------------------------------*/
typedef int (*bl_thread_func) (void* context);
/*---------------------------------------------------------------------------*/
typedef std::aligned_storage<
  sizeof (std::thread), std::alignment_of<std::thread>::value
  >::type bl_thread;
/*---------------------------------------------------------------------------*/
static inline bl_err bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  )
{
  bl_assert (t);
  try {
    new ((std::thread*) t) std::thread (f, context);
    return bl_mkok();
  }
  catch (...) {
    return bl_mkerr (bl_error);
  }
}
/*---------------------------------------------------------------------------*/
static inline void bl_thread_yield (void)
{
  std::this_thread::yield();
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_thread_join (bl_thread* t)
{
  bl_assert (t);
  try {
    ((std::thread*) t)->join();
    return bl_mkok();
  }
  catch (...) {
    return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/
static inline decltype(((std::thread*) 0)->native_handle())
  bl_thread_native_handle (bl_thread& t)
{
  return ((std::thread*) &t)->native_handle();
}
/*----------------------------------------------------------------------------*/
typedef std::mutex bl_mutex;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_init (bl_mutex* m)
{
  bl_assert (m);
  try {
    new ((std::mutex*) m) std::mutex();
    return bl_mkok();
  }
  catch (...) {
    return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_destroy (bl_mutex* m)
{
  bl_assert (m);
  try {
    m->~mutex(); /* try/catch in case it is tried to be removed twice */
    return bl_mkok();
  }
  catch (...) {
    return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_lock (bl_mutex* m)
{
  bl_assert (m);
  try {
    m->lock();
    return bl_mkok();
  }
  catch (...) {
    return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_trylock (bl_mutex* m)
{
  bl_assert (m);
  try {
    return m->try_lock() ? bl_mkok() : bl_mkerr (bl_busy);
  }
  catch (...) {
    return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mutex_unlock (bl_mutex* m)
{
  bl_assert (m);
  try {
    m->unlock();
    return bl_mkok();
  }
  catch (...) {
    return bl_mkerr (bl_error);
  }
}
/*----------------------------------------------------------------------------*/

}  /*extern "C" {*/

#if BL_OS_IS_MOSTLY_POSIX
  #include <bl/base/impl/tss_posix.h>
#elif BL_OS_IS (WINDOWS)
  #include <bl/base/impl/tss_windows.h>
#else
  #error "TSS unavailable on this platform"
#endif

#endif /* __BL_CPP_THREAD_H__ */
