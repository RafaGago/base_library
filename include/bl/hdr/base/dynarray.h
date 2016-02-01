#ifndef __BL_DYN_ARRAY_H__
#define __BL_DYN_ARRAY_H__

#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/error.h>
#include <bl/hdr/base/allocator.h>

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
#define declare_dynarray_funcs(prefix, content_type, linkage_and_modif)\
\
linkage_and_modif \
bl_err prefix##_init (prefix* d, uword size, alloc_tbl const* alloc);\
\
linkage_and_modif \
bl_err prefix##_resize (prefix* d, uword new_size, alloc_tbl const* alloc);\
\
\
static inline \
void prefix##_destroy (prefix* d, alloc_tbl const* alloc)\
{\
  if (!d->arr) {\
    return;\
  }\
  bl_dealloc (alloc, d->arr);\
  d->arr  = nullptr;\
  d->size = 0;\
}\
\
static inline \
uword prefix##_size (prefix const* d)\
{\
  bl_assert (d);\
  return d->size;\
}\
\
static inline \
content_type* prefix##_at (prefix const* d, uword idx)\
{\
  bl_assert (idx < d->size);\
  return d->arr + idx;\
}
/*---------------------------------------------------------------------------*/
#define define_dynarray_funcs(prefix, content_type, linkage_and_modif)\
linkage_and_modif \
bl_err prefix##_init (prefix* d, uword size, alloc_tbl const* alloc)\
{\
  bl_assert (d);\
  d->arr  = nullptr;\
  d->size = size;\
  if (size == 0) {\
    return bl_ok;\
  }\
  return prefix##_resize (d, size, alloc);\
}\
\
linkage_and_modif \
bl_err prefix##_resize (prefix* d, uword new_size, alloc_tbl const* alloc)\
{\
  bl_assert (d);\
  bl_assert (new_size != 0);\
  d->arr = bl_realloc (alloc, d->arr, new_size * sizeof (d->arr[0]));\
  if (d->arr) {\
    d->size = new_size;\
    return bl_ok;\
  }\
  else {\
    d->size = 0;\
    return bl_alloc;\
  }\
}
/*---------------------------------------------------------------------------*/

#endif /* __BL_DYN_ARRAY_H__ */

