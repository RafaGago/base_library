/*
Autogenerated file by running:
scripts/autogenerator.sh -f source-generation/array_linked_list.gen.h -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64

Don't edit.
*/
/* AUTOGENERATE: src/bl/base/generated/alnls/array_linked_list_u8_extended.c */
/*---------------------------------------------------------------------------*/

#include <bl/base/impl/generated/array_linked_list_u8.h>

#ifdef __cplusplus
extern "C" {
#endif

static void bl_alnls_u8_find_two_previous_nodes(
  bl_alnls_u8*    l,
  bl_alnls_u8_it* prev,
  bl_alnls_u8_it* prev2,
  bl_alnls_u8_it  val
  )
{
  bl_assert (!bl_alnls_u8_is_empty (l));
  *prev2       = bl_alnls_u8_it_end (l);
  *prev        = bl_alnls_u8_it_end (l);
  bl_alnls_u8_it now = bl_alnls_u8_it_begin (l);

  while (now != val) {
    *prev2 = *prev;
    *prev  = now;
    now    = bl_alnls_u8_it_next (l, now);
  }
}
/* TODO: regular insert ? */
/*---------------------------------------------------------------------------*/
BL_EXPORT void bl_alnls_u8_insert_tail(
  bl_alnls_u8* l, bl_alnls_u8_it n
  )
{
  bl_assert (bl_alnls_u8_it_next (l, n) == bl_alnls_u8_it_end (l));
  if (!bl_alnls_u8_is_empty (l)) {
    bl_alnls_u8_it tail, subtail;
    bl_alnls_u8_find_two_previous_nodes(
      l, &tail, &subtail, bl_alnls_u8_it_end (l)
      );
    l->nodes[tail] = n;
    bl_assert (bl_alnls_u8_it_next (l, tail) == n); /* overflow check */
  }
  else {
    l->head = n;
    bl_assert (bl_alnls_u8_it_begin (l) == n); /* overflow check */
  }
  ++l->size;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_alnls_u8_it
  bl_alnls_u8_drop (bl_alnls_u8* l, bl_alnls_u8_it n)
{
  bl_assert (bl_alnls_u8_it_in_range( l, n));
  bl_assert (!bl_alnls_u8_node_is_free (l, n));

  if (bl_alnls_u8_is_empty (l)) {
    return bl_alnls_u8_it_end (l);
  }
  bl_alnls_u8_it should_be_n, prev, find;
  find = bl_alnls_u8_it_next (l, n);
  bl_alnls_u8_find_two_previous_nodes (l, &should_be_n, &prev, find);
  bl_assert (should_be_n == n);
  if (prev != bl_alnls_u8_it_end (l)) {
    l->nodes[prev] = find;
  }
  else {
    l->head = bl_alnls_u8_it_end (l);
  }
  l->nodes[n] = bl_alnls_u8_it_end (l);
  --l->size;
  return prev;
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_alnls_u8_it
  bl_alnls_u8_drop_tail (bl_alnls_u8* l)
{
  if (bl_alnls_u8_is_empty (l)) {
    return bl_alnls_u8_it_end (l);
  }
  bl_alnls_u8_it tail, subtail;
  bl_alnls_u8_find_two_previous_nodes(
    l, &tail, &subtail, bl_alnls_u8_it_end (l)
    );
  if (subtail != bl_alnls_u8_it_end (l)) {
    l->nodes[subtail] = bl_alnls_u8_it_end (l);
  }
  else {
    l->head = bl_alnls_u8_it_end (l);
  }
  --l->size;
  return tail;
}
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
