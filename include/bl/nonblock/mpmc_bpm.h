#ifndef __BL_NONBLOCK_MPMC_BOUNDED_PRODUCER_MULTISLOT_H__
#define __BL_NONBLOCK_MPMC_BOUNDED_PRODUCER_MULTISLOT_H__

#include <bl/nonblock/libexport.h>
#include <bl/nonblock/mpmc_b_common.h>
#include <bl/base/platform.h>
#include <bl/base/cache.h>
#include <bl/base/alignment.h>
#include <bl/base/allocator.h>
#include <bl/base/error.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/utility.h>
#include <bl/base/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------------
This is the Dimitry Dyukov MPMC with added variations:

-It's broken in prepare commit blocks, giving direct access to the underlying
 array memory. The queue doubles as a FIFO and as a fixed-size memory allocator.

-The produce functions can reserve more than a slot at once, the consume
 functions retrieve a variable amount of slots.

-Adds some bits of the produce/consumer counters for communicating producer
 with producers or consumers with consumers. This feature is called "signals"
 and it's useful to reuse the CAS loop to broadcast state between producers.
 It uses 6 bits.

-Can be used as a backup allocator for other queues as long as the elements have
 very similar lifetimes. It is ideal to function as a per-CPU allocator (threads
 moved to another CPU won't make the queue to fail, as it's still multithreaded,
 but having a fast CPU heuristic could help fighting contention).

(TODO fix free alloc producer functions).

-TODO: Add fair blocking behavior

 The algorithm is veryfied by relacy and is race-free, but when in contention it
 has extra unfairness towards the producers trying to allocate more slots (they
 operate slowly).
