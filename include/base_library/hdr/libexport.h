#ifndef __BL_LIBEXPORT_H__
#define __BL_LIBEXPORT_H__

#include <base_library/hdr/platform.h>

#if defined (BL_GCC) && BL_GCC <= BL_GCC_VER (4, 0, 0)
    #define BL_EXPORT       __attribute__ ((visibility ("default")))
    #define BL_EXPORT_CLASS __attribute__ ((visibility ("default")))

#elif defined (_MSC_VER)
  #if defined (BL_SHAREDLIB_COMPILATION) && !defined (BL_SHAREDLIB_USING_DEF)
    #define BL_EXPORT       __declspec (dllexport)
    #define BL_EXPORT_CLASS __declspec (dllexport)

  #elif defined (BL_SHAREDLIB) && !defined (BL_SHAREDLIB_USING_DEF)
    #define BL_EXPORT       __declspec (dllimport)
    #define BL_EXPORT_CLASS __declspec (dllimport)

  #else
    #define BL_EXPORT
    #define BL_EXPORT_CLASS
  #endif

#else
  #define BL_EXPORT
  #define BL_EXPORT_CLASS
#endif

#endif /* __BL_LIBEXPORT_H__ */

