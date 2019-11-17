#include <bl/base/ringbuffer.h>


/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err
  bl_ringb_init_extern (bl_ringb* rb, void* mem, bl_uword capacity)
{
  bl_assert (rb && mem);
  bl_assert (bl_is_pow2 (capacity));

  if (!bl_is_pow2 (capacity)) {
    return bl_mkerr (bl_invalid);
  }
  rb->mask  = capacity - 1;
  rb->size  = 0;
  rb->begin = 0;
  rb->stor  = (bl_u8*) mem;
  return bl_mkok();
}
/*--------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_ringb_init(
  bl_ringb* rb, bl_uword elem_size, bl_uword capacity, bl_alloc_tbl const* alloc
  )
{
  bl_err err = bl_ringb_init_extern (rb, (void*) 1, capacity);
  if (err.own) {
    return err;
  }
  rb->stor = (bl_u8*) bl_alloc (alloc, capacity * elem_size);
  return bl_mkerr (rb->stor ? bl_ok : bl_alloc);
}
/*--------------------------------------------------------------------------*/
