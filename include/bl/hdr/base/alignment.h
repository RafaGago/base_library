#ifndef __BL_ALIGNMENT_H__
#define __BL_ALIGNMENT_H__

#include <bl/hdr/base/integer.h>
#include <stddef.h>
/*---------------------------------------------------------------------------*/
#define align_anonymous_priv(type1, type2)\
  struct { type1 a; type2 b; }
/*---------------------------------------------------------------------------*/
#define align_combined_size(type1, type2)\
  sizeof (align_anonymous_priv (type1, type2))
/*---------------------------------------------------------------------------*/
#define align_combined_type2_offset(type1, type2)\
  offsetof (align_anonymous_priv (type1, type2), b)
/*---------------------------------------------------------------------------*/
#include <bl/hdr/base/platform.h>
#ifdef __cplusplus
  #include <type_traits>
  #define bl_alignof(type) std::alignment_of<std::atomic<type>>::value  
#elif defined (BL_HAS_C11_STDALIGN)
  #include <stdalign.h>
  #define bl_alignof(type) alignof(type)
#elif defined (BL_GCC)
  #define bl_alignof(type) __alignof__(type)
#else
  /*let the compiler generate errors just if the macro is used*/
#endif
/*---------------------------------------------------------------------------*/
#define next_offset_aligned_to_type(offset_to_max_align_addr, type)\
  (((offset_to_max_align_addr) + bl_alignof (type) - 1) &\
   ~(bl_alignof (type) - 1))
/*---------------------------------------------------------------------------*/
#endif /* __BL_ALIGNMENT_H__ */

