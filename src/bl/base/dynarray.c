#include <bl/base/dynarray.h>
#include <bl/base/utility.h>
/*----------------------------------------------------------------------------*/
bl_err dynarray_resize(
  dynarray_stub*   d,
  uword            new_size,
  uword            elem_size,
  alloc_tbl const* alloc
  )
{
  bl_assert (d && alloc);
  bl_err err = bl_ok;
  void* new_ptr;

  if (new_size != 0) {
    new_ptr = bl_realloc (alloc, d->arr, new_size * elem_size);
    if (unlikely (!new_ptr)) {
      new_size = d->size;
      new_ptr  = d->arr;
      err      = bl_alloc;
    }
  }
  else {
    /*realloc with size 0 has implementation defined behavior on C11*/
    bl_dealloc (alloc, d->arr);
    new_ptr = nullptr;
  }
  d->size = new_size;
  d->arr  = new_ptr;
  return err;
}
/*----------------------------------------------------------------------------*/

