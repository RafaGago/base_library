#ifndef __BL_FLAT_DEADLINES_H__
#define __BL_FLAT_DEADLINES_H__

/*represents a fixed-size array-based associative structure with 
 wrapping/overflowing timestamps as the key and some struct as the value. to
 use with small datasets and structures.

 As this is a structure to manage deadlines it should be updated frequently
 (through "get_head_if_expired") to avoid letting the underlying tstamp
 unsigned integer advance half its resolution
 */
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/deadline.h>
#include <bl/base/ordered_ringbuffer.h>
#include <bl/base/error.h>
#include <bl/base/time.h>
#include <bl/base/allocator.h>
/*---------------------------------------------------------------------------*/
/* Type definition*/
/*---------------------------------------------------------------------------*/
#define define_flat_deadlines_types(prefix, value_type)\
typedef struct prefix##_entry {\
  tstamp     key;\
  value_type value;\
}\
prefix##_entry;\
\
/*--------------------------------------------------------------------------*/\
define_oringb_types (prefix##_ringb, prefix##_entry)\
/*--------------------------------------------------------------------------*/\
typedef struct prefix {\
  prefix##_ringb list;\
  tstamp         time_offset;\
}\
prefix;
/*---------------------------------------------------------------------------*/
/* Function declarations*/
/*---------------------------------------------------------------------------*/
#define declare_flat_deadlines_funcs(prefix, value_type, linkage_and_modif)\
/*--------------------------------------------------------------------------*/\
declare_oringb_funcs (prefix##_ringb, prefix##_entry, linkage_and_modif)\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
prefix##_entry const* prefix##_get_head (prefix* dl);\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_init_explicit(\
  prefix* dl, tstamp now, uword capacity, alloc_tbl const* alloc\
  );\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
void prefix##_destroy (prefix* dl, alloc_tbl const* alloc);\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_insert (prefix* dl, prefix##_entry* e);\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
prefix##_entry const* prefix##_get_head (prefix* dl);\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
prefix##_entry const* prefix##_get_head_if_expired_impl_priv(\
  prefix* dl, bool explicit, tstamp now\
  );\
/*--------------------------------------------------------------------------*/\
static inline \
prefix##_entry const* prefix##_get_head_if_expired_explicit(\
  prefix* dl, tstamp now\
  )\
{\
  return prefix##_get_head_if_expired_impl_priv (dl, true, now);\
}\
/*--------------------------------------------------------------------------*/\
static inline \
prefix##_entry const* prefix##_get_head_if_expired(\
  prefix* dl\
  )\
{\
  return prefix##_get_head_if_expired_impl_priv (dl, false, 0);\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
void prefix##_drop_head (prefix* dl);\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bool prefix##_try_get_and_drop(\
  prefix* dl, value_type* dst, tstamp d, value_type const* value\
  );\
/*--------------------------------------------------------------------------*/\
static inline bl_err prefix##_init(\
  prefix* dl, uword capacity, alloc_tbl const* alloc\
  )\
{\
  return prefix##_init_explicit (dl, bl_get_tstamp(), capacity, alloc);\
}\
/*--------------------------------------------------------------------------*/\
static inline uword prefix##_size (const prefix const* dl)\
{\
  return prefix##_ringb_size (&dl->list);\
}\
/*--------------------------------------------------------------------------*/\
static inline uword prefix##_capacity (const prefix const* dl)\
{\
  return prefix##_ringb_capacity (&dl->list);\
}\
/*--------------------------------------------------------------------------*/\
static inline bool prefix##_can_insert (const prefix const* dl)\
{\
  return prefix##_ringb_can_insert (&dl->list);\
}\
/*--------------------------------------------------------------------------*/\
static inline prefix##_entry const* prefix##_at (prefix const* dl, uword idx)\
{\
  return prefix##_ringb_at (&dl->list, idx);\
}
/*---------------------------------------------------------------------------*/
/* Function definitions */
/*---------------------------------------------------------------------------*/
#define define_flat_deadlines_funcs(\
  prefix, value_type, content_type_cmp_func, linkage_and_modif\
  )\
/*--------------------------------------------------------------------------*/\
static inline word prefix##_flat_deadlines_ordering_func(\
  void const* a, void const* b, void* context\
  )\
{\
  prefix* l = (prefix*) context;\
  return deadline_compare(\
    ((prefix##_entry*) a)->key - l->time_offset,\
    ((prefix##_entry*) b)->key - l->time_offset\
    );\
}\
/*--------------------------------------------------------------------------*/\
define_oringb_funcs(\
  prefix##_ringb,\
  prefix##_entry,\
  prefix##_flat_deadlines_ordering_func,\
  linkage_and_modif\
  )\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
prefix##_entry const* prefix##_get_head (prefix* dl)\
{\
  return prefix##_ringb_size (&dl->list) > 0 ? \
    prefix##_ringb_at_head (&dl->list) : nullptr;\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
prefix##_entry const* prefix##_get_head_if_expired_impl_priv(\
  prefix* dl, bool explicit, tstamp now\
  )\
{\
  prefix##_entry const* d = prefix##_get_head (dl);\
  if (!d) {\
    return nullptr;\
  }\
  if (unlikely (deadline_expired_explicit (d->key, dl->time_offset))) {\
    return d;\
  }\
  /* as the list ordering rotates to allow timestamp wrap around (and O(1) */\
  /* lookup for the next expired candidate) it is wrong to move the rotation */\
  /*   offset while still having outdated items on the list */\
  dl->time_offset = (explicit) ? now : bl_get_tstamp();\
  return deadline_expired_explicit (d->key, dl->time_offset) ? d : nullptr;\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
void prefix##_drop_head (prefix* dl)\
{\
  bl_assert (dl);\
  bl_assert (prefix##_ringb_size (&dl->list) > 0);\
  prefix##_ringb_drop_head (&dl->list);\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_insert (prefix* dl, prefix##_entry* e)\
{\
  if (unlikely (!prefix##_ringb_can_insert (&dl->list))) {\
    return bl_would_overflow;\
  }\
  prefix##_ringb_insert_fifo_duplicates (&dl->list, e, dl);\
  return bl_ok;\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bool prefix##_try_get_and_drop(\
  prefix* dl, value_type* dst, tstamp d, value_type const* value\
  )\
{\
  prefix##_entry find;\
  find.key = d;\
  uword idx  = prefix##_ringb_find (&dl->list, &find, dl);\
  uword size = prefix##_ringb_size (&dl->list);\
  if (idx >= size) {\
    return false;\
  }\
  prefix##_entry* f;\
  do {\
    /*duplicate linear check*/\
    f = prefix##_ringb_at (&dl->list, idx);\
    if (likely (content_type_cmp_func (value, &f->value) == 0)) {\
      *dst = f->value;\
      prefix##_ringb_drop (&dl->list, idx);\
      return true;\
    }\
    ++idx;\
  } \
  while(idx < size && f->key == d);\
  return false;\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_init_explicit(\
  prefix* dl, tstamp now, uword capacity, alloc_tbl const* alloc\
  )\
{\
  bl_assert (dl);\
  dl->time_offset = now;\
  capacity        = round_next_pow2_u (capacity);\
  return prefix##_ringb_init (&dl->list, capacity, alloc);\
}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
void prefix##_destroy (prefix* dl, alloc_tbl const* alloc)\
{\
  bl_assert (dl);\
  prefix##_ringb_destroy (&dl->list, alloc);\
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* __BL_FLAT_DEADLINES_H__ */
