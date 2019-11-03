/* Autogenerated file by running: scripts/autogenerator.sh -f include/bl/base/array_doubly_linked_list.gen.h -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64, don't edit */
/* AUTOGENERATE: src/bl/base/adlnls/array_doubly_linked_list_u32_basic.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_doubly_linked_list_u32.h>

BL_EXPORT void bl_adlnls_u32_init_impl(
  bl_adlnls_u32* l, bl_u32* nodes, bl_uword nodes_capacity
  )
{
  l->nodes     = (bl_adlnls_u32_hook*) nodes;
  l->capacity  = nodes_capacity;
  l->head.next = l->capacity;
  l->head.prev = l->capacity;
  for (bl_adlnls_u32_it it = 0; it < l->capacity; ++it) {
    bl_adlnls_u32_node_release (l, it);
  }
  l->size = 0;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_u32_it
  bl_adlnls_u32_try_acquire_node (bl_adlnls_u32* l, bl_adlnls_u32_it n)
{
  if (bl_adlnls_u32_node_is_free (l, n)) {
    bl_adlnls_u32_acquire_node_unsafe (l, n);
    return n;
  }
  return bl_adlnls_u32_it_end (l);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_u32_it
  bl_adlnls_u32_try_acquire_a_node (bl_adlnls_u32* l)
{
  bl_adlnls_u32_it i;
  for (i = 0; i < bl_adlnls_u32_it_end (l); ++i) {
    if (bl_adlnls_u32_node_is_free (l, i)) {
      bl_adlnls_u32_acquire_node_unsafe (l, i);
      break;
    }
  }
  return i;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void
  bl_adlnls_u32_insert_head (bl_adlnls_u32* l, bl_adlnls_u32_it n)
{
  bl_assert (bl_adlnls_u32_it_next (l, n) == bl_adlnls_u32_it_end (l));
  bl_assert (bl_adlnls_u32_it_prev (l, n) == bl_adlnls_u32_it_end (l));

  if (!bl_adlnls_u32_is_empty (l)) {
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
  bl_adlnls_u32_insert_tail (bl_adlnls_u32* l, bl_adlnls_u32_it n)
{
  bl_assert (bl_adlnls_u32_it_next (l, n) == bl_adlnls_u32_it_end (l));
  bl_assert (bl_adlnls_u32_it_prev (l, n) == bl_adlnls_u32_it_end (l));

  if (!bl_adlnls_u32_is_empty (l)) {
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
  adlns_u32_get_node_type (bl_adlnls_u32* l, bl_adlnls_u32_it n)
{
  return (l->nodes[n].next == bl_adlnls_u32_it_end (l)) |
       ((l->nodes[n].prev == bl_adlnls_u32_it_end (l) ) << 1);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT  bl_adlnls_u32_it bl_adlnls_u32_drop_explicit(
  bl_adlnls_u32* l, bl_adlnls_u32_it n, bool return_previous
  )
{
  bl_assert (bl_adlnls_u32_it_in_range (l, n));
  bl_assert (!bl_adlnls_u32_node_is_free (l, n));

  if (bl_adlnls_u32_is_empty (l)) {
    return bl_adlnls_u32_it_end (l);
  }
  bl_adlnls_u32_it ret;

  switch (adlns_u32_get_node_type (l, n)) {
  case bl_adlnls_node_type_tail_and_head: {
    ret          = bl_adlnls_u32_it_end (l);
    l->head.next = bl_adlnls_u32_it_end (l);
    l->head.prev = bl_adlnls_u32_it_end (l);
    goto do_return;
  }
  case bl_adlnls_node_type_tail:{
    ret = return_previous ? l->nodes[n].prev : bl_adlnls_u32_it_end (l);
    l->nodes[l->nodes[n].prev].next = bl_adlnls_u32_it_end (l);
    l->head.prev                    = l->nodes[n].prev;
    goto do_return;
  }
  case bl_adlnls_node_type_head: {
    ret = return_previous ? bl_adlnls_u32_it_end (l) : l->nodes[n].next;
    l->nodes[l->nodes[n].next].prev = bl_adlnls_u32_it_end (l);
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
    return bl_adlnls_u32_it_end (l);
  }
do_return:
  l->nodes[n].next = bl_adlnls_u32_it_end (l);
  l->nodes[n].prev = bl_adlnls_u32_it_end (l);
  --l->size;
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_u32_it bl_adlnls_u32_drop(
  bl_adlnls_u32* l, bl_adlnls_u32_it n
  )
{
  return bl_adlnls_u32_drop_explicit (l, n, true);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_u32_it bl_adlnls_u32_drop_head (bl_adlnls_u32* l)
{
  bl_adlnls_u32_it ret = l->head.next;
  (void) bl_adlnls_u32_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_adlnls_u32_it bl_adlnls_u32_drop_tail (bl_adlnls_u32* l)
{
  bl_adlnls_u32_it ret = l->head.prev;
  (void) bl_adlnls_u32_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
