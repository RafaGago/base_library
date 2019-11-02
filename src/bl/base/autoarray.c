#include <bl/base/dynarray.h>
#include <bl/base/autoarray.h>
#include <bl/base/integer_math.h>
#include <bl/base/utility.h>
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_autoarray_set_rounded_capacity(
  bl_autoarray_stub*     d,
  bl_uword               capacity,
  bl_uword               elem_size,
  bl_alloc_tbl const*    alloc
  )
{
  assert (d && alloc);
  assert (elem_size != 0);
  capacity = capacity != 0 ? bl_round_next_pow2_u (capacity) : 2;
  bl_assert (capacity >= d->capacity);
  return bl_dynarray_resize ((bl_dynarray_stub*) d, capacity, elem_size, alloc);
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_autoarray_insert_n(
  bl_autoarray_stub*  d,
  bl_uword            idx,
  void const*         c,
  bl_uword            c_count,
  bl_uword            elem_size,
  bl_alloc_tbl const* alloc
  )
{
  assert (d && (c || c_count == 0) && alloc);
  assert (idx <= d->size);
  assert (elem_size != 0);
  bl_err err     = bl_mkok();
  bl_uword required = d->size + c_count;
  if (required > d->capacity) {
    err = bl_autoarray_set_rounded_capacity (d, required, elem_size, alloc);
  }
  if (!err.bl) {
    bl_u8* arr = (bl_u8*) d->arr;
    memmove(
      arr + ((idx + c_count) * elem_size),
      arr + (idx * elem_size),
      (d->size - idx) * elem_size
      );
    memcpy (arr + (idx * elem_size), c, c_count * elem_size);
    d->size += c_count;
  }
  return err;
}
/*---------------------------------------------------------------------------*/

