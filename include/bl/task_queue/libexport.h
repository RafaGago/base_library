#ifndef __BL_TASKQ_LIBEXPORT_H__
#define __BL_TASKQ_LIBEXPORT_H__

#include <bl/base/platform.h>

#define BL_TASKQ_EXPORT

#if defined (BL_GCC)
  #if BL_GCC >= BL_GCC_VER (4, 0, 0)
    #if !defined (BL_TASKQ_PRIVATE_SYMS)
      #undef BL_TASKQ_EXPORT
      #define BL_TASKQ_EXPORT  __attribute__ ((visibility ("default")))
    #endif
  #endif

#elif defined (BL_MSC)
  #if (defined (BL_TASKQ_SHAREDLIB_COMPILATION) &&\
    !defined (BL_TASKQ_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_TASKQ_EXPORT
    #define BL_TASKQ_EXPORT __declspec (dllexport)

  #elif (defined (BL_TASKQ_SHAREDLIB) &&\
    !defined (BL_TASKQ_SHAREDLIB_USING_DEF)) ||\
    (defined (BL_ALL_LIBS_SHAREDLIB) &&\
    !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))

    #undef BL_TASKQ_EXPORT
    #define BL_TASKQ_EXPORT __declspec (dllimport)

  #endif
#endif

#endif /* __BL_LIBEXPORT_H__ */

