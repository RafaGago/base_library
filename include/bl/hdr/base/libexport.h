#ifndef __BL_LIBEXPORT_H__
#define __BL_LIBEXPORT_H__

#include <bl/hdr/base/platform.h>

#if defined (BL_GCC) && BL_GCC <= BL_GCC_VER (4, 0, 0)
    #define BL_EXPORT __attribute__ ((visibility ("default")))

#elif defined (_MSC_VER)
  #if (defined (BL_SHAREDLIB_COMPILATION) &&\
      !defined (BL_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define BL_EXPORT __declspec (dllexport)

  #elif (defined (BL_SHAREDLIB) &&\
      !defined (BL_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define BL_EXPORT __declspec (dllimport)

  #else
    #define BL_EXPORT
  #endif

#else
  #define BL_EXPORT
#endif

#endif /* __BL_LIBEXPORT_H__ */

