#ifndef BL_RINGBUFFER_H
#define BL_RINGBUFFER_H

#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/error.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/integer_math.h>
#include <bl/hdr/base/allocator.h>
#include <bl/hdr/base/utility.h>

/*---------------------------------------------------------------------------*/
/* A simple ringbuffer implementation for fixed sizes that are powers of two.*/
/*---------------------------------------------------------------------------*/
#define define_ringb_types(prefix, content_type)\
\
typedef struct prefix {\
  content_type* stor;\
  uword         mask;\
  uword         begin;\
  uword         size;\
}\
prefix;
/*---------------------------------------------------------------------------*/
#define declare_ringb_funcs(prefix, content_type, linkage_and_modif)\
\
linkage_and_modif \
bl_err prefix##_init_extern (prefix* rb, content_type* mem, uword capacity);\
\
static inline void prefix##_destroy_extern (prefix* rb) {}\
\
linkage_and_modif \
bl_err prefix##_init (prefix* rb, alloc_tbl const* alloc, uword capacity);\
\
static inline void prefix##_destroy (prefix* rb, alloc_tbl const* alloc)\
{\
  bl_assert (rb);\
  if (rb->stor) {\
    bl_deallocate (alloc, rb->stor);\
  }\
}\
\
static inline uword prefix##_capacity (const prefix* rb)\
{\
  bl_assert (rb);\
  return rb->mask + 1;\
}\
\
static inline uword prefix##_size (const prefix* rb)\
{\
  bl_assert (rb);\
  return rb->size;\
}\
\
/* in some cases and usage patterns you may avoid jumps from tail to head  */\
/* of the buffer by setting a start position different than 0. just to be* */\
/* called on empty ringbuffers                                             */\
static inline void prefix##_set_start_position (prefix* rb, uword idx)\
{\
  bl_assert (rb);\
  bl_assert (idx < prefix##_capacity (rb));\
  bl_assert (prefix##_size (rb) == 0);\
  rb->begin = idx;\
}\
\
static inline bool prefix##_can_insert (const prefix* rb)\
{\
  return prefix##_capacity (rb) > prefix##_size (rb);\
}\
\
static inline uword prefix##_contiguous_elems_from(\
  const prefix* rb, uword from_idx, uword element_count\
  )\
{\
  bl_assert (rb);\
  bl_assert ((from_idx + element_count) <= prefix##_size (rb));\
  return bl_min (element_count, prefix##_capacity (rb) - from_idx);\
}\
\
static inline content_type* prefix##_at (const prefix* rb, uword idx)\
{\
  bl_assert (rb);\
  bl_assert (idx < prefix##_size (rb));\
  return &rb->stor[(rb->begin + idx) & rb->mask];\
}\
\
static inline content_type* prefix##_at_head (const prefix* rb)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_size (rb) > 0);\
  return &rb->stor[rb->begin];\
}\
\
static inline content_type* prefix##_at_tail (const prefix* rb)\
{\
  return prefix##_at (rb, prefix##_size (rb) - 1);\
}\
\
static inline void prefix##_insert_head (prefix* rb, content_type const* v)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_can_insert (rb));\
  ++rb->size;\
  --rb->begin;\
  rb->begin &= rb->mask;\
  rb->stor[rb->begin] = *v;\
}\
\
static inline void prefix##_insert_tail (prefix* rb, content_type const* v)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_can_insert (rb));\
  rb->stor[(rb->begin + rb->size) & rb->mask] = *v;\
  ++rb->size;\
}\
\
static inline void prefix##_drop_head_n (prefix* rb, uword n)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_size (rb) >= n);\
  rb->size  -= n;\
  rb->begin += n;\
  rb->begin &= rb->mask;\
}\
\
static inline void prefix##_drop_tail_n (prefix* rb, uword n)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_size (rb) >= n);\
  rb->size -= n;\
}\
\
static inline void prefix##_drop_head (prefix* rb)\
{\
  prefix##_drop_head_n (rb, 1);\
}\
\
static inline void prefix##_drop_tail (prefix* rb)\
{\
  prefix##_drop_tail_n (rb, 1);\
}
/*---------------------------------------------------------------------------*/
#define define_ringb_funcs(prefix, content_type, linkage_and_modif)\
\
linkage_and_modif \
bl_err prefix##_init_extern (prefix* rb, content_type* mem, uword capacity)\
{\
  bl_assert (rb && mem);\
  bl_assert (is_pow2 (capacity));\
\
  if (!is_pow2 (capacity)) {\
    return bl_invalid;\
  }\
  rb->mask  = capacity - 1;\
  rb->size  = 0;\
  rb->begin = 0;\
  rb->stor  = mem;\
  return bl_ok;\
}\
\
linkage_and_modif \
bl_err prefix##_init (prefix* rb, alloc_tbl const* alloc, uword capacity)\
{\
  bl_err err = prefix##_init_extern (rb, (content_type*) 1, capacity);\
  if (err) {\
    return err;\
  }\
  rb->stor  = bl_allocate (alloc, capacity * sizeof (content_type));\
  return rb->stor ? bl_ok : bl_alloc;\
}
/*---------------------------------------------------------------------------*/
/* Expand with optional expensive O(n) random insertion and deletion         */
/*---------------------------------------------------------------------------*/
#define declare_ringb_extended_funcs(prefix, content_type, linkage_and_modif)\
linkage_and_modif \
void prefix##_insert (prefix* rb, content_type const* v, uword idx);\
\
linkage_and_modif \
void prefix##_drop (prefix* rb, uword idx);\
\
linkage_and_modif \
void prefix##_drop_range (prefix* rb, uword idx, uword element_count);\
/*---------------------------------------------------------------------------*/
#define define_ringb_extended_funcs(prefix, content_type, linkage_and_modif)\
static \
void prefix##_shift_forward_private(\
  prefix* rb, uword mid, uword end, content_type const* prev\
  )\
{\
  content_type buff[2];\
  buff[mid & 1] = *prev;                             /*save previous*/\
  for (; mid < end; ++mid) {\
    buff[~mid & 1]        = *prefix##_at (rb, mid);  /*save current*/\
    *prefix##_at (rb, mid) = buff[mid & 1];          /*retrieve previous*/\
  }\
}\
\
static \
void prefix##_shift_backward_private (prefix* rb, uword beg, uword mid_last)\
{\
  for (; beg < mid_last; ++beg) {\
    *prefix##_at (rb, beg) = *prefix##_at (rb, beg + 1);\
  }\
}\
\
linkage_and_modif \
void prefix##_insert (prefix* rb, content_type const* v, uword idx)\
{\
  bl_assert (rb && v);\
  bl_assert (prefix##_can_insert (rb));\
  bl_assert (idx <= prefix##_size (rb));\
  ++rb->size;\
  if (idx < (prefix##_size (rb) / 2)) {\
    /*head backwards*/\
    --rb->begin;\
    rb->begin &= rb->mask;\
    prefix##_shift_backward_private (rb, 0, idx);\
    *prefix##_at (rb, idx) = *v;\
  }\
  else {\
    prefix##_shift_forward_private (rb, idx, prefix##_size (rb), v);\
  }\
}\
linkage_and_modif \
void prefix##_drop (prefix* rb, uword idx)\
{\
  bl_assert (rb);\
  bl_assert (idx < prefix##_size (rb));\
\
  if (idx <= (prefix##_size (rb) / 2)) {\
    prefix##_shift_forward_private (rb, 1, idx + 1, prefix##_at (rb, 0));\
    ++rb->begin;\
    rb->begin &= rb->mask;\
  }\
  else {\
    prefix##_shift_backward_private (rb, idx, prefix##_size (rb) - 1);\
  }\
  --rb->size;\
}\
\
linkage_and_modif \
void prefix##_drop_range (prefix* rb, uword from_idx, uword element_count)\
{\
  bl_assert (rb);\
  bl_assert ((from_idx + element_count) <= prefix##_size (rb));\
\
  uword src;\
  uword dst;\
  uword count;\
  uword begin;\
  uword tail_count = prefix##_size (rb) - from_idx - element_count;\
\
  if (from_idx >= tail_count) {\
    /*tail has less elements to move*/\
    src   = from_idx + element_count;\
    dst   = from_idx;\
    count = tail_count;\
    begin = rb->begin;\
  }\
  else {\
    /*head has less elements to move*/\
    src   = 0;\
    dst   = element_count;\
    count = from_idx;\
    begin = dst;\
  }\
  while (count--) {\
    *prefix##_at (rb, dst) = *prefix##_at (rb, src);\
    ++src;\
    ++dst;\
  }\
  rb->size -= element_count;\
  rb->begin = begin;\
}\
/*---------------------------------------------------------------------------*/






































































#endif
