#include <bl/base/ordered_ringbuffer.h>

/*--------------------------------------------------------------------------*/
BL_EXPORT bl_uword bl_oringb_find_eq_or_gt(
  bl_oringb*    rb,
  bl_uword      elem_size,
  bl_oringb_cmp cmp_func,
  void const*   v,
  void*         context
  )
{
  bl_assert (rb && v && cmp_func);
  if (bl_ringb_size (rb) == 0) {
    return 0;
  }
  /*test head (avoid trashing cache (touching a lot of memory) on fast-path)*/
  bl_uword idx = 0;
  bl_word  res  = cmp_func (v, bl_ringb_at (rb, elem_size, idx), context);
  if (res <= 0) {
    return idx;
  }
  /*test tail (avoid trashing cache (touching a lot of memory) on fast-path)*/
  idx = bl_ringb_size (rb) - 1;
  res = cmp_func (v, bl_ringb_at (rb, elem_size, idx), context);
  if (res > 0) {
    return bl_ringb_size (rb);
  }
  else if (bl_unlikely (res == 0)) {
    /*linearly go backwards to find the first occurence*/
    do {
      --idx;
      res = cmp_func (v, bl_ringb_at (rb, elem_size, idx), context);
    }
    while (res == 0);
    return idx + 1;
  }
  /*binary search*/
  bl_uword head = 1;
  bl_uword tail = bl_ringb_size (rb) - 1;

  while (head < tail) {
    bl_uword idx = head + ((tail - head) / 2);
    bl_word  res = cmp_func (v, bl_ringb_at (rb, elem_size, idx), context);
    if (res > 0) {
      head = idx + 1;
    }
    else {
      tail = idx;
    }
  }
  return cmp_func (v, bl_ringb_at (rb, elem_size, head), context) == 0 ?
    head : /*match. if duplicate will have the lowest duplicate idx (LIFO)*/
    tail;  /*no match: tail is the first value greater than "v" */
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_oringb_insert_result bl_oringb_insert(
  bl_oringb*    rb,
  bl_uword      elem_size,
  bl_oringb_cmp cmp_func,
  void const*   v,
  void*         context
  )
{
  bl_assert (rb && v);
  bl_assert (bl_ringb_can_insert (rb));
  bl_oringb_insert_result r;
  r.idx = bl_oringb_find_eq_or_gt (rb, elem_size, cmp_func, v, context);
  r.new_insertion = r.idx == bl_ringb_size (rb) ||
    cmp_func (v, bl_ringb_at (rb, elem_size, r.idx), context) != 0;
  if (r.new_insertion) {
    bl_ringb_insert (rb, elem_size, v, r.idx);
  }
  return r;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_uword bl_oringb_insert_lifo_duplicates(
  bl_oringb*    rb,
  bl_uword      elem_size,
  bl_oringb_cmp cmp_func,
  void const*   v,
  void*         context
  )
{
  bl_assert (rb && v);
  bl_assert (bl_ringb_can_insert (rb));
  bl_uword idx = bl_oringb_find_eq_or_gt (rb, elem_size, cmp_func, v, context);
  bl_ringb_insert (rb, elem_size, v, idx);
  return idx;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_uword bl_oringb_insert_fifo_duplicates(
  bl_oringb*    rb,
  bl_uword      elem_size,
  bl_oringb_cmp cmp_func,
  void const*   v,
  void*         context
  )
{
  bl_assert (rb && v);
  bl_assert (bl_ringb_can_insert (rb));
  bl_uword idx = bl_oringb_find_eq_or_gt (rb, elem_size, cmp_func, v, context);
  /*linear advance to one position after the last duplicate*/
  while(
    idx < bl_ringb_size (rb) &&
    cmp_func (v, bl_ringb_at (rb, elem_size, idx), context) == 0
    ) {
    ++idx;
  }
  bl_ringb_insert (rb, elem_size, v, idx);
  return idx;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_uword bl_oringb_find(
  bl_oringb*    rb,
  bl_uword      elem_size,
  bl_oringb_cmp cmp_func,
  void const*   f,
  void*         context
  )
{
  bl_assert (rb && f);
  bl_uword pos  = bl_oringb_find_eq_or_gt (rb, elem_size, cmp_func, f, context);
  bl_uword size = bl_ringb_size (rb);
  if (size > 0 && pos < size) {
    pos = cmp_func (f, bl_ringb_at (rb, elem_size, pos), context) == 0 ?
     pos : size;
  }
  return pos;
}
/*---------------------------------------------------------------------------*/