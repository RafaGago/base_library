#include <stdio.h>
#include <bl/base/assert.h>
#include <bl/base/dynarray.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/utility.h>
#include <bl/base/string.h>
#include <bl/base/dynamic_string.h>

static bl_uword bl_dstr_minalloc = 32;

/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_set_capacity (bl_dstr* s, bl_uword newlen)
{
  if (newlen >= bl_dstr_len (s)) {
    newlen += (newlen != 0);
    return bl_dynarray_resize(
      (bl_dynarray_stub*) &s->da, newlen, sizeof (char), s->alloc
      );
  }
  return bl_mkerr (bl_invalid);
}
/*----------------------------------------------------------------------------*/
static bl_err bl_dstr_resize_if (bl_dstr *s, bl_uword newlen)
{
  if (bl_dstr_get_capacity (s) < newlen) {
    bl_uword overalloc = bl_round_to_next_multiple (newlen, bl_dstr_minalloc);
    overalloc      += bl_max (bl_next_pow2_u (newlen) / 32, bl_dstr_minalloc);
    return bl_dynarray_resize(
      (bl_dynarray_stub*) &s->da, overalloc, sizeof (char), s->alloc
      );
  }
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
static bl_err bl_dstr_append_impl (bl_dstr *s, char const *str, bl_uword len)
{
  bl_assert(
    ((s->da.str && s->da.size) ||
    (s->da.str == nullptr && s->da.size == 0)) &&
    (len > 0)
    );
  bl_uword oldlen = bl_dstr_len (s);
  bl_uword newlen = oldlen + len;
  bl_err err   = bl_dstr_resize_if (s, newlen);
  if (!err.own) {
    memcpy (s->da.str + oldlen, str, len);
    s->da.str[newlen] = 0;
    s->len = newlen;
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_set_l (bl_dstr *s, char const *str, bl_uword len)
{
  bl_assert (!str || strnlen (str, len) >= len);
  bl_dstr_clear (s);
  if (len > 0 && str) {
    return bl_dstr_append_impl (s, str, len);
  }
  return bl_mkerr (((!str && len == 0) || len == 0) ? bl_ok : bl_invalid);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_append_l (bl_dstr *s, char const *str, bl_uword len)
{
  bl_assert (strnlen (str, len) >= len);
  if (len > 0 && str) {
    return bl_dstr_append_impl (s, str, len);
  }
  return bl_mkerr ((str && len == 0) ? bl_ok : bl_invalid);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_insert_l(
  bl_dstr *s, bl_uword idx, char const *str, bl_uword len
  )
{
  bl_assert (strnlen (str, len) >= len);
  if (bl_unlikely (len == 0 || !str)) {
    return bl_mkerr ((len == 0 && !str) ? bl_ok : bl_invalid);
  }
  if (bl_unlikely (bl_dstr_len (s) == 0)) {
    return bl_dstr_set_l (s, str, len);
  }
  if (bl_unlikely (idx >= bl_dstr_len (s))) {
    return bl_dstr_append_l (s, str, len);
  }
  bl_uword newlen = s->len + len;
  if (newlen <= bl_dstr_get_capacity (s)) {
    if (len >= s->len - idx) {
      memcpy (s->da.str + len + idx, s->da.str + idx, s->len - idx);
    }
    else {
      memmove (s->da.str + len + idx, s->da.str + idx, s->len - idx);
    }
    memcpy (s->da.str + idx, str, len);
  }
  else {
    char* newstr = (char*) bl_alloc (s->alloc, newlen + 1);
    if (bl_unlikely (!newstr)) {
      return bl_mkerr (bl_alloc);
    }
    memcpy (newstr, s->da.str, idx);
    memcpy (newstr + idx, str, len);
    memcpy (newstr + idx + len, s->da.str + idx, s->len - idx);
    bl_dealloc (s->alloc, s->da.str);
    s->da.str  = newstr;
    s->da.size = newlen + 1;
  }
  s->len = newlen;
  s->da.str[s->len] = 0;
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
static bl_err bl_dstr_append_va_priv(
  bl_dstr *s, char const *fmt, va_list list
  )
{
  char *str = s->da.str;
  int len   = bl_vasprintf_ext(
    &str, str ? s->da.size : bl_dstr_minalloc, s->len, 1, s->alloc, fmt, list
    );
  if (bl_unlikely (len < 0)) {
    return bl_mkerr (-len);
  }
  s->da.str  = str;
  s->len    += len;
  s->da.size = bl_max (s->da.size, s->len + 1);
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_append_va (bl_dstr *s, char const *fmt, ...)
{
  va_list list;
  va_start (list, fmt);
  bl_err err = bl_dstr_append_va_priv (s, fmt, list);
  va_end (list);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_set_va (bl_dstr *s, char const *fmt, ...)
{
  bl_dstr_clear(s);
  va_list list;
  va_start (list, fmt);
  bl_err err = bl_dstr_append_va_priv (s, fmt, list);
  va_end (list);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_insert_va(
  bl_dstr *s, bl_uword idx, char const *fmt, ...
  )
{
  va_list list;
  va_start (list, fmt);
  bl_err err = bl_mkok();
  if (bl_dstr_len (s) != 0 && idx < bl_dstr_len (s) - 1) {
    bl_dstr newstr = bl_dstr_init_rv (s->alloc);
    err = bl_dstr_append_va_priv (&newstr, fmt, list);
    if (err.own) {
      goto done;
    }
    err = bl_dstr_insert_o (s, idx, &newstr);
    bl_dstr_destroy (&newstr);
  }
  else {
    err = bl_dstr_append_va_priv (s, fmt, list);
  }
done:
  va_end (list);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_erase (bl_dstr *s, bl_uword idx, bl_uword char_count)
{
  bl_uword newlen = bl_dstr_len (s) - char_count;
  if ((newlen - idx) > bl_dstr_len (s)) {
    return bl_mkerr (bl_range);
  }
  if (newlen != 0) {
    memmove (s->da.str + idx, s->da.str + idx + char_count, newlen - idx);
    s->len            = newlen;
    s->da.str[newlen] = 0;
    return bl_mkok();
  }
  bl_dstr_clear (s);
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_erase_tail (bl_dstr *s, bl_uword char_count)
{
  bl_uword newlen = bl_dstr_len (s) - char_count;
  if (newlen > bl_dstr_len (s)) {
    return bl_mkerr (bl_range);
  }
  if (newlen != 0) {
    s->len            = newlen;
    s->da.str[newlen] = 0;
    return bl_mkok();
  }
  bl_dstr_clear (s);
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_erase_head_while(
  bl_dstr *s, bl_ctype_func fn, int fnres
  )
{
  if (!fn) {
    return bl_mkerr (bl_invalid);
  }
  fnres = !!fnres;
  bl_uword i;
  for (i = 0; (i < bl_dstr_len (s)) && (!!fn (s->da.str[i]) == fnres); ++i) {}
  if (i == 0) {
    return bl_mkok();
  }
  return bl_dstr_erase_head (s, i);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_erase_tail_while(
  bl_dstr *s, bl_ctype_func fn, int fnres
  )
{
  if (!fn) {
    return bl_mkerr (bl_invalid);
  }
  fnres = !!fnres;
  bl_uword i;
  for (i = bl_dstr_len (s); (i-- > 0) && (!!fn (s->da.str[i]) == fnres); ) {}
  bl_uword trim = bl_dstr_len (s) - (i + 1);
  if (trim == 0) {
    return bl_mkok();
  }
  return bl_dstr_erase_tail (s, trim);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_erase_head_tail_while(
  bl_dstr *s, bl_ctype_func fn, int fnres
  )
{
  bl_err err = bl_dstr_erase_head_while(s, fn, fnres);
  if (err.own) {
    return err;
  }
  return bl_dstr_erase_tail_while(s, fn, fnres);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_apply(
  bl_dstr *s, bl_ctype_func fn, bl_uword start, bl_uword end
  )
{
  if (!fn) {
    return bl_mkerr (bl_invalid);
  }
  if (start > end || end >= s->da.size) {
    return bl_mkerr (bl_range);
  }
  for (bl_uword i = start; i < end; ++i) {
    s->da.str[i] = (char) fn (s->da.str[i]);
  }
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
BL_EXPORT char* bl_dstr_steal_ownership (bl_dstr *s)
{
  bl_dstr_set_capacity (s, bl_dstr_len (s));
  char* ret = s->da.str;
  bl_dstr_init (s, s->alloc);
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void bl_dstr_transfer_ownership_l(
  bl_dstr *s, char* str, bl_uword len
  )
{
  bl_assert (strnlen (str, len) >= len);
  bl_dstr_destroy (s);
  s->da.str  = str;
  s->len     = len;
  s->da.size = len + (len != 0);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_uword bl_dstr_find_l(
  bl_dstr* s, char const* search, bl_uword search_len, bl_uword offset
  )
{
/*This is optimized for:
  -small strings.
  -small find len.
  -plain text alphabets (big alphabets).
  -no setup time.
  -no extra space (allocations).

  If memchr is well optimized it will most bl_likely compare many chars with a few
  instructions (e.g. on x86-64 with SSE2 16), this and the lack of setup time
  can beat a generic (non architecture-specific) implementation of smarter
  algorithms with the types of input described here while the implementation
  remains trivial.

  If running long search strings on long strings are important, a separate
  efficient algorithm for these type of inputs can be implemented:
  http://www-igm.univ-mlv.fr/~lecroq/string/node2.html
  */
  if (bl_unlikely (search_len == 0 || !search || offset >= bl_dstr_len (s))) {
    return bl_dstr_len (s);
  }
  bl_assert (strnlen (search, search_len) >= search_len);
  char const* rptr = bl_dstr_beg (s) + offset;
  char const* end  = bl_dstr_end (s);
  while (true) {
    rptr = memchr (rptr, search[0], end - rptr);
    if (!rptr || (end - rptr) < search_len) {
      return bl_dstr_len (s);
    }
    if (memcmp (rptr + 1, search + 1, search_len - 1) == 0) {
      return rptr - bl_dstr_beg (s);
    }
    else {
      ++rptr;
    }
  }
}
/*----------------------------------------------------------------------------*/
bl_err bl_dstr_append_file (bl_dstr *s, FILE* file, bl_uword file_read_limit)
{
  bl_uword written;
  bl_dynarray_stub* da = (bl_dynarray_stub*) &s->da;
  bl_err err = bl_dynarray_from_file(
    da, &written, s->len, 1, 1, file, file_read_limit, s->alloc
    );
  if (!err.own && written) {
    s->len += written;
    s->da.str[s->len] = 0;
  }
  return err;
}
/*----------------------------------------------------------------------------*/
