#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <bl/base/platform.h>
#include <bl/base/assert.h>
#include <bl/base/utility.h>
#include <bl/base/dynarray.h>
#include <bl/base/static_integer_math.h>

#if BL_OS_IS_MOSTLY_POSIX

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
#define is_fifo(...) 0
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/
static bl_err bl_dynarray_from_file_realloc_if(
  bl_dynarray_stub*   d,
  bl_uword            d_offset,
  bl_uword            d_overalloc,
  bl_uword            d_realloc_multiple_of,
  bl_uword            file_bytes,
  bl_alloc_tbl const* alloc
  )
{
  if (bl_unlikely (file_bytes + d_overalloc + d_offset < file_bytes + d_offset)) {
    return bl_mkerr (bl_range);
  }
  if ((file_bytes + d_overalloc) > (d->size - d_offset)) {
    if (d_realloc_multiple_of == 0) {
      d_realloc_multiple_of = 1;
    }
    bl_uword newsize = bl_round_to_next_multiple(
      d_offset + file_bytes + d_overalloc, d_realloc_multiple_of
      );
    if (bl_unlikely (newsize < d_offset + file_bytes + d_overalloc)) {
      return bl_mkerr (bl_range);
    }
    bl_err err = bl_dynarray_resize (d, newsize, 1, alloc);
    if (bl_unlikely (err.own)) {
      return err;
    }
  }
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dynarray_from_file(
  bl_dynarray_stub*   d,
  bl_uword*           d_written,
  bl_uword            d_offset,
  bl_uword            d_overalloc,
  bl_uword            d_realloc_multiple_of,
  FILE*               fp,
  bl_uword            fp_read_limit,
  bl_alloc_tbl const* alloc
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
    bl_uword start = ftell (fp);
    if (bl_unlikely (fseek (fp, 0L, SEEK_END))) {
      return bl_mkerr_sys (bl_file, errno);
    }
    bl_uword total = ftell (fp);
    bl_assert(total >= start);
    if (bl_unlikely (fseek (fp, start, SEEK_SET))) {
      return bl_mkerr_sys (bl_file, errno);
    }
    total -= start;
    if (bl_unlikely (total == 0)) {
      return bl_mkok();
    }
    total = fp_read_limit != 0 ? bl_min (total, fp_read_limit) : total;
    err = bl_dynarray_from_file_realloc_if(
      d, d_offset, d_overalloc, d_realloc_multiple_of, total, alloc
      );
    if (err.own) {
      return err;
    }
    *d_written = fread (((bl_u8*) d->arr) + d_offset, 1, total, fp);
    err.sys = (bl_err_uint) errno;
    err.own  = ferror (fp) == 0 ? bl_ok : bl_file;
  }
#if BL_OS_IS_MOSTLY_POSIX
  else {
    bl_uword alloc_size = 0;
    bl_uword total      = 0;
    void* b             = NULL;
    bl_word  read_bytes = 0;
    bl_uword next_read  = 0;
    int fd              = fileno (fp);

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
      read_bytes = read (fd, ((bl_u8*) b) + total, next_read);
      if (bl_unlikely (read_bytes < 0)) {
        err.sys = (bl_err_uint) errno;
        err.own  = bl_file;
        goto dealloc;
      }
      total += read_bytes;
    }
    while (read_bytes == next_read && next_read == PIPE_BUF);

    err = bl_dynarray_from_file_realloc_if(
      d, d_offset, d_overalloc, d_realloc_multiple_of, total, alloc
      );
    if (err.own) {
      goto dealloc;
    }
    memcpy (((bl_u8*) d->arr) + d_offset, b, total);
    *d_written = total;
  dealloc:
    free (b);
  }
#endif
  return err;
}
/*---------------------------------------------------------------------------*/
