#include <string.h>

#include <bl/base/utility.h>
#include <bl/base/assert.h>
#include <bl/base/static_integer_math.h>
#include <bl/base/default_allocator.h>

#include <bl/tostr/itostr.h>
/*----------------------------------------------------------------------------*/
typedef enum bl_itostr_fmt_mode {
  bl_itostr_fmt_decimal,
  bl_itostr_fmt_hex_lower,
  bl_itostr_fmt_hex_upper,
  bl_itostr_fmt_octal,
}
bl_itostr_fmt_mode;
/*----------------------------------------------------------------------------*/
typedef enum bl_itostr_fmt_sign {
  bl_itostr_fmt_sign_default,
  bl_itostr_fmt_force_sign,
  bl_itostr_fmt_space_positive,
}
bl_itostr_fmt_sign;
/*----------------------------------------------------------------------------*/
typedef struct bl_itostr_fmt {
  bl_uword           width;
  bl_uword           precision;
  bl_uword           max_digits;
  char const*        prefix;
  bl_uword           prefix_len;
  bl_itostr_fmt_sign sign;
  bl_itostr_fmt_mode mode;
  bool               left_justify;
  bool               swallow_zero;
  char               padding;
}
bl_itostr_fmt;
/*----------------------------------------------------------------------------*/
#define BL_ITOSTR_MAX_SEP 8
/*----------------------------------------------------------------------------*/
typedef char bl_itostr_buffer[24]; /*any integer fits here, no sign, no null */
/*----------------------------------------------------------------------------*/
bl_err bl_itostr_parse_fmt(
  bl_itostr_fmt* f, char const* fstr, bl_uword type_bytes
  )
{
  bl_assert (f);
  bl_assert (bl_is_pow2 (type_bytes));
  bl_assert (type_bytes <= 8);

  memset (f, 0, sizeof *f);
  f->padding    = ' ';
  f->max_digits = (type_bytes != 1) ? ((type_bytes / 2) * 5) : 3;
  char const* fmt = fstr;

  if (!fmt || *fmt == 0) {
    return bl_mkok();
  }

  bool prefix_enabled  = false;
  bool max_precision   = false;
  bl_uword numeric_multi;

  /* flags */
  char const* block_start = fmt;
  while (true) {
    switch (*fmt) {
      case ' ':
        f->sign = bl_itostr_fmt_space_positive;
        break;
      case '#':
        prefix_enabled = true;
        break;
      case '+':
        f->sign = bl_itostr_fmt_force_sign;
        break;
      case '-':
        f->left_justify = true;
        f->padding = ' '; /* no zero padding hwn left justified */
        break;
      case '0':
        f->padding = f->left_justify ? ' ' : '0';
        break;
      default:
        goto process_width;
        break;
    }
    ++fmt;
    if (bl_unlikely (5 < fmt - block_start)) {
      /* insane format */
      return bl_mkerr (bl_invalid);
    }
  }

process_width:
  block_start = fmt;
  numeric_multi = 100;
  while (true) {
    if (*fmt >= '0' && *fmt <= '9') {
      numeric_multi /= 10;
      f->width += (*fmt - '0') * numeric_multi;
    }
    else {
      f->width /= numeric_multi;
      goto process_precision;
    }
    ++fmt;
    if (bl_unlikely (2 < fmt - block_start)) {
      /* restricted at two digits */
      return bl_mkerr (bl_invalid);
    }
  }

process_precision:
  if (*fmt != '.') {
    goto process_specifiers;
  }
  ++fmt;
  block_start = fmt;
  numeric_multi = 100;
  while (true) {
    if (*fmt >= '0' && *fmt <= '9') {
      if (bl_unlikely (max_precision)) {
        /* 'w' already set */
        return bl_mkerr (bl_invalid);
      }
      numeric_multi /= 10;
      f->precision += (*fmt - '0') * numeric_multi;
    }
    else if (*fmt == 'w') {
      if (bl_unlikely (numeric_multi != 100)) {
        /* some numeric value was set */
        return bl_mkerr (bl_invalid);
      }
      max_precision = true;
      f->precision  = 0;
    }
    else {
      if (numeric_multi != 100) {
        f->precision   /= numeric_multi;
        f->swallow_zero = (f->precision == 0);
      }
      else if (bl_unlikely (!max_precision)) {
        /* '.' with no numeric of 'm' specifiers */
        return bl_mkerr (bl_invalid);
      }
      f->padding = ' '; /* no zero padding with precision enabled */
      goto process_specifiers;
    }
    ++fmt;
    if (bl_unlikely (2 < fmt - block_start)) {
      /* restricted at two digits */
      return bl_mkerr (bl_invalid);
    }
  }

process_specifiers:
  if (*fmt == 'x' || *fmt == 'X') {
    f->mode = (*fmt == 'x') ? bl_itostr_fmt_hex_lower : bl_itostr_fmt_hex_upper;
    f->max_digits = type_bytes * 2;
    if (prefix_enabled) {
      f->prefix = (*fmt == 'x') ? "0x" : "0X";
      f->prefix_len = sizeof "0x" - 1;
    }
    ++fmt;
  }
  else if (*fmt == 'o') {
    f->max_digits = type_bytes * 3;
    f->mode = bl_itostr_fmt_octal;
    if (prefix_enabled) {
      f->prefix = "0";
      f->prefix_len = sizeof "0" - 1;
    }
    ++fmt;
  }
  if (bl_unlikely (*fmt != 0)) {
    return bl_mkerr (bl_invalid);
  }
  f->precision = max_precision ? f->max_digits : f->precision ;
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
static bl_uword bl_itostr_get_reversed_decimal(
  char* b, bl_u64 v, bool negative
  )
{
  bl_uword digits = 0;

  bl_u64 rem = !negative ? v : bl_two_complement_u (v);
  while (rem >= 99999) {
    b[digits++] = ((char) (rem % 10)) + '0';
    rem = rem / 10;
    b[digits++] = ((char) (rem % 10)) + '0';
    rem = rem / 10;
    b[digits++] = ((char) (rem % 10)) + '0';
    rem = rem / 10;
    b[digits++] = ((char) (rem % 10)) + '0';
    rem = rem / 10;
    b[digits++] = ((char) (rem % 10)) + '0';
    rem = rem / 10;
  }
  while (rem > 0) {
    b[digits++] = ((char) (rem % 10)) + '0';
    rem = rem / 10;
  }
  return digits;
}
/*----------------------------------------------------------------------------*/
static bl_uword bl_itostr_get_reversed_hex(
  char* b, bl_u64 v, bl_uword type_bytes, char const* lut
  )
{
  bl_uword digits = 0;

  bl_u64 rem = v & bl_u64_lsb_set (type_bytes * 8);
  while (rem >= 0x100000) {
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
  }
  while (rem >= 0x10) {
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
  }
  if (rem > 0) {
    b[digits++] = lut[rem & 15];
    rem = rem >> 4;
  }
  return digits;
}
/*----------------------------------------------------------------------------*/
static bl_uword bl_itostr_get_reversed_octal(
  char* b, bl_u64 v, bl_uword type_bytes
  )
{
  bl_uword digits = 0;

  bl_u64 rem = v & bl_u64_lsb_set (type_bytes * 8);
  while (rem >= 0x100000) {
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
  }
  while (rem >= 0x100) {
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
  }
  while (rem > 0) {
    b[digits++] = (rem & 7) + '0';
    rem = rem >> 3;
  }
  return digits;
}
/*----------------------------------------------------------------------------*/
typedef struct bl_itostr_len {
  bl_uword numerals; /* only numeral digits. includes precision 0 padding*/
  bl_uword number;   /* number + sign or prefixes */
  bl_uword total;    /* including width */
}
bl_itostr_len;
/*----------------------------------------------------------------------------*/
static bl_itostr_len bl_itostr_compute_len(
  bl_itostr_fmt const* fmt, bl_uword digits, bool is_negative
  )
{
  bl_itostr_len s;
  s.numerals = bl_max (digits, fmt->precision);
  s.number   = s.numerals + fmt->prefix_len;
  if (fmt->mode == bl_itostr_fmt_decimal) {
    s.number +=
      (is_negative || fmt->sign != bl_itostr_fmt_sign_default) ? 1 : 0;
  }
  s.total = bl_max (s.number, fmt->width);
  return s;
}
/*----------------------------------------------------------------------------*/
static bl_err bl_itostr_out_fmt(
  bl_strbuf*           buf,
  bl_itostr_fmt const* fmt,
  bl_u64               v,
  bool                 v_is_negative,
  bl_uword             v_type_bytes
  )
{
  bl_assert (buf && buf->str && fmt);
  bl_itostr_buffer rev_num;
  bl_uword digits = 0;

  switch (fmt->mode) {
    case bl_itostr_fmt_decimal:
      digits = bl_itostr_get_reversed_decimal (rev_num, v, v_is_negative);
      break;
    case bl_itostr_fmt_hex_upper:
    case bl_itostr_fmt_hex_lower:
      digits = bl_itostr_get_reversed_hex(
        rev_num,
        v,
        v_type_bytes,
        fmt->mode == bl_itostr_fmt_hex_upper
          ? "0123456789ABCDEF"
          : "0123456789abcdef"
        );
      break;
    case bl_itostr_fmt_octal:
      digits = bl_itostr_get_reversed_octal (rev_num, v, v_type_bytes);
      break;
  }
  bl_itostr_len flen = bl_itostr_compute_len(
    fmt, ((v != 0) ? digits : !fmt->swallow_zero), v_is_negative
    );
  if ((buf->maxlen - buf->len) < flen.total) {
    return bl_mkerr (bl_would_overflow);
  }

  char* dst = buf->str + buf->len;
  bl_uword padding_count = flen.total - flen.number;
  bl_uword padding_point = fmt->left_justify ? 2 : fmt->padding == '0';

  if (padding_point == 0) {
    /* no left justify, width padding with spaces */
    memset (dst, fmt->padding, padding_count);
    dst += padding_count;
  }
  memcpy (dst, fmt->prefix, fmt->prefix_len);
  dst += fmt->prefix_len;

  if (fmt->mode == bl_itostr_fmt_decimal) {
    if (v_is_negative) {
      *dst = '-';
      ++dst;
    }
    else if (fmt->sign != bl_itostr_fmt_sign_default) {
      *dst = fmt->sign == bl_itostr_fmt_force_sign ? '+' : ' ';
      ++dst;
    }
  }
  if (padding_point == 1) {
    /* no left justify, width padding with zeros */
    memset (dst, fmt->padding, padding_count);
    dst += padding_count;
  }
  if (v != 0) {
    memset (dst, '0', flen.numerals - digits);
    dst += flen.numerals - digits;
    for (bl_uword i = digits - 1; i != bl_utype_max (bl_uword); --i) {
      *dst = rev_num[i];
      ++dst;
    }
  }
  else {
    memset (dst, '0', flen.numerals);
    dst += flen.numerals;
  }
  if (padding_point == 2) {
    /* left justified  */
    memset (dst, fmt->padding, padding_count);
    dst += padding_count;
  }
  buf->len += flen.total;
  *dst = 0;
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
static bl_err bl_itostr_out_arr_fmt(
  bl_strbuf*           buf,
  bl_itostr_fmt const* fmt,
  char const*          sep,
  void const*          v,
  size_t               v_count,
  bool                 v_type_is_signed,
  bl_uword             v_type_bytes_log2
  )
{
  bl_assert (buf && buf->str && fmt);
  bl_assert (v_type_bytes_log2 <= 3);

  bl_uword sep_len = 0;
  if (sep) {
    sep_len = strnlen (sep, BL_ITOSTR_MAX_SEP);
  }
  size_t startlen = buf->len;
  bl_err err;

  unsigned type_id = v_type_bytes_log2 + ((v_type_is_signed) ? 4 : 0);

  for (bl_uword i = 0; i < v_count; ++i) {
    bl_u64 u = 0;
    bl_i64 s = 0;
    switch (type_id) {
    case 0:
      u = ((bl_u8*) v)[i];
      break;
    case 1:
      u = ((bl_u16*) v)[i];
      break;
    case 2:
      u = ((bl_u32*) v)[i];
      break;
    case 3:
      u = ((bl_u64*) v)[i];
      break;
    case 4:
      s = ((bl_i8*) v)[i];
      u = (bl_u64) s;
      break;
    case 5:
      s = ((bl_i16*) v)[i];
      u = (bl_u64) s;
      break;
    case 6:
      s = ((bl_i32*) v)[i];
      u = (bl_u64) s;
      break;
    case 7:
      s = ((bl_i64*) v)[i];
      u = (bl_u64) s;
      break;
    default:
      bl_assert (false);
      break;
    }
    err = bl_itostr_out_fmt (buf, fmt, u, s < 0, 1 << v_type_bytes_log2);
    if (bl_unlikely (err.own)) {
      goto on_error;

    }
    if (i + 1 != v_count) {
      if (bl_likely ((buf->maxlen - buf->len) >= sep_len)) {
        memcpy (buf->str + buf->len, sep, sep_len);
        buf->len += sep_len;
      }
      else {
        err = bl_mkerr (bl_would_overflow);
        goto on_error;
      }
    }
  }
  return bl_mkok();
on_error:
  buf->str[startlen] = 0; /* undo */
  buf->len           = startlen;
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_itostr(
  bl_strbuf*  buf,
  char const* fmt,
  bl_u64      v,
  bool        v_is_negative,
  bl_uword    v_type_bytes_log2
  )
{
  bl_assert (v_type_bytes_log2 <= 3);

  fmt = fmt ? fmt : "";

  bl_itostr_fmt f;
  bl_err err = bl_itostr_parse_fmt (&f, fmt, 1 << v_type_bytes_log2);
  if (bl_unlikely (err.own)) {
    return err;
  }
  return bl_itostr_out_fmt (buf, &f, v, v_is_negative, 1 << v_type_bytes_log2);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_itostr_arr(
  bl_strbuf*  buf,
  char const* fmt,
  char const* sep,
  void const* v,
  size_t      v_count,
  bool        v_type_is_signed,
  bl_uword    v_type_bytes_log2
  )
{
  if (bl_unlikely (v_count == 0)) {
    return bl_mkok();
  }
  bl_assert (v);
  bl_assert (v_type_bytes_log2 <= 3);

  fmt = fmt ? fmt : "";
  sep = sep ? sep : ", ";

  bl_itostr_fmt f;
  bl_err err = bl_itostr_parse_fmt (&f, fmt, 1 << v_type_bytes_log2);
  if (bl_unlikely (err.own)) {
    return err;
  }
  return bl_itostr_out_arr_fmt(
    buf, &f, sep, v, v_count, v_type_is_signed, v_type_bytes_log2
    );
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_itostr_dyn(
  bl_dstrbuf* buf,
  char const* fmt,
  bl_u64      v,
  bool        v_is_negative,
  bl_uword    v_type_bytes_log2
  )
{
  bl_assert (buf);
  bl_assert (buf->str != nullptr || ((buf->maxlen == 0) && (buf->len == 0)));
  bl_assert (buf->maxlen >= buf->len);

  bl_alloc_tbl const* alloc = buf->alloc ? buf->alloc : &bl_default_alloc;
  fmt = fmt ? fmt : "";

  bl_itostr_fmt f;
  bl_err err = bl_itostr_parse_fmt (&f, fmt, 1 << v_type_bytes_log2);
  if (bl_unlikely (err.own)) {
    return err;
  }
  bl_itostr_len flen = bl_itostr_compute_len (&f, f.max_digits, v_is_negative);

  bl_uword maxlen = buf->len + flen.total;
  if (buf->maxlen < maxlen) {
    /*making room */
    void* ptr = bl_realloc (alloc, (void*) buf->str, maxlen + 1);
    if (bl_unlikely (!ptr)) {
      return bl_mkerr (bl_alloc);
    }
    buf->maxlen = maxlen;
    buf->str    = (char*) ptr;
  }
  err = bl_itostr_out_fmt(
    (bl_strbuf*) buf, &f, v, v_is_negative, 1 << v_type_bytes_log2
    );
  bl_assert (!err.own && "bug!"); /* it can't fail: it had enough memory */
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_itostr_dyn_arr(
  bl_dstrbuf* buf,
  char const* fmt,
  char const* sep,
  void const* v,
  size_t      v_count,
  bool        v_type_is_signed,
  bl_uword    v_type_bytes_log2
  )
{
  if (bl_unlikely (v_count == 0)) {
    return bl_mkok();
  }
  bl_assert (v);
  bl_assert (buf);
  bl_assert (buf->str != nullptr || ((buf->maxlen == 0) && (buf->len == 0)));
  bl_assert (buf->maxlen >= buf->len);

  bl_alloc_tbl const* alloc = buf->alloc ? buf->alloc : &bl_default_alloc;
  fmt = fmt ? fmt : "";
  sep = sep ? sep : ", ";

  bl_itostr_fmt f;
  bl_err err = bl_itostr_parse_fmt (&f, fmt, 1 << v_type_bytes_log2);
  if (bl_unlikely (err.own)) {
    return err;
  }
  bl_itostr_len flen = bl_itostr_compute_len(
    &f, f.max_digits, v_type_is_signed
    );
  /* a value that will surely fit everything */
  size_t maxlen = buf->len;
  maxlen += (flen.total * v_count);
  maxlen += ((v_count - 1) * strnlen (sep, BL_ITOSTR_MAX_SEP));

  if (buf->maxlen < maxlen) {
    /*making room */
    void* ptr = bl_realloc (alloc, (void*) buf->str, maxlen + 1);
    if (bl_unlikely (!ptr)) {
      return bl_mkerr (bl_alloc);
    }
    buf->maxlen = maxlen;
    buf->str    = (char*) ptr;
  }
  err = bl_itostr_out_arr_fmt(
    (bl_strbuf*) buf, &f, sep, v, v_count, v_type_is_signed, v_type_bytes_log2
    );
  bl_assert (!err.own && "bug!"); /* it can't fail: it had enough memory */
  return err;
}
/*----------------------------------------------------------------------------*/
