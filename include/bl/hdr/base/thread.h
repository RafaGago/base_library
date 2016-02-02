#ifndef __BL_THREAD_H__
#define __BL_THREAD_H__

#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
/*---------------------------------------------------------------------------*/
#if defined (__cplusplus)
  #include <bl/hdr/base/impl/thread_cpp.hpp>
#elif defined (BL_HAS_C11_THREAD)
  #include <bl/hdr/base/impl/thread_c11.h>
  /*enough for now for detecting posix...*/
#elif (defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)))
  /*including <unistd.h> would define the _POSIX_VERSION, but this is enough
    for now...*/
  #include <bl/hdr/base/impl/thread_posix.h>
#else
  #error "threads unimplemented on this platform"
#endif
#include <bl/hdr/base/impl/micro_sleep.h>
/*---------------------------------------------------------------------------*/

#endif /* __BL_THREAD_H__ */

