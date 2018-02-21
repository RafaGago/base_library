#ifndef __BL_SERIAL_LIBEXPORT_H__
#define __BL_SERIAL_LIBEXPORT_H__

#include <bl/base/platform.h>

#define BL_SERIAL_EXPORT

#if defined (BL_GCC)
  #if BL_GCC >= BL_GCC_VER (4, 0, 0)
    #if !defined (BL_SERIAL_PRIVATE_SYMS)
      #undef BL_SERIAL_EXPORT
      #define BL_SERIAL_EXPORT  __attribute__ ((visibility ("default")))
    #endif
  #endif

#elif defined (BL_MSC)
  #if (defined (BL_SERIAL_SHAREDLIB_COMPILATION) &&\
    !defined (BL_SERIAL_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_SERIAL_EXPORT
    #define BL_SERIAL_EXPORT __declspec (dllexport)

  #elif (defined (BL_SERIAL_SHAREDLIB) &&\
    !defined (BL_SERIAL_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_SERIAL_EXPORT
    #define BL_SERIAL_EXPORT __declspec (dllimport)

  #endif
#endif

#endif /* __BL_LIBEXPORT_H__ */

