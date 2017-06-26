/* Autogenerated file, don't edit */
/* AUTOGENERATE: include/bl/base/impl/array_linked_list_u8.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_ARRAY_LINKED_LIST_u8_H__
#define __BL_ARRAY_LINKED_LIST_u8_H__

#include <bl/base/assert.h>
#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/utility.h>

typedef u8 alnls_u8_it;
/*---------------------------------------------------------------------------*/
typedef struct alnls_u8 {
  uword capacity;
  uword size;
  u8 head;
  u8* nodes;
}
alnls_u8;
/*---------------------------------------------------------------------------*/
#define alnls_u8_foreach(type_ptr, it_var)\
  for (alnls_u8_it alnls_u8_priv_curr_it = alnls_u8_it_begin ((type_ptr)),\
         it_var = alnls_u8_priv_curr_it;\
       alnls_u8_it_in_range ((type_ptr), alnls_u8_priv_curr_it) &&\
         (alnls_u8_priv_curr_it =\
            alnls_u8_it_next ((type_ptr), alnls_u8_priv_curr_it), 1);\
       it_var = alnls_u8_priv_curr_it\
       )
/*---------------------------------------------------------------------------*/
#define alnls_u8_foreach_read_only(type_ptr, it_var)\
  for (it_var = alnls_u8_it_begin ((type_ptr));\
       alnls_u8_it_in_range ((type_ptr), it_var);\
       it_var = alnls_u8_it_next ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define alnls_u8_init(list, backing_nodes_array)\
  alnls_u8_init_impl(\
    (list),\
    (backing_nodes_array),\
    arr_elems (backing_nodes_array)\
    )
/*---------------------------------------------------------------------------*/
static inline alnls_u8_it alnls_u8_it_end (alnls_u8* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline alnls_u8_it alnls_u8_it_begin (alnls_u8* l)
{
  return l->head;
}
/*---------------------------------------------------------------------------*/
static inline alnls_u8_it alnls_u8_capacity (alnls_u8* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline alnls_u8_it alnls_u8_size (alnls_u8* l)
{
  return l->size;
}
/*---------------------------------------------------------------------------*/
static inline alnls_u8_it alnls_u8_it_in_range (alnls_u8* l, alnls_u8_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bool alnls_u8_is_empty (alnls_u8* l)
{
  return alnls_u8_it_begin (l) == alnls_u8_it_end (l);
}
/*---------------------------------------------------------------------------*/
static inline alnls_u8_it alnls_u8_it_next (alnls_u8* l, alnls_u8_it it)
{
  return l->nodes[it];
}
/*---------------------------------------------------------------------------*/
static inline void alnls_u8_node_release (alnls_u8* l, alnls_u8_it it)
{
  l->nodes[it] = it;
}
/*---------------------------------------------------------------------------*/
static inline bool alnls_u8_node_is_free (alnls_u8* l, alnls_u8_it it)
{
  return l->nodes[it] == it;
}
/*---------------------------------------------------------------------------*/
static inline void alnls_u8_acquire_node_unsafe (alnls_u8* l, alnls_u8_it it)
{
  bl_assert (it < alnls_u8_capacity (l));
  bl_assert (alnls_u8_node_is_free (l, it));
  l->nodes[it] = alnls_u8_it_end (l);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void alnls_u8_init_impl(
  alnls_u8* l, u8* nodes, uword nodes_capacity
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_u8_it alnls_u8_try_acquire_node (alnls_u8* l, alnls_u8_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_u8_it alnls_u8_try_acquire_a_node (alnls_u8* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void alnls_u8_insert_head (alnls_u8* l, alnls_u8_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_u8_it alnls_u8_drop_head (alnls_u8* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void alnls_u8_insert_tail (alnls_u8* l, alnls_u8_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_u8_it alnls_u8_drop (alnls_u8* l, alnls_u8_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_u8_it alnls_u8_drop_tail (alnls_u8* l);

#endif /* #define __BL_ARRAY_LINKED_LIST_u8_H__ */

/*---------------------------------------------------------------------------*/
