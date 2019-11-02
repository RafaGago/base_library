#include <bl/base/dynarray.h>
#include <bl/base/utility.h>
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dynarray_resize(
  bl_dynarray_stub*   d,
  bl_uword            new_size,
  bl_uword            elem_size,
  bl_alloc_tbl const* alloc
  )
{
  bl_assert (d && alloc);
  bl_err err = bl_mkok();
  void* new_ptr;

  if (new_size != 0) {
    new_ptr = bl_realloc (alloc, d->arr, new_size * elem_size);
    if (bl_unlikely (!new_ptr)) {
      new_size = d->size;
      new_ptr  = d->arr;
      err      = bl_mkerr (bl_alloc);
    }
  }
  else {
    bl_dealloc (alloc, d->arr);
    new_ptr = nullptr;
  }
  d->size = new_size;
  d->arr  = new_ptr;
  return err;
}
/*----------------------------------------------------------------------------*/

