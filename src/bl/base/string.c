#include <bl/base/string.h>
#include <bl/base/assert.h>
#include <bl/base/utility.h>
#include <bl/base/error.h>
#include <stdio.h>

/*----------------------------------------------------------------------------*/
BL_EXPORT int bl_vasprintf_ext(
  char**           str,
  int              str_usable_bytes,
  int              alloc_bytes,
  alloc_tbl const* alloc,
  char const*      format,
  va_list          args
  )
{
  char* ret;
  char* in_buff = *str;
  *str          = nullptr;
  if (str_usable_bytes <= 1) {
    bl_assert (alloc_bytes > 1);
    in_buff = nullptr;
    ret     = bl_alloc (alloc, alloc_bytes);
    if (unlikely (!ret)) {
      return -bl_alloc;
    }
  }
  else {
    alloc_bytes = str_usable_bytes;
    ret         = in_buff;
  }
  va_list args_cp;
  va_copy (args_cp, args);
  int size = vsnprintf (ret, alloc_bytes, format, args);
  if (unlikely (size < 0)) {
    goto end;
  }
  if (str_usable_bytes >= 1) {
    if (size < str_usable_bytes) {
      /* successfully used the provided buffer, no realloc on the provided
         buffer, jump to end */
      goto end;
    }
    /* the provided buffer didn't fit, will reallocate a new buffer */
    ret = nullptr;
  }
  /* setting buffer to the correct size */
  char* ret_realloc = bl_realloc (alloc, ret, size + 1);
  if (unlikely (!ret_realloc)) {
    size = -bl_alloc;
    goto end;
  }
  ret = ret_realloc;
  if (size >= alloc_bytes) {
    /* it was truncated: retry with the right size */
    size = vsnprintf (ret, size + 1, format, args_cp);
  }
end:
  va_end (args_cp);
  if (likely (size >= 0)) {
    *str = ret;
  }
  else if (ret && ret != in_buff) {
    bl_dealloc (alloc, ret);
  }
  return size;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT int bl_asprintf(
  char **str, alloc_tbl const* alloc, const char *format, ...
  )
{
  va_list args;
  va_start (args, format);
  int count = bl_vasprintf (str, alloc, format, args);
  va_end (args);
  return count;
}
/*----------------------------------------------------------------------------*/
