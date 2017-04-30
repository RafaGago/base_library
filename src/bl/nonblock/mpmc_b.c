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
enum mpmc_b_flags {
  block_producers = 1 << 0,
};
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_init(
  mpmc_b*          q,
  alloc_tbl const* alloc,
  u32              slot_count,
  u32              data_size,
  u32              data_alignment
  )
{
  slot_count    = round_next_pow2_u (slot_count);
  u32 slot_size = mpmc_b_compute_slot_size (data_size, data_alignment);
  u32 offset    = slot_size - data_size;
  slot_size     = mpmc_b_round_slot_size (slot_size, data_alignment);
  if (slot_count < 2 || slot_count > mpmc_b_max_slots) {
    return bl_invalid;
  }
  q->buffer = (u8*) bl_alloc (alloc, slot_count * slot_size);
  if (!q->buffer) {
    return bl_alloc;
  }
  q->slot_count  = slot_count;
  q->slot_size   = slot_size;
  q->data_offset = offset;
  u32 v = 0;
  u8* ptr = q->buffer;
  while (v< slot_count) {
    atomic_u32_store_rlx ((atomic_u32*) ptr, v);
    ++v;
    ptr += slot_size;
  }
  atomic_u32_store_rlx (&q->push_slot, 0);
  atomic_u32_store_rlx (&q->pop_slot, 0);
  atomic_u32_store_rlx (&q->flags, 0);
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_b_destroy (mpmc_b* q, alloc_tbl const* alloc)
{
  if (q->buffer) {
    bl_dealloc (alloc, q->buffer);
  }
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_b_block_producers (mpmc_b* q)
{
  u32 flags = atomic_u32_load_rlx (&q->flags);
  do {
    if ((flags & block_producers) == block_producers) {
      return;
    }
  }
  while (!atomic_u32_weak_cas_rlx (&q->flags, &flags, flags | block_producers));
  /*TODO signals trashed*/
  atomic_u32_fetch_add_rlx (&q->push_slot, 2 * q->slot_count);
}
/*----------------------------------------------------------------------------*/
static bl_err mpmc_b_prepare_m_check_ver(
  mpmc_b*   q,
  mpmc_b_op op,
  u8**      data,
  u32       op_cmp_offset,
  bl_err    full_or_empty
  )
{
  mpmc_b_op ver;
  u8* slot   = slot_addr (q->buffer, q->slot_count, q->slot_size, op);
  ver        = atomic_u32_load ((atomic_u32*) slot, mo_acquire);
  op        += op_cmp_offset;
  i32 status = (i32) (mpmc_b_ticket_decode (ver) - mpmc_b_ticket_decode (op));
  if (status == 0) {
    *data  = slot + q->data_offset;
    return bl_ok;
  }
  if (status > 0) {
    return bl_busy;
  }
  return (status > -(2 * q->slot_count)) ? full_or_empty : bl_locked;
}
/*----------------------------------------------------------------------------*/
static bl_err mpmc_b_prepare_sig_fallback_m(
  mpmc_b*     q,
  mpmc_b_op*  op,
  u8**        data,
  bool        replace_sig,
  mpmc_b_sig  sig,
  mpmc_b_sig  sig_fmask,
  mpmc_b_sig  sig_fmatch,
  atomic_u32* op_var,
  u32         op_cmp_offset,
  bl_err      nodata_error
  )
{
  bl_assert (q && q->buffer && op && data);
  mpmc_b_op now = atomic_u32_load_rlx (op_var);
  while (1) {
    mpmc_b_sig signow = mpmc_b_sig_decode (now);
    if (unlikely ((signow & sig_fmask) == sig_fmatch)) {
      *op = now;
      return bl_preconditions;
    }
    bl_err err = mpmc_b_prepare_m_check_ver(
      q, now, data, op_cmp_offset, nodata_error
      );
    if (err == bl_ok) {
      mpmc_b_op next = mpmc_b_op_encode (now + 1, replace_sig ? sig : signow);
      if (atomic_u32_weak_cas_rlx (op_var, &now, next)) {
        *op = now;
        return err;
      }
    }
    else if (err == bl_busy){
      now = atomic_u32_load_rlx (op_var);
    }
    else {
      *op   = now;
      *data = nullptr;
      return err;
    }
  }
}
/*----------------------------------------------------------------------------*/
static bl_err mpmc_b_prepare_s(
  mpmc_b*     q,
  mpmc_b_op*  op,
  u8**        data,
  atomic_u32* op_var,
  u32         op_cmp_offset,
  bl_err      nodata_error
  )
{
  bl_assert (q && q->buffer && op && data);
  mpmc_b_op now = atomic_u32_load_rlx (op_var);
  bl_err err = mpmc_b_prepare_m_check_ver(
    q, now, data, op_cmp_offset, nodata_error
    );
  if (err == bl_ok) {
    *op = now;
    atomic_u32_store_rlx (op_var, mpmc_b_op_encode (now + 1, 0));
    return err;
  }
  bl_assert (err == nodata_error);
  return err;
}
/*----------------------------------------------------------------------------*/
static void mpmc_b_commit (mpmc_b* q, mpmc_b_op op, u32 increment)
{
  u8* slot = slot_addr (q->buffer, q->slot_count, q->slot_size, op);
  op += increment;
  atomic_u32_store ((atomic_u32*) slot, mpmc_b_op_encode (op, 0), mo_release);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_b_fifo_produce_prepare (mpmc_b* q, mpmc_b_op* op)
{
  *op = mpmc_b_op_encode (atomic_u32_fetch_add_rlx (&q->push_slot, 1), 0);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_fifo_produce_prepare_is_ready(
  mpmc_b* q, mpmc_b_op op, u8** data
  )
{
  bl_err err = mpmc_b_prepare_m_check_ver (q, op, data, 0, bl_would_overflow);
  if (likely (err != bl_would_overflow)) {
    return err;
  }
  /* if the counter has advanced more than the slot count either:
     - block has been called.
     - the user is using more threads than slots on
       "mpmc_b_fifo_produce_prepare" => user bug */
  mpmc_b_op now = atomic_u32_load_rlx (&q->push_slot);
  i32 status = (i32) (mpmc_b_ticket_decode (now) - mpmc_b_ticket_decode (op));
  return status < q->slot_count ? bl_would_overflow : bl_locked;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_produce_prepare_sig_fallback(
  mpmc_b*    q,
  mpmc_b_op* op,
  u8**       data,
  bool       replace_sig,
  mpmc_b_sig sig,
  mpmc_b_sig sig_fmask,
  mpmc_b_sig sig_fmatch
  )
{
  return mpmc_b_prepare_sig_fallback_m(
    q,
    op,
    data,
    replace_sig,
    sig,
    sig_fmask,
    sig_fmatch,
    &q->push_slot,
    0,
    bl_would_overflow
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_produce_prepare_sp(
    mpmc_b* q, mpmc_b_op* op, u8** data
    )
{
  return mpmc_b_prepare_s (q, op, data, &q->push_slot, 0, bl_would_overflow);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_b_produce_commit (mpmc_b* q, mpmc_b_op op)
{
  mpmc_b_commit (q, op, 1);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_consume_prepare_sig_fallback(
  mpmc_b*    q,
  mpmc_b_op* op,
  u8**       data,
  bool       replace_sig,
  mpmc_b_sig sig,
  mpmc_b_sig sig_fmask,
  mpmc_b_sig sig_fmatch
  )
{
  return mpmc_b_prepare_sig_fallback_m(
    q,
    op,
    data,
    replace_sig,
    sig,
    sig_fmask,
    sig_fmatch,
    &q->pop_slot,
    1,
    bl_empty
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_consume_prepare_sc(
  mpmc_b* q, mpmc_b_op* op, u8** data
  )
{
  return mpmc_b_prepare_s (q, op, data, &q->pop_slot, 1, bl_empty);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_b_consume_commit (mpmc_b *q, mpmc_b_op op)
{
  mpmc_b_commit (q, op, q->slot_count);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_producer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_producer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_op* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set_tmatch (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_consumer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_b_consumer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_op* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set_tmatch (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif
