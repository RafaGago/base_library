/*---------------------------------------------------------------------------*/
#include <bl/nonblock/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/cache.h>
#include <bl/base/alignment.h>
#include <bl/base/allocator.h>
#include <bl/base/error.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/utility.h>
#include <bl/base/atomic.h>
#include <bl/nonblock/mpmc_b_common_priv.h>
#include <bl/nonblock/mpmc_b.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
enum bl_mpmc_b_flags {
  block_producers = 1 << 0,
};
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_init(
  bl_mpmc_b*          q,
  bl_alloc_tbl const* alloc,
  bl_u32              slot_count,
  bl_u32              data_size,
  bl_u32              data_alignment
  )
{
  slot_count    = bl_round_next_pow2_u (slot_count);
  bl_u32 slot_size = bl_mpmc_b_compute_slot_size (data_size, data_alignment);
  bl_u32 offset    = slot_size - data_size;
  slot_size     = bl_mpmc_b_round_slot_size (slot_size, data_alignment);
  if (slot_count < 2 || slot_count > bl_mpmc_b_max_slots) {
    return bl_mkerr (bl_invalid);
  }
  q->buffer = (bl_u8*) bl_alloc (alloc, slot_count * slot_size);
  if (!q->buffer) {
    return bl_mkerr (bl_alloc);
  }
  q->slot_count  = slot_count;
  q->slot_size   = slot_size;
  q->data_offset = offset;
  bl_u32 v = 0;
  bl_u8* ptr = q->buffer;
  while (v< slot_count) {
    bl_atomic_u32_store_rlx ((bl_atomic_u32*) ptr, v);
    ++v;
    ptr += slot_size;
  }
  bl_atomic_u32_store_rlx (&q->push_slot, 0);
  bl_atomic_u32_store_rlx (&q->pop_slot, 0);
  bl_atomic_u32_store_rlx (&q->flags, 0);
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_b_destroy (bl_mpmc_b* q, bl_alloc_tbl const* alloc)
{
  if (q->buffer) {
    bl_dealloc (alloc, q->buffer);
  }
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_b_block_producers (bl_mpmc_b* q)
{
  bl_u32 flags = bl_atomic_u32_load_rlx (&q->flags);
  do {
    if ((flags & block_producers) == block_producers) {
      return;
    }
  }
  while (!bl_atomic_u32_weak_cas_rlx (&q->flags, &flags, flags | block_producers));
  /*TODO signals trashed*/
  bl_atomic_u32_fetch_add_rlx (&q->push_slot, 2 * q->slot_count);
}
/*----------------------------------------------------------------------------*/
static bl_err bl_mpmc_b_prepare_m_check_ver(
  bl_mpmc_b*   q,
  bl_mpmc_b_op op,
  bl_u8**      data,
  bl_u32       op_cmp_offset,
  bl_err    full_or_empty
  )
{
  bl_mpmc_b_op ver;
  bl_u8* slot   = slot_addr (q->buffer, q->slot_count, q->slot_size, op);
  ver        = bl_atomic_u32_load ((bl_atomic_u32*) slot, bl_mo_acquire);
  op        += op_cmp_offset;
  bl_i32 status = (bl_i32)
    (bl_mpmc_b_ticket_decode (ver) - bl_mpmc_b_ticket_decode (op));
  if (status == 0) {
    *data  = slot + q->data_offset;
    return bl_mkok();
  }
  if (status > 0) {
    return bl_mkerr (bl_busy);
  }
  return (status > -(2 * q->slot_count)) ?
    full_or_empty : bl_mkerr (bl_locked);
}
/*----------------------------------------------------------------------------*/
static bl_err bl_mpmc_b_prepare_sig_fallback_m(
  bl_mpmc_b*     q,
  bl_mpmc_b_op*  op,
  bl_u8**        data,
  bool        replace_sig,
  bl_mpmc_b_sig  sig,
  bl_mpmc_b_sig  sig_fmask,
  bl_mpmc_b_sig  sig_fmatch,
  bl_atomic_u32* op_var,
  bl_u32         op_cmp_offset,
  bl_err      nodata_error
  )
{
  bl_assert (q && q->buffer && op && data);
  bl_mpmc_b_op now = bl_atomic_u32_load_rlx (op_var);
  while (1) {
    bl_mpmc_b_sig signow = bl_mpmc_b_sig_decode (now);
    if (bl_unlikely ((signow & sig_fmask) == sig_fmatch)) {
      *op = now;
      return bl_mkerr (bl_preconditions);
    }
    bl_err err = bl_mpmc_b_prepare_m_check_ver(
      q, now, data, op_cmp_offset, nodata_error
      );
    if (err.bl == bl_ok) {
      bl_mpmc_b_op next =
        bl_mpmc_b_op_encode (now + 1, replace_sig ? sig : signow);
      if (bl_atomic_u32_weak_cas_rlx (op_var, &now, next)) {
        *op = now;
        return err;
      }
    }
    else if (err.bl == bl_busy){
      now = bl_atomic_u32_load_rlx (op_var);
    }
    else {
      *op   = now;
      *data = nullptr;
      return err;
    }
  }
}
/*----------------------------------------------------------------------------*/
static bl_err bl_mpmc_b_prepare_s(
  bl_mpmc_b*     q,
  bl_mpmc_b_op*  op,
  bl_u8**        data,
  bl_atomic_u32* op_var,
  bl_u32         op_cmp_offset,
  bl_err      nodata_error
  )
{
  bl_assert (q && q->buffer && op && data);
  bl_mpmc_b_op now = bl_atomic_u32_load_rlx (op_var);
  bl_err err = bl_mpmc_b_prepare_m_check_ver(
    q, now, data, op_cmp_offset, nodata_error
    );
  if (err.bl == bl_ok) {
    *op = now;
    bl_atomic_u32_store_rlx (op_var, bl_mpmc_b_op_encode (now + 1, 0));
    return err;
  }
  bl_assert (err.bl == nodata_error.bl);
  return err;
}
/*----------------------------------------------------------------------------*/
static void bl_mpmc_b_commit (bl_mpmc_b* q, bl_mpmc_b_op op, bl_u32 increment)
{
  bl_u8* slot = slot_addr (q->buffer, q->slot_count, q->slot_size, op);
  op += increment;
  bl_atomic_u32_store(
    (bl_atomic_u32*) slot, bl_mpmc_b_op_encode (op, 0), bl_mo_release
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void
  bl_mpmc_b_fifo_produce_prepare (bl_mpmc_b* q, bl_mpmc_b_op* op)
{
  *op = bl_mpmc_b_op_encode (bl_atomic_u32_fetch_add_rlx (&q->push_slot, 1), 0);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_fifo_produce_prepare_is_ready(
  bl_mpmc_b* q, bl_mpmc_b_op op, bl_u8** data
  )
{
  bl_err err = bl_mpmc_b_prepare_m_check_ver(
    q, op, data, 0, bl_mkerr (bl_would_overflow)
    );
  if (bl_likely (err.bl != bl_would_overflow)) {
    return err;
  }
  /* if the counter has advanced more than the slot count either:
     - block has been called.
     - the user is using more threads than slots on
       "bl_mpmc_b_fifo_produce_prepare" => user bug */
  bl_mpmc_b_op now = bl_atomic_u32_load_rlx (&q->push_slot);
  bl_i32 status = (bl_i32)
    (bl_mpmc_b_ticket_decode (now) - bl_mpmc_b_ticket_decode (op));
  return bl_mkerr (status < q->slot_count ? bl_would_overflow : bl_locked);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_produce_prepare_sig_fallback(
  bl_mpmc_b*    q,
  bl_mpmc_b_op* op,
  bl_u8**       data,
  bool          replace_sig,
  bl_mpmc_b_sig sig,
  bl_mpmc_b_sig sig_fmask,
  bl_mpmc_b_sig sig_fmatch
  )
{
  return bl_mpmc_b_prepare_sig_fallback_m(
    q,
    op,
    data,
    replace_sig,
    sig,
    sig_fmask,
    sig_fmatch,
    &q->push_slot,
    0,
    bl_mkerr (bl_would_overflow)
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_produce_prepare_sp(
    bl_mpmc_b* q, bl_mpmc_b_op* op, bl_u8** data
    )
{
  return bl_mpmc_b_prepare_s(
    q, op, data, &q->push_slot, 0, bl_mkerr (bl_would_overflow)
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_b_produce_commit (bl_mpmc_b* q, bl_mpmc_b_op op)
{
  bl_mpmc_b_commit (q, op, 1);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_consume_prepare_sig_fallback(
  bl_mpmc_b*    q,
  bl_mpmc_b_op* op,
  bl_u8**       data,
  bool          replace_sig,
  bl_mpmc_b_sig sig,
  bl_mpmc_b_sig sig_fmask,
  bl_mpmc_b_sig sig_fmatch
  )
{
  return bl_mpmc_b_prepare_sig_fallback_m(
    q,
    op,
    data,
    replace_sig,
    sig,
    sig_fmask,
    sig_fmatch,
    &q->pop_slot,
    1,
    bl_mkerr (bl_empty)
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_consume_prepare_sc(
  bl_mpmc_b* q, bl_mpmc_b_op* op, bl_u8** data
  )
{
  return bl_mpmc_b_prepare_s (q, op, data, &q->pop_slot, 1, bl_mkerr (bl_empty));
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_b_consume_commit (bl_mpmc_b *q, bl_mpmc_b_op op)
{
  bl_mpmc_b_commit (q, op, q->slot_count);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_producer_signal_try_set(
  bl_mpmc_b* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_producer_signal_try_set_tmatch(
  bl_mpmc_b* q, bl_mpmc_b_op* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set_tmatch (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_consumer_signal_try_set(
  bl_mpmc_b* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_b_consumer_signal_try_set_tmatch(
  bl_mpmc_b* q, bl_mpmc_b_op* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set_tmatch (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif
