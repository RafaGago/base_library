#ifndef __BL_TSS_WINDOWS_H__
#define __BL_TSS_WINDOWS_H__

#ifndef __BL_THREAD_H__
  #error "don't include this file directly, use <bl/base/thread.h> instead"
#endif

#include <bl/base/include_windows.h>
#include <bl/base/error.h>

/*----------------------------------------------------------------------------*/
#define bl_thread_local __declspec (thread)
#define bl_tss_dtor_callconv WINAPI
typedef DWORD bl_tss;
typedef PFLS_CALLBACK_FUNCTION bl_tss_dtor_callconv;
/*----------------------------------------------------------------------------*/
static inline bl_error bl_tss_init (bl_tss* key, bl_tss_cleanup_func cleanup)
{
  *key = FlsAlloc (cleanup);
  return (*key != FLS_OUT_OF_INDEXES) ? bl_ok : bl_alloc;
}
/*----------------------------------------------------------------------------*/
static inline void bl_tss_destroy (bl_tss key)
{
  FlsFree (key);
}
/*----------------------------------------------------------------------------*/
static inline bl_error bl_tss_set (bl_tss key, void const* val)
{
  return FlsSetValue (key, val) ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
static inline void* bl_tss_get (bl_tss key)
{
  return FlsGetValue (key);
}
/*----------------------------------------------------------------------------*/

#endif /* __BL_TSS_WINDOWS_H__ */