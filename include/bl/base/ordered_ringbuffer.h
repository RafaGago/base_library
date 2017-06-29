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
typedef ringb oringb;
/*---------------------------------------------------------------------------*/
typedef word (*oringb_cmp) (void const* a, void const* b, void* context);
/*--------------------------------------------------------------------------*/
extern BL_EXPORT uword oringb_find_eq_or_gt(
  ringb*      rb,
  uword       elem_size,
  oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT oringb_insert_result oringb_insert(
  ringb*      rb,
  uword       elem_size,
  oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT uword oringb_insert_lifo_duplicates(
  ringb*      rb,
  uword       elem_size,
  oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT uword oringb_insert_fifo_duplicates(
  ringb*      rb,
  uword       elem_size,
  oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT uword oringb_find(
  ringb*      rb,
  uword       elem_size,
  oringb_cmp  cmp_func,
  void const* f,
  void*       cmp_context
  );
/*---------------------------------------------------------------------------*/
#define define_oringb_funcs(prefix, content_type, cmp_func)\
static inline bl_err prefix##_init_extern(\
  oringb* rb, content_type* mem, uword capacity\
  )\
{\
  return ringb_init_extern (rb, (void*) mem, capacity);\
}\
\
static inline bl_err prefix##_init(\
  oringb* rb, uword capacity, alloc_tbl const* alloc\
  )\
{\
  return ringb_init (rb, sizeof (content_type), capacity, alloc);\
}\
\
static inline void prefix##_destroy_extern (oringb* rb) {}\
\
static inline void prefix##_destroy (oringb* rb, alloc_tbl const* alloc)\
{\
  ringb_destroy (rb, alloc);\
}\
\
static inline uword prefix##_capacity (const oringb* rb)\
{\
  return ringb_capacity (rb);\
}\
\
static inline uword prefix##_size (const oringb* rb)\
{\
  return ringb_size (rb);\
}\
\
static inline void prefix##_set_start_position (oringb* rb, uword idx)\
{\
  ringb_set_start_position (rb, idx);\
}\
\
static inline bool prefix##_can_insert_n (const oringb* rb, uword n)\
{\
  return ringb_capacity (rb) >= ringb_size (rb) + n;\
}\
\
static inline bool prefix##_can_insert (const oringb* rb)\
{\
  return ringb_can_insert (rb);\
}\
\
static inline uword prefix##_adjacent_elems_from(\
  const oringb* rb, uword from_idx, uword element_count\
  )\
{\
  return ringb_adjacent_elems_from (rb, from_idx, element_count);\
}\
\
static inline content_type* prefix##_at(const oringb* rb, uword idx)\
{\
  return (content_type*) ringb_at (rb, sizeof (content_type), idx);\
}\
\
static inline content_type* prefix##_at_head (const oringb* rb)\
{\
  return (content_type*) ringb_at_head (rb, sizeof (content_type));\
}\
\
static inline content_type* prefix##_at_tail (const oringb* rb)\
{\
  return (content_type*) ringb_at_tail (rb, sizeof (content_type));\
}\
\
static inline void prefix##_expand_head_n (oringb* rb, uword n)\
{\
  ringb_expand_head_n (rb, n);\
}\
\
static inline void prefix##_expand_tail_n (oringb* rb, uword n)\
{\
   ringb_expand_tail_n (rb, n);\
}\
\
static inline void prefix##_drop_head_n (oringb* rb, uword n)\
{\
  ringb_drop_head_n (rb, n);\
}\
\
static inline void prefix##_drop_tail_n (oringb* rb, uword n)\
{\
  ringb_drop_tail_n (rb, n);\
}\
\
static inline void prefix##_drop_head (oringb* rb)\
{\
  ringb_drop_head (rb);\
}\
\
static inline void prefix##_drop_tail (oringb* rb)\
{\
  ringb_drop_tail (rb);\
}\
\
static inline void prefix##_drop (oringb* rb, uword idx)\
{\
  ringb_drop (rb, sizeof (content_type), idx);\
}\
\
static inline void prefix##_drop_range(\
  oringb* rb, uword idx, uword element_count\
  )\
{\
  ringb_drop_range (rb, sizeof (content_type), idx, element_count);\
}\
\
static inline oringb_insert_result prefix##_insert(\
  oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return oringb_insert (rb, sizeof *v, cmp_func, v, cmp_context);\
}\
\
static inline uword prefix##_insert_lifo_duplicates(\
  oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return oringb_insert_lifo_duplicates(\
    rb, sizeof *v, cmp_func, v, cmp_context\
    );\
}\
\
static inline uword prefix##_insert_fifo_duplicates(\
  oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return oringb_insert_fifo_duplicates(\
    rb, sizeof *v, cmp_func, v, cmp_context\
    );\
}\
\
static inline uword prefix##_find(\
  oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return oringb_find (rb, sizeof *v, cmp_func, v, cmp_context);\
}

#endif /* __BL_ORDERED_RINGBUFFER_H__ */
