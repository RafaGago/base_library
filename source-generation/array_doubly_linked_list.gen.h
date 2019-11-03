/*
This file is used for autogeneration and is invalid C. The extension is kept to
make the code editor color the syntax.

To regenerate use scripts/autogenerate.sh:

This is BAD, but IMO it was worse to have criptic-macro based generic data
structures. This library is designed to be statically linked, so some of the
code has different translation units which can be easily discarded by the
linker.
*/
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: include/bl/base/impl/generated/array_doubly_linked_list_{TYPE}.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_ARRAY_DOUBLY_LINKED_LIST_{TYPE}_H__
#define __BL_ARRAY_DOUBLY_LINKED_LIST_{TYPE}_H__

#include <bl/base/assert.h>
#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/utility.h>

typedef bl_{TYPE} bl_adlnls_{TYPE}_it;
/*---------------------------------------------------------------------------*/
typedef struct bl_adlnls_{TYPE}_hook {
  bl_{TYPE} next : bl_type_bits (bl_{TYPE}) / 2;
  bl_{TYPE} prev : bl_type_bits (bl_{TYPE}) / 2;
}
bl_adlnls_{TYPE}_hook;
/*---------------------------------------------------------------------------*/
typedef struct bl_adlnls_{TYPE} {
  bl_adlnls_{TYPE}_hook head;
  bl_adlnls_{TYPE}_hook* nodes;
  bl_uword capacity;
  bl_uword size;
}
bl_adlnls_{TYPE};
/*---------------------------------------------------------------------------*/
#define bl_adlnls_{TYPE}_foreach(type_ptr, it_var)\
  for(\
    bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_priv_curr_it =\
        bl_adlnls_{TYPE}_it_begin ((type_ptr)),\
      it_var = bl_adlnls_{TYPE}_priv_curr_it;\
    bl_adlnls_{TYPE}_it_in_range ((type_ptr), bl_adlnls_{TYPE}_priv_curr_it) &&\
      (bl_adlnls_{TYPE}_priv_curr_it =\
        bl_adlnls_{TYPE}_it_next(\
          (type_ptr), bl_adlnls_{TYPE}_priv_curr_it), 1\
          );\
    it_var = bl_adlnls_{TYPE}_priv_curr_it\
    )
/*---------------------------------------------------------------------------*/
#define bl_adlnls_{TYPE}_foreach_read_only(type_ptr, it_var)\
  for(\
    it_var = bl_adlnls_{TYPE}_it_begin ((type_ptr));\
    bl_adlnls_{TYPE}_it_in_range ((type_ptr), it_var);\
    it_var = bl_adlnls_{TYPE}_it_next ((type_ptr), it_var)\
    )
/*---------------------------------------------------------------------------*/
/* "_r" reverse iteration */
#define bl_adlnls_{TYPE}_foreach_r(type_ptr, it_var)\
  for(\
    bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_priv_curr_it =\
        bl_adlnls_{TYPE}_it_rbegin ((type_ptr)),\
      it_var = bl_adlnls_{TYPE}_priv_curr_it;\
    bl_adlnls_{TYPE}_it_in_range_r(\
        (type_ptr), bl_adlnls_{TYPE}_priv_curr_it\
        ) &&\
      (bl_adlnls_{TYPE}_priv_curr_it =\
    bl_adlnls_{TYPE}_it_prev ((type_ptr), bl_adlnls_{TYPE}_priv_curr_it), 1);\
      it_var = bl_adlnls_{TYPE}_priv_curr_it\
    )
/*---------------------------------------------------------------------------*/
#define bl_adlnls_{TYPE}_foreach_r_read_only(type_ptr, it_var)\
  for(\
    it_var = bl_adlnls_{TYPE}_it_rbegin ((type_ptr));\
    bl_adlnls_{TYPE}_it_in_range_r ((type_ptr), it_var);\
    it_var = bl_adlnls_{TYPE}_it_prev ((type_ptr), it_var)\
    )
