#include <bl/base/string.h>
#include <bl/base/error.h>
#include <stdio.h>

/*----------------------------------------------------------------------------*/
BL_EXPORT int bl_vasprintf(
  char **str, alloc_tbl const* alloc, const char *format, va_list args
  )
{
  static const int guess_size = 256;
  *str = nullptr;
  char* ret = bl_alloc (alloc, guess_size);
  if (!ret) {
    return -bl_alloc;
  }
  va_list args_cp;
  va_copy (args_cp, args);
  int size = vsnprintf (ret, guess_size, format, args);
  if (size >= 0) {
    /* either get more extra bytes if truncated or shrink if it did fit */
    char* ret_realloc = bl_realloc (alloc, ret, size + 1);
    if (!ret_realloc) {
      size = -bl_alloc;
      goto end;
    }
    ret = ret_realloc;
    if (size >= guess_size) {
      /* it was truncated: retry with the right size */
      size = vsnprintf (ret, size + 1, format, args_cp);
    }
  }
end:
  va_end (args_cp);
  if (size >= 0) {
    *str = ret;
  }
  else {
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
