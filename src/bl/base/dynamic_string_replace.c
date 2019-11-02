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
static inline bl_err bl_dstr_replace_le_match(
  bl_dstr*    s,
  char const* match,
  bl_uword    match_len,
  char const* replace,
  bl_uword    replace_len,
  bl_uword    offset,
  bl_uword    max
  )
{
  bl_uword scan_start = offset;
  bl_uword found      = 0;
  char* wptr       = s->da.str + offset;

  while (found < max) {
    bl_uword f = bl_dstr_find_l (s, match, match_len, scan_start);
    if (f >= bl_dstr_len (s)) {
      break;
    }
    bl_uword advanced = f - scan_start;
    if (scan_start != offset) {
      memmove (wptr, s->da.str + scan_start, advanced);
    }
    wptr += advanced;
    memcpy (wptr, replace, replace_len);
    wptr      += replace_len;
    scan_start = f + match_len;
    ++found;
  }
  if (found) {
    bl_uword to_end = bl_dstr_len (s) - scan_start;
    memmove (wptr, s->da.str + scan_start, to_end);
    wptr             += to_end;
    s->len            = wptr - s->da.str;
    s->da.str[s->len] = 0;
  }
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_replace_gt_match_no_alloc(
  bl_dstr*    s,
  char const* match,
  bl_uword    match_len,
  char const* replace,
  bl_uword    replace_len,
  bl_uword    offset,
  bl_uword    max,
  bl_uword    len_bytes
  )
{
  /*Backwards copy (replace_len > match_len). The matches are saved on the
    string as a linked-list (the string is lost in case of an allocation
    failure)*/
  bl_assert (bl_dstr_len (s) < bl_itype_max (bl_word));
  bl_uword scan_start = offset;
  bl_uword last       = 0;
  bl_uword found      = 0;

  while (found < max) {
    bl_uword f = bl_dstr_find_l (s, match, match_len, scan_start);
    if (f >= bl_dstr_len (s)) {
      break;
    }
    for (bl_uword i = 0; i < len_bytes; ++i) {
      s->da.str[f + i] = (char) (last >> (i * 8));
    }
    last       = f;
    scan_start = f + match_len;
    ++found;
  }
  if (found == 0) {
    return bl_mkok();
  }
  bl_uword diff  = replace_len - match_len;
  bl_uword bytes = bl_dstr_len (s) + (found * diff);
  bl_err err  = bl_dstr_set_capacity (s, bl_max (bytes, bl_dstr_get_capacity (s)));
  if (err.bl) {
    bl_dstr_destroy (s); /* the string was modified */
    return err;
  }
  char* rptr_prev   = s->da.str + bl_dstr_len (s);
  s->len            = bytes;
  s->da.str[s->len] = 0;

  do {
    char* rptr = s->da.str + last + match_len;
    char *wptr = rptr + (found * diff);
    memmove (wptr, rptr, rptr_prev - rptr);
    rptr -= match_len;
    last  = 0;
    for (bl_uword i = 0; i < len_bytes; ++i) {
      /*loading last from the in-string linked list*/
      last |= ((bl_uword) rptr[i]) << (i * 8);
    }
    wptr -= replace_len;
    memcpy (wptr, replace, replace_len);
    rptr_prev = rptr;
  }
  while (--found);
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
bl_define_ac_dynarray_types (bl_u32arr, bl_u32)
bl_declare_ac_dynarray_funcs (bl_u32arr, bl_u32)
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_replace_gt_match(
  bl_dstr*    s,
  char const* match,
  bl_uword    match_len,
  char const* replace,
  bl_uword    replace_len,
  bl_uword    offset,
  bl_uword    max
  )
{
  /*Backwards copy (replace_len > match_len). The matches are saved on a dynamic
    array */
  bl_assert (bl_dstr_len (s) < bl_utype_max (bl_u32));
  bl_u32    stack_matches[32];
  bl_u32arr matches;

  bl_u32arr_init (&matches, stack_matches, bl_arr_elems (stack_matches));
  bl_err err       = bl_mkok();
  bl_uword scan_start = offset;
  bl_uword found      = 0;

  while (found < max) {
    bl_uword f = bl_dstr_find_l (s, match, match_len, scan_start);
    if (f >= bl_dstr_len (s)) {
      break;
    }
    if (bl_u32arr_size (&matches) <= found) {
      bl_err err = bl_u32arr_grow (&matches, 32, s->alloc);
      if (err.bl) {
        goto destroy_bl_dynarray;
      }
    }
    *bl_u32arr_at (&matches, found) = (bl_u32) f;
    scan_start = f + match_len;
    ++found;
  }
  if (found == 0) {
    goto destroy_bl_dynarray;
  }
  bl_uword diff  = replace_len - match_len;
  bl_uword bytes = bl_dstr_len (s) + (found * diff);
  err         = bl_dstr_set_capacity (s, bl_max (bytes, bl_dstr_get_capacity (s)));
  if (err.bl) {
    goto destroy_bl_dynarray;
  }
  char* rptr_prev   = s->da.str + bl_dstr_len (s);
  s->len            = bytes;
  s->da.str[s->len] = 0;
  do {
    --found;
    char* rptr = s->da.str + *bl_u32arr_at (&matches, found) + match_len;
    char *wptr = rptr + ((found + 1)* diff);
    memmove (wptr, rptr, rptr_prev - rptr);
    rptr -= match_len;
    wptr -= replace_len;
    memcpy (wptr, replace, replace_len);
    rptr_prev = rptr;
  }
  while (found);
destroy_bl_dynarray:
  bl_u32arr_destroy (&matches, s->alloc);
  return err;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_dstr_replace_l(
  bl_dstr*    s,
  char const* match,
  bl_uword    match_len,
  char const* replace,
  bl_uword    replace_len,
  bl_uword    offset,
  bl_uword    max_replace
  )
{
  if (bl_unlikely (bl_dstr_len (s) == 0 || match_len == 0)) {
    return bl_mkok();
  }
  if (bl_unlikely (!match || !replace || offset >= bl_dstr_len (s))) {
    bl_assert (false);
    return bl_mkerr (bl_invalid);
  }
  bl_uword max = max_replace ? max_replace : (bl_uword) -1;
  if (match_len >= replace_len) {
    return bl_dstr_replace_le_match(
      s, match, match_len, replace, replace_len, offset, max
      );
  }
  bl_uword len_bytes = bl_div_ceil (bl_log2_ceil_u (bl_dstr_len (s)), 8);
  if (match_len >= len_bytes) {
    return bl_dstr_replace_gt_match_no_alloc(
      s, match, match_len, replace, replace_len, offset, max, len_bytes
      );
  }
  else {
    return bl_dstr_replace_gt_match(
      s, match, match_len, replace, replace_len, offset, max
      );
  }
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
