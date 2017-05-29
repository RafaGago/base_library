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

/* Algorith optimization for:
 -small strings.
 -small match len.
 -plain text alphabets (big alphabets).
 -no setup time (low number of matches).
 -smallest number of allocations.
 -minimal extra space.
 -strings with enough capacity to host the changes (uses realloc instead of
  creating a new string and copying forward). Strings that are planned to be
  used for replacements have to be dimensioned properly if the performance is
  relevant.

NOTE: if some day RAW perf is more important than to have minimal allocations,
benchmark on many platforms by using "strstr" + separate allocation + forward
copy.

If strstr is linear O(M) (instead of O(M*N)) it will require setup time, to
get the match string length and will be looking for the null terminator. memchr
+ memcmp may be faster for most typical inputs, as it might be heavily optimized
(SIMD) and doesn't need to check for the NULL terminator.

If optimizations for long (haystack) strings and long match strings (needles)
are desired, implement a more advanced exact string matching algorithm. There
are many well-known efficient ones. */
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
  /*forward copy (replace_len <= match_len) */
  uword found = 0;
  char* rptr = s->da.str;
  char* end  = rptr + dstr_len (s);
  char* scanbegptr = rptr;
  char* wptr       = rptr;
  while (true) {
    rptr = memchr (rptr, match[0], end - rptr);
    if (!rptr || (end - rptr) < match_len) {
      break;
    }
    if (memcmp (rptr + 1, match + 1, match_len - 1) == 0) {
      uword advanced = rptr - scanbegptr;
      if (wptr != scanbegptr) {
        memmove (wptr, scanbegptr, advanced);
      }
      wptr      += advanced;
      memcpy (wptr, replace, replace_len);
      wptr      += replace_len;
      rptr      += match_len;
      scanbegptr = rptr;
      ++found;
      if (found == max) {
        break;
      }
    }
    else {
      ++rptr;
    }
  }
  if (wptr != scanbegptr) {
    uword to_end = end - scanbegptr;
    memmove (wptr, scanbegptr, to_end);
    wptr             += to_end;
    s->len            = wptr - s->da.str;
    s->da.str[s->len] = 0;
  }
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
static inline bl_err dstr_replace_gt_match_matches_stored_in_string(
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
  uword found = 0;
  char* rptr = s->da.str;
  char* end  = rptr + dstr_len (s);
  /*using the memory of the string to be replaced as an offset list*/
  uword last = 0;
  bl_assert (dstr_len (s) < itype_max (word));
  while (true) {
    rptr = memchr (rptr, match[0], end - rptr);
    if (!rptr || (end - rptr) < match_len) {
      break;
    }
    if (memcmp (rptr + 1, match + 1, match_len - 1) == 0) {
      for (uword i = 0; i < len_bytes; ++i) {
        /*saving prev inside the string as a linked list*/
        rptr[i] = (char) (last >> (i * 8));
      }
      last  = rptr - s->da.str;
      rptr += match_len;
      ++found;
      if (found == max) {
        break;
      }
    }
    else {
      ++rptr;
    }
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
  char* rptr_prev   = end;
  s->len            = bytes;
  s->da.str[s->len] = 0;
  do {
    rptr       = s->da.str + last + match_len;
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
  bl_err err  = bl_ok;
  uword found = 0;
  char* rptr  = s->da.str;
  char* end   = rptr + dstr_len (s);
  while (true) {
    rptr = memchr (rptr, match[0], end - rptr);
    if (!rptr || (end - rptr) < match_len) {
      break;
    }
    if (memcmp (rptr + 1, match + 1, match_len - 1) == 0) {
      if (u32arr_size (&matches) <= found) {
        bl_err err = u32arr_grow (&matches, 32, s->alloc);
        if (err) {
          goto destroy_dynarray;
        }
      }
      *u32arr_at (&matches, found) = (u32) (rptr - s->da.str);
      rptr += match_len;
      ++found;
      if (found == max) {
        break;
      }
    }
    else {
      ++rptr;
    }
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
  char* rptr_prev   = end;
  s->len            = bytes;
  s->da.str[s->len] = 0;
  do {
    --found;
    rptr       = s->da.str + *u32arr_at (&matches, found) + match_len;
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
    return dstr_replace_gt_match_matches_stored_in_string(
      s, match, match_len, replace, replace_len, max, len_bytes
      );
  }
  else
       {
    return dstr_replace_replace_gt_match(
      s, match, match_len, replace, replace_len, max
      );
  }
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
