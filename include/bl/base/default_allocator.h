#ifndef __BL_DEFAULT_ALLOCATOR_H__
#define __BL_DEFAULT_ALLOCATOR_H__

#include <bl/base/platform.h>
#include <bl/base/allocator.h>

/*---------------------------------------------------------------------------*/
static void* bl_default_alloc_func (size_t bytes, bl_alloc_tbl const* invoker)
{
  return malloc (bytes);
}
/*---------------------------------------------------------------------------*/
static void* bl_default_realloc_func(
  void* mem, size_t new_size, bl_alloc_tbl const* invoker
  )
{
  return realloc (mem, new_size);
}
/*---------------------------------------------------------------------------*/
static void bl_default_dealloc_func(
  void const* mem, bl_alloc_tbl const* invoker
  )
{
  free ((void*) mem);
}
/*---------------------------------------------------------------------------*/
static inline bl_alloc_tbl bl_get_default_alloc()
{
  bl_alloc_tbl default_alloc;
  default_alloc.alloc   = bl_default_alloc_func;
  default_alloc.realloc = bl_default_realloc_func;
  default_alloc.dealloc = bl_default_dealloc_func;
  return default_alloc;
}
/*---------------------------------------------------------------------------*/

#endif /* DEFAULT_ALLOCATOR_H_ */
