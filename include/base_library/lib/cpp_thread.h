#ifndef __BL_CPP_THREAD_H__
#define __BL_CPP_THREAD_H__

#include <base_library/hdr/integer.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
/* This is terrible now on old gcc versions, as it requires linking to       */
/* libstdc++                                                                 */
/*---------------------------------------------------------------------------*/
typedef int (*bl_thread_func) (void* context);
/*---------------------------------------------------------------------------*/
typedef uword bl_thread;
/*---------------------------------------------------------------------------*/
extern int bl_thread_init(
  bl_thread* t, bl_thread_func f, void* context
  );
/*---------------------------------------------------------------------------*/
extern void bl_thread_yield (void);
/*---------------------------------------------------------------------------*/
extern int bl_thread_join (bl_thread* t);
/*---------------------------------------------------------------------------*/
enum evk_thrd_err_e {
  bl_thread_success,
  bl_thread_nomem,
  bl_thread_busy,
  bl_thread_error,
};
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}  /* extern "C" {*/
#endif

#endif /* __BL_CPP_THREAD_H__ */

