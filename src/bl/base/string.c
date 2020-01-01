#include <stdio.h>

#include <bl/base/assert.h>
#include <bl/base/string.h>
#include <bl/base/utility.h>
#include <bl/base/error.h>
#include <bl/base/default_allocator.h>

#ifndef BL_VASPRINTF_DEFAULT_HINT
  #define BL_VASPRINTF_DEFAULT_HINT 128
#endif
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_vasnprintf(
  bl_dstrbuf* b, size_t strlen_hint, char const* format, va_list args
  )
{
  bl_assert (b);
  bl_assert (b->str != nullptr || b->maxlen == 0);

  bl_alloc_tbl const* alloc = b->alloc ? b->alloc : &bl_default_alloc;
  size_t maxlen = b->len;
  maxlen += (strlen_hint != 0) ? strlen_hint : (BL_VASPRINTF_DEFAULT_HINT - 1);

  bl_err err = bl_mkerr (bl_error);

  va_list args_cp;
  va_copy (args_cp, args);

  for (int i = 0; i < 2; ++i) {
    if (!b->str || b->maxlen < maxlen) {
      void* nbuff = (char*) bl_realloc (alloc, b->str, maxlen + 1);
      if (bl_unlikely (!nbuff)) {
        err = bl_mkerr (bl_alloc);
        break;
      }
      b->str    = nbuff;
      b->maxlen = maxlen;
    }
    int required;
    size_t available = maxlen - b->len;
    if (i == 0) {
      required = vsnprintf (b->str + b->len, available + 1, format, args);
    }
    else {
      required = vsnprintf (b->str + b->len, available + 1, format, args_cp);
    }
    if (required <= available) {
      if (bl_likely (required > 0)) {
        b->len += required;
        bl_assert ((b->len + required) > b->len); /* overflow */
        err = bl_mkok();
      }
      else {
        err = bl_mkerr (bl_invalid);
      }
      break;
    }
    maxlen = b->len + required; /* retry with the right size */
    bl_assert ((b->len + required) > b->len); /* overflow */
  }
  va_end (args_cp);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_asnprintf(
  bl_dstrbuf* buffer, size_t strlen_hint, char const* format, ...
  )
{
  va_list args;
  va_start (args, format);
  bl_err e = bl_vasnprintf (buffer, strlen_hint, format, args);
  va_end (args);
  return e;
}
/*----------------------------------------------------------------------------*/
