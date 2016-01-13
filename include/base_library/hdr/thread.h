#ifndef __BL_THREAD_H__
#define __BL_THREAD_H__

#include <base_library/hdr/platform.h>
#if defined (BL_GCC) && BL_GCC >= BL_GCC_VER (4, 9, 0)
  #define BL_HAS_C11_THREAD
#endif
/*---------------------------------------------------------------------------*/
#if defined (__cplusplus)
  #include <base_library/hdr/impl/thread_cpp.hpp>
#elif defined (BL_HAS_C11_THREAD)
  #include <base_library/hdr/impl/thread_c11.h>
  /*enough for now for detecting posix...*/
#elif (defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)))
  /*including <unistd.h> would define the _POSIX_VERSION, but this is enough
    for now...*/
  #include <base_library/hdr/impl/thread_posix.h>
#else
  #error "threads unimplemented on this platform"
#endif
/*---------------------------------------------------------------------------*/

#endif /* __BL_THREAD_H__ */

