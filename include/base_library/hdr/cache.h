#ifndef __BL_CACHE_H__
#define __BL_CACHE_H__

#include <base_library/hdr/integer_math.h>
/*---------------------------------------------------------------------------*/
#ifndef bl_cache_line_size
  #define bl_cache_line_size 64
#endif
/*---------------------------------------------------------------------------*/
#define declare_cache_pad_member\
  char tokconcat (padding, __LINE__)[bl_cache_line_size]
/*---------------------------------------------------------------------------*/
#define align_type_to_cache_line(type)\
  char tokconcat (padding, __LINE__)[\
    (div_ceil (sizeof (type), bl_cache_line_size) * bl_cache_line_size) -\
    bl_cache_line_size\
    ]
/*---------------------------------------------------------------------------*/
#endif /* __BL_CACHE_H__ */

