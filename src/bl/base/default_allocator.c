#include <bl/base/default_allocator.h>

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
BL_EXPORT const bl_alloc_tbl bl_default_alloc = {
  bl_default_alloc_func,
  bl_default_realloc_func,
  bl_default_dealloc_func
};
/*---------------------------------------------------------------------------*/
