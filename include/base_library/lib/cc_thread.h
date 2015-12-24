#ifndef __BL_CC_THREAD_H__
#define __BL_CC_THREAD_H__

#include <threads.h>
#include <base_library/hdr/platform.h>

typedef thrd_t       bl_thread;
typedef thrd_start_t bl_thread_func;
/*---------------------------------------------------------------------------*/
#define bl_thread_success thrd_success
#define bl_thread_nomem   thrd_timedout
#define bl_thread_busy    thrd_busy
#define bl_thread_error   thrd_error
/*---------------------------------------------------------------------------*/
static inline int bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  )
{
  return thrd_create (t, f, context);
}
/*---------------------------------------------------------------------------*/
static inline void bl_thread_yield (void)
{
  return thrd_yield();
}
/*---------------------------------------------------------------------------*/
static inline int bl_thread_join (bl_thread* t)
{
  return thrd_join (t, nullptr);
}
/*---------------------------------------------------------------------------*/

#endif /* __BL_CC_THREAD_H__ */

