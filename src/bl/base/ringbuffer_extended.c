#include <string.h>

#include <bl/base/ringbuffer.h>

/*--------------------------------------------------------------------------*/
static void bl_ringb_move_forward(
  bl_ringb* rb, bl_uword elem_size, bl_uword start_elem, bl_uword end_elem
  )
{
  bl_u8* start = (bl_u8*) bl_ringb_at (rb, elem_size, start_elem);
  bl_u8* end   = (bl_u8*) bl_ringb_at (rb, elem_size, end_elem);

  if (start < end) {
    memmove (start + elem_size, start, end - start);
  }
  else if (end < start) {
    bl_u8* bstart   = rb->stor;
    bl_u8* bend     = bstart + (bl_ringb_capacity (rb) * elem_size);
    memmove (bstart + elem_size, bstart, end - bstart);
    memcpy (bstart, bend - elem_size, elem_size);
    memmove (start + elem_size, start, bend - start - elem_size);
  }
}
/*--------------------------------------------------------------------------*/
static void bl_ringb_move_backward(
  bl_ringb* rb, bl_uword elem_size, bl_uword start_elem, bl_uword end_elem
  )
{
  bl_u8* start = (bl_u8*) bl_ringb_at (rb, elem_size, start_elem);
  bl_u8* end   = (bl_u8*) bl_ringb_at (rb, elem_size, end_elem);

  if (start < end) {
    memmove (start, start + elem_size, end - start);
  }
  else if (end < start) {
    bl_u8* bstart   = rb->stor;
    bl_u8* bend     = bstart + (bl_ringb_capacity (rb) * elem_size);
    memmove (start, start + elem_size, bend - start - elem_size);
    memcpy (bend - elem_size, bstart, elem_size);
    memmove (bstart, bstart + elem_size, end - bstart);
  }
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void bl_ringb_insert(
  bl_ringb* rb, bl_uword elem_size, void const* v, bl_uword idx
  )
{
  bl_assert (rb && v);
  bl_assert (bl_ringb_can_insert (rb));
  bl_assert (idx <= bl_ringb_size (rb));

  ++rb->size;
  if (idx < (bl_ringb_size (rb) / 2)) {
    /*head backwards*/
    --rb->begin;
    rb->begin &= rb->mask;
    bl_ringb_move_backward (rb, elem_size, 0, idx);
  }
  else {
    bl_ringb_move_forward (rb, elem_size, idx, bl_ringb_size (rb) - 1);
  }
  memcpy (bl_ringb_at (rb, elem_size, idx), v, elem_size);
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void bl_ringb_drop (bl_ringb* rb, bl_uword elem_size, bl_uword idx)
{
  bl_assert (rb);
  bl_assert (idx < bl_ringb_size (rb));

  if (idx <= (bl_ringb_size (rb) / 2)) {
    bl_ringb_move_forward (rb, elem_size, 0, idx);
    ++rb->begin;
    rb->begin &= rb->mask;
  }
  else {
    bl_ringb_move_backward (rb, elem_size, idx, bl_ringb_size (rb) - 1);
  }
  --rb->size;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void bl_ringb_drop_range(
  bl_ringb* rb, bl_uword elem_size, bl_uword from_idx, bl_uword element_count
  )
{
  bl_assert (rb);
  bl_assert ((from_idx + element_count) <= bl_ringb_size (rb));

  bl_uword src;
  bl_uword dst;
  bl_uword count;
  bl_uword begin;
  bl_uword tail_count = bl_ringb_size (rb) - from_idx - element_count;

  if (from_idx >= tail_count) {
    /*tail has less elements to move*/
    src   = from_idx + element_count;
    dst   = from_idx;
    count = tail_count;
    begin = rb->begin;
  }
  else {
    /*head has less elements to move*/
    src   = 0;
    dst   = element_count;
    count = from_idx;
    begin = dst;
  }
  while (count--) {
    /*TODO: improve with shifts*/
    memcpy (bl_ringb_at(
      rb, elem_size, dst), bl_ringb_at (rb, elem_size, src), elem_size
      );
    ++src;
    ++dst;
  }
  rb->size -= element_count;
  rb->begin = begin;
}
/*--------------------------------------------------------------------------*/