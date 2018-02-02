#ifndef __BL_DYN_ARRAY_H__
#define __BL_DYN_ARRAY_H__

#include <stdio.h> /* FILE* */

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/error.h>
#include <bl/base/allocator.h>
#include <bl/base/libexport.h>

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
typedef struct dynarray_stub {
  void* arr;
  uword size;
}
dynarray_stub;
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dynarray_resize(
  dynarray_stub*   d,
  uword            new_size,
  uword            elem_size,
  alloc_tbl const* alloc
  );
/*---------------------------------------------------------------------------*/
/* reads file contents to a dynarray.

  If "fp_read_limit" isn't zero fp will be advanced at most "fp_read_limit"
  bytes.

  If d is non empty its actual contents outside the range comprised between
  "d_offset" to  "d_offset" + "(bytes read from file)" are respected.

  On success the resulting array will always be at least
  "d_offset" + "(bytes read from file)" + "d_overalloc" bytes.

  If d has not enough space, the new dynarray size will be a multiple of
  "d_realloc_multiple_of". "d_realloc_multiple_of" = 0 will be interpreted as 1.

  fp must contain a succesfully open file descriptor. It won't be closed.
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err dynarray_from_file(
  dynarray_stub*   d,
  uword*           d_written,
  uword            d_offset,
  uword            d_overalloc,
  uword            d_realloc_multiple_of,
  FILE*            fp,
  uword            fp_read_limit,
  alloc_tbl const* alloc
  );
/*---------------------------------------------------------------------------*/
#define define_dynarray_types(prefix, content_type)\
\
typedef struct prefix {\
  content_type* arr;\
  uword         size;\
}\
prefix;
/*---------------------------------------------------------------------------*/
#define declare_dynarray_funcs(prefix, content_type)\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_resize (prefix* d, uword new_size, alloc_tbl const* alloc)\
{\
  return dynarray_resize(\
    (dynarray_stub*) d, new_size, sizeof *d->arr, alloc\
    );\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_init_empty (prefix* d)\
{\
  d->arr  = nullptr;\
  d->size = 0;\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_init (prefix* d, uword size, alloc_tbl const* alloc)\
{\
  prefix##_init_empty (d);\
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
content_type* prefix##_get_array_ownership (prefix* d)\
{\
  content_type* c = d->arr;\
  d->arr          = nullptr;\
  d->size         = 0;\
  return c;\
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
bl_err prefix##_grow (prefix* d, uword elem_count, alloc_tbl const* alloc)\
{\
  return prefix##_resize (d, prefix##_size (d) + elem_count, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_from_file(\
  prefix*          d,\
  uword*           d_elems_written,\
  uword            d_elem_offset,\
  uword            d_elem_overalloc,\
  FILE*            fp,\
  uword            fp_read_limit,\
  alloc_tbl const* alloc\
  )\
{\
  bl_err err = dynarray_from_file(\
    (dynarray_stub*) d,\
    d_elems_written,\
    d_elem_offset * sizeof *d->arr,\
    d_elem_overalloc * sizeof *d->arr,\
    sizeof *d->arr,\
    fp,\
    fp_read_limit,\
    alloc\
    );\
  *d_elems_written /= sizeof *d->arr;\
  return err;\
}\
/*--------------------------------------------------------------------------*/\

#endif /* __BL_DYN_ARRAY_H__ */

