#include <bl/nonblock/mpmc_bpm.h>
#include <bl/nonblock/mpmc_b_common_priv.h>

enum qflags {
  mpmc_bpm_fairness,
};
/*----------------------------------------------------------------------------*/
static inline bool fairness_is_set (mpmc_bpm const* q)
{
  return q->flags != 0;
}
/*----------------------------------------------------------------------------*/
static inline u32 get_slot_ceil (mpmc_bpm const* q, uword idx, uword slots)
{
  return bl_min (slots, q->slots - (idx)); ;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bpm_init(
  mpmc_bpm*        q,
  alloc_tbl const* alloc,
  u32              slot_count,
  u32              slot_max,
  u32              slot_size,
  u32              slot_alignment,
  bool             enable_fairness
  )
{
  slot_count = round_next_pow2_u (slot_count);
  if (slot_count == 0 || slot_size == 0 || slot_max == 0 ||
      slot_max >= slot_count || /* This can't be just gt, must be ge */
      !is_multiple (slot_size, sizeof (u32)) ||
      !is_multiple (slot_size, slot_alignment) ||
      !is_pow2 (slot_alignment) ||
      slot_alignment < sizeof (u32) ||
      slot_count >= mpmc_b_max_slots
    ) {
    return bl_mkerr (bl_invalid);
  }
  uword alloc_size = (slot_count + slot_max - 1) * slot_size;
  alloc_size      += slot_alignment + sizeof (atomic_u32);
  q->mem_unaligned = (u8*) bl_alloc (alloc, alloc_size);
  if (!q->mem_unaligned) {
    return bl_mkerr (bl_alloc);
  }
  uword alignaddr = (uword) q->mem_unaligned;
  alignaddr      += slot_alignment + sizeof (atomic_u32);
  alignaddr      &= ~((uword) slot_alignment - 1);
  alignaddr      -= sizeof (atomic_u32); /*the aligned address starts after*/
  q->mem = (u8*) alignaddr;

#ifndef BL_ATOMIC_USE_RELACY
  mem_order mo = mo_relaxed;
#else
  mem_order mo = mo_seq_cst;
#endif
  atomic_u32_store (&q->push_slot, 0, mo);
  atomic_u32_store (&q->pop_slot, 0, mo);
  q->flags     = enable_fairness ? mpmc_bpm_fairness : 0;
  q->slot_max  = slot_max;
  q->slot_size = slot_size;
  q->slots     = slot_count;

/*When simulating with relacy intialization will be done externally*/
#ifndef BL_ATOMIC_USE_RELACY
  u32 v   = 0;
  u8* ptr = q->mem;
  while (v < slot_count) {
    atomic_u32_store_rlx ((atomic_u32*) ptr, v);
    ++v;
    ptr += slot_size;
  }
#endif
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_bpm_destroy (mpmc_bpm* q, alloc_tbl const* alloc)
{
  bl_assert (q && alloc);
  if (q->mem_unaligned) {
    bl_dealloc (alloc, q->mem_unaligned);
  }
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bpm_produce_prepare_sig_fallback(
   mpmc_bpm*  q,
   mpmc_b_op* op,
   u8**       mem,
   u32        slots,
   bool       replace_sig,
   mpmc_b_sig sig,
   mpmc_b_sig sig_fmask,
   mpmc_b_sig sig_fmatch
   )
{
  bl_assert (q && op && mem && slots);
  if (unlikely (slots > q->slot_max)) {
    return bl_mkerr (bl_invalid);
  }
  mpmc_b_op now = atomic_u32_load_rlx (&q->push_slot);
resync:;
  mpmc_b_sig signow = mpmc_b_sig_decode (now);
  if (unlikely ((signow & sig_fmask) == sig_fmatch)) {
    *op = now;
    return bl_mkerr (bl_preconditions);
  }
  uword idx     = now & (q->slots - 1);
  u8* slot      = &q->mem[idx * q->slot_size];
  mpmc_b_op ver = atomic_u32_load ((atomic_u32*) slot, mo_acquire);

  i32 diff = (i32) (mpmc_b_ticket_decode (ver) - mpmc_b_ticket_decode (now));
  if (unlikely (diff < 0)) {
    return bl_mkerr (bl_would_overflow);
  }
  else if (unlikely (diff > 0)) {
      now = atomic_u32_load_rlx (&q->push_slot);
      goto resync;
  }

  u32 slots_ceil = get_slot_ceil (q, idx, slots);
  u8* slot_tail  = slot;
  uword i;
  for (i = 1; i < slots_ceil; ++i) {
    slot_tail         += q->slot_size;
    mpmc_b_op ver_tail = atomic_u32_load(
      (atomic_u32*) slot_tail, mo_acquire
      );
    i32 diff_tail = (i32) (
      mpmc_b_ticket_decode (ver_tail) - (mpmc_b_ticket_decode (now) + i)
      );
    if (unlikely (diff_tail != 0)) {
      mpmc_b_op old = now;
      now = atomic_u32_fetch_add_rlx (&q->push_slot, 0);
      if (now == old) {
        /* no collision with producer: not enough space */
        return bl_mkerr (bl_would_overflow);
      }
      /* collision with producer */
      goto resync;
    }
  }
  if (fairness_is_set (q)) {
    /*burning time by emulating the previous loop (touching memory and fetch
      adding  unnecessarily). The CAS should prevent the compiler optimizing
      away the doing-nothing loop */

    /* Random ideas: maybe this can be enabled dinamically when detecting a
       thread that is starved more than X amount of cycles and by writing a
       signal to the push slot (configurable)*/
    u32 slots_fairness = get_slot_ceil (q, idx, q->slot_max);
    for (; i < slots_fairness; ++i) {
      slot_tail += q->slot_size;
      mpmc_b_op ver_tail = atomic_u32_load(
        (atomic_u32*) slot_tail, mo_acquire
        );
      i32 diff_tail = (i32) (
        mpmc_b_ticket_decode (ver_tail) - (mpmc_b_ticket_decode (now) + i)
        );
      if (unlikely (diff_tail != 0)) {
        /*hopefylly the compiler won't try to optimize a zero add.*/
        atomic_u32_fetch_add_rlx (&q->push_slot, 0);
        break;
      }
    }
  }
  mpmc_b_op next = mpmc_b_op_encode(
    now + slots_ceil, replace_sig ? sig : signow
    );
  if (likely (atomic_u32_weak_cas_rlx (&q->push_slot, &now, next))) {
    *mem = slot + sizeof (atomic_u32);
    *op  = now;
    return bl_mkok();
  }
  goto resync;
}

/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_bpm_produce_commit(
  mpmc_bpm* q, mpmc_b_op op, u8* mem, u32 slots
  )
{
  bl_assert (q && mem && slots);
  bl_assert (((mem - q->mem) / q->slot_size) == (op & (q->slots - 1)));
  op += slots;
  atomic_u32_store(
    (atomic_u32*) (mem - sizeof (atomic_u32)),
    mpmc_b_op_encode (op, 0),
    mo_release
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bpm_consume_prepare_sig_fallback(
    mpmc_bpm*  q,
    mpmc_b_op* op,
    u8**       mem,
    u32*       slots,
    bool       replace_sig,
    mpmc_b_sig sig,
    mpmc_b_sig sig_fmask,
    mpmc_b_sig sig_fmatch
    )
{
  bl_assert (q && op && mem && slots);
  mpmc_b_op now = atomic_u32_load_rlx (&q->pop_slot);
resync:;
  mpmc_b_sig signow = mpmc_b_sig_decode (now);
  if (unlikely ((signow & sig_fmask) == sig_fmatch)) {
    *op = now;
    return bl_mkerr (bl_preconditions);
  }
  uword idx     = now & (q->slots - 1);
  u8* slot      = &q->mem[idx * q->slot_size];
  mpmc_b_op ver = atomic_u32_load ((atomic_u32*) slot, mo_acquire);

  i32 diff = (i32) (mpmc_b_ticket_decode (ver) - mpmc_b_ticket_decode (now));
  if (unlikely (diff <= 0)) {
    return bl_mkerr (bl_empty);
  }
  else if (unlikely ((u32) diff >= q->slots)) {
    now = atomic_u32_load_rlx (&q->pop_slot);
    goto resync;
  }
  u32 slots_ceil = get_slot_ceil (q, idx, diff);
  mpmc_b_op next = mpmc_b_op_encode(
    now + slots_ceil, replace_sig ? sig : signow
    );
  if (likely (atomic_u32_weak_cas_rlx (&q->pop_slot, &now, next))) {
    *mem   = slot + sizeof (atomic_u32);
    *op    = now;
    *slots = (u32) diff;
    return bl_mkok();
  }
  goto resync;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_bpm_consume_commit(
  mpmc_bpm* q, mpmc_b_op op, u8* mem, u32 slots
  )
{
  bl_assert (q && mem && slots);
  bl_assert (((mem - q->mem) / q->slot_size) == (op & (q->slots - 1)));
  mem      -= sizeof (atomic_u32);
  op        = mpmc_b_op_encode (op + q->slots, 0);
  u8* memcp = mem;
  slots     = get_slot_ceil (q, op & (q->slots - 1), slots);
  for (uword i = 1; i < slots; ++i) {
    memcp += q->slot_size;
    atomic_u32_store(
      (atomic_u32*) memcp, mpmc_b_op_encode (op + i, 0), mo_release
      );
  }
  atomic_u32_store ((atomic_u32*) mem, op, mo_release);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bpm_producer_signal_try_set(
  mpmc_bpm* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bpm_producer_signal_try_set_tmatch(
  mpmc_bpm* q, mpmc_b_op* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set_tmatch (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bpm_consumer_signal_try_set(
  mpmc_bpm* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bpm_consumer_signal_try_set_tmatch(
  mpmc_bpm* q, mpmc_b_op* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_try_set_tmatch (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT u8* mpmc_bpm_alloc (mpmc_bpm* q, uword slots)
{
  u8*       mem;
  mpmc_b_op op;
  bl_err err = mpmc_bpm_produce_prepare (q, &op, &mem, slots);
  if (unlikely (err.bl)) {
    return nullptr;
  }
  mpmc_bpm_produce_commit (q, op, mem, slots);
  return mem;
}
/*--------------------------- ------------------------------------------------*/
#ifdef BL_POSIX
  #include <signal.h>
#endif
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT void mpmc_bpm_dealloc (mpmc_bpm* q, u8* mem, uword slots)
{
  if (unlikely (!mem)) {
    return;
  }
  if (unlikely (!mpmc_bpm_allocation_is_in_range (q, mem))) {
#ifdef BL_POSIX
    raise (SIGSEGV);
#else
    bl_assert_always (false && "out of range");
#endif
  }
  uword idx    = (mem - q->mem) / q->slot_size;
  mpmc_b_op op =
    atomic_u32_load_rlx ((atomic_u32*) (mem - sizeof (atomic_u32)));
  uword slots_expected = (mpmc_b_ticket_decode (op) - idx) & (q->slots - 1);
  if (slots != 0) {
#ifdef BL_POSIX
    if (unlikely (slots != slots_expected)) {
      raise (SIGSEGV);
    }
#else
    bl_assert_always (slots == slots_expected);
#endif
  }
  else {
    slots = slots_expected;
  }
  op = mpmc_b_ticket_decode (op) - slots;
  mpmc_bpm_consume_commit (q, op, mem, slots);
}
/*--------------------------- ------------------------------------------------*/
