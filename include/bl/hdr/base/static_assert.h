#ifndef __BL_STATIC_ASSERT_H__
#define __BL_STATIC_ASSERT_H__

#if !defined __cplusplus && !defined (_MSC_VER) && !defined (static_assert)
  #define static_assert(cond, str) _Static_assert(cond, str)
#endif

#define static_assert_ns(cond) static_assert (cond, "")

/*
 Use these two when out of a function, so we will be able to fallback to 
 compilers without static_assert. Place static_asserts inside functions
 whenever it's possible. The over-verbose name is to encourage this.
*/

#define static_assert_outside_func(cond, str) static_assert (cond, str)
#define static_assert_outside_func_ns(cond) static_assert_ns (cond)

#endif /* __BL_STATIC_ASSERT_H__ */

