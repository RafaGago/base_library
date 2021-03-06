/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/array_doubly_linked_list.gen.h -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64

Don't edit.
*/
/* AUTOGENERATE: include/bl/base/impl/generated/array_doubly_linked_list_u8.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_ARRAY_DOUBLY_LINKED_LIST_u8_H__
#define __BL_ARRAY_DOUBLY_LINKED_LIST_u8_H__

#include <bl/base/assert.h>
#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/utility.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bl_u8 bl_adlnls_u8_it;
/*---------------------------------------------------------------------------*/
typedef struct bl_adlnls_u8_hook {
  bl_u8 next : bl_type_bits (bl_u8) / 2;
  bl_u8 prev : bl_type_bits (bl_u8) / 2;
}
bl_adlnls_u8_hook;
/*---------------------------------------------------------------------------*/
typedef struct bl_adlnls_u8 {
  bl_adlnls_u8_hook head;
  bl_adlnls_u8_hook* nodes;
  bl_uword capacity;
  bl_uword size;
}
bl_adlnls_u8;
/*---------------------------------------------------------------------------*/
#define bl_adlnls_u8_foreach(type_ptr, it_var)\
  for(\
    bl_adlnls_u8_it bl_adlnls_u8_priv_curr_it =\
        bl_adlnls_u8_it_begin ((type_ptr)),\
      it_var = bl_adlnls_u8_priv_curr_it;\
    bl_adlnls_u8_it_in_range ((type_ptr), bl_adlnls_u8_priv_curr_it) &&\
      (bl_adlnls_u8_priv_curr_it =\
        bl_adlnls_u8_it_next(\
          (type_ptr), bl_adlnls_u8_priv_curr_it), 1\
          );\
    it_var = bl_adlnls_u8_priv_curr_it\
    )
/*---------------------------------------------------------------------------*/
#define bl_adlnls_u8_foreach_read_only(type_ptr, it_var)\
  for(\
    it_var = bl_adlnls_u8_it_begin ((type_ptr));\
    bl_adlnls_u8_it_in_range ((type_ptr), it_var);\
    it_var = bl_adlnls_u8_it_next ((type_ptr), it_var)\
    )
/*---------------------------------------------------------------------------*/
/* "_r" reverse iteration */
#define bl_adlnls_u8_foreach_r(type_ptr, it_var)\
  for(\
    bl_adlnls_u8_it bl_adlnls_u8_priv_curr_it =\
        bl_adlnls_u8_it_rbegin ((type_ptr)),\
      it_var = bl_adlnls_u8_priv_curr_it;\
    bl_adlnls_u8_it_in_range_r(\
        (type_ptr), bl_adlnls_u8_priv_curr_it\
        ) &&\
      (bl_adlnls_u8_priv_curr_it =\
    bl_adlnls_u8_it_prev ((type_ptr), bl_adlnls_u8_priv_curr_it), 1);\
      it_var = bl_adlnls_u8_priv_curr_it\
    )
/*---------------------------------------------------------------------------*/
#define bl_adlnls_u8_foreach_r_read_only(type_ptr, it_var)\
  for(\
    it_var = bl_adlnls_u8_it_rbegin ((type_ptr));\
    bl_adlnls_u8_it_in_range_r ((type_ptr), it_var);\
    it_var = bl_adlnls_u8_it_prev ((type_ptr), it_var)\
    )
/*---------------------------------------------------------------------------*/
#define bl_adlnls_u8_init(list, backing_nodes_array)\
  bl_adlnls_u8_init_impl(\
    (list),\
    (backing_nodes_array),\
    bl_arr_elems (backing_nodes_array)\
    )
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it bl_adlnls_u8_it_end (bl_adlnls_u8* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it bl_adlnls_u8_it_rend (bl_adlnls_u8* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_it_begin (bl_adlnls_u8* l)
{
  return l->head.next;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_it_rbegin (bl_adlnls_u8* l)
{
  return l->head.prev;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_capacity (bl_adlnls_u8* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_size (bl_adlnls_u8* l)
{
  return l->size;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_it_in_range (bl_adlnls_u8* l, bl_adlnls_u8_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_it_in_range_r (bl_adlnls_u8* l, bl_adlnls_u8_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_adlnls_u8_is_empty (bl_adlnls_u8* l)
{
  return bl_adlnls_u8_it_begin (l) == bl_adlnls_u8_it_end (l);
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_it_next (bl_adlnls_u8* l, bl_adlnls_u8_it it)
{
  return l->nodes[it].next;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_u8_it
  bl_adlnls_u8_it_prev (bl_adlnls_u8* l, bl_adlnls_u8_it it)
{
  return l->nodes[it].prev;
}
/*---------------------------------------------------------------------------*/
static inline void
  bl_adlnls_u8_node_release (bl_adlnls_u8* l, bl_adlnls_u8_it it)
{
  l->nodes[it].next = it;
  l->nodes[it].prev = it;
}
/*---------------------------------------------------------------------------*/
static inline bool
  bl_adlnls_u8_node_is_free (bl_adlnls_u8* l, bl_adlnls_u8_it it)
{
  return l->nodes[it].next == it;
}
/*---------------------------------------------------------------------------*/
static inline void bl_adlnls_u8_acquire_node_unsafe(
  bl_adlnls_u8* l, bl_adlnls_u8_it it
  )
{
  bl_assert (it < bl_adlnls_u8_capacity (l));
  bl_assert (bl_adlnls_u8_node_is_free (l, it));
  l->nodes[it].prev = bl_adlnls_u8_it_end (l);
  l->nodes[it].next = bl_adlnls_u8_it_end (l);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void bl_adlnls_u8_init_impl(
  bl_adlnls_u8* l, bl_u8* nodes, bl_uword nodes_capacity
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_u8_it bl_adlnls_u8_try_acquire_node(
  bl_adlnls_u8* l, bl_adlnls_u8_it n
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_u8_it
  bl_adlnls_u8_try_acquire_a_node (bl_adlnls_u8* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void
  bl_adlnls_u8_insert_head (bl_adlnls_u8* l, bl_adlnls_u8_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void
  bl_adlnls_u8_insert_tail (bl_adlnls_u8* l, bl_adlnls_u8_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_u8_it
  bl_adlnls_u8_drop_head (bl_adlnls_u8* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_u8_it
  bl_adlnls_u8_drop_tail (bl_adlnls_u8* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_u8_it
  bl_adlnls_u8_drop (bl_adlnls_u8* l, bl_adlnls_u8_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_u8_it bl_adlnls_u8_drop_explicit(
  bl_adlnls_u8* l, bl_adlnls_u8_it n, bool return_previous
  );
#ifdef __cplusplus
} /*extern "C" {*/
#endif
#endif /* #define __BL_ARRAY_LINKED_LIST_u8_H__ */

/*---------------------------------------------------------------------------*/
