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
A simple bl_ringbuffer implementation for fixed sizes that are powers of two.

REMINDER: elem_size is passed around because it might be optimized by being
visible in some of the most common bl_ringbuffer operations (e.g. bl_ringbuffer_at,
bl_ringbuffer_insert_head...).
-----------------------------------------------------------------------------*/
typedef struct bl_ringb {
  bl_u8*   stor;
  bl_uword mask;
  bl_uword begin;
  bl_uword size;
}
bl_ringb;
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_ringb_init_extern(
  bl_ringb* rb, void* mem, bl_uword capacity
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_err bl_ringb_init(
  bl_ringb* rb, bl_uword elem_size, bl_uword capacity, bl_alloc_tbl const* alloc
  );
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_destroy_extern (bl_ringb* rb) {}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_destroy (bl_ringb* rb, bl_alloc_tbl const* alloc)
{
  bl_assert (rb);
  if (rb->stor) {
    bl_dealloc (alloc, rb->stor);
    rb->stor = nullptr;
  }
}
/*--------------------------------------------------------------------------*/
static inline bl_uword bl_ringb_capacity (const bl_ringb* rb)
{
  bl_assert (rb);
  return rb->mask + 1;
}
/*--------------------------------------------------------------------------*/
static inline bl_uword bl_ringb_size (const bl_ringb* rb)
{
  bl_assert (rb);
  return rb->size;
}
/*--------------------------------------------------------------------------*/
/* in some cases and usage patterns you may avoid jumps from tail to head  */
/* of the buffer by setting a start position different than 0. just to be* */
/* called on empty bl_ringbuffers                                             */
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_set_start_position (bl_ringb* rb, bl_uword idx)
{
  bl_assert (rb);
  bl_assert (idx < bl_ringb_capacity (rb));
  bl_assert (bl_ringb_size (rb) == 0);
  rb->begin = idx;
}
/*--------------------------------------------------------------------------*/
static inline bool bl_ringb_can_insert_n (const bl_ringb* rb, bl_uword n)
{
  return bl_ringb_capacity (rb) >= bl_ringb_size (rb) + n;
}
/*--------------------------------------------------------------------------*/
static inline bool bl_ringb_can_insert (const bl_ringb* rb)
{
  return bl_ringb_can_insert_n (rb, 1);
}
/*--------------------------------------------------------------------------*/
static inline bl_uword bl_ringb_adjacent_elems_from(
  const bl_ringb* rb, bl_uword from_idx, bl_uword element_count
  )
{
  bl_assert (rb);
  bl_assert ((from_idx + element_count) <= bl_ringb_size (rb));
  return bl_min (element_count, bl_ringb_capacity (rb) - from_idx);
}
/*--------------------------------------------------------------------------*/
static inline void*
  bl_ringb_at (const bl_ringb* rb, bl_uword elem_size, bl_uword idx)
{
  bl_assert (rb);
  bl_assert (idx < bl_ringb_size (rb));
  return &rb->stor[((rb->begin + idx) & rb->mask) * elem_size];
}
/*--------------------------------------------------------------------------*/
static inline void* bl_ringb_at_head (const bl_ringb* rb, bl_uword elem_size)
{
  bl_assert (rb);
  bl_assert (bl_ringb_size (rb) > 0);
  return bl_ringb_at (rb, elem_size, 0);
}
/*--------------------------------------------------------------------------*/
static inline void* bl_ringb_at_tail (const bl_ringb* rb, bl_uword elem_size)
{
  return bl_ringb_at (rb, elem_size, bl_ringb_size (rb) - 1);
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_expand_head_n (bl_ringb* rb, bl_uword n)
{
  bl_assert (rb);
  bl_assert (bl_ringb_can_insert_n (rb, n));
  rb->size  += n;
  rb->begin -= n;
  rb->begin &= rb->mask;
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_insert_head(
  bl_ringb* rb, bl_uword elem_size, void const* v
  )
{
  bl_assert (rb);
  bl_ringb_expand_head_n (rb, 1);
  memcpy (bl_ringb_at_head (rb, elem_size), v, elem_size);
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_expand_tail_n (bl_ringb* rb, bl_uword n)
{
  bl_assert (rb);
  bl_assert (bl_ringb_can_insert_n (rb, n));
  rb->size  += n;
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_insert_tail(
  bl_ringb* rb, bl_uword elem_size, void const* v
  )
{
  bl_assert (rb);
  bl_assert (bl_ringb_can_insert (rb));
  memcpy(
    &rb->stor[((rb->begin + rb->size) & rb->mask) * elem_size], v, elem_size
    );
  ++rb->size;
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_drop_head_n (bl_ringb* rb, bl_uword n)
{
  bl_assert (rb);
  bl_assert (bl_ringb_size (rb) >= n);
  rb->size  -= n;
  rb->begin += n;
  rb->begin &= rb->mask;
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_drop_tail_n (bl_ringb* rb, bl_uword n)
{
  bl_assert (rb);
  bl_assert (bl_ringb_size (rb) >= n);
  rb->size -= n;
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_drop_head (bl_ringb* rb)
{
  bl_ringb_drop_head_n (rb, 1);
}
/*--------------------------------------------------------------------------*/
static inline void bl_ringb_drop_tail (bl_ringb* rb)
{
  bl_ringb_drop_tail_n (rb, 1);
}
/*--------------------------------------------------------------------------*/
extern BL_EXPORT void bl_ringb_insert(
  bl_ringb* rb, bl_uword elem_size, void const* v, bl_uword idx
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT void
  bl_ringb_drop (bl_ringb* rb, bl_uword elem_size, bl_uword idx);
/*--------------------------------------------------------------------------*/
extern BL_EXPORT void bl_ringb_drop_range(
  bl_ringb* rb, bl_uword elem_size, bl_uword idx, bl_uword element_count
  );
/*----------------------------------------------------------------------------
  Wrapper functions: the only thing they do is to remove the "elem_size"
  parameter.
----------------------------------------------------------------------------*/
#define bl_define_ringb_funcs(prefix, content_type)\
static inline bl_err prefix##_init_extern(\
  bl_ringb* rb, content_type* mem, bl_uword capacity\
  )\
{\
  return bl_ringb_init_extern (rb, (void*) mem, capacity);\
}\
\
static inline bl_err prefix##_init(\
  bl_ringb* rb, bl_uword capacity, bl_alloc_tbl const* alloc\
  )\
{\
  return bl_ringb_init (rb, sizeof (content_type), capacity, alloc);\
}\
\
static inline void prefix##_destroy_extern (bl_ringb* rb) {}\
\
static inline void prefix##_destroy (bl_ringb* rb, bl_alloc_tbl const* alloc)\
{\
  bl_ringb_destroy (rb, alloc);\
}\
\
static inline bl_uword prefix##_capacity (const bl_ringb* rb)\
{\
  return bl_ringb_capacity (rb);\
}\
\
static inline bl_uword prefix##_size (const bl_ringb* rb)\
{\
  return bl_ringb_size (rb);\
}\
\
static inline void prefix##_set_start_position (bl_ringb* rb, bl_uword idx)\
{\
  bl_ringb_set_start_position (rb, idx);\
}\
\
static inline bool prefix##_can_insert_n (const bl_ringb* rb, bl_uword n)\
{\
  return bl_ringb_capacity (rb) >= bl_ringb_size (rb) + n;\
}\
\
static inline bool prefix##_can_insert (const bl_ringb* rb)\
{\
  return bl_ringb_can_insert (rb);\
}\
\
static inline bl_uword prefix##_adjacent_elems_from(\
  const bl_ringb* rb, bl_uword from_idx, bl_uword element_count\
  )\
{\
  return bl_ringb_adjacent_elems_from (rb, from_idx, element_count);\
}\
\
static inline content_type* prefix##_at(const bl_ringb* rb, bl_uword idx)\
{\
  return (content_type*) bl_ringb_at (rb, sizeof (content_type), idx);\
}\
\
static inline content_type* prefix##_at_head (const bl_ringb* rb)\
{\
  return (content_type*) bl_ringb_at_head (rb, sizeof (content_type));\
}\
\
static inline content_type* prefix##_at_tail (const bl_ringb* rb)\
{\
  return (content_type*) bl_ringb_at_tail (rb, sizeof (content_type));\
}\
\
static inline void prefix##_expand_head_n (bl_ringb* rb, bl_uword n)\
{\
  bl_ringb_expand_head_n (rb, n);\
}\
\
static inline void prefix##_insert_head (bl_ringb* rb, content_type const* v)\
{\
  bl_ringb_insert_head (rb, sizeof *v, (void const*) v);\
}\
\
static inline void prefix##_expand_tail_n (bl_ringb* rb, bl_uword n)\
{\
   bl_ringb_expand_tail_n (rb, n);\
}\
\
static inline void prefix##_insert_tail (bl_ringb* rb, content_type const* v)\
{\
  bl_ringb_insert_tail (rb, sizeof *v, (void const*) v);\
}\
\
static inline void prefix##_drop_head_n (bl_ringb* rb, bl_uword n)\
{\
  bl_ringb_drop_head_n (rb, n);\
}\
\
static inline void prefix##_drop_tail_n (bl_ringb* rb, bl_uword n)\
{\
  bl_ringb_drop_tail_n (rb, n);\
}\
\
static inline void prefix##_drop_head (bl_ringb* rb)\
{\
  bl_ringb_drop_head (rb);\
}\
\
static inline void prefix##_drop_tail (bl_ringb* rb)\
{\
  bl_ringb_drop_tail (rb);\
}\
\
static inline void prefix##_insert(\
  bl_ringb* rb, content_type const* v, bl_uword idx\
  )\
{\
  bl_ringb_insert (rb, sizeof *v, (void const*) v, idx);\
}\
\
static inline void prefix##_drop (bl_ringb* rb, bl_uword idx)\
{\
  bl_ringb_drop (rb, sizeof (content_type), idx);\
}\
\
static inline void prefix##_drop_range(\
  bl_ringb* rb, bl_uword idx, bl_uword element_count\
  )\
{\
  bl_ringb_drop_range (rb, sizeof (content_type), idx, element_count);\
}

#endif
