#ifndef __BL_THREAD_H__
#define __BL_THREAD_H__

/*---------------------------------------------------------------------------*/
#ifdef BL_HAS_C11_THREAD
  #include <base_library/lib/cc_thread.h>
#else
  #include <base_library/lib/cpp_thread.h>
#endif
/*---------------------------------------------------------------------------*/

#endif /* __BL_THREAD_H__ */

