#include <bl/base/dynarray.h>
#include <bl/base/autoarray.h>
#include <bl/base/integer_math.h>
#include <bl/base/utility.h>
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err autoarray_set_rounded_capacity(
  autoarray_stub*     d,
  uword               capacity,
  uword               elem_size,
  alloc_tbl const*    alloc
  )
{
  assert (d && alloc);
  assert (elem_size != 0);
  capacity = capacity != 0 ? round_next_pow2_u (capacity) : 2;
  bl_assert (capacity >= d->capacity);
  return dynarray_resize ((dynarray_stub*) d, capacity, elem_size, alloc);
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err autoarray_insert_n(
  autoarray_stub*     d,
  uword               idx,
  void const*         c,
  uword               c_count,
  uword               elem_size,
  alloc_tbl const*    alloc
  )
{
  assert (d && (c || c_count == 0) && alloc);
  assert (idx <= d->size);
  assert (elem_size != 0);
  bl_err err     = bl_ok;
  uword required = d->size + c_count;
  if (required > d->capacity) {
    err = autoarray_set_rounded_capacity (d, required, elem_size, alloc);
  }
  if (!err) {
    u8* arr = (u8*) d->arr;
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

