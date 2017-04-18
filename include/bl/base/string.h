#ifndef __BL_STRING_H__
#define __BL_STRING_H__

#include <bl/base/libexport.h>
#include <bl/base/allocator.h>

#include <stdarg.h>
/*------------------------------------------------------------------------------
str_usable_bytes: when > 0, signals that str already contains an usable buffer
  of "str_usable_bytes". The caller sees if this buffer was used or discarded by
  either comparing the output address of str for equality or by checking if the
  returned integer value is less than "str_usable_bytes" (but non negative).

first_alloc_bytes: now this call works by invoking "vsnprintf" first to retrieve
  the formatted string size and then by invoking again with the right size. As
  the required size isn't know before invoking, a buffer of a small size is
  allocated first to see if one of these calls can be saved. The buffer size is
  readjusted afterwards by using realloc. This is ignored when
  "str_usable_bytes" is > 0.
------------------------------------------------------------------------------*/
extern BL_EXPORT int bl_vasprintf_ext(
  char**           str,
  int              str_usable_bytes, /* str points to an allocated buffer */
  int              first_alloc_bytes, /* see source */
  alloc_tbl const* alloc,
  char const*      format,
  va_list          args
  );
/*----------------------------------------------------------------------------*/
static inline int bl_vasprintf(
  char **str, alloc_tbl const* alloc, const char *format, va_list args
  )
{
    return bl_vasprintf_ext (str, 0, 256, alloc, format, args);
}
/*----------------------------------------------------------------------------*/
extern BL_EXPORT int bl_asprintf(
  char **str, alloc_tbl const* alloc, const char *format, ...
  );
/*----------------------------------------------------------------------------*/

#endif /* __BL_STRING_H__ */
