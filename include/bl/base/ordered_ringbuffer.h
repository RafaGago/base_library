#ifndef __BL_ORDERED_RINGBUFFER_H__
#define __BL_ORDERED_RINGBUFFER_H__

#include <bl/base/assert.h>
#include <bl/base/error.h>
#include <bl/base/integer.h>
#include <bl/base/integer_math.h>
#include <bl/base/allocator.h>
#include <bl/base/utility.h>
#include <bl/base/ringbuffer.h>

/*---------------------------------------------------------------------------*/
typedef struct oringb_insert_result {
  uword idx;
  bool  new_insertion;
}
oringb_insert_result;
/*---------------------------------------------------------------------------*/
#define define_oringb_types(prefix, content_type)\
  define_ringb_types(prefix, content_type)\
  typedef prefix prefix##_private;\
/*---------------------------------------------------------------------------*/
#define declare_oringb_funcs(prefix, content_type, linkage_and_modif)\
declare_ringb_funcs(prefix##_private, content_type, linkage_and_modif)\
declare_ringb_extended_funcs(prefix##_private, content_type, linkage_and_modif)\
\
static inline \
bl_err prefix##_init_extern (prefix* rb, content_type* mem, uword capacity)\
{\
  return prefix##_private_init_extern (rb, mem, capacity);\
}\
\
static inline void prefix##_destroy_extern (prefix* rb)\
{\
  prefix##_private_destroy_extern (rb);\
}\
\
static inline \
bl_err prefix##_init (prefix* rb, alloc_tbl const* alloc, uword capacity)\
{\
  return prefix##_private_init (rb, alloc, capacity);\
}\
\
static inline void prefix##_destroy (prefix* rb, alloc_tbl const* alloc)\
{\
  prefix##_private_destroy (rb, alloc);\
}\
\
static inline uword prefix##_capacity (const prefix* rb)\
{\
  return prefix##_private_capacity (rb);\
}\
\
static inline uword prefix##_size (const prefix* rb)\
{\
  return prefix##_private_size (rb);\
}\
/* in some cases and usage patterns you may avoid jumps from tail to head  */\
/* of the buffer by setting a start position different than 0. just to be* */\
/* called on empty ringbuffers                                             */\
static inline void prefix##_set_start_position (prefix* rb, uword idx)\
{\
  prefix##_private_set_start_position (rb, idx);\
}\
\
static inline bool prefix##_can_insert (const prefix* rb)\
{\
  return prefix##_private_can_insert (rb);\
}\
\
static inline uword prefix##_adjacent_elems_from(\
  const prefix* rb, uword from_idx, uword element_count\
  )\
{\
  return prefix##_adjacent_elems_from (rb, from_idx, element_count);\
}\
\
static inline content_type* prefix##_at (const prefix* rb, uword idx)\
{\
  return prefix##_private_at (rb, idx);\
}\
\
static inline content_type* prefix##_at_head (const prefix* rb)\
{\
  return prefix##_private_at_head (rb);\
}\
\
static inline content_type* prefix##_at_tail (const prefix* rb)\
{\
  return prefix##_private_at_tail (rb);\
}\
\
static inline void prefix##_drop_head_n (prefix* rb, uword n)\
{\
  prefix##_private_drop_head_n (rb, n);\
}\
\
static inline void prefix##_drop_tail_n (prefix* rb, uword n)\
{\
  prefix##_private_drop_tail_n (rb, n);\
}\
\
static inline void prefix##_drop_head (prefix* rb)\
{\
  prefix##_private_drop_head (rb);\
}\
\
static inline void prefix##_drop_tail (prefix* rb)\
{\
  prefix##_private_drop_tail (rb);\
}\
\
static inline void prefix##_drop (prefix* rb, uword idx)\
{\
  prefix##_private_drop (rb, idx);\
}\
\
static inline void prefix##_drop_range(\
  prefix* rb, uword from_idx, uword element_count\
  )\
{\
  prefix##_private_drop_range (rb, from_idx, element_count);\
}\
\
/* will return true if the element was inserted or false if a duplicate was */\
/* present. idx will always have the index of the element with the key "v" */\
/* wether is a new insertion or not*/\
linkage_and_modif \
oringb_insert_result prefix##_insert(\
  prefix* rb, content_type const* v, void* cmp_context\
  );\
\
linkage_and_modif \
uword prefix##_insert_lifo_duplicates(\
  prefix* rb, content_type const* v, void* cmp_context\
  );\
\
linkage_and_modif \
uword prefix##_insert_fifo_duplicates(\
  prefix* rb, content_type const* v, void* cmp_context\
  );\
\
linkage_and_modif \
uword prefix##_find (prefix* rb, content_type const* f, void* cmp_context);\
\
linkage_and_modif \
uword prefix##_find_eq_or_gt(\
  prefix* rb, content_type const* f, void* cmp_context\
  );
