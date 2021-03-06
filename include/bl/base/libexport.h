#ifndef __BL_LIBEXPORT_H__
#define __BL_LIBEXPORT_H__

#include <bl/base/platform.h>

#define BL_EXPORT

#if BL_COMPILER_IS (GCC) || BL_COMPILER_IS (CLANG)
  #if !defined (BL_PRIVATE_SYMS)
    #undef BL_EXPORT
    #define BL_EXPORT BL_VISIBILITY_DEFAULT
  #endif

#elif BL_COMPILER_IS (MICROSOFT_VC)
  #if (defined (BL_SHAREDLIB_COMPILATION) &&\
    !defined (BL_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_EXPORT
    #define BL_EXPORT __declspec (dllexport)

  #elif (defined (BL_SHAREDLIB) &&\
    !defined (BL_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_EXPORT
    #define BL_EXPORT __declspec (dllimport)

   #endif
#endif

#endif /* __BL_LIBEXPORT_H__ */

