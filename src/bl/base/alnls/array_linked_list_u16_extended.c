/* Autogenerated file, don't edit */
/* AUTOGENERATE: src/bl/base/alnls/array_linked_list_u16_extended.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/array_linked_list_u16.h>

static void alnls_u16_find_two_previous_nodes(
  alnls_u16* l, alnls_u16_it* prev, alnls_u16_it* prev2, alnls_u16_it val
  )
{
  bl_assert (!alnls_u16_is_empty (l));
  *prev2       = alnls_u16_it_end (l);
  *prev        = alnls_u16_it_end (l);
  alnls_u16_it now = alnls_u16_it_begin (l);

  while (now != val) {
    *prev2 = *prev;
    *prev  = now;
    now    = alnls_u16_it_next (l, now);
  }
}
/* TODO: regular insert ? */
/*---------------------------------------------------------------------------*/
BL_EXPORT void alnls_u16_insert_tail (alnls_u16* l, alnls_u16_it n)
{
  bl_assert (alnls_u16_it_next (l, n) == alnls_u16_it_end (l));
  if (!alnls_u16_is_empty (l)) {
    alnls_u16_it tail, subtail;
    alnls_u16_find_two_previous_nodes (l, &tail, &subtail, alnls_u16_it_end (l));
    l->nodes[tail] = n;
    bl_assert (alnls_u16_it_next (l, tail) == n); /* overflow check */
  }
  else {
    l->head = n;
    bl_assert (alnls_u16_it_begin (l) == n); /* overflow check */
  }
  ++l->size;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT alnls_u16_it alnls_u16_drop (alnls_u16* l, alnls_u16_it n)
{
  bl_assert (alnls_u16_it_in_range( l, n));
  bl_assert (!alnls_u16_node_is_free (l, n));

  if (alnls_u16_is_empty (l)) {
    return alnls_u16_it_end (l);
  }
  alnls_u16_it should_be_n, prev, find;
  find = alnls_u16_it_next (l, n);
  alnls_u16_find_two_previous_nodes (l, &should_be_n, &prev, find);
  bl_assert (should_be_n == n);
  if (prev != alnls_u16_it_end (l)) {
    l->nodes[prev] = find;
  }
  else {
    l->head = alnls_u16_it_end (l);
  }
  l->nodes[n] = alnls_u16_it_end (l);
  --l->size;
  return prev;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT alnls_u16_it alnls_u16_drop_tail (alnls_u16* l)
{
  if (alnls_u16_is_empty (l)) {
    return alnls_u16_it_end (l);
  }
  alnls_u16_it tail, subtail;
  alnls_u16_find_two_previous_nodes (l, &tail, &subtail, alnls_u16_it_end (l));
  if (subtail != alnls_u16_it_end (l)) {
    l->nodes[subtail] = alnls_u16_it_end (l);
  }
  else {
    l->head = alnls_u16_it_end (l);
  }
  --l->size;
  return tail;
}
/*---------------------------------------------------------------------------*/
