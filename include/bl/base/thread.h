#ifndef __BL_THREAD_H__
#define __BL_THREAD_H__

#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
/*---------------------------------------------------------------------------*/
#if defined (__cplusplus)
  #include <bl/base/impl/thread_cpp.hpp>
#elif defined (BL_HAS_C11_THREAD)
  #include <bl/base/impl/thread_c11.h>
  /*enough for now for detecting posix...*/
#elif (defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)))
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

extern BL_EXPORT uword bl_thread_min_sleep_us (void);
extern BL_EXPORT void bl_thread_usleep (u32 us);

#if defined (__cplusplus)
} /* extern "C" { */
#endif
/*---------------------------------------------------------------------------*/

#endif /* __BL_THREAD_H__ */

