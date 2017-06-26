/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor color the syntax.

To regenerate use scripts/autogenerator.sh:
autogenerator.sh -f <THIS_FILE> -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures. This library is designed to be statically linked, so some of the
code has different translation units which can be easily discarded by the
linker.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/base/impl/array_doubly_linked_list_{TYPE}.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_ARRAY_DOUBLY_LINKED_LIST_{TYPE}_H__
#define __BL_ARRAY_DOUBLY_LINKED_LIST_{TYPE}_H__

#include <bl/base/assert.h>
#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/utility.h>

typedef {TYPE} adlnls_{TYPE}_it;
/*---------------------------------------------------------------------------*/
typedef struct adlnls_{TYPE}_hook {
  {TYPE} next : type_bits ({TYPE}) / 2;
  {TYPE} prev : type_bits ({TYPE}) / 2;
}
adlnls_{TYPE}_hook;
/*---------------------------------------------------------------------------*/
typedef struct adlnls_{TYPE} {
  adlnls_{TYPE}_hook head;
  adlnls_{TYPE}_hook* nodes;
  uword capacity;
  uword size;
}
adlnls_{TYPE};
/*---------------------------------------------------------------------------*/
#define adlnls_{TYPE}_foreach(type_ptr, it_var)\
  for (adlnls_{TYPE}_it adlnls_{TYPE}_priv_curr_it = adlnls_{TYPE}_it_begin ((type_ptr)),\
         it_var = adlnls_{TYPE}_priv_curr_it;\
       adlnls_{TYPE}_it_in_range ((type_ptr), adlnls_{TYPE}_priv_curr_it) &&\
         (adlnls_{TYPE}_priv_curr_it =\
            adlnls_{TYPE}_it_next ((type_ptr), adlnls_{TYPE}_priv_curr_it), 1);\
       it_var = adlnls_{TYPE}_priv_curr_it\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_{TYPE}_foreach_read_only(type_ptr, it_var)\
  for (it_var = adlnls_{TYPE}_it_begin ((type_ptr));\
       adlnls_{TYPE}_it_in_range ((type_ptr), it_var);\
       it_var = adlnls_{TYPE}_it_next ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_{TYPE}_foreach_r(type_ptr, it_var)\
  for (adlnls_{TYPE}_it adlnls_{TYPE}_priv_curr_it = adlnls_{TYPE}_it_rbegin ((type_ptr)),\
         it_var = adlnls_{TYPE}_priv_curr_it;\
       adlnls_{TYPE}_it_in_range_r ((type_ptr), adlnls_{TYPE}_priv_curr_it) &&\
         (adlnls_{TYPE}_priv_curr_it =\
            adlnls_{TYPE}_it_prev ((type_ptr), adlnls_{TYPE}_priv_curr_it), 1);\
       it_var = adlnls_{TYPE}_priv_curr_it\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_{TYPE}_foreach_r_read_only(type_ptr, it_var)\
  for (it_var = adlnls_{TYPE}_it_rbegin ((type_ptr));\
       adlnls_{TYPE}_it_in_range_r ((type_ptr), it_var);\
       it_var = adlnls_{TYPE}_it_prev ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_{TYPE}_init(list, backing_nodes_array)\
  adlnls_{TYPE}_init_impl(\
    (list),\
    (backing_nodes_array),\
    arr_elems (backing_nodes_array)\
    )
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_end (adlnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_rend (adlnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_begin (adlnls_{TYPE}* l)
{
  return l->head.next;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_rbegin (adlnls_{TYPE}* l)
{
  return l->head.prev;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_capacity (adlnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_size (adlnls_{TYPE}* l)
{
  return l->size;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_in_range (adlnls_{TYPE}* l, adlnls_{TYPE}_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_in_range_r (adlnls_{TYPE}* l, adlnls_{TYPE}_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bool adlnls_{TYPE}_is_empty (adlnls_{TYPE}* l)
{
  return adlnls_{TYPE}_it_begin (l) == adlnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_next (adlnls_{TYPE}* l, adlnls_{TYPE}_it it)
{
  return l->nodes[it].next;
}
/*---------------------------------------------------------------------------*/
static inline adlnls_{TYPE}_it adlnls_{TYPE}_it_prev (adlnls_{TYPE}* l, adlnls_{TYPE}_it it)
{
  return l->nodes[it].prev;
}
/*---------------------------------------------------------------------------*/
static inline void adlnls_{TYPE}_node_release (adlnls_{TYPE}* l, adlnls_{TYPE}_it it)
{
  l->nodes[it].next = it;
  l->nodes[it].prev = it;
}
/*---------------------------------------------------------------------------*/
static inline bool adlnls_{TYPE}_node_is_free (adlnls_{TYPE}* l, adlnls_{TYPE}_it it)
{
  return l->nodes[it].next == it;
}
/*---------------------------------------------------------------------------*/
static inline void adlnls_{TYPE}_acquire_node_unsafe (adlnls_{TYPE}* l, adlnls_{TYPE}_it it)
{
  bl_assert (it < adlnls_{TYPE}_capacity (l));
  bl_assert (adlnls_{TYPE}_node_is_free (l, it));
  l->nodes[it].prev = adlnls_{TYPE}_it_end (l);
  l->nodes[it].next = adlnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void adlnls_{TYPE}_init_impl(
  adlnls_{TYPE}* l, {TYPE}* nodes, uword nodes_capacity
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_try_acquire_node (adlnls_{TYPE}* l, adlnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_try_acquire_a_node (adlnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void adlnls_{TYPE}_insert_head (adlnls_{TYPE}* l, adlnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void adlnls_{TYPE}_insert_tail (adlnls_{TYPE}* l, adlnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_drop_head (adlnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_drop_tail (adlnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_drop (adlnls_{TYPE}* l, adlnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_drop_explicit(
  adlnls_{TYPE}* l, adlnls_{TYPE}_it n, bool return_previous
  );

#endif /* #define __BL_ARRAY_LINKED_LIST_{TYPE}_H__ */

/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: src/bl/base/adlnls/array_doubly_linked_list_{TYPE}_basic.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_doubly_linked_list_{TYPE}.h>

BL_EXPORT void adlnls_{TYPE}_init_impl(
  adlnls_{TYPE}* l, {TYPE}* nodes, uword nodes_capacity
  )
{
  l->nodes     = (adlnls_{TYPE}_hook*) nodes;
  l->capacity  = nodes_capacity;
  l->head.next = l->capacity;
  l->head.prev = l->capacity;
  for (adlnls_{TYPE}_it it = 0; it < l->capacity; ++it) {
    adlnls_{TYPE}_node_release (l, it);
  }
  l->size = 0;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_try_acquire_node (adlnls_{TYPE}* l, adlnls_{TYPE}_it n)
{
  if (adlnls_{TYPE}_node_is_free (l, n)) {
    adlnls_{TYPE}_acquire_node_unsafe (l, n);
    return n;
  }
  return adlnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_try_acquire_a_node (adlnls_{TYPE}* l)
{
  adlnls_{TYPE}_it i;
  for (i = 0; i < adlnls_{TYPE}_it_end (l); ++i) {
    if (adlnls_{TYPE}_node_is_free (l, i)) {
      adlnls_{TYPE}_acquire_node_unsafe (l, i);
      break;
    }
  }
  return i;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void adlnls_{TYPE}_insert_head (adlnls_{TYPE}* l, adlnls_{TYPE}_it n)
{
  bl_assert (adlnls_{TYPE}_it_next (l, n) == adlnls_{TYPE}_it_end (l));
  bl_assert (adlnls_{TYPE}_it_prev (l, n) == adlnls_{TYPE}_it_end (l));

  if (!adlnls_{TYPE}_is_empty (l)) {
    l->nodes[l->head.next].prev = n;
    bl_assert (l->nodes[l->head.next].prev == n);
    l->nodes[n].next = l->head.next;
  }
  else {
    l->head.prev = n;
    bl_assert (l->head.prev == n);
  }
  l->head.next = n;
  bl_assert (l->head.next == n); /* oveflow check */
  ++l->size;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void adlnls_{TYPE}_insert_tail (adlnls_{TYPE}* l, adlnls_{TYPE}_it n)
{
  bl_assert (adlnls_{TYPE}_it_next (l, n) == adlnls_{TYPE}_it_end (l));
  bl_assert (adlnls_{TYPE}_it_prev (l, n) == adlnls_{TYPE}_it_end (l));

  if (!adlnls_{TYPE}_is_empty (l)) {
    l->nodes[l->head.prev].next = n;
    bl_assert (l->nodes[l->head.prev].next == n);
    l->nodes[n].prev = l->head.prev;
  }
  else {
    l->head.next = n;
    bl_assert (l->head.next == n);
  }
  l->head.prev = n;
  bl_assert (l->head.prev == n); /* oveflow check */
  ++l->size;
}
/*----------------------------------------------------------------------------*/
#define adlnls_node_type_middle        0
#define adlnls_node_type_tail          1
#define adlnls_node_type_head          2
#define adlnls_node_type_tail_and_head 3
/*----------------------------------------------------------------------------*/
static inline uword adlns_{TYPE}_get_node_type (adlnls_{TYPE}* l, adlnls_{TYPE}_it n)
{
  return (l->nodes[n].next == adlnls_{TYPE}_it_end (l)) |
       ((l->nodes[n].prev == adlnls_{TYPE}_it_end (l) ) << 1);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT  adlnls_{TYPE}_it adlnls_{TYPE}_drop_explicit(
  adlnls_{TYPE}* l, adlnls_{TYPE}_it n, bool return_previous
  )
{
  bl_assert (adlnls_{TYPE}_it_in_range (l, n));
  bl_assert (!adlnls_{TYPE}_node_is_free (l, n));

  if (adlnls_{TYPE}_is_empty (l)) {
    return adlnls_{TYPE}_it_end (l);
  }
  adlnls_{TYPE}_it ret;

  switch (adlns_{TYPE}_get_node_type (l, n)) {
  case adlnls_node_type_tail_and_head: {
    ret          = adlnls_{TYPE}_it_end (l);
    l->head.next = adlnls_{TYPE}_it_end (l);
    l->head.prev = adlnls_{TYPE}_it_end (l);
    goto do_return;
  }
  case adlnls_node_type_tail:{
    ret = return_previous ? l->nodes[n].prev : adlnls_{TYPE}_it_end (l);
    l->nodes[l->nodes[n].prev].next = adlnls_{TYPE}_it_end (l);
    l->head.prev                    = l->nodes[n].prev;
    goto do_return;
  }
  case adlnls_node_type_head: {
    ret = return_previous ? adlnls_{TYPE}_it_end (l) : l->nodes[n].next;
    l->nodes[l->nodes[n].next].prev = adlnls_{TYPE}_it_end (l);
    l->head.next                    = l->nodes[n].next;
    goto do_return;
  }
  case adlnls_node_type_middle: {
    ret = return_previous ? l->nodes[n].prev : l->nodes[n].next;
    l->nodes[l->nodes[n].next].prev = l->nodes[n].prev;
    l->nodes[l->nodes[n].prev].next = l->nodes[n].next;
    goto do_return;
  }
  default:
    return adlnls_{TYPE}_it_end (l);
  }
do_return:
  l->nodes[n].next = adlnls_{TYPE}_it_end (l);
  l->nodes[n].prev = adlnls_{TYPE}_it_end (l);
  --l->size;
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_drop(
  adlnls_{TYPE}* l, adlnls_{TYPE}_it n
  )
{
  return adlnls_{TYPE}_drop_explicit (l, n, true);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_drop_head (adlnls_{TYPE}* l)
{
  adlnls_{TYPE}_it ret = l->head.next;
  (void) adlnls_{TYPE}_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_{TYPE}_it adlnls_{TYPE}_drop_tail (adlnls_{TYPE}* l)
{
  adlnls_{TYPE}_it ret = l->head.prev;
  (void) adlnls_{TYPE}_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
