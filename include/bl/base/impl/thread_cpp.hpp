#ifndef __BL_CPP_THREAD_H__
#define __BL_CPP_THREAD_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <hdr/thread.h> instead"
#endif

#ifndef __cplusplus
  #error "this file is being used by a non-C++ compiler"
#endif

#include <thread>
#include <type_traits>

#include <bl/base/assert.h>
#include <bl/base/integer.h>
#include <bl/base/platform.h>

extern "C" {
/*---------------------------------------------------------------------------*/
enum evk_thrd_err_e {
  bl_thread_success,
  bl_thread_nomem,
  bl_thread_busy,
  bl_thread_error,
};
/*---------------------------------------------------------------------------*/
typedef int (*bl_thread_func) (void* context);
/*---------------------------------------------------------------------------*/
typedef std::aligned_storage<
  sizeof (std::thread), std::alignment_of<std::thread>::value
  >::type bl_thread;
/*---------------------------------------------------------------------------*/
static inline int bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  )
{
  bl_assert (t);
  try {
    new ((std::thread*) t) std::thread (f, context);
    return bl_thread_success;
  }
  catch (...) {
    return bl_thread_error;
  }
}
/*---------------------------------------------------------------------------*/
static inline void bl_thread_yield (void)
{
  std::this_thread::yield();
}
/*---------------------------------------------------------------------------*/
static inline int bl_thread_join (bl_thread* t)
{
  bl_assert (t);
  try {
    ((std::thread*) t)->join();
    return bl_thread_success;
  }
  catch (...) {
    return bl_thread_error;
  }
}
/*---------------------------------------------------------------------------*/

}  /*extern "C" {*/

#if defined (BL_POSIX)
  #include <bl/base/impl/tss_posix.h>
#elif defined (BL_WINDOWS)
  #include <bl/base/impl/tss_windows.h>
#else
  #error "TSS unavailable on this platform"
#endif

#endif /* __BL_CPP_THREAD_H__ */
