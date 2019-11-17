#ifndef __BL_STATIC_ASSERT_H__
#define __BL_STATIC_ASSERT_H__

#include <bl/base/platform.h>

#if !defined bl_static_assert_private
  #include <bl/base/preprocessor_basic.h>
  #include <bl/base/integer.h>
  #ifdef BL_HAS___COUNTER__
    #define bl_static_assert_private(cond, lit) \
      typedef char bl_pp_tokconcat (bl_static_assert_impl_, __COUNTER__)\
        [(cond) ? 1u : -1];
  #else
    #define bl_static_assert_private(cond, lit) \
      typedef char bl_pp_tokconcat (bl_static_assert_impl_, __LINE__)\
        [(cond) ? 1u : -1];
  #endif
#endif

#if defined bl_static_assert_private
  #define bl_static_assert_funcscope(cond, lit) \
    bl_static_assert_private (cond, lit)
  #define bl_static_assert_ns_funcscope(cond) \
    bl_static_assert_private (cond, "")
  #define bl_maybe_static_assert_funcscope(cond, lit) \
    bl_static_assert_private (cond, lit)
  #define bl_maybe_static_assert_ns_funcscope(cond) \
    bl_static_assert_private (cond, "")
#else
  #include <bl/base/assert.h>
  #define bl_static_assert_funcscope(cond, lit)
  #define bl_static_assert_ns_funcscope(cond)
  #define bl_maybe_static_assert_funcscope(cond, lit) bl_assert (cond && lit);
  #define bl_maybe_static_assert_ns_funcscope(cond) bl_assert (cond);
#endif

/* use these on the global scope*/
#define bl_static_assert(cond, lit) bl_static_assert_private (cond, lit)
#define bl_static_assert_ns(cond) bl_static_assert_private (cond, "")

#endif /* __BL_STATIC_ASSERT_H__ */

