#include <assert.h>

#include <bl/lib/task_queue/delayed.h>

#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/time.h>
#include <bl/hdr/base/integer_math.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
static inline word delayed_ordering_func(
  void const* a, void const* b, void* context
  )
{
  delayed* l = (delayed*) context;
  return deadline_compare(
    ((delayed_entry*) a)->deadl - l->time_offset,
    ((delayed_entry*) b)->deadl - l->time_offset
    );
}
/*---------------------------------------------------------------------------*/
declare_oringb_funcs (delayed_ringb, delayed_entry, static inline)
define_oringb_funcs(
  delayed_ringb, delayed_entry, delayed_ordering_func, static inline
  )
/*---------------------------------------------------------------------------*/
delayed_entry const* delayed_get_head (delayed* dl)
{
  return delayed_ringb_size (&dl->list) > 0 ? 
    delayed_ringb_at_head (&dl->list) : nullptr;
}
/*---------------------------------------------------------------------------*/
delayed_entry const* delayed_get_head_if_expired (delayed* dl)
{
  delayed_entry const* d = delayed_get_head (dl);
  if (!d) {
    return nullptr;
  }
  if (unlikely (deadline_expired_explicit (d->deadl, dl->time_offset))) {
    return d;
  }
  /* as the list ordering rotates to allow timestamp wrap around (and O(1) 
     lookup for the next expired candidate) it is wrong to move the rotation
     offset while still having outdated items on the list */
  dl->time_offset = bl_get_tstamp();
  return deadline_expired_explicit (d->deadl, dl->time_offset) ? d : nullptr;
}
/*---------------------------------------------------------------------------*/
void delayed_drop_head (delayed* dl)
{
  bl_assert (dl);
  bl_assert (delayed_ringb_size (&dl->list) > 0);
  delayed_ringb_drop_head (&dl->list);
}
/*---------------------------------------------------------------------------*/
bl_err delayed_insert (delayed* dl, delayed_entry* e)
{
  if (unlikely (!delayed_ringb_can_insert (&dl->list))) {
    return bl_would_overflow;
  }
  delayed_ringb_insert_fifo_duplicates (&dl->list, e, dl);
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
bool delayed_try_get_task_and_drop(
  delayed* dl, taskq_task* dst, tstamp d, u32 id
  )
{
  delayed_entry find;
  find.deadl = d;
  uword idx  = delayed_ringb_find (&dl->list, &find, dl);
  uword size = delayed_ringb_size (&dl->list);
  if (idx >= size) {
    return false;
  }
  delayed_entry* f;
  do {
    /*duplicate timestamp check*/
    f = delayed_ringb_at (&dl->list, idx);    
    if (likely (id == f->id)) {
      *dst = f->task;
      delayed_ringb_drop (&dl->list, idx);
      return true;
    }
    ++idx;
  } 
  while(idx < size && f->deadl == d);
  return false;
}
/*---------------------------------------------------------------------------*/
bl_err delayed_init(
  delayed* dl, alloc_tbl const* alloc, uword capacity
  )
{
  bl_assert (dl);
  dl->time_offset = bl_get_tstamp();
  capacity        = round_next_pow2_u (capacity);
  return delayed_ringb_init (&dl->list, alloc, capacity);
}
/*---------------------------------------------------------------------------*/
void delayed_destroy (delayed* dl, alloc_tbl const* alloc)
{
  bl_assert (dl);
  delayed_ringb_destroy (&dl->list, alloc);
}
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