/*---------------------------------------------------------------------------*/
#define bl_adlnls_{TYPE}_init(list, backing_nodes_array)\
  bl_adlnls_{TYPE}_init_impl(\
    (list),\
    (backing_nodes_array),\
    bl_arr_elems (backing_nodes_array)\
    )
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_it_end (bl_adlnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_it_rend (bl_adlnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_it_begin (bl_adlnls_{TYPE}* l)
{
  return l->head.next;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_it_rbegin (bl_adlnls_{TYPE}* l)
{
  return l->head.prev;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_capacity (bl_adlnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_size (bl_adlnls_{TYPE}* l)
{
  return l->size;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_it_in_range (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_it_in_range_r (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_adlnls_{TYPE}_is_empty (bl_adlnls_{TYPE}* l)
{
  return bl_adlnls_{TYPE}_it_begin (l) == bl_adlnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_it_next (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it it)
{
  return l->nodes[it].next;
}
/*---------------------------------------------------------------------------*/
static inline bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_it_prev (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it it)
{
  return l->nodes[it].prev;
}
/*---------------------------------------------------------------------------*/
static inline void
  bl_adlnls_{TYPE}_node_release (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it it)
{
  l->nodes[it].next = it;
  l->nodes[it].prev = it;
}
/*---------------------------------------------------------------------------*/
static inline bool
  bl_adlnls_{TYPE}_node_is_free (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it it)
{
  return l->nodes[it].next == it;
}
/*---------------------------------------------------------------------------*/
static inline void bl_adlnls_{TYPE}_acquire_node_unsafe(
  bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it it
  )
{
  bl_assert (it < bl_adlnls_{TYPE}_capacity (l));
  bl_assert (bl_adlnls_{TYPE}_node_is_free (l, it));
  l->nodes[it].prev = bl_adlnls_{TYPE}_it_end (l);
  l->nodes[it].next = bl_adlnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void bl_adlnls_{TYPE}_init_impl(
  bl_adlnls_{TYPE}* l, bl_{TYPE}* nodes, bl_uword nodes_capacity
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_try_acquire_node(
  bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_try_acquire_a_node (bl_adlnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void
  bl_adlnls_{TYPE}_insert_head (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void
  bl_adlnls_{TYPE}_insert_tail (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_drop_head (bl_adlnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_drop_tail (bl_adlnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_drop (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_drop_explicit(
  bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n, bool return_previous
  );

#endif /* #define __BL_ARRAY_LINKED_LIST_{TYPE}_H__ */

/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: src/bl/base/generated/adlnls/array_doubly_linked_list_{TYPE}_basic.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/generated/array_doubly_linked_list_{TYPE}.h>

BL_EXPORT void bl_adlnls_{TYPE}_init_impl(
  bl_adlnls_{TYPE}* l, bl_{TYPE}* nodes, bl_uword nodes_capacity
  )
{
  l->nodes     = (bl_adlnls_{TYPE}_hook*) nodes;
  l->capacity  = nodes_capacity;
  l->head.next = l->capacity;
  l->head.prev = l->capacity;
  for (bl_adlnls_{TYPE}_it it = 0; it < l->capacity; ++it) {
    bl_adlnls_{TYPE}_node_release (l, it);
  }
  l->size = 0;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_try_acquire_node (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n)
{
  if (bl_adlnls_{TYPE}_node_is_free (l, n)) {
    bl_adlnls_{TYPE}_acquire_node_unsafe (l, n);
    return n;
  }
  return bl_adlnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_{TYPE}_it
  bl_adlnls_{TYPE}_try_acquire_a_node (bl_adlnls_{TYPE}* l)
{
  bl_adlnls_{TYPE}_it i;
  for (i = 0; i < bl_adlnls_{TYPE}_it_end (l); ++i) {
    if (bl_adlnls_{TYPE}_node_is_free (l, i)) {
      bl_adlnls_{TYPE}_acquire_node_unsafe (l, i);
      break;
    }
  }
  return i;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void
  bl_adlnls_{TYPE}_insert_head (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n)
{
  bl_assert (bl_adlnls_{TYPE}_it_next (l, n) == bl_adlnls_{TYPE}_it_end (l));
  bl_assert (bl_adlnls_{TYPE}_it_prev (l, n) == bl_adlnls_{TYPE}_it_end (l));

  if (!bl_adlnls_{TYPE}_is_empty (l)) {
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
BL_EXPORT void
  bl_adlnls_{TYPE}_insert_tail (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n)
{
  bl_assert (bl_adlnls_{TYPE}_it_next (l, n) == bl_adlnls_{TYPE}_it_end (l));
  bl_assert (bl_adlnls_{TYPE}_it_prev (l, n) == bl_adlnls_{TYPE}_it_end (l));

  if (!bl_adlnls_{TYPE}_is_empty (l)) {
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
#define bl_adlnls_node_type_middle        0
#define bl_adlnls_node_type_tail          1
#define bl_adlnls_node_type_head          2
#define bl_adlnls_node_type_tail_and_head 3
/*----------------------------------------------------------------------------*/
static inline bl_uword
  adlns_{TYPE}_get_node_type (bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n)
{
  return (l->nodes[n].next == bl_adlnls_{TYPE}_it_end (l)) |
       ((l->nodes[n].prev == bl_adlnls_{TYPE}_it_end (l) ) << 1);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT  bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_drop_explicit(
  bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n, bool return_previous
  )
{
  bl_assert (bl_adlnls_{TYPE}_it_in_range (l, n));
  bl_assert (!bl_adlnls_{TYPE}_node_is_free (l, n));

  if (bl_adlnls_{TYPE}_is_empty (l)) {
    return bl_adlnls_{TYPE}_it_end (l);
  }
  bl_adlnls_{TYPE}_it ret;

  switch (adlns_{TYPE}_get_node_type (l, n)) {
  case bl_adlnls_node_type_tail_and_head: {
    ret          = bl_adlnls_{TYPE}_it_end (l);
    l->head.next = bl_adlnls_{TYPE}_it_end (l);
    l->head.prev = bl_adlnls_{TYPE}_it_end (l);
    goto do_return;
  }
  case bl_adlnls_node_type_tail:{
    ret = return_previous ? l->nodes[n].prev : bl_adlnls_{TYPE}_it_end (l);
    l->nodes[l->nodes[n].prev].next = bl_adlnls_{TYPE}_it_end (l);
    l->head.prev                    = l->nodes[n].prev;
    goto do_return;
  }
  case bl_adlnls_node_type_head: {
    ret = return_previous ? bl_adlnls_{TYPE}_it_end (l) : l->nodes[n].next;
    l->nodes[l->nodes[n].next].prev = bl_adlnls_{TYPE}_it_end (l);
    l->head.next                    = l->nodes[n].next;
    goto do_return;
  }
  case bl_adlnls_node_type_middle: {
    ret = return_previous ? l->nodes[n].prev : l->nodes[n].next;
    l->nodes[l->nodes[n].next].prev = l->nodes[n].prev;
    l->nodes[l->nodes[n].prev].next = l->nodes[n].next;
    goto do_return;
  }
  default:
    return bl_adlnls_{TYPE}_it_end (l);
  }
do_return:
  l->nodes[n].next = bl_adlnls_{TYPE}_it_end (l);
  l->nodes[n].prev = bl_adlnls_{TYPE}_it_end (l);
  --l->size;
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_drop(
  bl_adlnls_{TYPE}* l, bl_adlnls_{TYPE}_it n
  )
{
  return bl_adlnls_{TYPE}_drop_explicit (l, n, true);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_drop_head (bl_adlnls_{TYPE}* l)
{
  bl_adlnls_{TYPE}_it ret = l->head.next;
  (void) bl_adlnls_{TYPE}_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_{TYPE}_it bl_adlnls_{TYPE}_drop_tail (bl_adlnls_{TYPE}* l)
{
  bl_adlnls_{TYPE}_it ret = l->head.prev;
  (void) bl_adlnls_{TYPE}_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
