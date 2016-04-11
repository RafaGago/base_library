#ifndef __BL_DYN_ARRAY_H__
#define __BL_DYN_ARRAY_H__

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/error.h>
#include <bl/base/allocator.h>

/* a resizeable and very raw memory chunk */
/*---------------------------------------------------------------------------*/
#define dynarray_foreach(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
#define dynarray_foreach_const(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type const* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
/* dynamic array (storing the size + realloc) syntax "sugar"                 */
/*---------------------------------------------------------------------------*/
#define define_dynarray_types(prefix, content_type)\
\
typedef struct prefix {\
  content_type* arr;\
  uword         size;\
}\
prefix;
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#define declare_dynarray_funcs(prefix, content_type, linkage_and_modif)\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_resize (prefix* d, uword new_size, alloc_tbl const* alloc);\
\
static inline \
bl_err prefix##_init (prefix* d, uword size, alloc_tbl const* alloc)\
{\
  d->arr  = nullptr;\
  d->size = 0;\
  return prefix##_resize (d, size, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_destroy (prefix* d, alloc_tbl const* alloc)\
{\
  prefix##_resize (d, 0, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
uword prefix##_size (prefix const* d)\
{\
  bl_assert (d);\
  return d->size;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_at (prefix const* d, uword idx)\
{\
  bl_assert (idx < d->size);\
  return d->arr + idx;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_beg (prefix const* d)\
{\
  return d->arr;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_last (prefix const* d)\
{\
  return prefix##_at (d, prefix##_size (d) - 1);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_end (prefix const* d)\
{\
  return d->arr + prefix##_size (d);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_grow (prefix* d, uword elem_count, alloc_tbl const* alloc)\
{\
  return prefix##_resize (d, prefix##_size (d) + elem_count, alloc);\
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#define define_dynarray_funcs(prefix, content_type, linkage_and_modif)\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_resize (prefix* d, uword new_size, alloc_tbl const* alloc)\
{\
  bl_assert (d && alloc);\
  bl_err err = bl_ok;\
  content_type* new_ptr;\
\
  if (new_size != 0) {\
    new_ptr = (content_type*) bl_realloc(\
      alloc, d->arr, new_size * sizeof (d->arr[0])\
      );\
    if (unlikely (!new_ptr)) {\
      new_size = d->size;\
      new_ptr  = d->arr;\
      err = bl_alloc;\
    }\
  }\
  else {\
    /*realloc with size 0 has implementation defined behavior on C11*/\
    bl_dealloc (alloc, d->arr);\
    new_ptr = nullptr;\
  }\
  d->size = new_size;\
  d->arr  = new_ptr;\
  return err;\
}
/*---------------------------------------------------------------------------*/

#endif /* __BL_DYN_ARRAY_H__ */

