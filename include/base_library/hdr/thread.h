#ifndef __BL_THREAD_H__
#define __BL_THREAD_H__

/*---------------------------------------------------------------------------*/
#if defined (__cplusplus)
  #include <base_library/hdr/impl/thread_cpp.hpp>
#elif defined (BL_HAS_C11_THREAD) /*as of now this is undetected*/
  #include <base_library/hdr/impl/thread_c11.h>
  /*enough for now for detecting posix...*/
#elif (defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)))
  /*including <unistd.h> would define the _POSIX_VERSION...*/
  #include <base_library/hdr/impl/thread_posix.h>
#else
  #error "threads unimplemented on this platform"
#endif
/*---------------------------------------------------------------------------*/

#endif /* __BL_THREAD_H__ */

