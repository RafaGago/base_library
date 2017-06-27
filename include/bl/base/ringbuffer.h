#ifndef BL_RINGBUFFER_H
#define BL_RINGBUFFER_H

#include <string.h>

#include <bl/base/assert.h>
#include <bl/base/error.h>
#include <bl/base/libexport.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/allocator.h>
#include <bl/base/utility.h>

/*---------------------------------------------------------------------------*/
/* A simple ringbuffer implementation for fixed sizes that are powers of two.*/
/*---------------------------------------------------------------------------*/
typedef struct ringb {
  u8*   stor;
  uword mask;
  uword begin;
  uword size;
}
ringb;
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_err ringb_init_extern(
  ringb* rb, void* mem, uword capacity
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_err ringb_init(
  ringb* rb, uword elem_size, uword capacity, alloc_tbl const* alloc
  );
/*--------------------------------------------------------------------------*/
static inline void ringb_destroy_extern (ringb* rb) {}
/*--------------------------------------------------------------------------*/
static inline void ringb_destroy (ringb* rb, alloc_tbl const* alloc)
{
  bl_assert (rb);
  if (rb->stor) {
    bl_dealloc (alloc, rb->stor);
  }
}
/*--------------------------------------------------------------------------*/
static inline uword ringb_capacity (const ringb* rb)
{
  bl_assert (rb);
  return rb->mask + 1;
}
/*--------------------------------------------------------------------------*/
static inline uword ringb_size (const ringb* rb)
{
  bl_assert (rb);
  return rb->size;
}
/*--------------------------------------------------------------------------*/
/* in some cases and usage patterns you may avoid jumps from tail to head  */
/* of the buffer by setting a start position different than 0. just to be* */
/* called on empty ringbuffers                                             */
/*--------------------------------------------------------------------------*/
static inline void ringb_set_start_position (ringb* rb, uword idx)
{
  bl_assert (rb);
  bl_assert (idx < ringb_capacity (rb));
  bl_assert (ringb_size (rb) == 0);
  rb->begin = idx;
}
/*--------------------------------------------------------------------------*/
static inline bool ringb_can_insert_n (const ringb* rb, uword n)
{
  return ringb_capacity (rb) >= ringb_size (rb) + n;
}
/*--------------------------------------------------------------------------*/
static inline bool ringb_can_insert (const ringb* rb)
{
  return ringb_can_insert_n (rb, 1);
}
/*--------------------------------------------------------------------------*/
static inline uword ringb_adjacent_elems_from(
  const ringb* rb, uword from_idx, uword element_count
  )
{
  bl_assert (rb);
  bl_assert ((from_idx + element_count) <= ringb_size (rb));
  return bl_min (element_count, ringb_capacity (rb) - from_idx);
}
/*--------------------------------------------------------------------------*/
static inline void* ringb_at (const ringb* rb, uword elem_size, uword idx)
{
  bl_assert (rb);
  bl_assert (idx < ringb_size (rb));
  return &rb->stor[((rb->begin + idx) & rb->mask) * elem_size];
}
/*--------------------------------------------------------------------------*/
static inline void* ringb_at_head (const ringb* rb, uword elem_size)
{
  bl_assert (rb);
  bl_assert (ringb_size (rb) > 0);
  return ringb_at (rb, elem_size, 0);
}
/*--------------------------------------------------------------------------*/
static inline void* ringb_at_tail (const ringb* rb, uword elem_size)
{
  return ringb_at (rb, elem_size, ringb_size (rb) - 1);
}
/*--------------------------------------------------------------------------*/
static inline void ringb_expand_head_n (ringb* rb, uword n)
{
  bl_assert (rb);
  bl_assert (ringb_can_insert_n (rb, n));
  rb->size  += n;
  rb->begin -= n;
  rb->begin &= rb->mask;
}
/*--------------------------------------------------------------------------*/
static inline void ringb_insert_head(
  ringb* rb, uword elem_size, void const* v
  )
{
  bl_assert (rb);
  ringb_expand_head_n (rb, 1);
  memcpy (ringb_at_head (rb, elem_size), v, elem_size);
}
/*--------------------------------------------------------------------------*/
static inline void ringb_expand_tail_n (ringb* rb, uword n)
{
  bl_assert (rb);
  bl_assert (ringb_can_insert_n (rb, n));
  rb->size  += n;
}
/*--------------------------------------------------------------------------*/
static inline void ringb_insert_tail(
  ringb* rb, uword elem_size, void const* v
  )
{
  bl_assert (rb);
  bl_assert (ringb_can_insert (rb));
  memcpy(
    &rb->stor[((rb->begin + rb->size) & rb->mask) * elem_size], v, elem_size
    );
  ++rb->size;
}
/*--------------------------------------------------------------------------*/
static inline void ringb_drop_head_n (ringb* rb, uword n)
{
  bl_assert (rb);
  bl_assert (ringb_size (rb) >= n);
  rb->size  -= n;
  rb->begin += n;
  rb->begin &= rb->mask;
}
/*--------------------------------------------------------------------------*/
static inline void ringb_drop_tail_n (ringb* rb, uword n)
{
  bl_assert (rb);
  bl_assert (ringb_size (rb) >= n);
  rb->size -= n;
}
/*--------------------------------------------------------------------------*/
static inline void ringb_drop_head (ringb* rb)
{
  ringb_drop_head_n (rb, 1);
}
/*--------------------------------------------------------------------------*/
static inline void ringb_drop_tail (ringb* rb)
{
  ringb_drop_tail_n (rb, 1);
}
/*--------------------------------------------------------------------------*/
extern BL_EXPORT void ringb_insert(
  ringb* rb, uword elem_size, void const* v, uword idx
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT void ringb_drop (ringb* rb, uword elem_size, uword idx);
/*--------------------------------------------------------------------------*/
extern BL_EXPORT void ringb_drop_range(
  ringb* rb, uword elem_size, uword idx, uword element_count
  );
/*----------------------------------------------------------------------------
  Wrapper functions: the only thing they do is to remove the "elem_size"
  parameter.
----------------------------------------------------------------------------*/
#define ringb_define_wrap_funcs(prefix, content_type)\
static inline bl_err prefix##_init_extern(\
  ringb* rb, content_type* mem, uword capacity\
  )\
{\
  return ringb_init_extern (rb, (void*) mem, capacity);\
}\
\
static inline bl_err prefix##_init(\
  ringb* rb, uword capacity, alloc_tbl const* alloc\
  )\
{\
  return ringb_init (rb, sizeof (content_type), capacity, alloc);\
}\
\
static inline void prefix##_destroy_extern (ringb* rb) {}\
\
static inline void prefix##_destroy (ringb* rb, alloc_tbl const* alloc)\
{\
  ringb_destroy (rb, alloc);\
}\
\
static inline uword prefix##_capacity (const ringb* rb)\
{\
  return ringb_capacity (rb);\
}\
\
static inline uword prefix##_size (const ringb* rb)\
{\
  return ringb_size (rb);\
}\
\
static inline void prefix##_set_start_position (ringb* rb, uword idx)\
{\
  ringb_set_start_position (rb, idx);\
}\
\
static inline bool prefix##_can_insert_n (const ringb* rb, uword n)\
{\
  return prefix##_capacity (rb) >= prefix##_size (rb) + n;\
}\
\
static inline bool prefix##_can_insert (const ringb* rb)\
{\
  return ringb_can_insert (rb);\
}\
\
static inline uword prefix##_adjacent_elems_from(\
  const ringb* rb, uword from_idx, uword element_count\
  )\
{\
  return ringb_adjacent_elems_from (rb, from_idx, element_count);\
}\
\
static inline content_type* prefix##_at(const ringb* rb, uword idx)\
{\
  return (content_type*) ringb_at (rb, sizeof (content_type), idx);\
}\
\
static inline content_type* prefix##_at_head (const ringb* rb)\
{\
  return (content_type*) ringb_at_head (rb, sizeof (content_type));\
}\
\
static inline content_type* prefix##_at_tail (const ringb* rb)\
{\
  return (content_type*) ringb_at_tail (rb, sizeof (content_type));\
}\
\
static inline void prefix##_expand_head_n (ringb* rb, uword n)\
{\
  ringb_expand_head_n (rb, n);\
}\
\
static inline void prefix##_insert_head (ringb* rb, content_type const* v)\
{\
  ringb_insert_head (rb, sizeof *v, (void const*) v);\
}\
\
static inline void prefix##_expand_tail_n (ringb* rb, uword n)\
{\
   prefix##_expand_tail_n (rb, n);\
}\
\
static inline void prefix##_insert_tail (ringb* rb, content_type const* v)\
{\
  ringb_insert_tail (rb, sizeof *v, (void const*) v);\
}\
\
static inline void prefix##_drop_head_n (ringb* rb, uword n)\
{\
  ringb_drop_head_n (rb, n);\
}\
\
static inline void prefix##_drop_tail_n (ringb* rb, uword n)\
{\
  ringb_drop_tail_n (rb, n);\
}\
\
static inline void prefix##_drop_head (ringb* rb)\
{\
  ringb_drop_head (rb);\
}\
\
static inline void prefix##_drop_tail (ringb* rb)\
{\
  ringb_drop_tail (rb);\
}\
\
static inline void prefix##_insert(\
  ringb* rb, content_type const* v, uword idx\
  )\
{\
  ringb_insert (rb, sizeof *v, (void const*) v, idx);\
}\
\
static inline void prefix##_drop (ringb* rb, uword idx)\
{\
  ringb_drop (rb, sizeof (content_type), idx);\
}\
\
static inline void prefix##_drop_range(\
  ringb* rb, uword idx, uword element_count\
  )\
{\
  ringb_drop_range (rb, sizeof (content_type), idx, element_count);\
}

