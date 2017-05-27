#ifndef __BL_DYNSTRING_H__
#define __BL_DYNSTRING_H__

#include <string.h>
#include <bl/base/platform.h>
#include <bl/base/libexport.h>
#include <bl/base/integer.h>
#include <bl/base/allocator.h>
#include <bl/base/dynarray.h>

/* one of the functions on ctype.h */
typedef int (*bl_ctype_func) (int character);
/*---------------------------------------------------------------------------*/
typedef struct dstr_arr {
  char* str;
  uword size;
}
dstr_arr;
/*---------------------------------------------------------------------------*/
typedef struct dstr {
  dstr_arr         da;
  alloc_tbl const* alloc;
  uword            len;
}
dstr;
/*---------------------------------------------------------------------------*/
static inline void dstr_init (dstr *s, alloc_tbl const* alloc)
{
  s->da.str  = nullptr;
  s->da.size = 0;
  s->len     = 0;
  s->alloc   = alloc;
}
/*---------------------------------------------------------------------------*/
/*adjust the internal string buffer capacity, setting it to "dstr_len" will
  optimize the string space usage. Setting it to a bigger value will save
  memory allocations at the expense of having potentially unused memory.

  The capacities always exclude the null terminator so a capacity of 1 will
  have room to allocate a character and the null termination.
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dstr_set_capacity (dstr* s, uword new_len);
static inline uword dstr_get_capacity (dstr const* s)
{
  return s->da.size != 0 ? s->da.size - 1 : 0;
}
/*---------------------------------------------------------------------------*/
static inline void dstr_clear (dstr *s)
{
  s->len = 0;
}
/*---------------------------------------------------------------------------*/
static inline void dstr_destroy (dstr *s)
{
  dstr_clear (s);
  dstr_set_capacity (s, 0);
}
/*---------------------------------------------------------------------------*/
static inline char const* dstr_get (dstr const *s)
{
  return s->da.str ? (char const*) s->da.str : "";
}
/*---------------------------------------------------------------------------*/
static inline uword dstr_len (dstr const *s)
{
  return s->len;
}
/*---------------------------------------------------------------------------*/
static inline alloc_tbl const* dstr_alloc (dstr const *s)
{
  return s->alloc;
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT char* dstr_steal_ownership (dstr *s);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void dstr_transfer_ownership(
  dstr *s, char* heap_string_from_same_alloc
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dstr_set_l (dstr *s, char const *str, uword len);
extern BL_EXPORT bl_err dstr_append_l (dstr *s, char const *str, uword len);
extern BL_EXPORT bl_err dstr_insert_l(
  dstr *s, uword idx, char const *str, uword len
  );
/*---------------------------------------------------------------------------*/
#define dstr_set_lit(s, lit) dstr_set_l ((s), lit, sizeof lit - 1)
#define dstr_append_lit(s, lit) dstr_append_l ((s), lit, sizeof lit - 1)
#define dstr_insert_lit(s, idx, lit)\
  dstr_insert_l ((s), (idx), lit, sizeof lit - 1)
/*---------------------------------------------------------------------------*/
#define dstr_set_lit(s, lit)     dstr_set_l ((s), lit, sizeof lit - 1)
#define dstr_append_lit(s, lit)  dstr_append_l ((s), lit, sizeof lit - 1)
#define dstr_prepend_lit(s, lit) dstr_prepend_l ((s), lit, sizeof lit - 1)
/*---------------------------------------------------------------------------*/
static inline bl_err dstr_set (dstr *s, char const *str)
{
  return str ? dstr_set_l (s, str, strlen (str)) : bl_ok;
}
static inline bl_err dstr_append (dstr *s, char const *str)
{
  return str ? dstr_append_l (s, str, strlen (str)) : bl_ok;
}
static inline bl_err dstr_insert (dstr *s, uword idx, char const *str)
{
  return str ? dstr_insert_l (s, idx, str, strlen (str)) : bl_ok;
}
/*---------------------------------------------------------------------------*/
static inline bl_err dstr_set_o (dstr *s, dstr const *str)
{
  return dstr_set_l (s, dstr_get (str), dstr_len (str));
}
static inline bl_err dstr_append_o (dstr *s, dstr const *str)
{
  return dstr_append_l (s, dstr_get (str), dstr_len (str));
}
static inline bl_err dstr_insert_o (dstr *s, uword idx, dstr const *str)
{
  return dstr_insert_l (s, idx, dstr_get (str), dstr_len (str));
}
/*---------------------------------------------------------------------------*/
/* all the *_va functions use a printf style format string plus varags */
extern BL_EXPORT bl_err
  dstr_set_va (dstr *s, char const* fmt, ...)
   BL_PRINTF_FORMAT (2, 3);

extern BL_EXPORT bl_err
  dstr_append_va (dstr *s, char const* fmt, ...)
    BL_PRINTF_FORMAT (2, 3);

extern BL_EXPORT bl_err
  dstr_insert_va (dstr *s, uword idx, char const* fmt, ...)
    BL_PRINTF_FORMAT (3, 4);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dstr_erase_head (dstr *s, uword char_count);
extern BL_EXPORT bl_err dstr_erase_tail (dstr *s, uword char_count);
/*---------------------------------------------------------------------------*/
/* erases characters from the head and the tail, e.g. to trim a string you
   would include ctype.h and do:

       dstr_erase_head_tail_while (s, isspace, 1);

   to remove everything that is not a digit (only from the head and the tail):

       dstr_erase_head_tail_while (s, isdigit, 0);

   Note that fnres is boolean and will be interpreted as 0 or nonzero. This is
   to match the return values of the functions in ctype.h
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dstr_erase_head_tail_while(
  dstr *s, bl_ctype_func fn, int fnres
  );
extern BL_EXPORT bl_err dstr_erase_head_while(
  dstr *s, bl_ctype_func fn, int fnres
  );
extern BL_EXPORT bl_err dstr_erase_tail_while(
  dstr *s, bl_ctype_func fn, int fnres
  );
/*---------------------------------------------------------------------------*/
/* applies transformation to range of chars, e.g. to convert to lower case you
   would include ctype.h and do:

        dstr_apply (s, tolower, 0, dstr_len(s));

   Note that there is the apply_all convenience function to do the same

        dstr_apply_all (s, tolower);
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dstr_apply(
    dstr *s, bl_ctype_func fn, uword start, uword end
    );
/*---------------------------------------------------------------------------*/
static inline bl_err dstr_apply_all (dstr *s, bl_ctype_func fn)
{
    return dstr_apply (s, fn, 0, dstr_len (s));
}
/*---------------------------------------------------------------------------*/
/*convenience init funcs*/
static inline bl_err dstr_init_str(
  dstr *s, char const* str, alloc_tbl const* alloc
  )
{
  dstr_init (s, alloc);
  return dstr_set (s, str);
}
/*---------------------------------------------------------------------------*/
static inline bl_err dstr_init_str_o(
  dstr *s, dstr const *str, alloc_tbl const* alloc
  )
{
  dstr_init (s, alloc);
  return dstr_set_o (s, str);
}
/*---------------------------------------------------------------------------*/
static inline dstr dstr_init_rv (alloc_tbl const* alloc)
{
  dstr r;
  dstr_init (&r, alloc);
  return r;
}
/*---------------------------------------------------------------------------*/
#endif
