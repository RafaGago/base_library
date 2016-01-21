#ifndef BL_UTILITY_H
#define BL_UTILITY_H

#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/static_integer_math.h>
/*---------------------------------------------------------------------------*/
#define bl_min(v1, v2) ((v1) <= (v2) ? (v1) : (v2))
#define bl_max(v1, v2) ((v1) <= (v2) ? (v2) : (v1))
/*---------------------------------------------------------------------------*/
#define sizeof_member(struct_type, member_name)\
  (sizeof ((struct_type*) 0)->member_name)
/*---------------------------------------------------------------------------*/
#define arr_elems(arr) (sizeof (arr) / sizeof ((arr)[0]))
#define arr_elems_member(struct_type, member_name)\
  (sizeof_member (struct_type, member_name) /\
   sizeof ((struct_type*) 0)->member_name[0])
/*---------------------------------------------------------------------------*/
#define arr2d_rows(arr2d) arr_elems (arr2d)
#define arr2d_columns(arr2d) (sizeof ((arr2d)[0]) / sizeof ((arr2d)[0][0]))
/*---------------------------------------------------------------------------*/
/*end = first value out of range*/
#define point_in_range(point, range_beg, range_end)\
  ((point) >= (range_beg) && (point) < (range_end))

/*end = first value out of range*/
#define subrange_in_range(subrange_beg, subrange_end, range_beg, range_end)\
  (\
   point_in_range ((subrange_beg), (range_beg), (range_end)) &&\
   point_in_range ((subrange_end) - 1, (range_beg), (range_end))\
  )
/*---------------------------------------------------------------------------*/
#define type_bits(type) (sizeof (type) * 8)
/*---------------------------------------------------------------------------*/
#ifdef NDEBUG
  #define side_effect_assert(expr) (void) (expr)
#else
  #define side_effect_assert(expr) assert (expr)
#endif
/*---------------------------------------------------------------------------*/
#define ptr_aligned_to_builtin_type (ptr, type)\
  (is_pow2 (sizeof (type)) && ((((uword) ptr) & (sizeof (type) - 1)) == 0))
/*---------------------------------------------------------------------------*/
typedef struct empty_struct {} empty_struct;
/*---------------------------------------------------------------------------*/
#ifdef BL_GCC
  #define likely(x)   __builtin_expect (!!(x), 1)
  #define unlikely(x) __builtin_expect (!!(x), 0)
#else
  #define likely(x)   (x)
  #define unlikely(x) (x)
#endif
/*---------------------------------------------------------------------------*/
#endif /*LGC_UTILITY_H*/

