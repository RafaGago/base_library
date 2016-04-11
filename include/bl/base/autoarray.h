#ifndef __BL_AUTO_ARRAY_H__
#define __BL_AUTO_ARRAY_H__

#include <string.h>
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/error.h>
#include <bl/base/allocator.h>
#include <bl/base/integer_math.h>
#include <bl/base/utility.h>

/*a dynamic array seen as a collection of data */

/*---------------------------------------------------------------------------*/
#define autoarray_foreach(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
#define autoarray_foreach_const(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type const* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
/* dynamic array (storing the size + realloc) syntax "sugar"                 */
/*---------------------------------------------------------------------------*/
#define define_autoarray_types(prefix, content_type)\
\
typedef struct prefix {\
  content_type* arr;\
  uword         size;\
  uword         capacity;\
}\
prefix;\
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#define declare_autoarray_funcs(prefix, content_type, linkage_and_modif)\
\
linkage_and_modif \
bl_err prefix##_set_capacity_private(\
  prefix* d, uword new_capacity, alloc_tbl const* alloc\
  );\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_init (prefix* d, uword capacity, alloc_tbl const* alloc)\
{\
  d->arr      = nullptr;\
  d->size     = 0;\
  d->capacity = 0;\
  return prefix##_set_capacity_private (d, capacity, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_destroy (prefix* d, alloc_tbl const* alloc)\
{\
  prefix##_set_capacity_private (d, 0, alloc);\
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
  prefix* d, uword new_capacity, alloc_tbl const* alloc\
  )\
{\
  assert (d && alloc);\
  return (new_capacity >= d->size) ?\
    prefix##_set_capacity_private (d, new_capacity, alloc) :\
    bl_not_allowed;\
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
uword prefix##_capacity (prefix const* d)\
{\
  bl_assert (d);\
  return d->capacity;\
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
linkage_and_modif \
bl_err prefix##_insert_n(\
  prefix*             d,\
  uword               idx,\
  content_type const* c,\
  uword               c_count,\
  alloc_tbl const*    alloc\
  );\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_insert(\
  prefix* d, uword idx, content_type const* c, alloc_tbl const* alloc\
  )\
{\
  return prefix##_insert_n (d, idx, c, 1, alloc);\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_insert_tail(\
  prefix* d, content_type const* c, alloc_tbl const* alloc\
  );\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_insert_tail_n(\
  prefix* d, content_type const* c, uword c_count, alloc_tbl const* alloc\
  )\
{\
  return prefix##_insert_n (d, d->size, c, c_count, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_drop_tail_n (prefix* d, uword count)\
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
void prefix##_drop_n (prefix* d, uword idx, uword count)\
{\
  assert (d);\
  uword move_elems = d->size - (idx + count);\
  assert (move_elems <= d->size);\
  memcpy (&d->arr[idx], &d->arr[idx + count], move_elems * sizeof d->arr[0]);\
  d->size -= count;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_drop (prefix* d, uword idx)\
{\
  prefix##_drop_n (d, idx, 1);\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_insert_prepare_private(\
  prefix* d, uword count, alloc_tbl const* alloc\
  );\
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#define define_autoarray_funcs(prefix, content_type, linkage_and_modif)\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_set_capacity_private(\
  prefix* d, uword new_capacity, alloc_tbl const* alloc\
  )\
{\
  bl_assert (d && alloc);\
  bl_err err = bl_ok;\
  content_type* new_ptr;\
\
  if (new_capacity != 0) {\
    new_ptr = (content_type*) bl_realloc(\
      alloc, d->arr, new_capacity * sizeof (d->arr[0])\
      );\
    if (unlikely (!new_ptr)) {\
      new_capacity = d->capacity;\
      new_ptr      = d->arr;\
      err = bl_alloc;\
    }\
  }\
  else {\
    /*realloc with size 0 has implementation defined behavior on C11*/\
    bl_dealloc (alloc, d->arr);\
    new_ptr = nullptr;\
  }\
  d->capacity = new_capacity;\
  d->arr      = new_ptr;\
  return err;\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_insert_prepare_private(\
  prefix* d, uword count, alloc_tbl const* alloc\
  )\
{\
  assert (d && alloc);\
  uword  required = d->size + count;\
  bl_err err      = bl_ok;\
  if (required > d->capacity) {\
    required = required != 0 ? round_next_pow2_u (required) : 2;\
    bl_assert (required >= d->capacity);\
    err      = prefix##_set_capacity_private (d, required, alloc);\
  }\
  return err;\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_insert_tail(\
  prefix* d, content_type const* c, alloc_tbl const* alloc\
  )\
{\
  assert (d && c && alloc);\
  bl_err err = prefix##_insert_prepare_private (d, 1, alloc);\
  if (!err) {\
    *prefix##_end (d) = *c;\
    ++d->size;\
  }\
  return err;\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_insert_n(\
  prefix*             d,\
  uword               idx,\
  content_type const* c,\
  uword               c_count,\
  alloc_tbl const*    alloc\
  )\
{\
  assert (d && c && alloc);\
  assert (idx <= d->size);\
  bl_err err = prefix##_insert_prepare_private (d, c_count, alloc);\
  if (!err) {\
    uword bsz = c_count * sizeof (d->arr[0]);\
    if (idx == d->size) {\
      memcpy (prefix##_end (d), c, bsz);\
    }\
    else {\
      memmove (&d->arr[idx + c_count], &d->arr[idx], bsz);\
      memcpy (&d->arr[idx], c, bsz);\
    }\
    d->size += c_count;\
  }\
  return err;\
}
/*---------------------------------------------------------------------------*/

#endif /* __BL_AUTO_ARRAY_H__ */

