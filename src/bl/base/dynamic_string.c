#include <stdio.h>
#include <bl/base/assert.h>
#include <bl/base/dynarray.h>
#include <bl/base/dynamic_string.h>
#include <bl/base/string.h>
#include <string.h>

/*----------------------------------------------------------------------------*/
static bl_err dstr_alloc_copy (dstr *s, char const *str, uword str_len)
{
  bl_assert (s->da.str == nullptr && s->da.size == 0);
  bl_err err = dynarray_resize(
    (dynarray_stub*) &s->da, str_len + 1, sizeof (char), s->alloc
    );
  if (err) {
    return err;
  }
  memcpy (s->da.str, str, str_len + 1);
  s->da.str[str_len] = 0;
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
static bl_err dstr_append_impl (dstr *s, char const *str, uword str_len)
{
  bl_assert(
    ((s->da.str && s->da.size) ||
    (s->da.str == nullptr && s->da.size == 0)) &&
    (str_len > 0)
    );
  uword oldlen = dstr_len (s);
  uword newlen = oldlen + str_len + 1;
  if (newlen < str_len) {
    return bl_would_overflow;
  }
  bl_err err = dynarray_resize(
    (dynarray_stub*) &s->da, newlen, sizeof (char), s->alloc
    );
  if (err) {
    return err;
  }
  memcpy (s->da.str + oldlen, str, str_len);
  s->da.str[s->da.size - 1] = 0;
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_set (dstr *s, char const *str)
{
  dstr_clear (s);
  if (str) {
    dstr stro;
    stro.da.str  = (char*) str;
    stro.da.size = strlen (str) + 1;
    stro.alloc   = s->alloc;
    return dstr_set_o (s, &stro);
  }
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_append (dstr *s, char const *str)
{
  if (str) {
    dstr stro;
    stro.da.str  = (char*) str;
    stro.da.size = strlen (str) + 1;
    stro.alloc   = s->alloc;
    return dstr_append_o (s, &stro);
  }
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_prepend (dstr *s, char const *str)
{
  if (str) {
    dstr stro;
    stro.da.str  = (char*) str;
    stro.da.size = strlen(str) + 1;
    stro.alloc   = s->alloc;
    return dstr_prepend_o (s, &stro);
  }
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_set_o (dstr *s, dstr const *str)
{
  dstr_clear (s);
  if (dstr_len (str) > 0) {
    return dstr_alloc_copy (s, dstr_get (str), dstr_len (str));
  }
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_append_o (dstr *s, dstr const *str)
{
  if (dstr_len (str) > 0) {
    return dstr_append_impl (s, dstr_get (str), dstr_len (str));
  }
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_prepend_o (dstr *s, dstr const *str)
{
  if (dstr_len (str) > 0) {
    if (dstr_len (s) > 0) {
      dstr res      = dstr_init_rv (s->alloc);
      uword s_len   = dstr_len (s);
      uword str_len = dstr_len (str);
      bl_err err    = dynarray_resize(
        (dynarray_stub*) &res.da, s_len + str_len + 1, sizeof (char), s->alloc
        );
      if (err) {
        return err;
      }
      memcpy (res.da.str, dstr_get (str), str_len);
      memcpy (res.da.str + str_len, dstr_get (s), s_len);
      res.da.str[s_len + str_len] = 0;
      dstr_destroy (s);
      *s = res;
      return bl_ok;
    }
    return dstr_set_o (s, str);
  }
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
static bl_err dstr_append_va_priv(
  dstr *s, char const *fmt, va_list list
  )
{
  char* str;
  int str_len = bl_vasprintf (&str, s->alloc, fmt, list);
  if (str_len > 0) {
    if (dstr_len (s) == 0) {
      s->da.str  = str;
      s->da.size = str_len + 1;
      return bl_ok;
    }
    bl_err err = dstr_append_impl (s, str, str_len);
    bl_dealloc (s->alloc, str);
    return err;
  }
  return (str_len == 0) ? bl_ok : bl_alloc;
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
BL_EXPORT bl_err dstr_prepend_va (dstr *s, char const *fmt, ...)
{
  va_list list;
  va_start (list, fmt);
  bl_err err = 0;
  if (dstr_len (s) != 0) {
    dstr newstr = dstr_init_rv (s->alloc);
    err = dstr_append_va_priv (&newstr, fmt, list);
    if (err) {
      goto done;
    }
    err = dstr_append_o (&newstr, s);
    if (err) {
      dstr_destroy (&newstr);
      goto done;
    }
    dstr_destroy (s);
    *s = newstr;
  }
  else {
    err = dstr_append_va_priv (s, fmt, list);
  }
done:
  va_end (list);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_erase_head (dstr *s, uword char_count)
{
  uword newlen = dstr_len (s) - char_count;
  if (newlen > dstr_len (s)) {
    return bl_range;
  }
  if (newlen != 0) {
    dstr tmp   = dstr_init_rv (s->alloc);
    bl_err err = dstr_alloc_copy (&tmp, s->da.str + char_count, newlen);
    if (err) {
       return err;
    }
    dstr_destroy (s);
    *s = tmp;
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
    bl_err err = dynarray_resize(
      (dynarray_stub*) &s->da, newlen + 1, sizeof (char), s->alloc
      );
    if (err) {
      return err;
    }
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
/*----------------------------------------------------------------------------*/
BL_EXPORT void dstr_clear (dstr *s)
{
  if (s->da.str) {
    dynarray_resize ((dynarray_stub*) &s->da, 0, sizeof (char), s->alloc);
    dstr_init (s, s->alloc);
  }
}
/*----------------------------------------------------------------------------*/
