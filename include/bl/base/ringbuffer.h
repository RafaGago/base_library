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

/*-----------------------------------------------------------------------------
A simple ringbuffer implementation for fixed sizes that are powers of two.

REMINDER: elem_size is passed around because it might be optimized by being
visible in some of the most common ringbuffer operations (e.g. ringbuffer_at,
ringbuffer_insert_head...).
-----------------------------------------------------------------------------*/
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
    rb->stor = nullptr;
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
#define define_ringb_funcs(prefix, content_type)\
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
  return ringb_capacity (rb) >= ringb_size (rb) + n;\
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
   ringb_expand_tail_n (rb, n);\
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

#endif
