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
typedef struct bl_oringb_insert_result {
  bl_uword idx;
  bool  new_insertion;
}
bl_oringb_insert_result;
/*---------------------------------------------------------------------------*/
typedef bl_ringb bl_oringb;
/*---------------------------------------------------------------------------*/
typedef bl_word (*bl_oringb_cmp) (void const* a, void const* b, void* context);
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_uword bl_oringb_find_eq_or_gt(
  bl_ringb*      rb,
  bl_uword       elem_size,
  bl_oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_oringb_insert_result bl_oringb_insert(
  bl_ringb*      rb,
  bl_uword       elem_size,
  bl_oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_uword bl_oringb_insert_lifo_duplicates(
  bl_ringb*      rb,
  bl_uword       elem_size,
  bl_oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_uword bl_oringb_insert_fifo_duplicates(
  bl_ringb*      rb,
  bl_uword       elem_size,
  bl_oringb_cmp  cmp_func,
  void const* v,
  void*       cmp_context
  );
/*--------------------------------------------------------------------------*/
extern BL_EXPORT bl_uword bl_oringb_find(
  bl_ringb*      rb,
  bl_uword       elem_size,
  bl_oringb_cmp  cmp_func,
  void const* f,
  void*       cmp_context
  );
/*---------------------------------------------------------------------------*/
#define define_bl_oringb_funcs(prefix, content_type, cmp_func)\
static inline bl_err prefix##_init_extern(\
  bl_oringb* rb, content_type* mem, bl_uword capacity\
  )\
{\
  return bl_ringb_init_extern (rb, (void*) mem, capacity);\
}\
\
static inline bl_err prefix##_init(\
  bl_oringb* rb, bl_uword capacity, bl_alloc_tbl const* alloc\
  )\
{\
  return bl_ringb_init (rb, sizeof (content_type), capacity, alloc);\
}\
\
static inline void prefix##_destroy_extern (bl_oringb* rb) {}\
\
static inline void prefix##_destroy (bl_oringb* rb, bl_alloc_tbl const* alloc)\
{\
  bl_ringb_destroy (rb, alloc);\
}\
\
static inline bl_uword prefix##_capacity (const bl_oringb* rb)\
{\
  return bl_ringb_capacity (rb);\
}\
\
static inline bl_uword prefix##_size (const bl_oringb* rb)\
{\
  return bl_ringb_size (rb);\
}\
\
static inline void prefix##_set_start_position (bl_oringb* rb, bl_uword idx)\
{\
  bl_ringb_set_start_position (rb, idx);\
}\
\
static inline bool prefix##_can_insert_n (const bl_oringb* rb, bl_uword n)\
{\
  return bl_ringb_capacity (rb) >= bl_ringb_size (rb) + n;\
}\
\
static inline bool prefix##_can_insert (const bl_oringb* rb)\
{\
  return bl_ringb_can_insert (rb);\
}\
\
static inline bl_uword prefix##_adjacent_elems_from(\
  const bl_oringb* rb, bl_uword from_idx, bl_uword element_count\
  )\
{\
  return bl_ringb_adjacent_elems_from (rb, from_idx, element_count);\
}\
\
static inline content_type* prefix##_at(const bl_oringb* rb, bl_uword idx)\
{\
  return (content_type*) bl_ringb_at (rb, sizeof (content_type), idx);\
}\
\
static inline content_type* prefix##_at_head (const bl_oringb* rb)\
{\
  return (content_type*) bl_ringb_at_head (rb, sizeof (content_type));\
}\
\
static inline content_type* prefix##_at_tail (const bl_oringb* rb)\
{\
  return (content_type*) bl_ringb_at_tail (rb, sizeof (content_type));\
}\
\
static inline void prefix##_expand_head_n (bl_oringb* rb, bl_uword n)\
{\
  bl_ringb_expand_head_n (rb, n);\
}\
\
static inline void prefix##_expand_tail_n (bl_oringb* rb, bl_uword n)\
{\
   bl_ringb_expand_tail_n (rb, n);\
}\
\
static inline void prefix##_drop_head_n (bl_oringb* rb, bl_uword n)\
{\
  bl_ringb_drop_head_n (rb, n);\
}\
\
static inline void prefix##_drop_tail_n (bl_oringb* rb, bl_uword n)\
{\
  bl_ringb_drop_tail_n (rb, n);\
}\
\
static inline void prefix##_drop_head (bl_oringb* rb)\
{\
  bl_ringb_drop_head (rb);\
}\
\
static inline void prefix##_drop_tail (bl_oringb* rb)\
{\
  bl_ringb_drop_tail (rb);\
}\
\
static inline void prefix##_drop (bl_oringb* rb, bl_uword idx)\
{\
  bl_ringb_drop (rb, sizeof (content_type), idx);\
}\
\
static inline void prefix##_drop_range(\
  bl_oringb* rb, bl_uword idx, bl_uword element_count\
  )\
{\
  bl_ringb_drop_range (rb, sizeof (content_type), idx, element_count);\
}\
\
static inline bl_oringb_insert_result prefix##_insert(\
  bl_oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return bl_oringb_insert (rb, sizeof *v, cmp_func, v, cmp_context);\
}\
\
static inline bl_uword prefix##_insert_lifo_duplicates(\
  bl_oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return bl_oringb_insert_lifo_duplicates(\
    rb, sizeof *v, cmp_func, v, cmp_context\
    );\
}\
\
static inline bl_uword prefix##_insert_fifo_duplicates(\
  bl_oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return bl_oringb_insert_fifo_duplicates(\
    rb, sizeof *v, cmp_func, v, cmp_context\
    );\
}\
\
static inline bl_uword prefix##_find(\
  bl_oringb* rb, content_type const* v, void* cmp_context\
  )\
{\
  return bl_oringb_find (rb, sizeof *v, cmp_func, v, cmp_context);\
}

#endif /* __BL_ORDERED_RINGBUFFER_H__ */
