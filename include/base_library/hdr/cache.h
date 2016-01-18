#ifndef __BL_CACHE_H__
#define __BL_CACHE_H__

#include <base_library/hdr/static_integer_math.h>
#include <base_library/hdr/preprocessor.h>
/*---------------------------------------------------------------------------*/
#ifndef BL_CACHE_LINE_SIZE
  #define BL_CACHE_LINE_SIZE 64
#endif
/*---------------------------------------------------------------------------*/
#define declare_cache_pad_member\
  char pp_tokconcat (padding, __LINE__)[BL_CACHE_LINE_SIZE]
/*---------------------------------------------------------------------------*/
#define align_type_to_cache_line(type)\
  char pp_tokconcat (padding, __LINE__)[\
    (div_ceil (sizeof (type), BL_CACHE_LINE_SIZE) * BL_CACHE_LINE_SIZE) -\
    BL_CACHE_LINE_SIZE\
    ]
/*---------------------------------------------------------------------------*/
#endif /* __BL_CACHE_H__ */

