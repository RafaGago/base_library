#include <bl/base/flat_deadlines.h>
#include <bl/base/deadline.h>

/*--------------------------------------------------------------------------*/
static inline bl_timept get_fdbl_timept (void const* v)
{
  return *((bl_timept*) v);
}
/*--------------------------------------------------------------------------*/
static bl_word bl_flat_deadlines_ordering_func(
  void const* a, void const* b, void* context
  )
{
  bl_flat_deadlines* l = (bl_flat_deadlines*) context;
  return bl_deadline_compare(
    get_fdbl_timept (a) - l->time_offset, get_fdbl_timept (b) - l->time_offset
    );
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void const* bl_flat_deadlines_get_head_if_expired(
  bl_flat_deadlines* dl,
  bl_uword           elem_size,
  bool               dont_acquire_new_timept,
  bl_timept          now
  )
{
  bl_timept const* d = (bl_timept*) bl_flat_deadlines_get_head (dl, elem_size);

  if (!d) {
    return nullptr;
  }
  if (bl_unlikely (bl_deadline_expired_explicit (*d, dl->time_offset))) {
    return (void*) d;
  }
  /* as the list ordering rotates to allow timepoint wrap around (and O(1) */
  /* lookup for the next expired candidate) it is wrong to move the rotation */
  /*   offset while still having outdated items on the list */
  dl->time_offset = (dont_acquire_new_timept) ? now : bl_timept_get();
  return (void*)
    bl_deadline_expired_explicit (*d, dl->time_offset) ? d : nullptr;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_flat_deadlines_insert(
  bl_flat_deadlines* dl, bl_uword elem_size, void const* e
  )
{
  if (bl_unlikely (!bl_ringb_can_insert (&dl->list))) {
    return bl_mkerr (bl_would_overflow);
  }
  bl_oringb_insert_fifo_duplicates(
    &dl->list, elem_size, bl_flat_deadlines_ordering_func, e, dl
    );
  return bl_mkok();
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bool bl_flat_deadlines_try_get_and_drop(
  bl_flat_deadlines*           dl,
  bl_uword                     elem_size,
  flat_dealines_value_cmp_func cmpf,
  void*                        dst,
  void const*                  find
  )
{
  bl_uword idx = bl_oringb_find(
    &dl->list, elem_size, bl_flat_deadlines_ordering_func, find, dl
    );
  bl_uword size = bl_ringb_size (&dl->list);
  if (idx >= size) {
    return false;
  }
  void* f;
  if (!cmpf) {
    f = bl_ringb_at (&dl->list, elem_size, idx);
    goto success;
  }
  do {
    /*duplicate linear check*/
    f = bl_ringb_at (&dl->list, elem_size, idx);
    if (bl_likely (cmpf (find, f) == 0)) {
      goto success;
    }
    ++idx;
  }
  while (idx < size && get_fdbl_timept (f) == get_fdbl_timept (find));
  return false;
success:
  memcpy (dst, f, elem_size);
  bl_ringb_drop (&dl->list, elem_size, idx);
  return true;
}
/*--------------------------------------------------------------------------*/
