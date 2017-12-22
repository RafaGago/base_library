#include <stdio.h>
#include <bl/base/assert.h>
#include <bl/base/dynarray.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/utility.h>
#include <bl/base/string.h>
#include <bl/base/dynamic_string.h>

static uword dstr_minalloc = 32;

/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_set_capacity (dstr* s, uword newlen)
{
  if (newlen >= dstr_len (s)) {
    newlen += (newlen != 0);
    return dynarray_resize(
      (dynarray_stub*) &s->da, newlen, sizeof (char), s->alloc
      );
  }
  return bl_invalid;
}
/*----------------------------------------------------------------------------*/
static bl_err dstr_resize_if (dstr *s, uword newlen)
{
  bl_err err = bl_ok;
  if (dstr_get_capacity (s) < newlen) {
    uword overalloc = round_to_next_multiple (newlen, dstr_minalloc);
    overalloc      += bl_max (next_pow2_u (newlen) / 32, dstr_minalloc);
    return dynarray_resize(
      (dynarray_stub*) &s->da, overalloc, sizeof (char), s->alloc
      );
  }
  return err;
}
/*----------------------------------------------------------------------------*/
static bl_err dstr_append_impl (dstr *s, char const *str, uword len)
{
  bl_assert(
    ((s->da.str && s->da.size) ||
    (s->da.str == nullptr && s->da.size == 0)) &&
    (len > 0)
    );
  uword oldlen = dstr_len (s);
  uword newlen = oldlen + len;
  bl_err err   = dstr_resize_if (s, newlen);
  if (!err) {
    memcpy (s->da.str + oldlen, str, len);
    s->da.str[newlen] = 0;
    s->len = newlen;
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_set_l (dstr *s, char const *str, uword len)
{
  bl_assert (!str || strnlen (str, len) >= len);
  dstr_clear (s);
  if (len > 0 && str) {
    return dstr_append_impl (s, str, len);
  }
  return ((!str && len == 0) || len == 0) ? bl_ok : bl_invalid;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_append_l (dstr *s, char const *str, uword len)
{
  bl_assert (strnlen (str, len) >= len);
  if (len > 0 && str) {
    return dstr_append_impl (s, str, len);
  }
  return (str && len == 0) ? bl_ok : bl_invalid;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_insert_l(
  dstr *s, uword idx, char const *str, uword len
  )
{
  bl_assert (strnlen (str, len) >= len);
  if (unlikely (len == 0 || !str)) {
    return (len == 0 && !str) ? bl_ok : bl_invalid;
  }
  if (unlikely (dstr_len (s) == 0)) {
    return dstr_set_l (s, str, len);
  }
  if (unlikely (idx >= dstr_len (s))) {
    return dstr_append_l (s, str, len);
  }
  uword newlen = s->len + len;
  if (newlen <= dstr_get_capacity (s)) {
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
    if (unlikely (!newstr)) {
      return bl_alloc;
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
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
static bl_err dstr_append_va_priv(
  dstr *s, char const *fmt, va_list list
  )
{
  char *str = s->da.str;
  int len   = bl_vasprintf_ext(
    &str, str ? s->da.size : dstr_minalloc, s->len, 1, s->alloc, fmt, list
    );
  if (unlikely (len < 0)) {
    return -len;
  }
  s->da.str  = str;
  s->len    += len;
  s->da.size = bl_max (s->da.size, s->len + 1);
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_append_va (dstr *s, char const *fmt, ...)
{
  va_list list;
  va_start (list, fmt);
  bl_err err = dstr_append_va_priv (s, fmt, list);
  va_end (list);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_set_va (dstr *s, char const *fmt, ...)
{
  dstr_clear(s);
  va_list list;
  va_start (list, fmt);
  bl_err err = dstr_append_va_priv (s, fmt, list);
  va_end (list);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_insert_va (dstr *s, uword idx, char const *fmt, ...)
{
  va_list list;
  va_start (list, fmt);
  bl_err err = 0;
  if (dstr_len (s) != 0 && idx < dstr_len (s) - 1) {
    dstr newstr = dstr_init_rv (s->alloc);
    err = dstr_append_va_priv (&newstr, fmt, list);
    if (err) {
      goto done;
    }
    err = dstr_insert_o (s, idx, &newstr);
    dstr_destroy (&newstr);
  }
  else {
    err = dstr_append_va_priv (s, fmt, list);
  }
done:
  va_end (list);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_erase (dstr *s, uword idx, uword char_count)
{
  uword newlen = dstr_len (s) - char_count;
  if ((newlen - idx) > dstr_len (s)) {
    return bl_range;
  }
  if (newlen != 0) {
    memmove (s->da.str + idx, s->da.str + idx + char_count, newlen - idx);
    s->len            = newlen;
    s->da.str[newlen] = 0;
    return bl_ok;
  }
  dstr_clear (s);
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_erase_tail (dstr *s, uword char_count)
{
  uword newlen = dstr_len (s) - char_count;
  if (newlen > dstr_len (s)) {
    return bl_range;
  }
  if (newlen != 0) {
    s->len            = newlen;
    s->da.str[newlen] = 0;
    return bl_ok;
  }
  dstr_clear (s);
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_erase_head_while (dstr *s, bl_ctype_func fn, int fnres)
{
  if (!fn) {
    return bl_invalid;
  }
  fnres = !!fnres;
  uword i;
  for (i = 0; (i < dstr_len (s)) && (!!fn (s->da.str[i]) == fnres); ++i) {}
  if (i == 0) {
    return bl_ok;
  }
  return dstr_erase_head (s, i);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_erase_tail_while (dstr *s, bl_ctype_func fn, int fnres)
{
  if (!fn) {
    return bl_invalid;
  }
  fnres = !!fnres;
  uword i;
  for (i = dstr_len (s); (i-- > 0) && (!!fn (s->da.str[i]) == fnres); ) {}
  uword trim = dstr_len (s) - (i + 1);
  if (trim == 0) {
    return bl_ok;
  }
  return dstr_erase_tail (s, trim);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_erase_head_tail_while(
  dstr *s, bl_ctype_func fn, int fnres
  )
{
  bl_err err = dstr_erase_head_while(s, fn, fnres);
  if (err) {
    return err;
  }
  return dstr_erase_tail_while(s, fn, fnres);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_apply(
  dstr *s, bl_ctype_func fn, uword start, uword end
  )
{
  if (!fn) {
    return bl_invalid;
  }
  if (start > end || end >= s->da.size) {
    return bl_range;
  }
  for (uword i = start; i < end; ++i) {
    s->da.str[i] = (char) fn (s->da.str[i]);
  }
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT char* dstr_steal_ownership (dstr *s)
{
  dstr_set_capacity (s, dstr_len (s));
  char* ret = s->da.str;
  dstr_init (s, s->alloc);
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void dstr_transfer_ownership_l (dstr *s, char* str, uword len)
{
  bl_assert (strnlen (str, len) >= len);
  dstr_destroy (s);
  s->da.str  = str;
  s->len     = len;
  s->da.size = len + (len != 0);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT uword dstr_find_l(
  dstr* s, char const* search, uword search_len, uword offset
  )
{
/*This is optimized for:
  -small strings.
  -small find len.
  -plain text alphabets (big alphabets).
  -no setup time.
  -no extra space (allocations).

  If memchr is well optimized it will most likely compare many chars with a few
  instructions (e.g. on x86-64 with SSE2 16), this and the lack of setup time
  can beat a generic (non architecture-specific) implementation of smarter
  algorithms with the types of input described here while the implementation
  remains trivial.

  If running long search strings on long strings are important, a separate
  efficient algorithm for these type of inputs can be implemented:
  http://www-igm.univ-mlv.fr/~lecroq/string/node2.html
  */
  if (unlikely (search_len == 0 || !search || offset >= dstr_len (s))) {
    return dstr_len (s);
  }
  bl_assert (strnlen (search, search_len) >= search_len);
  char const* rptr = dstr_beg (s) + offset;
  char const* end  = dstr_end (s);
  while (true) {
    rptr = memchr (rptr, search[0], end - rptr);
    if (!rptr || (end - rptr) < search_len) {
      return dstr_len (s);
    }
    if (memcmp (rptr + 1, search + 1, search_len - 1) == 0) {
      return rptr - dstr_beg (s);
    }
    else {
      ++rptr;
    }
  }
}
/*----------------------------------------------------------------------------*/