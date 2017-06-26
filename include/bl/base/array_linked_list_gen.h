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

typedef {TYPE} alnls_{TYPE}_it;
/*---------------------------------------------------------------------------*/
typedef struct alnls_{TYPE} {
  uword head;
  uword capacity;
  uword size;
  {TYPE}* nodes;
}
alnls_{TYPE};
/*---------------------------------------------------------------------------*/
#define alnls_{TYPE}_foreach(type_ptr, it_var)\
  for (alnls_{TYPE}_it alnls_{TYPE}_priv_curr_it = alnls_{TYPE}_it_begin ((type_ptr)),\
         it_var = alnls_{TYPE}_priv_curr_it;\
       alnls_{TYPE}_it_in_range ((type_ptr), alnls_{TYPE}_priv_curr_it) &&\
         (alnls_{TYPE}_priv_curr_it =\
            alnls_{TYPE}_it_next ((type_ptr), alnls_{TYPE}_priv_curr_it), 1);\
       it_var = alnls_{TYPE}_priv_curr_it\
       )
/*---------------------------------------------------------------------------*/
#define alnls_{TYPE}_foreach_read_only(type_ptr, it_var)\
  for (it_var = alnls_{TYPE}_it_begin ((type_ptr));\
       alnls_{TYPE}_it_in_range ((type_ptr), it_var);\
       it_var = alnls_{TYPE}_it_next ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define alnls_{TYPE}_init(list, backing_nodes_array)\
  alnls_{TYPE}_init_impl(\
    (list),\
    (backing_nodes_array),\
    arr_elems (backing_nodes_array)\
    )
/*---------------------------------------------------------------------------*/
static inline alnls_{TYPE}_it alnls_{TYPE}_it_end (alnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline alnls_{TYPE}_it alnls_{TYPE}_it_begin (alnls_{TYPE}* l)
{
  return l->head;
}
/*---------------------------------------------------------------------------*/
static inline alnls_{TYPE}_it alnls_{TYPE}_capacity (alnls_{TYPE}* l)
{
  return l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline alnls_{TYPE}_it alnls_{TYPE}_size (alnls_{TYPE}* l)
{
  return l->size;
}
/*---------------------------------------------------------------------------*/
static inline alnls_{TYPE}_it alnls_{TYPE}_it_in_range (alnls_{TYPE}* l, alnls_{TYPE}_it it)
{
  return it < l->capacity;
}
/*---------------------------------------------------------------------------*/
static inline bool alnls_{TYPE}_is_empty (alnls_{TYPE}* l)
{
  return alnls_{TYPE}_it_begin (l) == alnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
static inline alnls_{TYPE}_it alnls_{TYPE}_it_next (alnls_{TYPE}* l, alnls_{TYPE}_it it)
{
  return l->nodes[it];
}
/*---------------------------------------------------------------------------*/
static inline void alnls_{TYPE}_node_release (alnls_{TYPE}* l, alnls_{TYPE}_it it)
{
  l->nodes[it] = it;
}
/*---------------------------------------------------------------------------*/
static inline bool alnls_{TYPE}_node_is_free (alnls_{TYPE}* l, alnls_{TYPE}_it it)
{
  return l->nodes[it] == it;
}
/*---------------------------------------------------------------------------*/
static inline void alnls_{TYPE}_acquire_node_unsafe (alnls_{TYPE}* l, alnls_{TYPE}_it it)
{
  bl_assert (it < alnls_{TYPE}_capacity (l));
  bl_assert (alnls_{TYPE}_node_is_free (l, it));
  l->nodes[it] = alnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void alnls_{TYPE}_init_impl(
  alnls_{TYPE}* l, {TYPE}* nodes, uword nodes_capacity
  );
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_try_acquire_node (alnls_{TYPE}* l, alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_try_acquire_a_node (alnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void alnls_{TYPE}_insert_head (alnls_{TYPE}* l, alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_drop_head (alnls_{TYPE}* l);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT void alnls_{TYPE}_insert_tail (alnls_{TYPE}* l, alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_drop (alnls_{TYPE}* l, alnls_{TYPE}_it n);
/*---------------------------------------------------------------------------*/
extern BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_drop_tail (alnls_{TYPE}* l);

#endif /* #define __BL_ARRAY_LINKED_LIST_{TYPE}_H__ */

/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: src/bl/base/alnls/array_linked_list_{TYPE}_basic.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_linked_list_{TYPE}.h>

BL_EXPORT void alnls_{TYPE}_init_impl(
  alnls_{TYPE}* l, {TYPE}* nodes, uword nodes_capacity
  )
{
  l->nodes    = nodes;
  l->capacity = nodes_capacity;
  l->head     = l->capacity;
  for (alnls_{TYPE}_it it = 0; it < l->capacity; ++it) {
    alnls_{TYPE}_node_release (l, it);
  }
  l->size = 0;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_try_acquire_node (alnls_{TYPE}* l, alnls_{TYPE}_it n)
{
  if (alnls_{TYPE}_node_is_free (l, n)) {
    alnls_{TYPE}_acquire_node_unsafe (l, n);
    return n;
  }
  return alnls_{TYPE}_it_end (l);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_try_acquire_a_node (alnls_{TYPE}* l)
{
  alnls_{TYPE}_it i;
  for (i = 0; i < alnls_{TYPE}_it_end (l); ++i) {
    if (alnls_{TYPE}_node_is_free (l, i)) {
      alnls_{TYPE}_acquire_node_unsafe (l, i);
      break;
    }
  }
  return i;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void alnls_{TYPE}_insert_head (alnls_{TYPE}* l, alnls_{TYPE}_it n)
{
  bl_assert (alnls_{TYPE}_it_next (l, n) == alnls_{TYPE}_it_end (l));
  l->nodes[n] = l->head;
  l->head     = ({TYPE}) n;
  bl_assert (alnls_{TYPE}_it_begin (l) == n); /* oveflow check */
  ++l->size;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_drop_head (alnls_{TYPE}* l)
{
  if (alnls_{TYPE}_is_empty (l)) {
    return alnls_{TYPE}_it_end (l);
  }
  alnls_{TYPE}_it prev_head = alnls_{TYPE}_it_begin (l);
  l->head             = alnls_{TYPE}_it_next (l, prev_head);
  l->nodes[prev_head] = alnls_{TYPE}_it_end (l);
  --l->size;
  return prev_head;
}
/*---------------------------------------------------------------------------*/
/* AUTOGENERATE: src/bl/base/alnls/array_linked_list_{TYPE}_extended.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_linked_list_{TYPE}.h>

static void alnls_{TYPE}_find_two_previous_nodes(
  alnls_{TYPE}* l, alnls_{TYPE}_it* prev, alnls_{TYPE}_it* prev2, alnls_{TYPE}_it val
  )
{
  bl_assert (!alnls_{TYPE}_is_empty (l));
  *prev2       = alnls_{TYPE}_it_end (l);
  *prev        = alnls_{TYPE}_it_end (l);
  alnls_{TYPE}_it now = alnls_{TYPE}_it_begin (l);

  while (now != val) {
    *prev2 = *prev;
    *prev  = now;
    now    = alnls_{TYPE}_it_next (l, now);
  }
}
/* TODO: regular insert ? */
/*---------------------------------------------------------------------------*/
BL_EXPORT void alnls_{TYPE}_insert_tail (alnls_{TYPE}* l, alnls_{TYPE}_it n)
{
  bl_assert (alnls_{TYPE}_it_next (l, n) == alnls_{TYPE}_it_end (l));
  if (!alnls_{TYPE}_is_empty (l)) {
    alnls_{TYPE}_it tail, subtail;
    alnls_{TYPE}_find_two_previous_nodes (l, &tail, &subtail, alnls_{TYPE}_it_end (l));
    l->nodes[tail] = n;
    bl_assert (alnls_{TYPE}_it_next (l, tail) == n); /* overflow check */
  }
  else {
    l->head = n;
    bl_assert (alnls_{TYPE}_it_begin (l) == n); /* overflow check */
  }
  ++l->size;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_drop (alnls_{TYPE}* l, alnls_{TYPE}_it n)
{
  bl_assert (alnls_{TYPE}_it_in_range( l, n));
  bl_assert (!alnls_{TYPE}_node_is_free (l, n));

  if (alnls_{TYPE}_is_empty (l)) {
    return alnls_{TYPE}_it_end (l);
  }
  alnls_{TYPE}_it should_be_n, prev, find;
  find = alnls_{TYPE}_it_next (l, n);
  alnls_{TYPE}_find_two_previous_nodes (l, &should_be_n, &prev, find);
  bl_assert (should_be_n == n);
  if (prev != alnls_{TYPE}_it_end (l)) {
    l->nodes[prev] = find;
  }
  else {
    l->head = alnls_{TYPE}_it_end (l);
  }
  l->nodes[n] = alnls_{TYPE}_it_end (l);
  --l->size;
  return prev;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT alnls_{TYPE}_it alnls_{TYPE}_drop_tail (alnls_{TYPE}* l)
{
  if (alnls_{TYPE}_is_empty (l)) {
    return alnls_{TYPE}_it_end (l);
  }
  alnls_{TYPE}_it tail, subtail;
  alnls_{TYPE}_find_two_previous_nodes (l, &tail, &subtail, alnls_{TYPE}_it_end (l));
  if (subtail != alnls_{TYPE}_it_end (l)) {
    l->nodes[subtail] = alnls_{TYPE}_it_end (l);
  }
  else {
    l->head = alnls_{TYPE}_it_end (l);
  }
  --l->size;
  return tail;
}
/*---------------------------------------------------------------------------*/