/*---------------------------------------------------------------------------*/
/*
  cmp_func is a function with the next signature:

 "word name_of_the_function (content_type* a, content_type* b, void* context)"

  Which returns:
  -N: "a" is smaller (to be placed towards the head of the buffer from b).
  -0: equal
  -N: "a" is bigger. 
*/
/*---------------------------------------------------------------------------*/
#define define_oringb_funcs(\
  prefix, content_type, cmp_func, linkage_and_modif \
  )\
define_ringb_funcs (prefix##_private, content_type, linkage_and_modif)\
define_ringb_extended_funcs (prefix##_private, content_type, linkage_and_modif)\
\
linkage_and_modif \
uword prefix##_find_eq_or_gt (prefix* rb, content_type const* v, void* context)\
{\
  bl_assert (rb && v);\
  if (prefix##_size (rb) == 0) {\
    return 0;\
  }\
  /*test head (avoid trashing cache (touching a lot of memory) on fast-path)*/\
  uword idx = 0;\
  word  res  = cmp_func (v, prefix##_at (rb, idx), context);\
  if (res <= 0) {\
    return idx;\
  }\
  /*test tail (avoid trashing cache (touching a lot of memory) on fast-path)*/\
  idx = prefix##_size (rb) - 1;\
  res = cmp_func (v, prefix##_at (rb, idx), context);\
  if (res > 0) {\
    return prefix##_size (rb);\
  }\
  else if (unlikely (res == 0)) {\
    /*linearly go backwards to find the first occurence*/\
    do {\
      --idx;\
      res = cmp_func (v, prefix##_at (rb, idx), context);\
    }\
    while (res == 0);\
    return idx + 1;\
  }\
  /*binary search*/\
  uword head    = 1;\
  uword tail    = prefix##_size (rb) - 1;\
\
  while (head < tail) {\
    uword idx = head + ((tail - head) / 2);\
    word  res = cmp_func (v, prefix##_at (rb, idx), context);\
    if (res > 0) {\
      head = idx + 1;\
    }\
    else {\
      tail = idx;\
    }\
  }\
  return cmp_func (v, prefix##_at (rb, head), context) == 0 ?\
    head : /*match. if duplicate will have the lowest duplicate idx (LIFO)*/\
    tail;  /*no match: tail is the first value greater than "v" */\
}\
\
linkage_and_modif \
oringb_insert_result prefix##_insert(\
  prefix* rb, content_type const* v, void* cmp_context\
  )\
{\
  bl_assert (rb && v);\
  bl_assert (prefix##_can_insert (rb));\
  oringb_insert_result r;\
  r.idx           = prefix##_find_eq_or_gt (rb, v, cmp_context);\
  r.new_insertion = r.idx == prefix##_size (rb) ||\
                    cmp_func (v, prefix##_at (rb, r.idx), cmp_context) != 0;\
  if (r.new_insertion) {\
    prefix##_private_insert (rb, v, r.idx);\
  }\
  return r;\
}\
\
linkage_and_modif \
uword prefix##_insert_lifo_duplicates(\
  prefix* rb, content_type const* v, void* cmp_context\
  )\
{\
  bl_assert (rb && v);\
  bl_assert (prefix##_can_insert (rb));\
  uword idx = prefix##_find_eq_or_gt (rb, v, cmp_context);\
  prefix##_private_insert (rb, v, idx);\
  return idx;\
}\
\
linkage_and_modif \
uword prefix##_insert_fifo_duplicates(\
  prefix* rb, content_type const* v, void* cmp_context\
  )\
{\
  bl_assert (rb && v);\
  bl_assert (prefix##_can_insert (rb));\
  uword idx = prefix##_find_eq_or_gt (rb, v, cmp_context);\
  /*linear advance to one position after the last duplicate*/\
  while(\
    idx < prefix##_size (rb) &&\
    cmp_func (v, prefix##_at (rb, idx), cmp_context) == 0\
    ) {\
    ++idx;\
  }\
  prefix##_private_insert (rb, v, idx);\
  return idx;\
}\
\
linkage_and_modif \
uword prefix##_find (prefix* rb, content_type const* f, void* cmp_context)\
{\
  bl_assert (rb && f);\
  uword pos  = prefix##_find_eq_or_gt (rb, f, cmp_context);\
  uword size = prefix##_size (rb);\
  if (size > 0 && pos < size) {\
    pos = cmp_func (f, prefix##_at (rb, pos), cmp_context) == 0 ? pos : size;\
  }\
  return pos;\
}\
/*---------------------------------------------------------------------------*/

#endif /* __BL_ORDERED_RINGBUFFER_H__ */

