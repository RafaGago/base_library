#ifndef __BL_SEMAPHORE_H__
#define __BL_SEMAPHORE_H__

#include <bl/base/platform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
#define bl_tm_sem_infinity 0
/*----------------------------------------------------------------------------*/
#if BL_OS_IS (LINUX)
  #include <bl/base/impl/semaphore_linux.h>
#elif BL_OS_IS (WINDOWS)
  #include <bl/base/impl/semaphore_win.h>
#else
 #error "semaphore unimplemented on this platform"
#endif
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} //extern "C" {
#endif

#endif /* __BL_SEMAPHORE_H__ */

