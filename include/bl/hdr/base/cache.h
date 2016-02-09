#ifndef __BL_CACHE_H__
#define __BL_CACHE_H__

/*to be deprecated in favor of "alignas (BL_CACHE_LINE_SIZE)" */

#include <bl/hdr/base/preprocessor_basic.h>
/*---------------------------------------------------------------------------*/
#ifndef BL_CACHE_LINE_SIZE
  #define BL_CACHE_LINE_SIZE 64
#endif
/*---------------------------------------------------------------------------*/
#define declare_cache_pad_member\
  char pp_tokconcat (padding_, __LINE__)[BL_CACHE_LINE_SIZE]
/*---------------------------------------------------------------------------*/
#ifndef __cplusplus
#define align_type_to_cache_line(type)\
  char pp_tokconcat (padding_, __LINE__)[BL_CACHE_LINE_SIZE % sizeof (type)]
#else
/*---------------------------------------------------------------------------*/
template <int N>
struct align_type_to_cache_line_priv {
  char arr[N];
};
template <>
struct align_type_to_cache_line_priv<0> {};
/*---------------------------------------------------------------------------*/
#define align_type_to_cache_line(type)\
align_type_to_cache_line_priv<\
  BL_CACHE_LINE_SIZE % sizeof (type)\
  > pp_tokconcat (padding_, __LINE__)
#endif
/*---------------------------------------------------------------------------*/

#endif /* __BL_CACHE_H__ */
