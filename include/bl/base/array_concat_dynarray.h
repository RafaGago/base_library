#ifndef __BL_ARRAY_CONCAT_DYNARRAY_H__
#define __BL_ARRAY_CONCAT_DYNARRAY_H__

#include <bl/base/dynarray.h>
#include <bl/base/assert.h>
#include <bl/base/preprocessor_basic.h>

/* a concatenation of a fixed sized array with a dynamic array. useful for e.g.
   optimize allocations by defining a fixed stack array for small sizes and
   handle bigger sizes with a dynamic array.*/
#define bl_define_ac_dynarray_types(prefix, content_type)\
\
bl_define_dynarray_types (prefix##_da, content_type)\
\
typedef struct prefix {\
  content_type*   arr;\
  bl_uword           size;\
  prefix##_da     da;\
}\
prefix;

#define bl_declare_ac_dynarray_funcs(prefix, content_type)\
/*--------------------------------------------------------------------------*/\
bl_declare_dynarray_funcs (prefix##_da, content_type)\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_init (prefix* d, content_type* arr, bl_uword arr_size)\
{\
  bl_assert ((arr && arr_size > 0) || arr_size == 0);\
  d->arr     = arr;\
  d->size    = arr_size;\
  d->da.arr  = nullptr;\
  d->da.size = 0;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_destroy (prefix* d, bl_alloc_tbl const* alloc)\
{\
  prefix##_da_resize (&d->da, 0, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_resize (prefix* d, bl_uword new_size, bl_alloc_tbl const* alloc)\
{\
  if (new_size <= d->size) {\
    prefix##_da_resize (&d->da, 0, alloc);\
    return bl_mkok();\
  }\
  return prefix##_da_resize (&d->da, new_size - d->size, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_uword prefix##_arr_size (prefix const* d)\
{\
  bl_assert (d);\
  return d->size;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_uword prefix##_darr_size (prefix const* d)\
{\
  bl_assert (d);\
  return + prefix##_da_size (&d->da);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_uword prefix##_size (prefix const* d)\
{\
  return prefix##_arr_size (d) + prefix##_darr_size (d);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_grow (prefix* d, bl_uword add, bl_alloc_tbl const* alloc)\
{\
  return prefix##_resize (d, prefix##_size (d) + add, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_at (prefix const* d, bl_uword idx)\
{\
  bl_assert (idx < prefix##_size (d));\
  return (idx < d->size) ? \
    d->arr + idx : prefix##_da_at (&d->da, idx - d->size);\
}\
/*--------------------------------------------------------------------------*/

#endif /* __BL_ARRAY_CONCAT_DYNARRAY_H__ */
