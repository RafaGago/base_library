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
#define bl_dynarray_foreach(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
#define bl_dynarray_foreach_const(prefix, content_type, instance_ptr, varname)\
  for(\
    content_type const* varname = prefix##_beg (instance_ptr);\
    varname < prefix##_end (instance_ptr);\
    ++varname\
    )
/*---------------------------------------------------------------------------*/
/* dynamic array (storing the size + realloc) syntax "sugar"                 */
/*---------------------------------------------------------------------------*/
typedef struct bl_dynarray_stub {
  void* arr;
  bl_uword size;
}
bl_dynarray_stub;
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_dynarray_resize(
  bl_dynarray_stub*   d,
  bl_uword               new_size,
  bl_uword               elem_size,
  bl_alloc_tbl const* alloc
  );
/*---------------------------------------------------------------------------*/
/* reads file contents to a bl_dynarray.

  If "fp_read_limit" isn't zero fp will be advanced at most "fp_read_limit"
  bytes.

  If d is non empty its actual contents outside the range comprised between
  "d_offset" to  "d_offset" + "(bytes read from file)" are respected.

  On success the resulting array will always be at least
  "d_offset" + "(bytes read from file)" + "d_overalloc" bytes.

  If d has not enough space, the new bl_dynarray size will be a multiple of
  "d_realloc_multiple_of". "d_realloc_multiple_of" = 0 will be interpreted as 1.

  fp must contain a succesfully open file descriptor. It won't be closed.
*/
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_dynarray_from_file(
  bl_dynarray_stub*   d,
  bl_uword*              d_written,
  bl_uword               d_offset,
  bl_uword               d_overalloc,
  bl_uword               d_realloc_multiple_of,
  FILE*               fp,
  bl_uword               fp_read_limit,
  bl_alloc_tbl const* alloc
  );
/*---------------------------------------------------------------------------*/
#define bl_define_dynarray_types(prefix, content_type)\
\
typedef struct prefix {\
  content_type* arr;\
  bl_uword         size;\
}\
prefix;
/*---------------------------------------------------------------------------*/
#define bl_declare_dynarray_funcs(prefix, content_type)\
/*--------------------------------------------------------------------------*/\
static inline bl_err \
  prefix##_resize (prefix* d, bl_uword new_size, bl_alloc_tbl const* alloc)\
{\
  return bl_dynarray_resize(\
    (bl_dynarray_stub*) d, new_size, sizeof *d->arr, alloc\
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
bl_err prefix##_init (prefix* d, bl_uword size, bl_alloc_tbl const* alloc)\
{\
  prefix##_init_empty (d);\
  return prefix##_resize (d, size, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
void prefix##_destroy (prefix* d, bl_alloc_tbl const* alloc)\
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
bl_uword prefix##_size (prefix const* d)\
{\
  bl_assert (d);\
  return d->size;\
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
static inline bl_err \
  prefix##_grow (prefix* d, bl_uword elem_count, bl_alloc_tbl const* alloc)\
{\
  return prefix##_resize (d, prefix##_size (d) + elem_count, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
bl_err prefix##_from_file(\
  prefix*             d,\
  bl_uword*           d_elems_written,\
  bl_uword            d_elem_offset,\
  bl_uword            d_elem_overalloc,\
  FILE*               fp,\
  bl_uword            fp_read_limit,\
  bl_alloc_tbl const* alloc\
  )\
{\
  bl_err err = bl_dynarray_from_file(\
    (bl_dynarray_stub*) d,\
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

