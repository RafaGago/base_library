/* this file is on a separate source file (translation unit) just to make
 trivial for linkers to remove it when unused.*/

#include <stdio.h>
#include <bl/base/assert.h>
#include <bl/base/array_concat_dynarray.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/utility.h>
#include <bl/base/string.h>
#include <bl/base/dynamic_string.h>

/*---------------------------------------------------------------------------*/
static inline bl_err dstr_replace_replace_le_match(
  dstr*       s,
  char const* match,
  uword       match_len,
  char const* replace,
  uword       replace_len,
  uword       max
  )
{
  uword scan_start = 0;
  uword found      = 0;
  char* wptr       = s->da.str;

  while (found < max) {
    uword f = dstr_find_l (s, scan_start, match, match_len);
    if (f >= dstr_len (s)) {
      break;
    }
    uword advanced = f - scan_start;
    memmove (wptr, s->da.str + scan_start, advanced);
    wptr += advanced;
    memcpy (wptr, replace, replace_len);
    wptr      += replace_len;
    scan_start = f + match_len;
    ++found;
  }
  if (found) {
    uword to_end = dstr_len (s) - scan_start;
    memmove (wptr, s->da.str + scan_start, to_end);
    wptr             += to_end;
    s->len            = wptr - s->da.str;
    s->da.str[s->len] = 0;
  }
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
static inline bl_err dstr_replace_replace_gt_match_no_alloc(
  dstr*       s,
  char const* match,
  uword       match_len,
  char const* replace,
  uword       replace_len,
  uword       max,
  uword       len_bytes
  )
{
  /*Backwards copy (replace_len > match_len). The matches are saved on the
    string as a linked-list (the string is lost in case of an allocation
    failure)*/
  bl_assert (dstr_len (s) < itype_max (word));
  uword scan_start = 0;
  uword last       = 0;
  uword found      = 0;

  while (found < max) {
    uword f = dstr_find_l (s, scan_start, match, match_len);
    if (f >= dstr_len (s)) {
      break;
    }
    for (uword i = 0; i < len_bytes; ++i) {
      s->da.str[f + i] = (char) (last >> (i * 8));
    }
    last       = f;
    scan_start = f + match_len;
    ++found;
  }
  if (found == 0) {
    return bl_ok;
  }
  uword diff  = replace_len - match_len;
  uword bytes = dstr_len (s) + (found * diff);
  bl_err err  = dstr_set_capacity (s, bl_max (bytes, dstr_get_capacity (s)));
  if (err) {
    dstr_destroy (s); /* the string was modified */
    return err;
  }
  char* rptr_prev   = s->da.str + dstr_len (s);
  s->len            = bytes;
  s->da.str[s->len] = 0;

  do {
    char* rptr = s->da.str + last + match_len;
    char *wptr = rptr + (found * diff);
    memmove (wptr, rptr, rptr_prev - rptr);
    rptr -= match_len;
    last  = 0;
    for (uword i = 0; i < len_bytes; ++i) {
      /*loading last from the in-string linked list*/
      last |= ((uword) rptr[i]) << (i * 8);
    }
    wptr -= replace_len;
    memcpy (wptr, replace, replace_len);
    rptr_prev = rptr;
  }
  while (--found);
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
define_ac_dynarray_types (u32arr, u32)
declare_ac_dynarray_funcs (u32arr, u32)
/*---------------------------------------------------------------------------*/
static inline bl_err dstr_replace_replace_gt_match(
  dstr*       s,
  char const* match,
  uword       match_len,
  char const* replace,
  uword       replace_len,
  uword       max
  )
{
  /*Backwards copy (replace_len > match_len). The matches are saved on a dynamic
    array */
  bl_assert (dstr_len (s) < utype_max (u32));
  u32    stack_matches[32];
  u32arr matches;

  u32arr_init (&matches, stack_matches, arr_elems (stack_matches));
  bl_err err       = bl_ok;
  uword scan_start = 0;
  uword found      = 0;

  while (found < max) {
    uword f = dstr_find_l (s, scan_start, match, match_len);
    if (f >= dstr_len (s)) {
      break;
    }
    if (u32arr_size (&matches) <= found) {
      bl_err err = u32arr_grow (&matches, 32, s->alloc);
      if (err) {
        goto destroy_dynarray;
      }
    }
    *u32arr_at (&matches, found) = (u32) f;
    scan_start = f + match_len;
    ++found;
  }
  if (found == 0) {
    goto destroy_dynarray;
  }
  uword diff  = replace_len - match_len;
  uword bytes = dstr_len (s) + (found * diff);
  err         = dstr_set_capacity (s, bl_max (bytes, dstr_get_capacity (s)));
  if (err) {
    goto destroy_dynarray;
  }
  char* rptr_prev   = s->da.str + dstr_len (s);
  s->len            = bytes;
  s->da.str[s->len] = 0;
  do {
    --found;
    char* rptr = s->da.str + *u32arr_at (&matches, found) + match_len;
    char *wptr = rptr + ((found + 1)* diff);
    memmove (wptr, rptr, rptr_prev - rptr);
    rptr -= match_len;
    wptr -= replace_len;
    memcpy (wptr, replace, replace_len);
    rptr_prev = rptr;
  }
  while (found);
destroy_dynarray:
  u32arr_destroy (&matches, s->alloc);
  return err;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_err dstr_replace_l(
  dstr*       s,
  char const* match,
  uword       match_len,
  char const* replace,
  uword       replace_len,
  uword       max_replace
  )
{
  if (unlikely (dstr_len (s) == 0 || match_len == 0)) {
    return bl_ok;
  }
  if (unlikely (!match || !replace)) {
    bl_assert (false);
    return bl_invalid;
  }
  uword max = max_replace ? max_replace : (uword) -1;
  if (match_len >= replace_len) {
    return dstr_replace_replace_le_match(
      s, match, match_len, replace, replace_len, max
      );
  }
  uword len_bytes = div_ceil (log2_ceil_u (dstr_len (s)), 8);
  if (match_len >= len_bytes) {
    return dstr_replace_replace_gt_match_no_alloc(
      s, match, match_len, replace, replace_len, max, len_bytes
      );
  }
  else {
    return dstr_replace_replace_gt_match(
      s, match, match_len, replace, replace_len, max
      );
  }
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
