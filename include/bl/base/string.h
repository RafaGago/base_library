#ifndef __BL_STRING_H__
#define __BL_STRING_H__

#include <stdarg.h>

#include <bl/base/error.h>
#include <bl/base/platform.h>
#include <bl/base/stringbuffer.h>
#include <bl/base/libexport.h>
#include <bl/base/allocator.h>
#include <bl/base/assert.h>
#include <bl/base/preprocessor_basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------
bl_vasprintf_hint: vasprintf with reallocation and offset (append) capabilities.

This functions works by doing a dummy call to "vsnprintf" to retrieve the
the size and then executing "vsnprintf" again with a big enough buffer if
there wasn't enough buffer space available.

PARAMETERS:

  buffer:
    Buffer data.

    "buffer.str":
      An existing buffer or NULL. This buffer will be newly alloated or
      reallocated if necessary.

    "buffer.len":
      At input:
        "bl_vasprintf_ext" will start writing at "buffer.str" + len,
        independently of if the passed buffer is freshly allocated or existed
        before.

      At output.
        Will contain the total string length (against buffer.str[0]).

    -"buffer.maxlen":
      At input:
        The actual string capacity on "buffer.str" (not counting the trailing
        null)

      At output:
        The new string capacity.

    Has to be zero when "buffer.str" is NULL

    When "buffer.str" is not NULL:

      -"buffer.len": bl_vasprintf_ext"

       -"buffer.maxlen": Has to be zero.

   -"buffer.alloc": allocator. When NULL the standard library's heap will be
    used.

  strlen_hint:
    If this parameter is non-zero, "strlen_hint" bytes will be allocated after
    "buffer.str" + "buffer.len" if necessary before doing the first "vsnprintf"
    call, so the first call has more chances to succeed.

  max_tail_bytes:
    The amount of acceptable overallocation on exit. If
    "buffer.maxlen" - "buffer.len" exceeds this value the buffer will be tried
    to be trimmed to "buffer.len" + "max_tail_bytes" at exit. This is a
    best-effort parameter. A failure to trim the buffer doesn't signal an error.
------------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_vasnprintf(
  bl_dstrbuf* b, size_t strlen_hint, char const* format, va_list args
  );
/*----------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_asnprintf(
  bl_dstrbuf* buffer, size_t strlen_hint, char const* format, ...
  )
  BL_PRINTF_FORMAT (3, 4);
/*----------------------------------------------------------------------------*/
/* A compile-time estimate about the probable size of a printf string. Can be
improved by using C11

Notice that to avoid macro expansion pitfalls, the format string is included
as the first argument on __VA_ARGS__
*/
/*----------------------------------------------------------------------------*/
#define BL_PRINTF_LEN_HINT_ARGB(avg_arg_bytes, ...) \
  (sizeof ("" bl_pp_vargs_first (__VA_ARGS__)) - sizeof ("") + \
    ((bl_pp_nonzero_vargs_count (__VA_ARGS__) - 1) * (avg_arg_bytes)) \
    )
/*----------------------------------------------------------------------------*/
#define BL_PRINTF_LEN_HINT(...) \
  BL_PRINTF_LEN_HINT_ARGB(11, __VA_ARGS__)
/*----------------------------------------------------------------------------*/
/* vasnprintf with a BL_PRINTF_LEN_HINT_AB */
#define bl_asnprintf_h(buffer, ...) \
  bl_asprintf ((buffer), BL_PRINTF_LEN_HINT (__VA_ARGS__), __VA_ARGS__)
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} //extern "C" {
#endif

#endif /* __BL_STRING_H__ */
