#include <stdio.h>

#include <bl/base/string.h>
#include <bl/base/utility.h>
#include <bl/base/error.h>

/*----------------------------------------------------------------------------*/
BL_EXPORT int bl_vasprintf_ext(
  char**           str,
  int              str_alloc_size,
  int              str_offset,
  int              do_realloc,
  alloc_tbl const* alloc,
  char const*      format,
  va_list          args
  )
{
  bl_assert(alloc && format && str);
  char* out_buff;
  char* in_buff      = *str;
  int   usable_bytes = str_alloc_size - str_offset;
  *str               = nullptr;

  if (unlikely(
      str_alloc_size < 1 || usable_bytes <= 0 || str_offset < 0
      )) {
    return -bl_invalid;
  }
  if (in_buff) {
    out_buff = in_buff;
  }
  else {
    out_buff   = bl_alloc (alloc, str_alloc_size);
    do_realloc = 1;
    if (unlikely (!out_buff)) {
      return -bl_alloc;
    }
  }
  va_list args_cp;
  va_copy (args_cp, args);

  int size = vsnprintf(out_buff + str_offset, usable_bytes, format, args);
  if (size < usable_bytes) {
    size = (size >= 0) ? size : -bl_invalid;
    goto end;
  }

  int required_bytes  = str_offset + size + 1;
  char* out_buff_prev = out_buff;
  out_buff = bl_realloc(
    alloc, do_realloc ? out_buff : nullptr, required_bytes
    );
  if (unlikely (!out_buff)) {
    if (!in_buff) {
      bl_dealloc (alloc, out_buff_prev);
    }
    return -bl_alloc;
  }

  size = vsnprintf (out_buff + str_offset, required_bytes, format, args_cp);
  bl_assert (size == required_bytes - str_offset - 1);
end:
  va_end (args_cp);
  if (likely (size >= 0)) {
    *str = out_buff;
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
