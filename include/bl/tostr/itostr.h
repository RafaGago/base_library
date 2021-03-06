#ifndef __BL_ITOSTR_H__
#define __BL_ITOSTR_H__

#include <bl/tostr/libexport.h>

#include <bl/base/integer.h>
#include <bl/base/error.h>
#include <bl/base/allocator.h>
#include <bl/base/static_integer_math.h>
#include <bl/base/stringbuffer.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/

/*
Conversion from integer types to strings.

These functions use the printf format, with the next difference:

- no "%" char required.
- no ".*" precision allowed.
- no length specifiers required.
- adds the "m" precision specifier which expands to the width of the maximum
  value of a type, either in decimal, hexadecimal or octal.
*/

/*------------------------------------------------------------------------------
writes an integral type to a string representation.

Parameters:
  buffer:
    .str:
      buffer to write into.
    .len:
      on input:
        If this value is non-zero it assumes that ".str" already contains a
        string, so this function starts writing (concatenating) after ".len"
        bytes.
      on output:
        The total length of the string contained in ".str".
    .maxlen:
      size of the "buffer.str" buffer, excluding trailing 0.
  fmt:
    printf-like format string without type and "%"
  v:
    the number to convert

Return:
  bl_ok:             success
  bl_invalid:        invalid parameters
  bl_would_overflow: dst buffer too small

You are meant to use the "bl_itostr_u" and "bl_itostr_i" macros for unsigned
integer and signed integer types respectively instead of this function.
------------------------------------------------------------------------------*/
extern BL_TOSTR_EXPORT bl_err bl_itostr(
  bl_strbuf*  buffer,
  char const* fmt,
  bl_u64      v,
  bool        v_is_negative,
  bl_uword    v_type_bytes_log2
  );

/* for unsigned types */
#define bl_itostr_u(b, f, v)\
  bl_itostr ((b), (f), (v), false, bl_static_log2_ceil_u8 (sizeof (v)))

/* for signed types */
#define bl_itostr_i(b, f, v)\
  bl_itostr( \
    (b), (f), (bl_u64) (v), (v) < 0, bl_static_log2_ceil_u8 (sizeof (v)) \
    )
/*------------------------------------------------------------------------------
writes an integral type array to a string representation.

Parameters:

  buffer:
    .str:
      buffer to write into.
    .len:
      on input:
        If this value is non-zero it assumes that ".str" already contains a
        string, so this function starts writing (concatenating) after ".len"
        bytes.
      on output:
        The total length of the string contained in ".str".
    .maxlen:
      size of the "buffer.str" buffer, excluding trailing 0.
  fmt:
    printf-like format string without type and "%"
  sep:
    value separator, e.g. ", "
  v:
    pointer to the array
  v_count:
    number of elements in the array

Return:
  bl_ok:             success
  bl_invalid:        invalid parameters
  bl_would_overflow: dst buffer too small

You are meant to use the "bl_itostr_arr_u" and "bl_itostr_arr_i" macros for
unsigned integer and signed integer types respectively instead of this function.
------------------------------------------------------------------------------*/
extern BL_TOSTR_EXPORT bl_err bl_itostr_arr(
  bl_strbuf*  buffer,
  char const* fmt,
  char const* sep,
  void const* v,
  size_t      v_count,
  bool        v_type_is_signed,
  bl_uword    v_type_bytes_log2
  );

/* for unsigned types */
#define bl_itostr_arr_u(b, f, s, v, vc)\
  bl_itostr_arr( \
    (b), \
    (f), \
    (s), \
    (void const*) &(v)[0], \
    (vc), \
    false, \
    bl_static_log2_ceil_u8 (sizeof ((v)[0])) \
    )

/* for signed types */
#define bl_itostr_arr_i(b, f, s, v, vc)\
  bl_itostr_arr( \
    (b), \
    (f), \
    (s), \
    (void const*) &(v)[0], \
    (vc), \
    true, \
    bl_static_log2_ceil_u8 (sizeof ((v)[0])) \
    )
