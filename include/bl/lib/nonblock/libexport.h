#ifndef __NONBLOCK_LIBEXPORT_H__
#define __NONBLOCK_LIBEXPORT_H__

#include <bl/hdr/base/platform.h>

#if defined (BL_GCC) && BL_GCC <= BL_GCC_VER (4, 0, 0)
    #define NONBLOCK_EXPORT __attribute__ ((visibility ("default")))

#elif defined (_MSC_VER)
  #if (defined (NONBLOCK_SHAREDLIB_COMPILATION) &&\
      !defined (NONBLOCK_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define NONBLOCK_EXPORT __declspec (dllexport)

  #elif (defined (NONBLOCK_SHAREDLIB) &&\
      !defined (NONBLOCK_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define NONBLOCK_EXPORT __declspec (dllimport)

  #else
    #define NONBLOCK_EXPORT
  #endif

#else
  #define NONBLOCK_EXPORT
#endif

#endif /* __BL_LIBEXPORT_H__ */

