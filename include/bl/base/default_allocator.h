#ifndef __BL_DEFAULT_ALLOCATOR_H__
#define __BL_DEFAULT_ALLOCATOR_H__

#include <bl/base/libexport.h>
#include <bl/base/allocator.h>

/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_alloc_tbl const bl_default_alloc;
/*---------------------------------------------------------------------------*/
static inline bl_alloc_tbl bl_get_default_alloc()
{
  return bl_default_alloc; /* copies by value */
}
/*---------------------------------------------------------------------------*/

#endif /* DEFAULT_ALLOCATOR_H_ */
