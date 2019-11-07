#ifndef __BL_AUTO_ARRAY_H__
#define __BL_AUTO_ARRAY_H__

#include <stddef.h>
#include <string.h>
#include <bl/base/libexport.h>
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/error.h>
#include <bl/base/allocator.h>
#include <bl/base/utility.h>
#include <bl/base/dynarray.h> /*needs "bl_dynarray_resize"*/

#ifdef __cplusplus
extern "C" {
#endif

/*a dynamic array seen as a collection of data */

/*---------------------------------------------------------------------------*/
#define bl_autoarray_foreach(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
#define bl_autoarray_foreach_const(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type const* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
typedef struct bl_autoarray_stub {
  void* arr;
  bl_uword capacity;
  bl_uword size;
}
bl_autoarray_stub;
/*---------------------------------------------------------------------------*/
bl_static_assert_global_ns(
  offsetof (bl_autoarray_stub, capacity) == offsetof (bl_dynarray_stub, size)
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_autoarray_set_rounded_capacity(
  bl_autoarray_stub*  d,
  bl_uword            capacity,
  bl_uword            elem_size,
  bl_alloc_tbl const* alloc
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_autoarray_insert_n(
  bl_autoarray_stub*  d,
  bl_uword            idx,
  void const*         c,
  bl_uword            c_count,
  bl_uword            elem_size,
  bl_alloc_tbl const* alloc
  );
/*--------------------------------------------------------------------------*/
#define bl_define_autoarray_types(prefix, content_type)\
\
typedef struct prefix {\
  content_type* arr;\
  bl_uword      capacity;\
  bl_uword      size;\
}\
prefix;\
/*---------------------------------------------------------------------------*/
#define bl_declare_autoarray_funcs(prefix, content_type)\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_init (prefix* d, bl_uword capacity, bl_alloc_tbl const* alloc)\
{\
  d->arr      = nullptr;\
  d->size     = 0;\
  d->capacity = 0;\
  return bl_dynarray_resize(\
    (bl_dynarray_stub*) d, capacity, sizeof *d->arr, alloc\
    );\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_destroy (prefix* d, bl_alloc_tbl const* alloc)\
{\
  bl_dynarray_resize ((bl_dynarray_stub*) d, 0, sizeof *d->arr, alloc);\
  d->size = 0;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_get_array_ownership (prefix* d)\
{\
  content_type* c = d->arr;\
  d->arr          = nullptr;\
  d->size         = 0;\
  d->capacity     = 0;\
  return c;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_set_capacity(\
  prefix* d, bl_uword new_capacity, bl_alloc_tbl const* alloc\
  )\
{\
  assert (d && alloc);\
  return (new_capacity >= d->size) \
    ? bl_dynarray_resize( \
        (bl_dynarray_stub*) d, new_capacity, sizeof *d->arr, alloc \
        ) \
    : bl_mkerr (bl_preconditions);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_uword prefix##_size (prefix const* d)\
{\
  bl_assert (d);\
  return d->size;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_uword prefix##_capacity (prefix const* d)\
{\
  bl_assert (d);\
  return d->capacity;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_at (prefix const* d, bl_uword idx)\
{\
  bl_assert (idx < d->size);\
  return d->arr + idx;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
content_type* prefix##_beg (prefix const* d)\
{\
  bl_assert (d);\
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
  bl_assert (d);\
  return d->arr + prefix##_size (d);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_insert_n(\
  prefix*             d,\
  bl_uword               idx,\
  content_type const* c,\
  bl_uword               c_count,\
  bl_alloc_tbl const*    alloc\
  )\
{\
  return bl_autoarray_insert_n(\
    (bl_autoarray_stub*) d, idx, c, c_count, sizeof *d->arr, alloc\
    );\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_insert(\
  prefix* d, bl_uword idx, content_type const* c, bl_alloc_tbl const* alloc\
  )\
{\
  return prefix##_insert_n (d, idx, c, 1, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_insert_tail(\
  prefix* d, content_type const* c, bl_alloc_tbl const* alloc\
  )\
{\
  assert (d && c && alloc);\
  bl_err err = bl_mkok();\
  if (d->size + 1 > d->capacity) {\
    err = bl_autoarray_set_rounded_capacity(\
      (bl_autoarray_stub*) d, d->size + 1, sizeof *d->arr, alloc\
      );\
  }\
  if (!err.bl) {\
    *prefix##_end (d) = *c;\
    ++d->size;\
  }\
  return err;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_insert_tail_n(\
  prefix* d, content_type const* c, bl_uword c_count, bl_alloc_tbl const* alloc\
  )\
{\
  return prefix##_insert_n (d, d->size, c, c_count, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_drop_tail_n (prefix* d, bl_uword count)\
{\
  assert (d);\
  assert (count <= d->size);\
  d->size -= count;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_drop_tail (prefix* d)\
{\
  prefix##_drop_tail_n (d, 1);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_drop_n (prefix* d, bl_uword idx, bl_uword count)\
{\
  assert (d);\
  bl_uword move_elems = d->size - (idx + count);\
  assert (move_elems <= d->size);\
  memmove (&d->arr[idx], &d->arr[idx + count], move_elems * sizeof d->arr[0]);\
  d->size -= count;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_drop (prefix* d, bl_uword idx)\
{\
  prefix##_drop_n (d, idx, 1);\
}
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
} //extern "C" {
#endif

#endif /* __BL_AUTO_ARRAY_H__ */

