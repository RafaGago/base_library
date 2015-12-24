#ifndef __BL_ALIGNMENT_H__
#define __BL_ALIGNMENT_H__

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
#endif /* __BL_ALIGNMENT_H__ */