/*------------------------------------------------------------------------------
writes an integral type to a string representation using dynamic memory
allocations.

Parameters:

  buffer:
    Memory to write into.

    .str
      on input:
        If the pointer points to NULL the memory will be freshly allocated. If
        it doesn't "realloc" will be used on the passed buffer pointer.
      on output:
        The buffer containing the string.

    .maxlen:
      on input:
        If "*str" contains a non-NULL value, contains the current number of
        characters that the whole "dst" buffer can fit excluding the 0 string
        terminator. E.g. if "*dst" is non-NULL and this is 2 "*dst" should have
        room for 2 chars and a 0 termination.
      on output:
        Will contain the total string capacity (the buffer size minus one byte
        for the 0 termination).

    .len:
      on input:
        If this value is non-zero it assumes that ".str" already contains a
        string, so this function starts writing (concatenating) after ".len"
        bytes.

        If reallocations are necessary, bytes from 0 to ".len" will be
        respected on the outputed string.
      on output:
        The total length of the string contained in ".str".

    .alloc:
      The allocator to use for (re)allocating. If ".str" is non-NULL it has to
      be the same that was used to obtain ".str", as "realloc" will be called on
      it.

      If this value is NULL the C standard library allocation functions will be
      used.

  fmt:
    printf-like format string without type and "%"

  v:
    the number to convert

Return:
  bl_ok:      success
  bl_invalid: invalid parameters
  bl_alloc:   allocation error

You are meant to use the "bl_itostr_dyn_u" and "bl_itostr_dyn_i" macros for
unsigned integer and signed integer types respectively instead of this function.

Notice too "dst_maxlen" doesn't include the null terminator, so this function
can be used seamlessly with "bl_dstr" by using "bl_dstr_steal_ownership" and
"bl_dstr_transfer_ownership_lc".
------------------------------------------------------------------------------*/
extern BL_TOSTR_EXPORT bl_err bl_itostr_dyn(
  bl_dstrbuf* buffer,
  char const* fmt,
  bl_u64      v,
  bool        v_is_negative,
  bl_uword    v_type_bytes_log2
  );

/* for unsigned types */
#define bl_itostr_dyn_u(b, f, v)\
  bl_itostr_dyn ((b), (f), (v), false, bl_static_log2_ceil_u8 (sizeof (v)))

/* for signed types */
#define bl_itostr_dyn_i(b, f, v)\
  bl_itostr_dyn( \
    (b), \
    (f), \
    (bl_u64) (v), \
    (v) < 0 , \
    bl_static_log2_ceil_u8 (sizeof (v)) \
    )
/*------------------------------------------------------------------------------
writes an integral type array to a string representation using dynamic memory
allocations.

Parameters:

  buffer:
    Memory to write into.

    .str
      on input:
        If the pointer points to NULL the memory will be freshly allocated. If
        it doesn't "realloc" will be used on the passed buffer pointer.
      on output:
        The buffer containing the string.

    .maxlen:
      on input:
        If "*str" contains a non-NULL value, contains the current number of
        characters that the whole "dst" buffer can fit excluding the 0 string
        terminator. E.g. if "*dst" is non-NULL and this is 2 "*dst" should have
        room for 2 chars and a 0 termination.
      on output:
        Will contain the total string capacity (the buffer size minus one byte
        for the 0 termination).

    .len:
      on input:
        If this value is non-zero it assumes that "dst" already contains a
        string, so this function starts writing (concatenating) after ".len"
        bytes.

        If reallocations are necessary, bytes from 0 to ".len" will be
        respected on the outputed string.
      on output:
        The total length of the string contained in ".str".

    .alloc:
      The allocator to use for (re)allocating. If ".str" is non-NULL it has to
      be the same that was used to obtain ".str", as "realloc" will be called on
      it.

      If this value is NULL the C standard library allocation functions will be
      used.

  fmt:
    printf-like format string without type and "%"
  sep:
    value separator, e.g. ", "
  v:
    pointer to the array
  v_count:
    number of elements in the array

Return:
  bl_ok:      success
  bl_invalid: invalid parameters
  bl_alloc:   allocation error

You are meant to use the "bl_itostr_dyn_arr_u" and "bl_itostr_dyn_arr_i" macros
for unsigned integer and signed integer types respectively instead of this
function.

Notice too "dst_maxlen" doesn't include the null terminator, so this function
can be used seamlessly with "bl_dstr" by using "bl_dstr_steal_ownership" and
"bl_dstr_transfer_ownership_lc".

This function might overallocate, you can call "bl_dstrbuf_try_trim" if
overallocations are not desired.
------------------------------------------------------------------------------*/
extern BL_TOSTR_EXPORT bl_err bl_itostr_dyn_arr(
  bl_dstrbuf* buffer,
  char const* fmt,
  char const* sep,
  void const* v,
  size_t      v_count,
  bool        v_type_is_signed,
  bl_uword    v_type_bytes_log2
  );

/* for unsigned types */
#define bl_itostr_dyn_arr_u(b, f, s, v, vc)\
  bl_itostr_dyn_arr( \
    (b), \
    (f), \
    (s), \
    (void const*) &(v)[0], \
    (vc), \
    false, \
    bl_static_log2_ceil_u8 (sizeof (v)[0]) \
    )

/* for signed types */
#define bl_itostr_dyn_arr_i(b, f, s, v, vc)\
  bl_itostr_dyn_arr( \
    (b), \
    (f), \
    (s), \
    (void const*) &(v)[0], \
    (vc), \
    true, \
    bl_static_log2_ceil_u8 (sizeof (v)[0]) \
    )
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} // extern "C" {
#endif
#endif /* _BL_ITOSTR_H__*/