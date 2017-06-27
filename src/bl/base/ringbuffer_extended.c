#include <string.h>

#include <bl/base/ringbuffer.h>

/*--------------------------------------------------------------------------*/
static void ringb_move_forward(
  ringb* rb, uword elem_size, uword start_elem, uword end_elem
  )
{
  u8* start = ringb_at (rb, elem_size, start_elem);
  u8* end   = ringb_at (rb, elem_size, end_elem);

  if (start < end) {
    memmove (start + elem_size, start, end - start);
  }
  else if (end < start) {
    u8* bstart   = rb->stor;
    u8* bend     = bstart + (ringb_capacity (rb) * elem_size);
    memmove (bstart + elem_size, bstart, end - bstart);
    memcpy (bstart, bend - elem_size, elem_size);
    memmove (start + elem_size, start, bend - start - elem_size);
  }
}
/*--------------------------------------------------------------------------*/
static void ringb_move_backward(
  ringb* rb, uword elem_size, uword start_elem, uword end_elem
  )
{
  u8* start = ringb_at (rb, elem_size, start_elem);
  u8* end   = ringb_at (rb, elem_size, end_elem);

  if (start < end) {
    memmove (start, start + elem_size, end - start);
  }
  else if (end < start) {
    u8* bstart   = rb->stor;
    u8* bend     = bstart + (ringb_capacity (rb) * elem_size);
    memmove (start, start + elem_size, bend - start - elem_size);
    memcpy (bend - elem_size, bstart, elem_size);
    memmove (bstart, bstart + elem_size, end - bstart);
  }
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void ringb_insert(
  ringb* rb, uword elem_size, void const* v, uword idx
  )
{
  bl_assert (rb && v);
  bl_assert (ringb_can_insert (rb));
  bl_assert (idx <= ringb_size (rb));

  ++rb->size;
  if (idx < (ringb_size (rb) / 2)) {
    /*head backwards*/
    --rb->begin;
    rb->begin &= rb->mask;
    ringb_move_backward (rb, elem_size, 0, idx);
  }
  else {
    ringb_move_forward (rb, elem_size, idx, ringb_size (rb) - 1);
  }
  memcpy (ringb_at (rb, elem_size, idx), v, elem_size);
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void ringb_drop (ringb* rb, uword elem_size, uword idx)
{
  bl_assert (rb);
  bl_assert (idx < ringb_size (rb));

  if (idx <= (ringb_size (rb) / 2)) {
    ringb_move_forward (rb, elem_size, 0, idx);
    ++rb->begin;
    rb->begin &= rb->mask;
  }
  else {
    ringb_move_backward (rb, elem_size, idx, ringb_size (rb) - 1);
  }
  --rb->size;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void ringb_drop_range(
  ringb* rb, uword elem_size, uword from_idx, uword element_count
  )
{
  bl_assert (rb);
  bl_assert ((from_idx + element_count) <= ringb_size (rb));

  uword src;
  uword dst;
  uword count;
  uword begin;
  uword tail_count = ringb_size (rb) - from_idx - element_count;

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
    /*TODO: fix with shifts*/
    memcpy(
      ringb_at (rb, elem_size, dst), ringb_at (rb, elem_size, src), elem_size
      );
    ++src;
    ++dst;
  }
  rb->size -= element_count;
  rb->begin = begin;
}
/*--------------------------------------------------------------------------*/