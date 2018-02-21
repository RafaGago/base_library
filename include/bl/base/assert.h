#ifndef __BL_ASSERT_H__
#define __BL_ASSERT_H__

/*WARNING: If you use this in a translation unit that compiles external code
  their assertions will become active even when "NDEBUG" is defined. E.g
  inline functions.
*/

#if defined (NDEBUG) && !defined (BL_ASSERT_DEFAULT_BEHAVIOR)
  #ifdef BL_ASSERT_RELEASE_BEHAVIOR_PRIVATE
    #error "trying to cheat by using private macro"
  #endif
  #define BL_ASSERT_RELEASE_BEHAVIOR_PRIVATE 1
  #undef  NDEBUG
#endif

#include <assert.h>

#ifdef BL_ASSERT_RELEASE_BEHAVIOR_PRIVATE
  #undef BL_ASSERT_RELEASE_BEHAVIOR_PRIVATE
  #define NDEBUG                        1
  #define bl_assert(expr)
  #define bl_assert_always(expr)        assert (expr)
  #define bl_assert_side_effect(expr)   (void) (expr)
#else
  #define bl_assert(expr)               assert (expr)
  #define bl_assert_always(expr)        assert (expr)

  #ifdef NDEBUG
    #define bl_assert_side_effect(expr) (void) (expr)
  #else
    #define bl_assert_side_effect(expr) assert (expr)
  #endif
#endif

#endif /* __BL_ASSERT_H__ */

