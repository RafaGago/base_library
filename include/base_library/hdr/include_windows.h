#ifndef __BL_INCLUDE_WINDOWS_H__
#define __BL_INCLUDE_WINDOWS_H__

/*----------------------------------------------------------------------------*/
#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN 1
  #define UNDEF_WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMIMMAX
  #define NOMINMAX 1
  #define UNDEF_NOMINMAX 1
#endif
/*----------------------------------------------------------------------------*/
#include <windows.h>
/*----------------------------------------------------------------------------*/
#ifdef UNDEF_WIN32_LEAN_AND_MEAN
  #undef WIN32_LEAN_AND_MEAN
  #undef UNDEF_WIN32_LEAN_AND_MEAN
#endif

#ifndef UNDEF_NOMIMMAX
  #undef NOMINMAX
  #undef UNDEF_NOMINMAX
#endif
/*----------------------------------------------------------------------------*/

#endif /* __BL_INCLUDE_WINDOWS_H__ */

