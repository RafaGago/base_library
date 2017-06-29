#ifndef __BL_FLAT_DEADLINES_H__
#define __BL_FLAT_DEADLINES_H__

/*represents a fixed-size array-based associative structure with
  wrapping/overflowing timestamps. It can substitute time wheels in some cases.

  As this is a structure to manage deadlines it should be updated frequently
  (through "get_head_if_expired") to avoid letting the underlying tstamp
  unsigned integer advance half its resolution

  It can contain structs with custom data attached (see content_type parameter).
 */
#include <stddef.h>

#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/static_assert.h>
#include <bl/base/ordered_ringbuffer.h>
#include <bl/base/error.h>
#include <bl/base/time.h>
#include <bl/base/allocator.h>
#include <bl/base/utility.h>

/*--------------------------------------------------------------------------*/
typedef struct flat_deadlines {
  oringb list;
  tstamp time_offset;
}
flat_deadlines;
/*--------------------------------------------------------------------------*/
/* receives the type passed on "content_type" */
typedef word (*flat_dealines_value_cmp_func) (void const* a, void const* b);
/*--------------------------------------------------------------------------*/
static inline bl_err flat_deadlines_init_extern(
  flat_deadlines* fd, tstamp now, void* mem, uword capacity
  )
{
  fd->time_offset = now;
  return ringb_init_extern (&fd->list, mem, capacity);
}
/*--------------------------------------------------------------------------*/
static inline bl_err flat_deadlines_init(
  flat_deadlines*  fd,
  uword            elem_size,
  tstamp           now,
  uword            capacity,
  alloc_tbl const* alloc
  )
{
  fd->time_offset = now;
  return ringb_init (&fd->list, elem_size, capacity, alloc);
}
/*--------------------------------------------------------------------------*/
static inline void* flat_deadlines_get_head(
  const flat_deadlines* fd, uword elem_size
  )
{
  return ringb_size (&fd->list) > 0 ?
    ringb_at_head (&fd->list, elem_size) : nullptr;
}
/*--------------------------------------------------------------------------*/
static inline void* flat_deadlines_get_tail(
  const flat_deadlines* fd, uword elem_size
  )
{
  return ringb_size (&fd->list) > 0 ?
    ringb_at_head (&fd->list, elem_size) : nullptr;
}
/*--------------------------------------------------------------------------*/
extern BL_EXPORT void const* flat_deadlines_get_head_if_expired(
  flat_deadlines* fd, uword elem_size, bool dont_acquire_new_tstamp, tstamp now
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_err flat_deadlines_insert(
  flat_deadlines* fd, uword elem_size, void const* e
  );
/*----------------------------------------------------------------------------
cmp: The function to use for comparison
dst: Will contain a copy of the dropped value.
find: Will contain the value to find.

Note that:

-The find field must always have the timestamp filled.

-It can seem that the "dst" parameter is superfluous, but the "cmp"
 implementation can compare only some fields of the structure and having some
 others used as storage-only. Those storage-only fields can be left empty
 on the "find" parameter but will be retrieved on "dst".
----------------------------------------------------------------------------*/
extern BL_EXPORT bool flat_deadlines_try_get_and_drop(
  flat_deadlines*              fd,
  uword                        elem_size,
  flat_dealines_value_cmp_func cmp,
  void*                        dst,
  void const*                  find
  );
/*--------------------------------------------------------------------------
"content_type" must be a struct always containing a timestap as the element
at offset 0. It can contain custom fields declared after.

struct content_type {
  tstamp timestamp;
  <your fields>
};

"cmpeq_func" a function invoked to compare two "content_type" structures. If
"content_type" has only the timestamp you can pass null here.
----------------------------------------------------------------------------*/
#define define_flat_deadlines_funcs(prefix, content_type, cmpeq_func) \
\
static_assert_outside_func( \
  offsetof (content_type, time) == 0, \
  "\"content_type\" must contain a field named \"time\" at offset 0."\
  ); \
\
static_assert_outside_func( \
  sizeof_member (content_type, time) == sizeof (tstamp), \
  "\"content_type\" must contain a field named \"time\" of type tstamp."\
  ); \
\
static inline bl_err prefix##_init_extern( \
  flat_deadlines* fd, tstamp now, content_type* mem, uword capacity \
  ) \
{ \
  return flat_deadlines_init_extern (fd, now, (void*) mem, capacity); \
} \
\
static inline bl_err prefix##_init( \
  flat_deadlines* fd, tstamp now, uword capacity, alloc_tbl const* alloc \
  ) \
{ \
  return flat_deadlines_init( \
    fd, sizeof (content_type), now, capacity, alloc \
    ); \
} \
\
static inline void prefix##_destroy_extern (flat_deadlines* fd) {} \
\
static inline void prefix##_destroy( \
  flat_deadlines* fd, alloc_tbl const* alloc \
  ) \
{ \
  ringb_destroy (&fd->list, alloc); \
} \
\
static inline uword prefix##_capacity (const flat_deadlines* fd) \
{ \
  return ringb_capacity (&fd->list); \
} \
\
static inline uword prefix##_size (const flat_deadlines* fd) \
{ \
  return ringb_size (&fd->list); \
} \
\
static inline void prefix##_set_start_position (flat_deadlines* fd, uword idx) \
{ \
  ringb_set_start_position (&fd->list, idx); \
} \
\
static inline bool prefix##_can_insert_n (const flat_deadlines* fd, uword n) \
{ \
  return ringb_capacity (&fd->list) >= ringb_size (&fd->list) + n; \
} \
\
static inline bool prefix##_can_insert (const flat_deadlines* fd) \
{ \
  return ringb_can_insert (&fd->list); \
} \
\
static inline uword prefix##_adjacent_elems_from( \
  const flat_deadlines* fd, uword from_idx, uword element_count \
  ) \
{ \
  return ringb_adjacent_elems_from (&fd->list, from_idx, element_count); \
} \
\
static inline content_type* prefix##_at(const flat_deadlines* fd, uword idx) \
{ \
  return (content_type*) ringb_at (&fd->list, sizeof (content_type), idx); \
} \
\
static inline content_type* prefix##_get_head (const flat_deadlines* fd) \
{ \
  return (content_type*) flat_deadlines_get_head (fd,  sizeof (content_type)); \
} \
\
static inline content_type* prefix##_get_tail (const flat_deadlines* fd) \
{ \
  return (content_type*) flat_deadlines_get_tail (fd,  sizeof (content_type)); \
} \
\
static inline void prefix##_drop_head_n (flat_deadlines* fd, uword n) \
{ \
  ringb_drop_head_n (&fd->list, n); \
} \
\
static inline void prefix##_drop_tail_n (flat_deadlines* fd, uword n) \
{ \
  ringb_drop_tail_n (&fd->list, n); \
} \
\
static inline void prefix##_drop_head (flat_deadlines* fd) \
{ \
  ringb_drop_head (&fd->list); \
} \
\
static inline void prefix##_drop_tail (flat_deadlines* fd) \
{ \
  ringb_drop_tail (&fd->list); \
} \
\
static inline content_type const* prefix##_get_head_if_expired( \
  flat_deadlines* dl, bool dont_acquire_new_tstamp, tstamp now \
  ) \
{ \
  return (content_type*) flat_deadlines_get_head_if_expired( \
    dl, sizeof (content_type), dont_acquire_new_tstamp, now \
    ); \
} \
\
static inline bl_err prefix##_insert( \
  flat_deadlines* dl, content_type const* e \
  ) \
{ \
  return flat_deadlines_insert (dl, sizeof *e, (void const*) e); \
} \
\
static inline bool prefix##_try_get_and_drop( \
  flat_deadlines* dl, content_type* dst, content_type const* find \
  ) \
{ \
  return flat_deadlines_try_get_and_drop( \
    dl, sizeof *dst, cmpeq_func, (void*) dst, (void const*) find \
    ); \
}

#if 0
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
typedef struct prefix {\
  oringb list;\
  tstamp time_offset;\
}\
prefix;
/*---------------------------------------------------------------------------*/
/* Function declarations*/
/*---------------------------------------------------------------------------*/
#define declare_flat_deadlines_funcs(\
  prefix, value_type, linkage_and_modif\
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
  prefix##_ringb, prefix##_entry, prefix##_flat_deadlines_ordering_func\
  )\
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

#endif /*if 0*/

#endif /* __BL_FLAT_DEADLINES_H__ */