------------------------------------------------------------------------------*/
typedef struct mpmc_bpm {
  u8*                       mem;
  u8*                       mem_unaligned;
  u32                       slots;
  u32                       slot_max;
  u32                       slot_size;
  u32                       flags;
  declare_cache_pad_member;
  atomic_u32                push_slot;
  declare_cache_pad_member;
  atomic_u32                pop_slot;
  declare_cache_pad_member;
}
mpmc_bpm;
/*----------------------------------------------------------------------------*/
static inline u32 mpmc_bpm_slot_size (mpmc_bpm const* q)
{
  return q->slot_size;
}
/*----------------------------------------------------------------------------*/
static inline uword mpmc_bpm_required_slots (mpmc_bpm const* q, uword payload)
{
  return round_to_next_multiple (payload + sizeof (u32), q->slot_size);
}
/*----------------------------------------------------------------------------*/
static inline uword mpmc_bpm_slot_payload (mpmc_bpm const* q, uword slots)
{
  return (q->slot_size * slots) - sizeof (u32);
}
/*----------------------------------------------------------------------------*/
static inline u32 mpmc_bpm_slot_count (mpmc_bpm const* q)
{
  return q->slots;
}
/*------------------------------------------------------------------------------
slot_count: number of queue slots. a power of two. max is "mpmc_bpm_max_slots".
  The slot count must be bigger than the number of producer threads if you use
  "mpmc_bpm_fifo_produce_prepare".

slot_size: Absolute size of the slot including a leading u32 variable.

slot_max: the maximum amount of slots to be reserved at once. This parameter
  causes the queue to a tail of free memory so the last slot of the queue can
  reserve "slot_max" slots. A queue of "slot_count = 8" and "slot_max" = 8 would
  result in a queue of (2 * 8) - 1 = 15 slots.

slot_alignment: the required alignment of the memory pointer returned by the
  produce calls.

enable_fairness: When disabled reserving higher "slot_count" values take
  more time to complete that the lower ones, which can lead to noticieable
  unfairness when the queue is under contention. Enabling this flag equates all
  slow counts to the worst case. In any case this queue fits best for elements
  without a big size differences.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bpm_init(
    mpmc_bpm*        q,
    alloc_tbl const* alloc,
    u32              slot_count,
    u32              slot_max,
    u32              slot_size,
    u32              slot_alignment,
    bool             enable_fairness
    );
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void mpmc_bpm_destroy (mpmc_bpm* q, alloc_tbl const* alloc);
/*------------------------------------------------------------------------------
"mpmc_bpm_produce_prepare*" funtion call variants.

"produce_prepare" Gets a FIFO memory chunk from the queue for producing. On a
successful call the memory chunk can be written. The memory write is signaled
by calling "mpmc_bpm_produce_commit".

op: when there is no error will contain the current ticket id
  and the signal that was present on the producer's "channel". You unpack them
  with "mpmc_b_sig_decode (op)" and "mpmc_b_ticket_decode (op)"

  On return of an error code the operation has been blocked and can't be
  commited.

  On return of the "bl_preconditions" error code the operation has been
  blocked because of "sig_fallback_mask" and "sig_fallback_match". The signal
  returned will be the signal that was present on the channel, the ticket
  will be the ticket of the next insertion's ticket (which should be
  ignored).

mem: on success will contain a pointer to contiguous memory, nullptr otherwise.

replace_sigl + sig_replacement: If "replace_signal" is "true" the current
  producer channel signal value will be replaced on success with
  "signal_replacement". If "replace_signal" is false the "signal_replacement"
  value is ignored.

sig_fallback_mask + sig_fallback_match: The call will early fail with
  the "bl_preconditions" error code if signal present on the channel now
  matches the condition below:

  ("signal_on_channel_now" & sig_fallback_mask) == sig_fallback_match

  This can be used e.g. on termination contexts, the signal is set to a value
  that represents "on termination" and with an according mask + match the
  producers will be blocked.

Errors returned:

- bl_ok: Succes, "mem" can be written and "mpmpc_ba_produce_commit" can
  be called.
- bl_preconditions: see sig_fallback_mask + sig_fallback_match.
- bl_would_overflow: queue can't allocate that number of bytes - queue full.
- bl_locked: The queue is blocked

Example usage snippet:

  TODO:

------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bpm_produce_prepare_sig_fallback(
   mpmc_bpm*  q,
   mpmc_b_op* op,
   u8**       mem,
   u32        slots,
   bool       replace_sig,
   mpmc_b_sig sig_replacement,
   mpmc_b_sig sig_fallback_mask,
   mpmc_b_sig sig_fallback_match
   );
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bpm_produce_prepare_fallback(
  mpmc_bpm*  q,
  mpmc_b_op* op,
  u8**       mem,
  u32        slots,
  mpmc_b_sig sig_fallback_mask,
  mpmc_b_sig sig_fallback_match
  )
{
  return mpmc_bpm_produce_prepare_sig_fallback(
    q, op, mem, slots, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bpm_produce_prepare_sig(
  mpmc_bpm*  q,
  mpmc_b_op* op,
  u8**       mem,
  u32        slots,
  mpmc_b_sig sig_replacement
  )
{
  return mpmc_bpm_produce_prepare_sig_fallback(
    q, op, mem, slots, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bpm_produce_prepare(
  mpmc_bpm* q, mpmc_b_op* op, u8** mem, u32 slots
  )
{
  return mpmc_bpm_produce_prepare_sig_fallback(
    q, op, mem, slots, false, 0, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void mpmc_bpm_produce_commit(
    mpmc_bpm* q, mpmc_b_op op, u8* mem, u32 slots
    );
/*------------------------------------------------------------------------------
The interface is exactly the same as on the "mpmc_bpm_produce_prepare*" funtion
call variants but consuming instead.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bpm_consume_prepare_sig_fallback(
    mpmc_bpm*  q,
    mpmc_b_op* op,
    u8**       mem,
    u32*       slots,
    bool       replace_sig,
    mpmc_b_sig sig_replacement,
    mpmc_b_sig sig_fallback_mask,
    mpmc_b_sig sig_fallback_match
    );
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bpm_consume_prepare_fallback(
  mpmc_bpm*  q,
  mpmc_b_op* op,
  u8**       mem,
  u32*       slots,
  mpmc_b_sig sig_fallback_mask,
  mpmc_b_sig sig_fallback_match
  )
{
  return mpmc_bpm_consume_prepare_sig_fallback(
    q, op, mem, slots, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bpm_consume_prepare_sig(
  mpmc_bpm*  q,
  mpmc_b_op* op,
  u8**       mem,
  u32*       slots,
  mpmc_b_sig sig_replacement
  )
{
  return mpmc_bpm_consume_prepare_sig_fallback(
    q, op, mem, slots, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bpm_consume_prepare(
  mpmc_bpm* q, mpmc_b_op* op, u8** mem, u32* slots
  )
{
  return mpmc_bpm_consume_prepare_sig_fallback(
    q, op, mem, slots, false, 0, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void mpmc_bpm_consume_commit(
    mpmc_bpm* q, mpmc_b_op op, u8* mem, u32 slots
    );
/*------------------------------------------------------------------------------
  This queue can be used just as an allocator, but the underlying queue is the
  same so:

  -Deallocations must happen in a near-FIFO order (e.g. this queue could be the
   allocator of a linked list based MPxC queue). The producers (alloc callers)
   will be blocked when they find an allocated slot
  -When on extreme contention, the "alloc" calls with less slots will have more
   chances to finish that those that request less slots unless the fairness is
   activated.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT u8* mpmc_bpm_alloc(
  mpmc_bpm* q, uword slots
  );
/*--------------------------- ------------------------------------------------*/
extern BL_NONBLOCK_EXPORT void mpmc_bpm_dealloc(
  mpmc_bpm* q, u8* mem, uword slots
  );
/*----------------------------------------------------------------------------*/
static inline void mpmc_bpm_dealloc_unsafe (mpmc_bpm* q, u8* mem)
{
  mpmc_bpm_dealloc (q, mem, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool mpmc_bpm_allocation_is_in_range (mpmc_bpm* q, u8* mem)
{
  return (mem >= q->mem) && (mem < (q->mem + (q->slots * q->slot_size)));
}
/*------------------------------------------------------------------------------
  See mpmc_b.h for documentation on these functions, they work exactly the same
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bpm_producer_signal_try_set(
    mpmc_bpm* q, mpmc_b_sig* expected, mpmc_b_sig desired
    );
/*-----------------------------------------------------------------------------
 Same as the above but the last successful ticket has to match too. If
 the queue is unitialized the last successful ticket is uint_max;
-----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bpm_producer_signal_try_set_tmatch(
    mpmc_bpm* q, mpmc_b_ticket* expected, mpmc_b_sig desired
    );
/*-----------------------------------------------------------------------------
  Same as above but for consumers. Can only be used on MPMC or SPMC mode
-----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bpm_consumer_signal_try_set(
    mpmc_bpm* q, mpmc_b_sig* expected, mpmc_b_sig desired
    );
/*--------------------------- ------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bpm_consumer_signal_try_set_tmatch(
    mpmc_bpm* q, mpmc_b_ticket* expected, mpmc_b_sig desired
    );
/*--------------------------- ------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCK_MPMC_BOUNDED_PRODUCER_MULTI_H__ */
