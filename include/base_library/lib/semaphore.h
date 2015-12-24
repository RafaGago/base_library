#ifndef __BL_SEMAPHORE_H__
#define __BL_SEMAPHORE_H__

#include <base_library/hdr/platform.h>
/*----------------------------------------------------------------------------*/
#define bl_tm_sem_infinity 0
/*----------------------------------------------------------------------------*/
#if defined (BL_LINUX)
  #include <base_library/lib/semaphore_linux.h>
#elif defined (BL_WINDOWS)
  #include <base_library/lib/semaphore_win.h>
#else
 #error "semaphore unimplemented on this platform"
#endif
/*----------------------------------------------------------------------------*/
#endif /* __BL_SEMAPHORE_H__ */

