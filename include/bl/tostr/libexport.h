#ifndef __BL_TOSTR_LIBEXPORT_H__
#define __BL_TOSTR_LIBEXPORT_H__

#include <bl/base/platform.h>

#define BL_TOSTR_EXPORT

#if BL_COMPILER_IS (GCC) || BL_COMPILER_IS (CLANG)
  #if !defined (BL_TOSTR_PRIVATE_SYMS)
    #undef BL_TOSTR_EXPORT
    #define BL_TOSTR_EXPORT BL_VISIBILITY_DEFAULT
  #endif

#elif BL_COMPILER_IS (MICROSOFT_VC)
  #if (defined (BL_TOSTR_SHAREDLIB_COMPILATION) &&\
    !defined (BL_TOSTR_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_TOSTR_EXPORT
    #define BL_TOSTR_EXPORT __declspec (dllexport)

  #elif (defined (BL_TOSTR_SHAREDLIB) &&\
    !defined (BL_TOSTR_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_TOSTR_EXPORT
    #define BL_TOSTR_EXPORT __declspec (dllimport)

  #endif
#endif

#endif /* __BL_TOSTR_LIBEXPORT_H__ */