#if 0

/*---------------------------------------------------------------------------*/
/* A simple ringbuffer implementation for fixed sizes that are powers of two.*/
/*---------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------*/
#define declare_ringb_funcs(prefix, content_type, linkage_and_modif)\
\
linkage_and_modif \
bl_err prefix##_init_extern (prefix* rb, content_type* mem, uword capacity);\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_destroy_extern (prefix* rb) {}\
/*--------------------------------------------------------------------------*/\
linkage_and_modif \
bl_err prefix##_init (prefix* rb, uword capacity, alloc_tbl const* alloc);\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_destroy (prefix* rb, alloc_tbl const* alloc)\
{\
  bl_assert (rb);\
  if (rb->stor) {\
    bl_dealloc (alloc, rb->stor);\
  }\
}\
/*--------------------------------------------------------------------------*/\
static inline uword prefix##_capacity (const prefix* rb)\
{\
  bl_assert (rb);\
  return rb->mask + 1;\
}\
/*--------------------------------------------------------------------------*/\
static inline uword prefix##_size (const prefix* rb)\
{\
  bl_assert (rb);\
  return rb->size;\
}\
/*--------------------------------------------------------------------------*/\
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
/*--------------------------------------------------------------------------*/\
static inline bool prefix##_can_insert_n (const prefix* rb, uword n)\
{\
  return prefix##_capacity (rb) >= prefix##_size (rb) + n;\
}\
/*--------------------------------------------------------------------------*/\
static inline bool prefix##_can_insert (const prefix* rb)\
{\
  return prefix##_can_insert_n (rb, 1);\
}\
/*--------------------------------------------------------------------------*/\
static inline uword prefix##_adjacent_elems_from(\
  const prefix* rb, uword from_idx, uword element_count\
  )\
{\
  bl_assert (rb);\
  bl_assert ((from_idx + element_count) <= prefix##_size (rb));\
  return bl_min (element_count, prefix##_capacity (rb) - from_idx);\
}\
/*--------------------------------------------------------------------------*/\
static inline content_type* prefix##_at (const prefix* rb, uword idx)\
{\
  bl_assert (rb);\
  bl_assert (idx < prefix##_size (rb));\
  return &rb->stor[(rb->begin + idx) & rb->mask];\
}\
/*--------------------------------------------------------------------------*/\
static inline content_type* prefix##_at_head (const prefix* rb)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_size (rb) > 0);\
  return &rb->stor[rb->begin];\
}\
/*--------------------------------------------------------------------------*/\
static inline content_type* prefix##_at_tail (const prefix* rb)\
{\
  return prefix##_at (rb, prefix##_size (rb) - 1);\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_expand_head_n (prefix* rb, uword n)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_can_insert_n (rb, n));\
  rb->size  += n;\
  rb->begin -= n;\
  rb->begin &= rb->mask;\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_insert_head (prefix* rb, content_type const* v)\
{\
  bl_assert (rb);\
  prefix##_expand_head_n (rb, 1);\
  rb->stor[rb->begin] = *v;\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_expand_tail_n (prefix* rb, uword n)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_can_insert_n (rb, n));\
  rb->size  += n;\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_insert_tail (prefix* rb, content_type const* v)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_can_insert (rb));\
  rb->stor[(rb->begin + rb->size) & rb->mask] = *v;\
  ++rb->size;\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_drop_head_n (prefix* rb, uword n)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_size (rb) >= n);\
  rb->size  -= n;\
  rb->begin += n;\
  rb->begin &= rb->mask;\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_drop_tail_n (prefix* rb, uword n)\
{\
  bl_assert (rb);\
  bl_assert (prefix##_size (rb) >= n);\
  rb->size -= n;\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_drop_head (prefix* rb)\
{\
  prefix##_drop_head_n (rb, 1);\
}\
/*--------------------------------------------------------------------------*/\
static inline void prefix##_drop_tail (prefix* rb)\
{\
  prefix##_drop_tail_n (rb, 1);\
}
/*---------------------------------------------------------------------------*/
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
bl_err prefix##_init (prefix* rb, uword capacity, alloc_tbl const* alloc)\
{\
  bl_err err = prefix##_init_extern (rb, (content_type*) 1, capacity);\
  if (err) {\
    return err;\
  }\
  rb->stor = (content_type*) bl_alloc(\
    alloc, capacity * sizeof (content_type)\
    );\
  return rb->stor ? bl_ok : bl_alloc;\
}
/*---------------------------------------------------------------------------*/
/* Expand with optional expensive O(n) random insertion and deletion         */
/*---------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/\
static \
void prefix##_shift_backward_private (prefix* rb, uword beg, uword mid_last)\
{\
  for (; beg < mid_last; ++beg) {\
    *prefix##_at (rb, beg) = *prefix##_at (rb, beg + 1);\
  }\
}\
/*--------------------------------------------------------------------------*/\
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
/*--------------------------------------------------------------------------*/\
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
/*--------------------------------------------------------------------------*/\
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

#endif /* if 0 */

#endif
