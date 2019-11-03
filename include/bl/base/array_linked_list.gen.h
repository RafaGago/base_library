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
/* AUTOGENERATE: include/bl/base/impl/array_linked_list_{TYPE}.h */
/*---------------------------------------------------------------------------*/
#ifndef __BL_ARRAY_LINKED_LIST_{TYPE}_H__
#define __BL_ARRAY_LINKED_LIST_{TYPE}_H__

#include <bl/base/assert.h>
#include <bl/base/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/utility.h>

typedef bl_{TYPE} bl_alnls_{TYPE}_it;
/*---------------------------------------------------------------------------*/
typedef struct bl_alnls_{TYPE} {
  bl_uword capacity;
  bl_uword size;
  bl_{TYPE}  head;
  bl_{TYPE}* nodes;
}
bl_alnls_{TYPE};
/*---------------------------------------------------------------------------*/
#define bl_alnls_{TYPE}_foreach(type_ptr, it_var)\
  for(\
    bl_alnls_{TYPE}_it bl_alnls_{TYPE}_priv_curr_it =\
        bl_alnls_{TYPE}_it_begin ((type_ptr)),\
      it_var = bl_alnls_{TYPE}_priv_curr_it;\
    bl_alnls_{TYPE}_it_in_range ((type_ptr), bl_alnls_{TYPE}_priv_curr_it) &&\
      (bl_alnls_{TYPE}_priv_curr_it =\
        bl_alnls_{TYPE}_it_next ((type_ptr), bl_alnls_{TYPE}_priv_curr_it), 1);\
    it_var = bl_alnls_{TYPE}_priv_curr_it\
    )
/*---------------------------------------------------------------------------*/
#define bl_alnls_{TYPE}_foreach_read_only(type_ptr, it_var)\
  for(\
    it_var = bl_alnls_{TYPE}_it_begin ((type_ptr));\
    bl_alnls_{TYPE}_it_in_range ((type_ptr), it_var);\
    it_var = bl_alnls_{TYPE}_it_next ((type_ptr), it_var)\
    )
/*---------------------------------------------------------------------------*/
#define bl_alnls_{TYPE}_init(list, backing_nodes_array)\
  bl_alnls_{TYPE}_init_impl(\
    (list),\
    (backing_nodes_array),\
    bl_arr_elems (backing_nodes_array)\
    )
