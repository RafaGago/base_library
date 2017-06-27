#include <bl/base/ringbuffer.h>


/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err ringb_init_extern (ringb* rb, void* mem, uword capacity)
{
  bl_assert (rb && mem);
  bl_assert (is_pow2 (capacity));

  if (!is_pow2 (capacity)) {
    return bl_invalid;
  }
  rb->mask  = capacity - 1;
  rb->size  = 0;
  rb->begin = 0;
  rb->stor  = (u8*) mem;
  return bl_ok;
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err ringb_init(
  ringb* rb, uword elem_size, uword capacity, alloc_tbl const* alloc
  )
{
  bl_err err = ringb_init_extern (rb, (void*) 1, capacity);
  if (err) {
    return err;
  }
  rb->stor = (u8*) bl_alloc (alloc, capacity * elem_size);
  return rb->stor ? bl_ok : bl_alloc;
}
/*--------------------------------------------------------------------------*/
