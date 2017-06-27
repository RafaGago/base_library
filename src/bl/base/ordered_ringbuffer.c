#include <bl/base/ordered_ringbuffer.h>

/*--------------------------------------------------------------------------*/
BL_EXPORT uword oringb_find_eq_or_gt(
  oringb* rb, uword elem_size, oringb_cmp cmp_func, void const* v, void* context
  )
{
  bl_assert (rb && v && cmp_func);
  if (ringb_size (rb) == 0) {
    return 0;
  }
  /*test head (avoid trashing cache (touching a lot of memory) on fast-path)*/
  uword idx = 0;
  word  res  = cmp_func (v, ringb_at (rb, elem_size, idx), context);
  if (res <= 0) {
    return idx;
  }
  /*test tail (avoid trashing cache (touching a lot of memory) on fast-path)*/
  idx = ringb_size (rb) - 1;
  res = cmp_func (v, ringb_at (rb, elem_size, idx), context);
  if (res > 0) {
    return ringb_size (rb);
  }
  else if (unlikely (res == 0)) {
    /*linearly go backwards to find the first occurence*/
    do {
      --idx;
      res = cmp_func (v, ringb_at (rb, elem_size, idx), context);
    }
    while (res == 0);
    return idx + 1;
  }
  /*binary search*/
  uword head = 1;
  uword tail = ringb_size (rb) - 1;

  while (head < tail) {
    uword idx = head + ((tail - head) / 2);
    word  res = cmp_func (v, ringb_at (rb, elem_size, idx), context);
    if (res > 0) {
      head = idx + 1;
    }
    else {
      tail = idx;
    }
  }
  return cmp_func (v, ringb_at (rb, elem_size, head), context) == 0 ?
    head : /*match. if duplicate will have the lowest duplicate idx (LIFO)*/
    tail;  /*no match: tail is the first value greater than "v" */
}
/*--------------------------------------------------------------------------*/
BL_EXPORT oringb_insert_result oringb_insert(
  oringb* rb, uword elem_size, oringb_cmp cmp_func, void const* v, void* context
  )
{
  bl_assert (rb && v);
  bl_assert (ringb_can_insert (rb));
  oringb_insert_result r;
  r.idx = oringb_find_eq_or_gt (rb, elem_size, cmp_func, v, context);
  r.new_insertion = r.idx == ringb_size (rb) ||
    cmp_func (v, ringb_at (rb, elem_size, r.idx), context) != 0;
  if (r.new_insertion) {
    ringb_insert (rb, elem_size, v, r.idx);
  }
  return r;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT uword oringb_insert_lifo_duplicates(
  oringb* rb, uword elem_size, oringb_cmp cmp_func, void const* v, void* context
  )
{
  bl_assert (rb && v);
  bl_assert (ringb_can_insert (rb));
  uword idx = oringb_find_eq_or_gt (rb, elem_size, cmp_func, v, context);
  ringb_insert (rb, elem_size, v, idx);
  return idx;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT uword oringb_insert_fifo_duplicates(
  oringb* rb, uword elem_size, oringb_cmp cmp_func, void const* v, void* context
  )
{
  bl_assert (rb && v);
  bl_assert (ringb_can_insert (rb));
  uword idx = oringb_find_eq_or_gt (rb, elem_size, cmp_func, v, context);
  /*linear advance to one position after the last duplicate*/
  while(
    idx < ringb_size (rb) &&
    cmp_func (v, ringb_at (rb, elem_size, idx), context) == 0
    ) {
    ++idx;
  }
  ringb_insert (rb, elem_size, v, idx);
  return idx;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT uword oringb_find(
  oringb* rb, uword elem_size, oringb_cmp cmp_func, void const* f, void* context
  )
{
  bl_assert (rb && f);
  uword pos  = oringb_find_eq_or_gt (rb, elem_size, cmp_func, f, context);
  uword size = ringb_size (rb);
  if (size > 0 && pos < size) {
    pos = cmp_func (f, ringb_at (rb, elem_size, pos), context) == 0 ?
     pos : size;
  }
  return pos;
}
/*---------------------------------------------------------------------------*/