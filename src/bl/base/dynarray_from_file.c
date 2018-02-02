#include <stdio.h>

#include <bl/base/assert.h>
#include <bl/base/utility.h>
#include <bl/base/dynamic_string.h>
#include <bl/base/static_integer_math.h>

bl_err dynarray_from_file(
  dynarray_stub*   d,
  uword*           d_written,
  uword            d_offset,
  uword            d_overalloc,
  uword            d_realloc_multiple_of,
  FILE*            fp,
  uword            fp_read_limit,
  alloc_tbl const* alloc
  )
{
  bl_assert (d && d_written && fp);
  uword start = ftell (fp);
  if (unlikely (fseek (fp, 0L, SEEK_END))) {
    return bl_file;
  }
  uword rsize = ftell (fp);
  bl_assert(rsize >= start);
  if (unlikely (fseek (fp, start, SEEK_SET))) {
    return bl_file;
  }
  rsize -= start;
  if (unlikely (rsize == 0)) {
    return bl_ok;
  }
  rsize = fp_read_limit != 0 ? bl_min (rsize, fp_read_limit) : rsize;
  if (unlikely (rsize + d_overalloc < rsize)) {
    return bl_range;
  }
  if ((rsize + d_overalloc) > (d->size - d_offset)) {
    if (d_realloc_multiple_of == 0) {
      d_realloc_multiple_of = 1;
    }
    uword newsize = round_to_next_multiple(
      d_offset + rsize + d_overalloc, d_realloc_multiple_of
      );
    if (unlikely (newsize < rsize + d_overalloc)) {
      return bl_range;
    }
    bl_err err = dynarray_resize (d, newsize, 1, alloc);
    if (unlikely (err)) {
      return err;
    }
  }
  *d_written = fread (((u8*) d->arr) + d_offset, 1, rsize, fp);
  return ferror (fp) == 0 ? bl_ok : bl_file;
}
