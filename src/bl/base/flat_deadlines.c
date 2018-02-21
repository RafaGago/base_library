#include <bl/base/flat_deadlines.h>
#include <bl/base/deadline.h>

/*--------------------------------------------------------------------------*/
static inline tstamp get_fdtstamp (void const* v)
{
  return *((tstamp*) v);
}
/*--------------------------------------------------------------------------*/
static word flat_deadlines_ordering_func(
  void const* a, void const* b, void* context
  )
{
  flat_deadlines* l = (flat_deadlines*) context;
  return deadline_compare(
    get_fdtstamp (a) - l->time_offset, get_fdtstamp (b) - l->time_offset
    );
}
/*--------------------------------------------------------------------------*/
BL_EXPORT void const* flat_deadlines_get_head_if_expired(
  flat_deadlines* dl, uword elem_size, bool dont_acquire_new_tstamp, tstamp now
  )
{
  tstamp const* d = (tstamp*) flat_deadlines_get_head (dl, elem_size);

  if (!d) {
    return nullptr;
  }
  if (unlikely (deadline_expired_explicit (*d, dl->time_offset))) {
    return (void*) d;
  }
  /* as the list ordering rotates to allow timestamp wrap around (and O(1) */
  /* lookup for the next expired candidate) it is wrong to move the rotation */
  /*   offset while still having outdated items on the list */
  dl->time_offset = (dont_acquire_new_tstamp) ? now : bl_get_tstamp();
  return (void*) deadline_expired_explicit (*d, dl->time_offset) ? d : nullptr;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err flat_deadlines_insert(
  flat_deadlines* dl, uword elem_size, void const* e
  )
{
  if (unlikely (!ringb_can_insert (&dl->list))) {
    return bl_mkerr (bl_would_overflow);
  }
  oringb_insert_fifo_duplicates(
    &dl->list, elem_size, flat_deadlines_ordering_func, e, dl
    );
  return bl_mkok();
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bool flat_deadlines_try_get_and_drop(
  flat_deadlines*              dl,
  uword                        elem_size,
  flat_dealines_value_cmp_func cmpf,
  void*                        dst,
  void const*                  find
  )
{
  uword idx = oringb_find(
    &dl->list, elem_size, flat_deadlines_ordering_func, find, dl
    );
  uword size = ringb_size (&dl->list);
  if (idx >= size) {
    return false;
  }
  void* f;
  if (!cmpf) {
    f = ringb_at (&dl->list, elem_size, idx);
    goto success;
  }
  do {
    /*duplicate linear check*/
    f = ringb_at (&dl->list, elem_size, idx);
    if (likely (cmpf (find, f) == 0)) {
      goto success;
    }
    ++idx;
  }
  while (idx < size && get_fdtstamp (f) == get_fdtstamp (find));
  return false;
success:
  memcpy (dst, f, elem_size);
  ringb_drop (&dl->list, elem_size, idx);
  return true;
}
/*--------------------------------------------------------------------------*/
