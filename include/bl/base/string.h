#ifndef __BL_STRING_H__
#define __BL_STRING_H__

#include <stdarg.h>

#include <bl/base/libexport.h>
#include <bl/base/allocator.h>
#include <bl/base/assert.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------------
bl_vasprintf_ext: vasprintf with reallocation and offset (append) capabilities.

PARAMETERS:

It has two behaviors/use cases depending on if the pointer pointed by "str" is
null or not. When it's null:

str: Null at input, points to the newly allocated formatted string at output.

str_alloc_size: An initial buffer of "str_alloc_size" will be allocated. This
  buffer will be used at the same time that the formatted string length is
  computed so in case it has enough space the operation can complete in a single
  step. The value has to be at least 1.

str_offset: Extra room will be left at the head of the allocated buffer (free
  for other uses after the call).

do_realloc: The value is ignored.

When the pointer pointed by "str" isn't null:

str: At input time points a buffer (see "do_realloc" for constraints on it). On
  return it contains the formatted string.

str_alloc_size: The total size (bytes) of this buffer.

str_offset: offset bytes to begin to write the string to. This can be used e.g.
  to implement string appending.

do_realloc: Specifies if the buffer pointed to by "str" can be reallocated by
  the provided allocator ("alloc" parameter).

  When this is nonzero "realloc" may be called on the provided "str" buffer, so
  the buffer can _never_ point to buffers .e.g on the stack, buffers provided by
  another allocator or buffers provided by the passed allocator whose address is
  at an offset.

RETURNS:
  negative bl_error or the length of the string without null termination and
  excluding "str_offset". On error the string pointed by str will always be
  null.

NOTE that:
  -On return the address pointed by ptr can change.
  -The address pointed by str may not change but its buffer size may have been
   expanded via "realloc". On successful calls the size of the buffer pointed
   by str will either be "str_alloc_size" or "str_offset" + "return value" + 1.
  -When "do_realloc": Is false the buffer pointed by str will never be
   reallocated, so if the function needed a new buffer and the provided buffer
   was on the heap you may need to deallocate it manually.
------------------------------------------------------------------------------*/
extern BL_EXPORT int bl_vasprintf_ext(
  char**           str,
  int              str_alloc_size,
  int              str_offset,
  int              do_realloc,
  bl_alloc_tbl const* alloc,
  char const*         format,
  va_list             args
  );
/*----------------------------------------------------------------------------*/
static inline int bl_vasprintf(
  char **str, bl_alloc_tbl const* alloc, const char *format, va_list args
  )
{
    *str = nullptr;
    return bl_vasprintf_ext (str, 256, 0, 0, alloc, format, args);
}
/*----------------------------------------------------------------------------*/
extern BL_EXPORT int bl_asprintf(
  char **str, bl_alloc_tbl const* alloc, const char *format, ...
  );
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} //extern "C" {
#endif

#endif /* __BL_STRING_H__ */
