#ifndef __BL_SEMAPHORE_H__
#define __BL_SEMAPHORE_H__

#include <bl/base/platform.h>
/*----------------------------------------------------------------------------*/
#define bl_tm_sem_infinity 0
/*----------------------------------------------------------------------------*/
#if defined (BL_LINUX)
  #include <bl/base/impl/semaphore_linux.h>
#elif defined (BL_WINDOWS)
  #include <bl/base/impl/semaphore_win.h>
#else
 #error "semaphore unimplemented on this platform"
#endif
/*----------------------------------------------------------------------------*/
#endif /* __BL_SEMAPHORE_H__ */

