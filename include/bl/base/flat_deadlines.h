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

#endif /* __BL_FLAT_DEADLINES_H__ */