/*---------------------------------------------------------------------------*/
static inline bl_alnls_{TYPE}_it bl_alnls_{TYPE}_it_end (bl_alnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_alnls_{TYPE}_it bl_alnls_{TYPE}_it_begin (bl_alnls_{TYPE}* l)
{
  return l->head;
}
/*---------------------------------------------------------------------------*/
static inline bl_alnls_{TYPE}_it bl_alnls_{TYPE}_capacity (bl_alnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bl_alnls_{TYPE}_it bl_alnls_{TYPE}_size (bl_alnls_{TYPE}* l)
{
  return l->size;
}
/*---------------------------------------------------------------------------*/
static inline bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_it_in_range (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bool bl_alnls_{TYPE}_is_empty (bl_alnls_{TYPE}* l)
{
  return bl_alnls_{TYPE}_it_begin (l) == bl_alnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
static inline bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_it_next (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it it)
{
  return l->nodes[it];
}
/*---------------------------------------------------------------------------*/
static inline void
  bl_alnls_{TYPE}_node_release (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it it)
{
  l->nodes[it] = it;
}
/*---------------------------------------------------------------------------*/
static inline bool
  bl_alnls_{TYPE}_node_is_free (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it it)
{
  return l->nodes[it] == it;
}
/*---------------------------------------------------------------------------*/
static inline void bl_alnls_{TYPE}_acquire_node_unsafe(
  bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it it
  )
{
  bl_assert (it < bl_alnls_{TYPE}_capacity (l));
  bl_assert (bl_alnls_{TYPE}_node_is_free (l, it));
  l->nodes[it] = bl_alnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void bl_alnls_{TYPE}_init_impl(
  bl_alnls_{TYPE}* l, bl_{TYPE}* nodes, bl_uword nodes_capacity
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_try_acquire_node (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_try_acquire_a_node (bl_alnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void
  bl_alnls_{TYPE}_insert_head (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_drop_head (bl_alnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void
  bl_alnls_{TYPE}_insert_tail (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_drop (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_drop_tail (bl_alnls_{TYPE}* l);

#endif /* #define __BL_ARRAY_LINKED_LIST_{TYPE}_H__ */

/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: src/bl/base/alnls/array_linked_list_{TYPE}_basic.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_linked_list_{TYPE}.h>

BL_EXPORT void bl_alnls_{TYPE}_init_impl(
  bl_alnls_{TYPE}* l, bl_{TYPE}* nodes, bl_uword nodes_capacity
  )
{
  l->nodes    = nodes;
  l->capacity = nodes_capacity;
  l->head     = l->capacity;
  for (bl_alnls_{TYPE}_it it = 0; it < l->capacity; ++it) {
    bl_alnls_{TYPE}_node_release (l, it);
  }
  l->size = 0;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_try_acquire_node (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n)
{
  if (bl_alnls_{TYPE}_node_is_free (l, n)) {
    bl_alnls_{TYPE}_acquire_node_unsafe (l, n);
    return n;
  }
  return bl_alnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_try_acquire_a_node (bl_alnls_{TYPE}* l)
{
  bl_alnls_{TYPE}_it i;
  for (i = 0; i < bl_alnls_{TYPE}_it_end (l); ++i) {
    if (bl_alnls_{TYPE}_node_is_free (l, i)) {
      bl_alnls_{TYPE}_acquire_node_unsafe (l, i);
      break;
    }
  }
  return i;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void
  bl_alnls_{TYPE}_insert_head (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n)
{
  bl_assert (bl_alnls_{TYPE}_it_next (l, n) == bl_alnls_{TYPE}_it_end (l));
  l->nodes[n] = l->head;
  l->head     = (bl_{TYPE}) n;
  bl_assert (bl_alnls_{TYPE}_it_begin (l) == n); /* oveflow check */
  ++l->size;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_drop_head (bl_alnls_{TYPE}* l)
{
  if (bl_alnls_{TYPE}_is_empty (l)) {
    return bl_alnls_{TYPE}_it_end (l);
  }
  bl_alnls_{TYPE}_it prev_head = bl_alnls_{TYPE}_it_begin (l);
  l->head             = bl_alnls_{TYPE}_it_next (l, prev_head);
  l->nodes[prev_head] = bl_alnls_{TYPE}_it_end (l);
  --l->size;
  return prev_head;
}
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: src/bl/base/alnls/array_linked_list_{TYPE}_extended.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_linked_list_{TYPE}.h>

static void bl_alnls_{TYPE}_find_two_previous_nodes(
  bl_alnls_{TYPE}*    l,
  bl_alnls_{TYPE}_it* prev,
  bl_alnls_{TYPE}_it* prev2,
  bl_alnls_{TYPE}_it  val
  )
{
  bl_assert (!bl_alnls_{TYPE}_is_empty (l));
  *prev2       = bl_alnls_{TYPE}_it_end (l);
  *prev        = bl_alnls_{TYPE}_it_end (l);
  bl_alnls_{TYPE}_it now = bl_alnls_{TYPE}_it_begin (l);

  while (now != val) {
    *prev2 = *prev;
    *prev  = now;
    now    = bl_alnls_{TYPE}_it_next (l, now);
  }
}
/* TODO: regular insert ? */
/*---------------------------------------------------------------------------*/
BL_EXPORT void bl_alnls_{TYPE}_insert_tail(
  bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n
  )
{
  bl_assert (bl_alnls_{TYPE}_it_next (l, n) == bl_alnls_{TYPE}_it_end (l));
  if (!bl_alnls_{TYPE}_is_empty (l)) {
    bl_alnls_{TYPE}_it tail, subtail;
    bl_alnls_{TYPE}_find_two_previous_nodes(
      l, &tail, &subtail, bl_alnls_{TYPE}_it_end (l)
      );
    l->nodes[tail] = n;
    bl_assert (bl_alnls_{TYPE}_it_next (l, tail) == n); /* overflow check */
  }
  else {
    l->head = n;
    bl_assert (bl_alnls_{TYPE}_it_begin (l) == n); /* overflow check */
  }
  ++l->size;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_drop (bl_alnls_{TYPE}* l, bl_alnls_{TYPE}_it n)
{
  bl_assert (bl_alnls_{TYPE}_it_in_range( l, n));
  bl_assert (!bl_alnls_{TYPE}_node_is_free (l, n));

  if (bl_alnls_{TYPE}_is_empty (l)) {
    return bl_alnls_{TYPE}_it_end (l);
  }
  bl_alnls_{TYPE}_it should_be_n, prev, find;
  find = bl_alnls_{TYPE}_it_next (l, n);
  bl_alnls_{TYPE}_find_two_previous_nodes (l, &should_be_n, &prev, find);
  bl_assert (should_be_n == n);
  if (prev != bl_alnls_{TYPE}_it_end (l)) {
    l->nodes[prev] = find;
  }
  else {
    l->head = bl_alnls_{TYPE}_it_end (l);
  }
  l->nodes[n] = bl_alnls_{TYPE}_it_end (l);
  --l->size;
  return prev;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_alnls_{TYPE}_it
  bl_alnls_{TYPE}_drop_tail (bl_alnls_{TYPE}* l)
{
  if (bl_alnls_{TYPE}_is_empty (l)) {
    return bl_alnls_{TYPE}_it_end (l);
  }
  bl_alnls_{TYPE}_it tail, subtail;
  bl_alnls_{TYPE}_find_two_previous_nodes(
    l, &tail, &subtail, bl_alnls_{TYPE}_it_end (l)
    );
  if (subtail != bl_alnls_{TYPE}_it_end (l)) {
    l->nodes[subtail] = bl_alnls_{TYPE}_it_end (l);
  }
  else {
    l->head = bl_alnls_{TYPE}_it_end (l);
  }
  --l->size;
  return tail;
}
/*---------------------------------------------------------------------------*/
