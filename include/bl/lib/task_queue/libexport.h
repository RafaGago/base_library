#ifndef __TASKQ_LIBEXPORT_H__
#define __TASKQ_LIBEXPORT_H__

#include <bl/hdr/base/platform.h>

#if defined (BL_GCC) && BL_GCC <= BL_GCC_VER (4, 0, 0)
    #define TASKQ_EXPORT __attribute__ ((visibility ("default")))

#elif defined (_MSC_VER)
  #if (defined (TASKQ_SHAREDLIB_COMPILATION) &&\
      !defined (TASKQ_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB_COMPILATION) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define TASKQ_EXPORT __declspec (dllexport)

  #elif (defined (TASKQ_SHAREDLIB) &&\
      !defined (TASKQ_SHAREDLIB_USING_DEF)) ||\
      (defined (BL_ALL_LIBS_SHAREDLIB) &&\
      !defined (BL_ALL_LIBS_SHAREDLIB_USING_DEF))
    #define TASKQ_EXPORT __declspec (dllimport)

  #else
    #define TASKQ_EXPORT
  #endif

#else
  #define TASKQ_EXPORT
#endif

#endif /* __BL_LIBEXPORT_H__ */

