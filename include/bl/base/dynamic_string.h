#ifndef __BL_DYNSTRING_H__
#define __BL_DYNSTRING_H__

/* A naive non-optimized staightforward dynamic string */

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
}
dstr;
/*---------------------------------------------------------------------------*/
static inline void dstr_init (dstr *s, alloc_tbl const* alloc)
{
  s->da.str  = nullptr;
  s->da.size = 0;
  s->alloc   = alloc;
}
/*---------------------------------------------------------------------------*/
static inline char* dstr_take_ownership (dstr *s)
{
  char* ret = s->da.str;
  dstr_init (s, s->alloc);
  return ret;
}
/*---------------------------------------------------------------------------*/
static inline char const* dstr_get (dstr const *s)
{
  return s->da.str ? (char const*) s->da.str : "";
}
/*---------------------------------------------------------------------------*/
static inline uword dstr_len (dstr const *s)
{
  return s->da.str ? s->da.size - 1 : 0;
}
/*---------------------------------------------------------------------------*/
static inline alloc_tbl const* dstr_alloc (dstr const *s)
{
  return s->alloc;
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dstr_set (dstr *s, char const *str);
extern BL_EXPORT bl_err dstr_append (dstr *s, char const *str);
extern BL_EXPORT bl_err dstr_prepend (dstr *s, char const *str);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dstr_set_o (dstr *s, dstr const *str);
extern BL_EXPORT bl_err dstr_append_o (dstr *s, dstr const *str);
extern BL_EXPORT bl_err dstr_prepend_o (dstr *s, dstr const *str);
/*---------------------------------------------------------------------------*/
/* all the *_va functions use a printf style format string plus varags */
extern BL_EXPORT bl_err dstr_set_va (dstr *s, char const* fmt, ...);
extern BL_EXPORT bl_err dstr_append_va (dstr *s, char const* fmt, ...);
extern BL_EXPORT bl_err dstr_prepend_va (dstr *s, char const* fmt, ...);
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
extern BL_EXPORT void dstr_clear (dstr *s);
/*---------------------------------------------------------------------------*/
static inline void dstr_destroy (dstr *s)
{
  dstr_clear (s);
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
