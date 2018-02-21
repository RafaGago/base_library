#include <stdio.h>
#include <string.h>

#include <bl/base/platform.h>
#include <bl/base/assert.h>
#include <bl/base/utility.h>
#include <bl/base/dynarray.h>
#include <bl/base/static_integer_math.h>

#ifdef BL_POSIX

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

/*---------------------------------------------------------------------------*/
static inline int is_fifo (FILE* fp)
{
  struct stat st;
  return fstat (fileno (fp), &st) == 0 ? S_ISFIFO (st.st_mode) : -errno;
}
/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/
#define PIPE_BUF 1
#define is_fifo(...) 0 /* TBI */
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/
static bl_err dynarray_from_file_realloc_if(
  dynarray_stub*   d,
  uword            d_offset,
  uword            d_overalloc,
  uword            d_realloc_multiple_of,
  uword            file_bytes,
  alloc_tbl const* alloc
  )
{
  if (unlikely (file_bytes + d_overalloc + d_offset < file_bytes + d_offset)) {
    return bl_mkerr (bl_range);
  }
  if ((file_bytes + d_overalloc) > (d->size - d_offset)) {
    if (d_realloc_multiple_of == 0) {
      d_realloc_multiple_of = 1;
    }
    uword newsize = round_to_next_multiple(
      d_offset + file_bytes + d_overalloc, d_realloc_multiple_of
      );
    if (unlikely (newsize < d_offset + file_bytes + d_overalloc)) {
      return bl_mkerr (bl_range);
    }
    bl_err err = dynarray_resize (d, newsize, 1, alloc);
    if (unlikely (err.bl)) {
      return err;
    }
  }
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
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
  bl_err err = bl_mkok();

  bl_assert (d && d_written && fp && alloc);

  *d_written = 0;
  if (d_offset + d_overalloc < d_offset) {
    return bl_mkerr (bl_range);
  }
  int fifo = is_fifo (fp);
  if (fifo < 0) {
    return bl_mkerr_sys (bl_file, -fifo);
  }
  if (!fifo) {
    /* ftell/fseek will work */
    uword start = ftell (fp);
    if (unlikely (fseek (fp, 0L, SEEK_END))) {
      return bl_mkerr_sys (bl_file, errno);
    }
    uword total = ftell (fp);
    bl_assert(total >= start);
    if (unlikely (fseek (fp, start, SEEK_SET))) {
      return bl_mkerr_sys (bl_file, errno);
    }
    total -= start;
    if (unlikely (total == 0)) {
      return bl_mkok();
    }
    total = fp_read_limit != 0 ? bl_min (total, fp_read_limit) : total;
    err = dynarray_from_file_realloc_if(
      d, d_offset, d_overalloc, d_realloc_multiple_of, total, alloc
      );
    if (err.bl) {
      return err;
    }
    *d_written = fread (((u8*) d->arr) + d_offset, 1, total, fp);
    err.sys = (bl_err_uint) errno;
    err.bl  = ferror (fp) == 0 ? bl_ok : bl_file;
    return err;
  }
  else {
    uword alloc_size   = 0;
    uword total        = 0;
    void* b            = NULL;
    ssize_t read_bytes = 0;
    uword next_read    = 0;
    int fd             = fileno (fp);

    do {
      void* bprev = b;
      alloc_size += PIPE_BUF;
      b           = realloc (bprev, alloc_size);
      if (!b) {
        b   = bprev;
        err = bl_mkerr (bl_alloc);
        goto dealloc;
      }
      next_read = PIPE_BUF;
      if (fp_read_limit > 0) {
        next_read  = bl_min (total + PIPE_BUF, fp_read_limit);
        next_read -= total;
      }
      read_bytes = read (fd, b + total, next_read);
      if (unlikely (read_bytes < 0)) {
        err.sys = (bl_err_uint) errno;
        err.bl  = bl_file;
        goto dealloc;
      }
      total += read_bytes;
    }
    while (read_bytes == next_read && next_read == PIPE_BUF);

    err = dynarray_from_file_realloc_if(
      d, d_offset, d_overalloc, d_realloc_multiple_of, total, alloc
      );
    if (err.bl) {
      goto dealloc;
    }
    memcpy (((u8*) d->arr) + d_offset, b, total);
    *d_written = total;
  dealloc:
    free (b);
    return err;
  }
}
/*---------------------------------------------------------------------------*/
