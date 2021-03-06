#ifndef __BL_THREAD_H__
#define __BL_THREAD_H__

#include <bl/base/assert.h>
#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
/*---------------------------------------------------------------------------*/
#if defined (__cplusplus)
  #include <bl/base/impl/thread_cpp.hpp>
#elif BL_HAS_C11_THREAD
  #include <bl/base/impl/thread_c11.h>
  /*enough for now for detecting posix...*/
#elif BL_OS_IS_MOSTLY_POSIX
  /*including <unistd.h> would define the _POSIX_VERSION, but this is enough
    for now...*/
  #include <bl/base/impl/thread_posix.h>
#else
  #error "threads unimplemented on this platform"
#endif
/*---------------------------------------------------------------------------*/
/*
  static inline bl_err bl_thread_init(
    bl_thread* t, bl_thread_func f, void* context
    );

  static inline void bl_thread_yield (void);

  static inline bl_err bl_thread_join (bl_thread* t);

  bl_thread_local;
  bl_tss;
  bl_tss_dtor;
  bl_tss_dtor_callconv; #windows requires stdcall on the tss destructor

  static inline bl_err bl_tss_init (bl_tss* key, bl_tss_cleanup_func cleanup);
  static inline void bl_tss_destroy (bl_tss key);
  static inline bl_err bl_tss_set (bl_tss key, void* val);
  static inline void* bl_tss_get (bl_tss key);
*/
/*---------------------------------------------------------------------------*/
#if defined (__cplusplus)
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_uword bl_thread_min_sleep_us (void);
extern BL_EXPORT void bl_thread_usleep (bl_u32 us);
/*---------------------------------------------------------------------------*/

#if BL_OS_IS (LINUX)

#include <sched.h>
#include <pthread.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
static inline bl_uword bl_get_cpu_count (void)
{
  return sysconf (_SC_NPROCESSORS_ONLN);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_get_cpu (void)
{
  unsigned int v = sched_getcpu();
#ifndef BL_THREAD_GET_CPU_NO_ASSERT
  bl_assert (v >=0 && v < bl_get_cpu_count());
#endif
  return (bl_uword) (v >= 0 ? v : 0);
}
/*---------------------------------------------------------------------------*/
typedef cpu_set_t bl_affinity_mask;
/*---------------------------------------------------------------------------*/
static inline void bl_affinity_mask_init (bl_affinity_mask* m)
{
  CPU_ZERO (m);
}
/*---------------------------------------------------------------------------*/
static inline void bl_affinity_mask_set_cpu (bl_affinity_mask* m, bl_uword cpu)
{
  bl_assert (cpu < bl_get_cpu_count());
  CPU_SET (cpu, m);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_affinity_mask_cpu_is_set(
  bl_affinity_mask const* m, bl_uword cpu
  )
{
  bl_assert (cpu < bl_get_cpu_count());
  return CPU_ISSET (cpu, m);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_set_thread_affinity(
  bl_thread t, bl_affinity_mask const* m
  )
{
  int e = pthread_setaffinity_np (bl_thread_native_handle (t), sizeof *m, m);
  return !e ? bl_mkok() : bl_mkerr (bl_error);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_get_thread_affinity (bl_thread t, bl_affinity_mask* m)
{
  bl_assert (m);
  int e = pthread_getaffinity_np (bl_thread_native_handle (t), sizeof *m, m);
  return !e ? bl_mkok() : bl_mkerr (bl_error);
}
/*---------------------------------------------------------------------------*/

/*TODO: function to get a bl_thread from the current thread need to be added,
  but C++ can't get a std::thread from this_thread. This may require a refactor
  of c++'s' bl_thread */

#elif BL_OS_IS (WINDOWS)

#include <bl/base/include_windows.h>
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_get_cpu_count (void)
{
  SYSTEM_INFO sysinfo;
  GetSystemInfo (&sysinfo);
  return (bl_uword) sysinfo.dwNumberOfProcessors;
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_get_cpu (void)
{
  /* TODO: check that this isn't heavyweight */
  bl_uword v = (bl_uword) GetCurrentProcessorNumber();
#ifndef BL_THREAD_GET_CPU_NO_ASSERT
  bl_assert (v < bl_get_cpu_count());
#endif
  return v;
}
/*---------------------------------------------------------------------------*/
typedef DWORD_PTR bl_affinity_mask;
/*---------------------------------------------------------------------------*/
static inline void bl_affinity_mask_init (bl_affinity_mask* m)
{
  bl_assert (m);
  *m = 0;
}
/*---------------------------------------------------------------------------*/
static inline void bl_affinity_mask_set_cpu (bl_affinity_mask* m, bl_uword cpu)
{
  *m |= (1 << cpu);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_affinity_mask_cpu_is_set(
  bl_affinity_mask const* m, bl_uword cpu
  )
{
  bl_assert (m);
  bl_assert (cpu < bl_get_cpu_count());
  return (*m & (1 << cpu)) != 0;
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_set_thread_affinity(
  bl_thread t, bl_affinity_mask const* m
  )
{
  bl_assert (m);
  bl_affinity_mask e = SetThreadAffinityMask (bl_thread_native_handle (t), *m);
  return e ? bl_mkok() : bl_mkerr (bl_error);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_get_thread_affinity (bl_thread t, bl_affinity_mask* m)
{
  bl_assert (m);
  bl_affinity_mask dummy = 1;
  *m = SetThreadAffinityMask (bl_thread_native_handle (t), dummy);
  if (*m) {
    /* error ignored: impossible to recover*/
    (void) SetThreadAffinityMask (bl_thread_native_handle (t), *m);
  }
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
#else
  #error "Implement or #ifdef out this error..."
#endif

#if defined (__cplusplus)
} /* extern "C" { */
#endif
/*---------------------------------------------------------------------------*/

#endif /* __BL_THREAD_H__ */
