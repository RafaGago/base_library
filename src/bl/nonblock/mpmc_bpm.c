#include <bl/nonblock/mpmc_bpm.h>
#include <bl/nonblock/mpmc_b_common_priv.h>

enum qflags {
  bl_mpmc_bpm_fairness,
};
/*----------------------------------------------------------------------------*/
static inline bool fairness_is_set (bl_mpmc_bpm const* q)
{
  return q->flags != 0;
}
/*----------------------------------------------------------------------------*/
static inline bl_u32
  get_slot_ceil (bl_mpmc_bpm const* q, bl_uword idx, bl_uword slots)
{
  return bl_min (slots, q->slots - (idx)); ;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bpm_init(
  bl_mpmc_bpm*        q,
  bl_alloc_tbl const* alloc,
  bl_u32              slot_count,
  bl_u32              slot_max,
  bl_u32              slot_size,
  bl_u32              slot_alignment,
  bool             enable_fairness
  )
{
  slot_count = bl_round_next_pow2_u (slot_count);
  if (slot_count == 0 || slot_size == 0 || slot_max == 0 ||
      slot_max >= slot_count || /* This can't be just gt, must be ge */
      !bl_is_multiple (slot_size, sizeof (bl_u32)) ||
      !bl_is_multiple (slot_size, slot_alignment) ||
      !bl_is_pow2 (slot_alignment) ||
      slot_alignment < sizeof (bl_u32) ||
      slot_count >= bl_mpmc_b_max_slots
    ) {
    return bl_mkerr (bl_invalid);
  }
  bl_uword alloc_size = (slot_count + slot_max - 1) * slot_size;
  alloc_size      += slot_alignment + sizeof (bl_atomic_u32);
  q->mem_unaligned = (bl_u8*) bl_alloc (alloc, alloc_size);
  if (!q->mem_unaligned) {
    return bl_mkerr (bl_alloc);
  }
  bl_uword alignaddr = (bl_uword) q->mem_unaligned;
  alignaddr      += slot_alignment + sizeof (bl_atomic_u32);
  alignaddr      &= ~((bl_uword) slot_alignment - 1);
  alignaddr      -= sizeof (bl_atomic_u32); /*the aligned address starts after*/
  q->mem = (bl_u8*) alignaddr;

#ifndef BL_ATOMIC_USE_RELACY
  bl_mem_order mo = bl_mo_relaxed;
#else
  bl_mem_order mo = bl_mo_seq_cst;
#endif
  bl_atomic_u32_store (&q->push_slot, 0, mo);
  bl_atomic_u32_store (&q->pop_slot, 0, mo);
  q->flags     = enable_fairness ? bl_mpmc_bpm_fairness : 0;
  q->slot_max  = slot_max;
  q->slot_size = slot_size;
  q->slots     = slot_count;

/*When simulating with relacy intialization will be done externally*/
#ifndef BL_ATOMIC_USE_RELACY
  bl_u32 v   = 0;
  bl_u8* ptr = q->mem;
  while (v < slot_count) {
    bl_atomic_u32_store_rlx ((bl_atomic_u32*) ptr, v);
    ++v;
    ptr += slot_size;
  }
#endif
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_bpm_destroy (bl_mpmc_bpm* q, bl_alloc_tbl const* alloc)
{
  bl_assert (q && alloc);
  if (q->mem_unaligned) {
    bl_dealloc (alloc, q->mem_unaligned);
  }
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bpm_produce_prepare_sig_fallback(
   bl_mpmc_bpm*  q,
   bl_mpmc_b_op* op,
   bl_u8**       mem,
   bl_u32        slots,
   bool       replace_sig,
   bl_mpmc_b_sig sig,
   bl_mpmc_b_sig sig_fmask,
   bl_mpmc_b_sig sig_fmatch
   )
{
  bl_assert (q && op && mem && slots);
  if (bl_unlikely (slots > q->slot_max)) {
    return bl_mkerr (bl_invalid);
  }
  bl_mpmc_b_op now = bl_atomic_u32_load_rlx (&q->push_slot);
resync:;
  bl_mpmc_b_sig signow = bl_mpmc_b_sig_decode (now);
  if (bl_unlikely ((signow & sig_fmask) == sig_fmatch)) {
    *op = now;
    return bl_mkerr (bl_preconditions);
  }
  bl_uword idx     = now & (q->slots - 1);
  bl_u8* slot      = &q->mem[idx * q->slot_size];
  bl_mpmc_b_op ver = bl_atomic_u32_load ((bl_atomic_u32*) slot, bl_mo_acquire);

  bl_i32 diff = (bl_i32) (bl_mpmc_b_ticket_decode (ver) - bl_mpmc_b_ticket_decode (now));
  if (bl_unlikely (diff < 0)) {
    return bl_mkerr (bl_would_overflow);
  }
  else if (bl_unlikely (diff > 0)) {
      now = bl_atomic_u32_load_rlx (&q->push_slot);
      goto resync;
  }

  bl_u32 slots_ceil = get_slot_ceil (q, idx, slots);
  bl_u8* slot_tail  = slot;
  bl_uword i;
  for (i = 1; i < slots_ceil; ++i) {
    slot_tail         += q->slot_size;
    bl_mpmc_b_op ver_tail = bl_atomic_u32_load(
      (bl_atomic_u32*) slot_tail, bl_mo_acquire
      );
    bl_i32 diff_tail = (bl_i32) (
      bl_mpmc_b_ticket_decode (ver_tail) - (bl_mpmc_b_ticket_decode (now) + i)
      );
    if (bl_unlikely (diff_tail != 0)) {
      bl_mpmc_b_op old = now;
      now = bl_atomic_u32_fetch_add_rlx (&q->push_slot, 0);
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
    bl_u32 slots_fairness = get_slot_ceil (q, idx, q->slot_max);
    for (; i < slots_fairness; ++i) {
      slot_tail += q->slot_size;
      bl_mpmc_b_op ver_tail = bl_atomic_u32_load(
        (bl_atomic_u32*) slot_tail, bl_mo_acquire
        );
      bl_i32 diff_tail = (bl_i32) (
        bl_mpmc_b_ticket_decode (ver_tail) - (bl_mpmc_b_ticket_decode (now) + i)
        );
      if (bl_unlikely (diff_tail != 0)) {
        /*hopefylly the compiler won't try to optimize a zero add.*/
        bl_atomic_u32_fetch_add_rlx (&q->push_slot, 0);
        break;
      }
    }
  }
  bl_mpmc_b_op next = bl_mpmc_b_op_encode(
    now + slots_ceil, replace_sig ? sig : signow
    );
  if (bl_likely (bl_atomic_u32_weak_cas_rlx (&q->push_slot, &now, next))) {
    *mem = slot + sizeof (bl_atomic_u32);
    *op  = now;
    return bl_mkok();
  }
  goto resync;
}

/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_bpm_produce_commit(
  bl_mpmc_bpm* q, bl_mpmc_b_op op, bl_u8* mem, bl_u32 slots
  )
{
  bl_assert (q && mem && slots);
  bl_assert (((mem - q->mem) / q->slot_size) == (op & (q->slots - 1)));
  op += slots;
  bl_atomic_u32_store(
    (bl_atomic_u32*) (mem - sizeof (bl_atomic_u32)),
    bl_mpmc_b_op_encode (op, 0),
    bl_mo_release
    );
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bpm_consume_prepare_sig_fallback(
    bl_mpmc_bpm*  q,
    bl_mpmc_b_op* op,
    bl_u8**       mem,
    bl_u32*       slots,
    bool       replace_sig,
    bl_mpmc_b_sig sig,
    bl_mpmc_b_sig sig_fmask,
    bl_mpmc_b_sig sig_fmatch
    )
{
  bl_assert (q && op && mem && slots);
  bl_mpmc_b_op now = bl_atomic_u32_load_rlx (&q->pop_slot);
resync:;
  bl_mpmc_b_sig signow = bl_mpmc_b_sig_decode (now);
  if (bl_unlikely ((signow & sig_fmask) == sig_fmatch)) {
    *op = now;
    return bl_mkerr (bl_preconditions);
  }
  bl_uword idx     = now & (q->slots - 1);
  bl_u8* slot      = &q->mem[idx * q->slot_size];
  bl_mpmc_b_op ver = bl_atomic_u32_load ((bl_atomic_u32*) slot, bl_mo_acquire);

  bl_i32 diff = (bl_i32) (bl_mpmc_b_ticket_decode (ver) - bl_mpmc_b_ticket_decode (now));
  if (bl_unlikely (diff <= 0)) {
    return bl_mkerr (bl_empty);
  }
  else if (bl_unlikely ((bl_u32) diff >= q->slots)) {
    now = bl_atomic_u32_load_rlx (&q->pop_slot);
    goto resync;
  }
  bl_u32 slots_ceil = get_slot_ceil (q, idx, diff);
  bl_mpmc_b_op next = bl_mpmc_b_op_encode(
    now + slots_ceil, replace_sig ? sig : signow
    );
  if (bl_likely (bl_atomic_u32_weak_cas_rlx (&q->pop_slot, &now, next))) {
    *mem   = slot + sizeof (bl_atomic_u32);
    *op    = now;
    *slots = (bl_u32) diff;
    return bl_mkok();
  }
  goto resync;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_bpm_consume_commit(
  bl_mpmc_bpm* q, bl_mpmc_b_op op, bl_u8* mem, bl_u32 slots
  )
{
  bl_assert (q && mem && slots);
  bl_assert (((mem - q->mem) / q->slot_size) == (op & (q->slots - 1)));
  mem      -= sizeof (bl_atomic_u32);
  op        = bl_mpmc_b_op_encode (op + q->slots, 0);
  bl_u8* memcp = mem;
  slots     = get_slot_ceil (q, op & (q->slots - 1), slots);
  for (bl_uword i = 1; i < slots; ++i) {
    memcp += q->slot_size;
    bl_atomic_u32_store(
      (bl_atomic_u32*) memcp, bl_mpmc_b_op_encode (op + i, 0), bl_mo_release
      );
  }
  bl_atomic_u32_store ((bl_atomic_u32*) mem, op, bl_mo_release);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bpm_producer_signal_try_set(
  bl_mpmc_bpm* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bpm_producer_signal_try_set_tmatch(
  bl_mpmc_bpm* q, bl_mpmc_b_op* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set_tmatch (&q->push_slot, expected, desired);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bpm_consumer_signal_try_set(
  bl_mpmc_bpm* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bpm_consumer_signal_try_set_tmatch(
  bl_mpmc_bpm* q, bl_mpmc_b_op* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_signal_try_set_tmatch (&q->pop_slot, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_u8* bl_mpmc_bpm_alloc (bl_mpmc_bpm* q, bl_uword slots)
{
  bl_u8*       mem;
  bl_mpmc_b_op op;
  bl_err err = bl_mpmc_bpm_produce_prepare (q, &op, &mem, slots);
  if (bl_unlikely (err.bl)) {
    return nullptr;
  }
  bl_mpmc_bpm_produce_commit (q, op, mem, slots);
  return mem;
}
/*--------------------------- ------------------------------------------------*/
#ifdef BL_POSIX
  #include <signal.h>
#endif
/*--------------------------- ------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpmc_bpm_dealloc (bl_mpmc_bpm* q, bl_u8* mem, bl_uword slots)
{
  if (bl_unlikely (!mem)) {
    return;
  }
  if (bl_unlikely (!bl_mpmc_bpm_allocation_is_in_range (q, mem))) {
#ifdef BL_POSIX
    raise (SIGSEGV);
#else
    bl_assert_always (false && "out of range");
#endif
  }
  bl_uword idx    = (mem - q->mem) / q->slot_size;
  bl_mpmc_b_op op =
    bl_atomic_u32_load_rlx ((bl_atomic_u32*) (mem - sizeof (bl_atomic_u32)));
  bl_uword slots_expected = (bl_mpmc_b_ticket_decode (op) - idx) & (q->slots - 1);
  if (slots != 0) {
#ifdef BL_POSIX
    if (bl_unlikely (slots != slots_expected)) {
      raise (SIGSEGV);
    }
#else
    bl_assert_always (slots == slots_expected);
#endif
  }
  else {
    slots = slots_expected;
  }
  op = bl_mpmc_b_ticket_decode (op) - slots;
  bl_mpmc_bpm_consume_commit (q, op, mem, slots);
}
/*--------------------------- ------------------------------------------------*/
