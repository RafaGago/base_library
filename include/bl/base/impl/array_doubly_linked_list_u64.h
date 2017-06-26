/* Autogenerated file, don't edit */
/* AUTOGENERATE: include/bl/base/impl/array_doubly_linked_list_u64.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_ARRAY_DOUBLY_LINKED_LIST_u64_H__
#define __BL_ARRAY_DOUBLY_LINKED_LIST_u64_H__

/* SELF-COMMENT: why not save code size and make the head the first element of
   "nodes" */

#include <bl/base/assert.h>
#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/utility.h>

typedef u64 adlnls_u64_it;
/*---------------------------------------------------------------------------*/
typedef struct adlnls_u64_hook {
  u64 next : type_bits (u64) / 2;
  u64 prev : type_bits (u64) / 2;
}
adlnls_u64_hook;
/*---------------------------------------------------------------------------*/
typedef struct adlnls_u64 {
  adlnls_u64_hook head;
  adlnls_u64_hook* nodes;
  uword capacity;
  uword size;
}
adlnls_u64;
/*---------------------------------------------------------------------------*/
#define adlnls_u64_foreach(type_ptr, it_var)\
  for (adlnls_u64_it adlnls_u64_priv_curr_it = adlnls_u64_it_begin ((type_ptr)),\
         it_var = adlnls_u64_priv_curr_it;\
       adlnls_u64_it_in_range ((type_ptr), adlnls_u64_priv_curr_it) &&\
         (adlnls_u64_priv_curr_it =\
            adlnls_u64_it_next ((type_ptr), adlnls_u64_priv_curr_it), 1);\
       it_var = adlnls_u64_priv_curr_it\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_u64_foreach_read_only(type_ptr, it_var)\
  for (it_var = adlnls_u64_it_begin ((type_ptr));\
       adlnls_u64_it_in_range ((type_ptr), it_var);\
       it_var = adlnls_u64_it_next ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_u64_foreach_r(type_ptr, it_var)\
  for (adlnls_u64_it adlnls_u64_priv_curr_it = adlnls_u64_it_rbegin ((type_ptr)),\
         it_var = adlnls_u64_priv_curr_it;\
       adlnls_u64_it_in_range_r ((type_ptr), adlnls_u64_priv_curr_it) &&\
         (adlnls_u64_priv_curr_it =\
            adlnls_u64_it_prev ((type_ptr), adlnls_u64_priv_curr_it), 1);\
       it_var = adlnls_u64_priv_curr_it\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_u64_foreach_r_read_only(type_ptr, it_var)\
  for (it_var = adlnls_u64_it_rbegin ((type_ptr));\
       adlnls_u64_it_in_range_r ((type_ptr), it_var);\
       it_var = adlnls_u64_it_prev ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_u64_init(list, backing_nodes_array)\
  adlnls_u64_init_impl(\
    (list),\
    (backing_nodes_array),\
    arr_elems (backing_nodes_array)\
    )
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_end (adlnls_u64* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_rend (adlnls_u64* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_begin (adlnls_u64* l)
{
  return l->head.next;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_rbegin (adlnls_u64* l)
{
  return l->head.prev;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_capacity (adlnls_u64* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_size (adlnls_u64* l)
{
  return l->size;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_in_range (adlnls_u64* l, adlnls_u64_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_in_range_r (adlnls_u64* l, adlnls_u64_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bool adlnls_u64_is_empty (adlnls_u64* l)
{
  return adlnls_u64_it_begin (l) == adlnls_u64_it_end (l);
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_next (adlnls_u64* l, adlnls_u64_it it)
{
  return l->nodes[it].next;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_u64_it adlnls_u64_it_prev (adlnls_u64* l, adlnls_u64_it it)
{
  return l->nodes[it].prev;
}
/*---------------------------------------------------------------------------*/
static inline void adlnls_u64_node_release (adlnls_u64* l, adlnls_u64_it it)
{
  l->nodes[it].next = it;
  l->nodes[it].prev = it;
}
/*---------------------------------------------------------------------------*/
static inline bool adlnls_u64_node_is_free (adlnls_u64* l, adlnls_u64_it it)
{
  return l->nodes[it].next == it;
}
/*---------------------------------------------------------------------------*/
static inline void adlnls_u64_acquire_node_unsafe (adlnls_u64* l, adlnls_u64_it it)
{
  bl_assert (it < adlnls_u64_capacity (l));
  bl_assert (adlnls_u64_node_is_free (l, it));
  l->nodes[it].prev = adlnls_u64_it_end (l);
  l->nodes[it].next = adlnls_u64_it_end (l);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void adlnls_u64_init_impl(
  adlnls_u64* l, u64* nodes, uword nodes_capacity
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_u64_it adlnls_u64_try_acquire_node (adlnls_u64* l, adlnls_u64_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_u64_it adlnls_u64_try_acquire_a_node (adlnls_u64* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void adlnls_u64_insert_head (adlnls_u64* l, adlnls_u64_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void adlnls_u64_insert_tail (adlnls_u64* l, adlnls_u64_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_u64_it adlnls_u64_drop_head (adlnls_u64* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_u64_it adlnls_u64_drop_tail (adlnls_u64* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_u64_it adlnls_u64_drop (adlnls_u64* l, adlnls_u64_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_u64_it adlnls_u64_drop_explicit(
  adlnls_u64* l, adlnls_u64_it n, bool return_previous
  );

#endif /* #define __BL_ARRAY_LINKED_LIST_u64_H__ */

/*---------------------------------------------------------------------------*/
