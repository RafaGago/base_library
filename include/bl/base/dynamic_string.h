#ifndef __BL_DYNSTRING_H__
#define __BL_DYNSTRING_H__

#include <string.h>
#include <bl/base/string.h>
#include <bl/base/platform.h>
#include <bl/base/libexport.h>
#include <bl/base/integer.h>
#include <bl/base/allocator.h>
#include <bl/base/dynarray.h>
#include <bl/base/default_allocator.h>
#include <bl/base/stringbuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

/* A near trivial dynamic string for small string sizes */

/* one of the functions on ctype.h */
typedef int (*bl_ctype_func) (int character);
/*---------------------------------------------------------------------------*/
typedef struct bl_dstr_arr {
  char*    str;
  bl_uword size;
}
bl_dstr_arr;
/*---------------------------------------------------------------------------*/
typedef struct bl_dstr {
  bl_dstr_arr         da;
  bl_alloc_tbl const* alloc;
  bl_uword            len;
}
bl_dstr;
/*---------------------------------------------------------------------------*/
static inline void bl_dstr_init (bl_dstr *s, bl_alloc_tbl const* alloc)
{
  s->da.str  = nullptr;
  s->da.size = 0;
  s->len     = 0;
  s->alloc   = alloc ? alloc : &bl_default_alloc;
}
/*---------------------------------------------------------------------------*/
/*adjust the internal string buffer capacity, setting it to "bl_dstr_len" will
  optimize the string space usage. Setting it to a bigger value will save
  memory allocations at the expense of having potentially unused memory.

  The capacities always exclude the null terminator so a capacity of 1 will
  have room to allocate a character and the null termination.
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_dstr_set_capacity (bl_dstr* s, bl_uword new_len);
static inline bl_uword bl_dstr_get_capacity (bl_dstr const* s)
{
  return s->da.size != 0 ? s->da.size - 1 : 0;
}
/*---------------------------------------------------------------------------*/
static inline void bl_dstr_clear (bl_dstr *s)
{
  s->len = 0;
}
/*---------------------------------------------------------------------------*/
static inline void bl_dstr_destroy (bl_dstr *s)
{
  bl_dstr_clear (s);
  bl_dstr_set_capacity (s, 0);
}
/*---------------------------------------------------------------------------*/
static inline char const* bl_dstr_get (bl_dstr const *s)
{
  return s->len ? (char const*) s->da.str : "";
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_dstr_len (bl_dstr const *s)
{
  return s->len;
}
/*---------------------------------------------------------------------------*/
static inline char const* bl_dstr_beg (bl_dstr const *s)
{
  return bl_dstr_get (s);
}
/*---------------------------------------------------------------------------*/
static inline char const* bl_dstr_end (bl_dstr const *s)
{
  return bl_dstr_beg (s) + bl_dstr_len (s);
}
/*---------------------------------------------------------------------------*/
static inline bl_alloc_tbl const* bl_dstr_alloc (bl_dstr const *s)
{
  return s->alloc;
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_dstrbuf bl_dstr_steal_ownership (bl_dstr *s);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void bl_dstr_transfer_ownership (bl_dstr *s, bl_dstrbuf* b);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err
  bl_dstr_set_l (bl_dstr *s, char const *str, bl_uword len);
extern BL_EXPORT bl_err
  bl_dstr_append_l(bl_dstr *s, char const *str, bl_uword len);
extern BL_EXPORT bl_err bl_dstr_insert_l(
  bl_dstr *s, bl_uword idx, char const *str, bl_uword len
  );
extern BL_EXPORT bl_uword bl_dstr_find_l(
  bl_dstr* s, char const* search, bl_uword search_len, bl_uword offset
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_dstr_replace_l(
  bl_dstr*    s,
  char const* match,
  bl_uword    match_len,
  char const* replace,
  bl_uword    replace_len,
  bl_uword    offset,
  bl_uword    max_replace_count /* 0 replaces all matches */
  );
/*---------------------------------------------------------------------------*/
#define bl_dstr_set_lit(s, lit) bl_dstr_set_l ((s), lit, sizeof lit - 1)
#define bl_dstr_append_lit(s, lit) bl_dstr_append_l ((s), lit, sizeof lit - 1)
#define bl_dstr_insert_lit(s, idx, lit)\
  bl_dstr_insert_l ((s), (idx), lit, sizeof lit - 1)
#define bl_dstr_replace_lit(s, match_lit, replace_lit, offset, count)\
  bl_dstr_replace_l(\
    (s),\
    match_lit,\
    sizeof match_lit - 1,\
    replace_lit,\
    sizeof replace_lit - 1,\
    (offset),\
    (count)\
    )
#define bl_dstr_find_lit(s, search_lit, offset)\
  bl_dstr_find_l ((s), search_lit, sizeof search_lit - 1, (offset))
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_set (bl_dstr *s, char const *str)
{
  return bl_dstr_set_l (s, str, str ? strlen (str) : 0);
}
static inline bl_err bl_dstr_append (bl_dstr *s, char const *str)
{
  return str ? bl_dstr_append_l (s, str, strlen (str)) : bl_mkok();
}
static inline bl_err bl_dstr_insert (bl_dstr *s, bl_uword idx, char const *str)
{
  return str ? bl_dstr_insert_l (s, idx, str, strlen (str)) : bl_mkok();
}
static inline bl_err bl_dstr_replace(
  bl_dstr    *s,
  char const *match,
  char const *replace,
  bl_uword    offset,
  bl_uword    count
  )
{
  return bl_dstr_replace_l(
    s, match, strlen (match), replace, strlen (replace), offset, count
    );
}
static inline bl_uword
  l_dstr_find (bl_dstr* s, char const* seach, bl_uword offset)
{
  return bl_dstr_find_l (s, seach, strlen (seach), offset);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_set_o (bl_dstr *s, bl_dstr const *str)
{
  return bl_dstr_set_l (s, bl_dstr_get (str), bl_dstr_len (str));
}
static inline bl_err bl_dstr_append_o (bl_dstr *s, bl_dstr const *str)
{
  return bl_dstr_append_l (s, bl_dstr_get (str), bl_dstr_len (str));
}
static inline bl_err
  bl_dstr_insert_o (bl_dstr *s, bl_uword idx, bl_dstr const *str)
{
  return bl_dstr_insert_l (s, idx, bl_dstr_get (str), bl_dstr_len (str));
}
static inline bl_err bl_dstr_replace_o(
  bl_dstr       *s,
  bl_dstr const *match,
  bl_dstr const *replace,
  bl_uword       offset,
  bl_uword       count
  )
{
  return bl_dstr_replace_l(
    s,
    bl_dstr_get (match),
    bl_dstr_len (match),
    bl_dstr_get (replace),
    bl_dstr_len (replace),
    offset,
    count
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_set_char (bl_dstr *s, char c)
{
  return bl_dstr_set_l (s, &c, 1);
}
static inline bl_err bl_dstr_append_char (bl_dstr *s, char c)
{
  return bl_dstr_append_l (s, &c, 1);
}
static inline bl_err bl_dstr_insert_char (bl_dstr *s, bl_uword idx, char c)
{
  return bl_dstr_insert_l (s, idx, &c, 1);
}
static inline bl_err bl_dstr_replace_char(
  bl_dstr *s, char match, char replace, bl_uword offset, bl_uword count
  )
{
  return bl_dstr_replace_l (s, &match, 1, &replace, 1, offset, count);
}
/*---------------------------------------------------------------------------*/
/* all the *_va functions use a printf style format string plus varags */
extern BL_EXPORT bl_err
  bl_dstr_set_va (bl_dstr *s, size_t strlen_hint, char const* fmt, ...)
   BL_PRINTF_FORMAT (3, 4);

extern BL_EXPORT bl_err
  bl_dstr_append_va (bl_dstr *s, size_t strlen_hint, char const* fmt, ...)
    BL_PRINTF_FORMAT (3, 4);

extern BL_EXPORT bl_err
  bl_dstr_insert_va(
    bl_dstr *s, size_t strlen_hint, bl_uword idx, char const* fmt, ...
    )
    BL_PRINTF_FORMAT (4, 5);

#ifndef BL_NO_PRINTF_LEN_HINT

#define bl_dstr_set_va_h(s, ...) \
  bl_dstr_set_va( \
    (s), \
    BL_PRINTF_LEN_HINT (__VA_ARGS__), \
    __VA_ARGS__ \
    )

#define bl_dstr_append_va_h(s, ...) \
  bl_dstr_append_va( \
    (s), \
    BL_PRINTF_LEN_HINT (__VA_ARGS__), \
    __VA_ARGS__ \
    )

#define bl_dstr_insert_va_h(s, idx, ...) \
  bl_dstr_insert_va( \
    (s), \
    BL_PRINTF_LEN_HINT (__VA_ARGS__), \
    (idx), \
    __VA_ARGS__ \
    )

#endif /* #ifndef BL_NO_PRINTF_LEN_HINT */
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err
  bl_dstr_erase (bl_dstr *s, bl_uword idx, bl_uword char_count);

static inline bl_err bl_dstr_erase_head (bl_dstr *s, bl_uword char_count)
{
  return bl_dstr_erase (s, 0, char_count);
}
extern BL_EXPORT bl_err bl_dstr_erase_tail (bl_dstr *s, bl_uword char_count);
/*---------------------------------------------------------------------------*/
/* erases characters from the head and the tail, e.g. to trim a string you
   would include ctype.h and do:

       bl_dstr_erase_head_tail_while (s, isspace, 1);

   to remove everything that is not a digit (only from the head and the tail):

       bl_dstr_erase_head_tail_while (s, isdigit, 0);

   Note that fnres is boolean and will be interpreted as 0 or nonzero. This is
   to match the return values of the functions in ctype.h
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_dstr_erase_head_tail_while(
  bl_dstr *s, bl_ctype_func fn, int fnres
  );
extern BL_EXPORT bl_err bl_dstr_erase_head_while(
  bl_dstr *s, bl_ctype_func fn, int fnres
  );
extern BL_EXPORT bl_err bl_dstr_erase_tail_while(
  bl_dstr *s, bl_ctype_func fn, int fnres
  );
/*---------------------------------------------------------------------------*/
/* applies transformation to range of chars, e.g. to convert to lower case you
   would include ctype.h and do:

        bl_dstr_apply (s, tolower, 0, bl_dstr_len(s));

   Note that there is the apply_all convenience function to do the same

        bl_dstr_apply_all (s, tolower);
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_dstr_apply(
    bl_dstr *s, bl_ctype_func fn, bl_uword start, bl_uword end
    );
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_apply_all (bl_dstr *s, bl_ctype_func fn)
{
    return bl_dstr_apply (s, fn, 0, bl_dstr_len (s));
}
/*---------------------------------------------------------------------------*/
/* sets from file */
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_dstr_append_file(
  bl_dstr *s, FILE* file, bl_uword file_read_limit
  );
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_set_file(
  bl_dstr *s, FILE* file, bl_uword file_read_limit
  )
{
  bl_dstr_clear (s);
  return bl_dstr_append_file (s, file, file_read_limit);
}
/*---------------------------------------------------------------------------*/
/*convenience init funcs*/
static inline bl_err bl_dstr_init_str(
  bl_dstr *s, char const* str, bl_alloc_tbl const* alloc
  )
{
  bl_dstr_init (s, alloc);
  return bl_dstr_set (s, str);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_dstr_init_str_o(
  bl_dstr *s, bl_dstr const *str, bl_alloc_tbl const* alloc
  )
{
  bl_dstr_init (s, alloc);
  return bl_dstr_set_o (s, str);
}
/*---------------------------------------------------------------------------*/
static inline bl_dstr bl_dstr_init_rv (bl_alloc_tbl const* alloc)
{
  bl_dstr r;
  bl_dstr_init (&r, alloc);
  return r;
}
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} //extern "C" {
#endif

#endif
