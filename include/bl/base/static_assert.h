#ifndef __BL_STATIC_ASSERT_H__
#define __BL_STATIC_ASSERT_H__

#include <assert.h>

#if defined __cplusplus || defined static_assert
  #define BL_HAS_STATIC_ASSERT 1
  #define bl_static_assert(cond, str) static_assert(cond, str)
#else
  #define BL_HAS_STATIC_ASSERT 0
  #define bl_static_assert(cond, str) assert(str && cond)
#endif

#define bl_static_assert_ns(cond) bl_static_assert (cond, "")

#if BL_HAS_STATIC_ASSERT == 1
  #define bl_static_assert_outside_func(cond, str) bl_static_assert (cond, str)
  #define bl_static_assert_outside_func_ns(cond) bl_static_assert_ns (cond)
#else
  #define bl_static_assert_outside_func(cond, str) /* warning ? */
  #define bl_static_assert_outside_func_ns(cond) /* warning ? */
#endif

#endif /* __BL_STATIC_ASSERT_H__ */

