#ifndef __BL_CACHE_H__
#define __BL_CACHE_H__

/*to be deprecated in favor of "alignas (BL_CACHE_LINE_SIZE)" */

#include <bl/base/preprocessor_basic.h>
/*---------------------------------------------------------------------------*/
#ifndef BL_CACHE_LINE_SIZE
  #define BL_CACHE_LINE_SIZE 64
#endif
/*---------------------------------------------------------------------------*/
#define bl_declare_cache_pad_member\
  char bl_pp_tokconcat (padding_, __LINE__)[BL_CACHE_LINE_SIZE]
/*---------------------------------------------------------------------------*/
#ifndef __cplusplus
#define bl_align_type_to_cache_line(type)\
  char bl_pp_tokconcat (padding_, __LINE__)[BL_CACHE_LINE_SIZE % sizeof (type)]
#else
/*---------------------------------------------------------------------------*/
template <int N>
struct bl_align_type_to_cache_line_priv {
  char arr[N];
};
template <>
struct bl_align_type_to_cache_line_priv<0> {};
/*---------------------------------------------------------------------------*/
#define bl_align_type_to_cache_line(type)\
bl_align_type_to_cache_line_priv<\
  BL_CACHE_LINE_SIZE % sizeof (type)\
  > bl_pp_tokconcat (padding_, __LINE__)
#endif
/*---------------------------------------------------------------------------*/

#endif /* __BL_CACHE_H__ */
