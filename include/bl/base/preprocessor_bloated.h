#ifndef __BL_PREPROCESSOR_BLOATED_H__
#define __BL_PREPROCESSOR_BLOATED_H__

/* A header that adds functions relying on preprocessor arithmetic requiring
lots of token definitions, check "bl/base/impl/preprocessor_inc_dec.h" to get
an idea about why the name of this header */

#include <bl/base/preprocessor.h>
#include <bl/base/impl/preprocessor_inc_dec.h>

#ifndef BL_PP_MAX_INC_DEC
  #define BL_PP_MAX_INC_DEC (BL_PP_MAX_RECURSION * 2)
#endif
/*---------------------------------------------------------------------------*/
/**
Increment/decrement the numeric token value.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_inc(x) bl_pp_tokconcat (bl_pp_inc, x)
#define bl_pp_dec(x) bl_pp_tokconcat (bl_pp_dec, x)
/*---------------------------------------------------------------------------*/
/*
bl_pp_varg_count_arith(...) : Counts va_args e.g.

  bl_pp_varg_count (a, b, c) -> is evaluated to 3.
  bl_pp_varg_count() -> is evaluated to 0.

  This function allows as many arguments as BL_PP_MAX_INC_DEC, but it's
  functionally the same than "bl_pp_vargs_count".
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_vargs_count_arith_private_name() bl_pp_vargs_count_arith_private

#define bl_pp_vargs_count_arith_private(cnt,cur_val, ...) \
  bl_pp_if_else (bl_pp_not (bl_pp_has_vargs (__VA_ARGS__)))(\
    (cnt),\
    (bl_pp_defer2 (bl_pp_vargs_count_arith_private_name) () (\
      bl_pp_inc (cnt), __VA_ARGS__) \
      )\
    )

#define bl_pp_vargs_count_arith(...) \
  bl_pp_if_else (bl_pp_has_vargs (__VA_ARGS__))(\
    bl_pp_eval (bl_pp_vargs_count_arith_private (1, __VA_ARGS__)),\
    0\
    )
/*---------------------------------------------------------------------------*/
/*
bl_pp_add (x, y) : Positive numeric addition

  bl_pp_add (4, 11) -> is evaluated to 15.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_add_private_name() bl_pp_add_private

#define bl_pp_add_private(x,y) \
  bl_pp_if_else (bl_pp_not (y))(\
    x,\
    bl_pp_defer2 (bl_pp_add_private_name) () (bl_pp_inc (x), bl_pp_dec (y))\
    )

#define bl_pp_add(x, y) bl_pp_eval (bl_pp_add_private (x, y))
/*---------------------------------------------------------------------------*/
/*
bl_pp_add (x, y) : Positive numeric substraction. Result has to be positive too.

  bl_pp_sub (11, 4) -> 7.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_sub_private_name() bl_pp_sub_private

#define bl_pp_sub_private(x,y) \
  bl_pp_if_else (bl_pp_not (y))(\
    x,\
    bl_pp_defer2 (bl_pp_sub_private_name) () (bl_pp_dec (x), bl_pp_dec (y))\
    )

#define bl_pp_sub(x, y) bl_pp_eval (bl_pp_sub_private (x, y))
/*----------------------------------------------------------------------------*/
#endif /* __BL_PREPROCESSOR_BLOATED_H__ */
