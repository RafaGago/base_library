#ifndef __BL_DEFAULT_ALLOCATOR_H__
#define __BL_DEFAULT_ALLOCATOR_H__

#include <bl/hdr/base/allocator.h>

/*---------------------------------------------------------------------------*/
static void* default_alloc_func (size_t bytes, alloc_tbl const* invoker)
{
  return malloc (bytes);
}
/*---------------------------------------------------------------------------*/
static void* default_realloc_func(
  void* mem, size_t new_size, alloc_tbl const* invoker
  )
{
  return realloc (mem, new_size);
}
/*---------------------------------------------------------------------------*/
static void default_dealloc_func (void* mem, alloc_tbl const* invoker)
{
  free (mem);
}
/*---------------------------------------------------------------------------*/
static inline alloc_tbl get_default_alloc()
{
  alloc_tbl default_alloc = {
    .alloc   = default_alloc_func,
    .realloc = default_realloc_func,
    .dealloc = default_dealloc_func,    
  };
  return default_alloc;
}
/*---------------------------------------------------------------------------*/

#endif /* DEFAULT_ALLOCATOR_H_ */
