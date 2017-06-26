/* Autogenerated file, don't edit */
/* AUTOGENERATE: src/bl/base/adlnls/array_doubly_linked_list_u8_basic.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_doubly_linked_list_u8.h>

BL_EXPORT void adlnls_u8_init_impl(
  adlnls_u8* l, u8* nodes, uword nodes_capacity
  )
{
  l->nodes     = (adlnls_u8_hook*) nodes;
  l->capacity  = nodes_capacity;
  l->head.next = l->capacity;
  l->head.prev = l->capacity;
  for (adlnls_u8_it it = 0; it < l->capacity; ++it) {
    adlnls_u8_node_release (l, it);
  }
  l->size = 0;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_u8_it adlnls_u8_try_acquire_node (adlnls_u8* l, adlnls_u8_it n)
{
  if (adlnls_u8_node_is_free (l, n)) {
    adlnls_u8_acquire_node_unsafe (l, n);
    return n;
  }
  return adlnls_u8_it_end (l);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_u8_it adlnls_u8_try_acquire_a_node (adlnls_u8* l)
{
  adlnls_u8_it i;
  for (i = 0; i < adlnls_u8_it_end (l); ++i) {
    if (adlnls_u8_node_is_free (l, i)) {
      adlnls_u8_acquire_node_unsafe (l, i);
      break;
    }
  }
  return i;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT void adlnls_u8_insert_head (adlnls_u8* l, adlnls_u8_it n)
{
  bl_assert (adlnls_u8_it_next (l, n) == adlnls_u8_it_end (l));
  bl_assert (adlnls_u8_it_prev (l, n) == adlnls_u8_it_end (l));

  if (!adlnls_u8_is_empty (l)) {
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
BL_EXPORT void adlnls_u8_insert_tail (adlnls_u8* l, adlnls_u8_it n)
{
  bl_assert (adlnls_u8_it_next (l, n) == adlnls_u8_it_end (l));
  bl_assert (adlnls_u8_it_prev (l, n) == adlnls_u8_it_end (l));

  if (!adlnls_u8_is_empty (l)) {
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
static inline uword adlns_u8_get_node_type (adlnls_u8* l, adlnls_u8_it n)
{
  return (l->nodes[n].next == adlnls_u8_it_end (l)) |
       ((l->nodes[n].prev == adlnls_u8_it_end (l) ) << 1);
}
/*----------------------------------------------------------------------------*/
BL_EXPORT  adlnls_u8_it adlnls_u8_drop_explicit(
  adlnls_u8* l, adlnls_u8_it n, bool return_previous
  )
{
  bl_assert (adlnls_u8_it_in_range (l, n));
  bl_assert (!adlnls_u8_node_is_free (l, n));

  if (adlnls_u8_is_empty (l)) {
    return adlnls_u8_it_end (l);
  }
  adlnls_u8_it ret;

  switch (adlns_u8_get_node_type (l, n)) {
  case adlnls_node_type_tail_and_head: {
    ret          = adlnls_u8_it_end (l);
    l->head.next = adlnls_u8_it_end (l);
    l->head.prev = adlnls_u8_it_end (l);
    goto do_return;
  }
  case adlnls_node_type_tail:{
    ret = return_previous ? l->nodes[n].prev : adlnls_u8_it_end (l);
    l->nodes[l->nodes[n].prev].next = adlnls_u8_it_end (l);
    l->head.prev                    = l->nodes[n].prev;
    goto do_return;
  }
  case adlnls_node_type_head: {
    ret = return_previous ? adlnls_u8_it_end (l) : l->nodes[n].next;
    l->nodes[l->nodes[n].next].prev = adlnls_u8_it_end (l);
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
    return adlnls_u8_it_end (l);
  }
do_return:
  l->nodes[n].next = adlnls_u8_it_end (l);
  l->nodes[n].prev = adlnls_u8_it_end (l);
  --l->size;
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_u8_it adlnls_u8_drop(
  adlnls_u8* l, adlnls_u8_it n
  )
{
  return adlnls_u8_drop_explicit (l, n, true);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_u8_it adlnls_u8_drop_head (adlnls_u8* l)
{
  adlnls_u8_it ret = l->head.next;
  (void) adlnls_u8_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT adlnls_u8_it adlnls_u8_drop_tail (adlnls_u8* l)
{
  adlnls_u8_it ret = l->head.prev;
  (void) adlnls_u8_drop (l, ret);
  return ret;
}
/*---------------------------------------------------------------------------*/
