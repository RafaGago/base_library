#ifndef BL_UTILITY_H
#define BL_UTILITY_H

#include <bl/base/platform.h>
#include <bl/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define bl_min(v1, v2) (((v1) <= (v2)) ? (v1) : (v2))
#define bl_max(v1, v2) (((v1) <= (v2)) ? (v2) : (v1))
/*---------------------------------------------------------------------------*/
#define bl_sizeof_member(struct_type, member_name)\
  (sizeof ((struct_type*) 0)->member_name)
/*---------------------------------------------------------------------------*/
#define bl_arr_elems(arr) (sizeof (arr) / sizeof ((arr)[0]))
#define bl_arr_elems_member(struct_type, member_name)\
  (bl_sizeof_member (struct_type, member_name) /\
   sizeof ((struct_type*) 0)->member_name[0])
/*---------------------------------------------------------------------------*/
#define bl_arr2d_rows(arr2d) bl_arr_elems (arr2d)
#define bl_arr2d_columns(arr2d) (sizeof ((arr2d)[0]) / sizeof ((arr2d)[0][0]))
/*---------------------------------------------------------------------------*/
/*end = first value out of range*/
#define bl_point_in_range(point, range_beg, range_end)\
  ((point) >= (range_beg) && (point) < (range_end))

/*end = first value out of range*/
#define bl_subrange_in_range(subrange_beg, subrange_end, range_beg, range_end)\
  (\
   bl_point_in_range ((subrange_beg), (range_beg), (range_end)) &&\
   bl_point_in_range ((subrange_end) - 1, (range_beg), (range_end))\
  )
/*---------------------------------------------------------------------------*/
#define bl_type_bits(type) (sizeof (type) * 8)
/*---------------------------------------------------------------------------*/
#define bl_ptr_aligned_to_builtin_type (ptr, type)\
  (bl_is_pow2 (sizeof (type)) && \
    ((((bl_uword) ptr) & (sizeof (type) - 1)) == 0))
/*---------------------------------------------------------------------------*/
#define bl_lit_len(literal) (sizeof (literal) - 1)
/*---------------------------------------------------------------------------*/
#define bl_lit_strcmp(str, lit) strncmp ((str), lit, bl_lit_len (lit))
/*---------------------------------------------------------------------------*/
#if BL_COMPILER_IS (GCC) || BL_COMPILER_IS (CLANG)
  #define bl_likely(x)   __builtin_expect (!!(x), 1)
  #define bl_unlikely(x) __builtin_expect (!!(x), 0)
#else
  #define bl_likely(x)   (x)
  #define bl_unlikely(x) (x)
#endif
/*---------------------------------------------------------------------------*/
#endif /*LGC_UTILITY_H*/

