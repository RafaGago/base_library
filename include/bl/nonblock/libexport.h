#ifndef __BL_NONBLOCK_LIBEXPORT_H__
#define __BL_NONBLOCK_LIBEXPORT_H__

#include <bl/base/platform.h>

#if defined (BL_GCC) && BL_GCC >= BL_GCC_VER (4, 0, 0)
    #define BL_NONBLOCK_EXPORT __attribute__ ((visibility ("default")))

#elif defined (BL_MSC)
  #if (defined (BL_NONBLOCK_SHAREDLIB_COMPILATION) &&\
      !defined (BL_NONBLOCK_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define BL_NONBLOCK_EXPORT __declspec (dllexport)

  #elif (defined (BL_NONBLOCK_SHAREDLIB) &&\
      !defined (BL_NONBLOCK_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define BL_NONBLOCK_EXPORT __declspec (dllimport)

  #else
    #define BL_NONBLOCK_EXPORT
  #endif

#else
  #define BL_NONBLOCK_EXPORT
#endif

#endif /* __BL_LIBEXPORT_H__ */

