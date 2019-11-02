/*---------------------------------------------------------------------------*/
#ifndef __BL_NONBLOCKMC_B_PREPARE_COMMIT_H__
#define __BL_NONBLOCKMC_B_PREPARE_COMMIT_H__

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

-Can be used as MPMC SPSC MPSC and SPMC.

-It's broken in prepare commit blocks, giving direct access to the underlying
 array memory. The queue doubles as a FIFO and as a fixed-size memory allocator.

-Adds some bits of the produce/consumer counters for communicating producer
 with producers or consumers with consumers. This feature is called "signals"
 and it's useful to reuse the CAS loop to broadcast state between producers.
 It uses 6 bits.

-Producers that are allowed to block on the queue can enqueue by using
 fetch-and-add, getting strict FIFO guarantees (at the expense of losing the
 signal feature described above).
------------------------------------------------------------------------------*/
typedef struct bl_mpmc_b {
  bl_u8*                       buffer;
  bl_u32                       slot_count;
  bl_u32                       slot_size;
  bl_u32                       data_offset;
  bl_atomic_u32                flags;
  bl_declare_cache_pad_member;
  bl_atomic_u32                push_slot;
  bl_declare_cache_pad_member;
  bl_atomic_u32                pop_slot;
  bl_declare_cache_pad_member;
}
bl_mpmc_b;
/*----------------------------------------------------------------------------*/
static inline bl_u32 bl_mpmc_b_slot_size (bl_mpmc_b const* q)
{
  return q->slot_size;
}
/*----------------------------------------------------------------------------*/
static inline bl_u32 bl_mpmc_b_data_size (bl_mpmc_b const* q)
{
  return q->slot_size - q->data_offset;
}
/*----------------------------------------------------------------------------*/
static inline bl_u32 bl_mpmc_b_slot_count (bl_mpmc_b const* q)
{
  return q->slot_count;
}
/*------------------------------------------------------------------------------
slot_count: number of queue slots. a power of two. max is "bl_mpmc_b_max_slots".
  The slot count must be bigger than the number of producer threads if you use
  "bl_mpmc_b_fifo_produce_prepare".

data_size: size of data that is going to be stored-

data_alignment: the required alignment of the data to be stored.

The effective slot size will be calculated as:

  bl_mpmc_b_round_slot_size(
    bl_mpmc_b_compute_slot_size (data_size, data_alignment),
    data_alignment
    );
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_init(
    bl_mpmc_b*          q,
    bl_alloc_tbl const* alloc,
    bl_u32              slot_count,
    bl_u32              data_size,
    bl_u32              data_alignment
    );
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_mpmc_b_destroy (bl_mpmc_b* q, bl_alloc_tbl const* alloc);
/*------------------------------------------------------------------------------
Irreversibly blocks the queue for the producers when in mp mode. All producer
operations will return "bl_locked".
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_mpmc_b_block_producers (bl_mpmc_b* q);
/*------------------------------------------------------------------------------
"bl_mpmc_b_produce_prepare*" funtion call variants.

"produce_prepare" Gets a FIFO memory chunk from the queue for producing. On a
successful call the memory chunk can be written. The memory write is signaled
by calling "bl_mpmc_b_produce_commit".

op: when there is no error will contain the current ticket id
  and the signal that was present on the producer's "channel". You unpack them
  with "bl_mpmc_b_sig_decode (op)" and "bl_mpmc_b_ticket_decode (op)"

  On return of an error code the operation has been blocked and can't be
  commited.

  On return of the "bl_preconditions" error code the operation has been
  blocked because of "sig_fallback_mask" and "sig_fallback_match". The signal
  returned will be the signal that was present on the channel, the ticket
  will be the ticket of the next insertion's ticket (which should be
  ignored).

data: on success will contain a pointer to contiguous memory, nullptr otherwise.

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

- bl_ok: Succes, "data" can be written and "mpmpc_ba_produce_commit" can
  be called.
- bl_preconditions: see sig_fallback_mask + sig_fallback_match.
- bl_would_overflow: queue can't allocate that number of bytes - queue full.
- bl_locked: The queue is blocked

Example usage snippet:

  bl_mpmc_b_op op;
  bl_u8*       data;
  bl_err err = bl_mpmc_b_produce_prepare (q, &op, &data);
  if (err.bl) {
    return err;
  }
  write_data_for_consumers (data, bl_mpmc_b_slot_payload (q));
  bl_mpmc_b_produce_commit (q, op);

------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_produce_prepare_sig_fallback(
   bl_mpmc_b*    q,
   bl_mpmc_b_op* op,
   bl_u8**       data,
   bool          replace_sig,
   bl_mpmc_b_sig sig_replacement,
   bl_mpmc_b_sig sig_fallback_mask,
   bl_mpmc_b_sig sig_fallback_match
   );
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_b_produce_prepare_fallback(
  bl_mpmc_b*    q,
  bl_mpmc_b_op* op,
  bl_u8**       data,
  bl_mpmc_b_sig sig_fallback_mask,
  bl_mpmc_b_sig sig_fallback_match
  )
{
  return bl_mpmc_b_produce_prepare_sig_fallback(
    q, op, data, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_b_produce_prepare_sig(
  bl_mpmc_b* q, bl_mpmc_b_op* op, bl_u8** data, bl_mpmc_b_sig sig_replacement
  )
{
  return bl_mpmc_b_produce_prepare_sig_fallback(
    q, op, data, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_b_produce_prepare(
  bl_mpmc_b* q, bl_mpmc_b_op* op, bl_u8** data
  )
{
  return bl_mpmc_b_produce_prepare_sig_fallback (q, op, data, false, 0, 0, 1);
}
/*------------------------------------------------------------------------------
The same as "bl_mpmc_b_produce_prepare*" but for a single producer. Remember
that you can't mix modes in the same queue.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_produce_prepare_sp (bl_mpmc_b* q, bl_mpmc_b_op* op, bl_u8** data);
/*------------------------------------------------------------------------------
FIFO bl_mpmc_b_fifo_produce_prepare: Takes ownership of the next slot as
consumer, even if the queue is full.

This ensures FIFO ordering and evens out the latency between calls when the
producers are allowed to block on the queue.

But unfortunately it makes the transaction unbreakable, once you have called
"bl_mpmc_b_fifo_produce_prepare" you can't roll-back, you have to block
on "bl_mpmc_b_fifo_produce_prepare_is_ready" until you succeed and then commit,
otherwise the queue will be trashed.

Using this call invalidates the signals feature on the producer channel. It
internally uses fetch-add.

Warning: to use this function the queue size must always be bigger or equal
than the producer number, otherwise the queue will __silently__ start to
malfunction.

Example snippet:
  bl_mpmc_b_op op;
  bl_mpmc_b_fifo_produce_prepare (q, &op);
  bl_u8*    data;
  bl_err err;
  do {
    err = bl_mpmc_b_fifo_produce_prepare_is_ready (q, op, &data);
  }
  while (err.bl == bl_would_overflow);
  if (!err.bl) {
    write_data_for_consumers (data, bl_mpmc_b_slot_payload (q));
    bl_mpmc_b_produce_commit (q, op);
  }
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_mpmc_b_fifo_produce_prepare (bl_mpmc_b* q, bl_mpmc_b_op* op);
/*------------------------------------------------------------------------------
  See "bl_mpmc_b_fifo_produce_prepare" explanation.

  Errors returned:

- bl_ok: Succes, "data" can be written and "mpmpc_ba_produce_commit" can
  be called.
- bl_locked: The queue is blocked for producers, you can safely fall back.
- bl_would_overflow: keep waiting for a consumer to release the entry.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_fifo_produce_prepare_is_ready(
    bl_mpmc_b* q, bl_mpmc_b_op op, bl_u8** data
    );
/*-----------------------------------------------------------------------------
See "bl_mpmc_b_produce_prepare*" snippet
-----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_mpmc_b_produce_commit (bl_mpmc_b*q, bl_mpmc_b_op op);
/*------------------------------------------------------------------------------
The interface is exactly the same as on the "bl_mpmc_b_produce_prepare*" funtion
call variants but consuming instead.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_consume_prepare_sig_fallback(
    bl_mpmc_b*    q,
    bl_mpmc_b_op* op,
    bl_u8**       data,
    bool          replace_sig,
    bl_mpmc_b_sig sig_replacement,
    bl_mpmc_b_sig sig_fallback_mask,
    bl_mpmc_b_sig sig_fallback_match
    );
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_b_consume_prepare_fallback(
  bl_mpmc_b*    q,
  bl_mpmc_b_op* op,
  bl_u8**       data,
  bl_mpmc_b_sig sig_fallback_mask,
  bl_mpmc_b_sig sig_fallback_match
  )
{
  return bl_mpmc_b_consume_prepare_sig_fallback(
    q, op, data, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_b_consume_prepare_sig(
  bl_mpmc_b* q, bl_mpmc_b_op* op, bl_u8** data, bl_mpmc_b_sig sig_replacement
  )
{
  return bl_mpmc_b_consume_prepare_sig_fallback(
    q, op, data, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_b_consume_prepare(
  bl_mpmc_b* q, bl_mpmc_b_op*     op, bl_u8** data
  )
{
  return bl_mpmc_b_consume_prepare_sig_fallback (q, op, data, false, 0, 0, 1);
}
/*------------------------------------------------------------------------------
The same as "bl_mpmc_b_consume_prepare*" but for a single producer. Remember
that you can't mix modes in the same queue.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT bl_err  bl_mpmc_b_consume_prepare_sc(
  bl_mpmc_b* q, bl_mpmc_b_op* op, bl_u8** data
  );
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  void bl_mpmc_b_consume_commit (bl_mpmc_b*q, bl_mpmc_b_op info);
/*-----------------------------------------------------------------------------
  Can only be used on MPSC or MPMC mode. The interface
  is similar to the CAS interface on the C11 atomic library. The value is only
  changed if "expected" matches. On return "expected" always has the previous
  value. If expected didn't match the error code "bl_preconditions" is returned
  and the modification is not done.

  The error "bl_preconditions" will be returned in case that "expected" didn't
  match. It is up to the user to retry or not.
-----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_producer_signal_try_set(
    bl_mpmc_b* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
    );
/*-----------------------------------------------------------------------------
 Same as the above but the last successful ticket has to match too. If
 the queue is unitialized the last successful ticket is uint_max;
-----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_producer_signal_try_set_tmatch(
    bl_mpmc_b* q, bl_mpmc_b_ticket* expected, bl_mpmc_b_sig desired
    );
/*-----------------------------------------------------------------------------
  Same as above but for consumers. Can only be used on MPMC or SPMC mode
-----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_consumer_signal_try_set(
    bl_mpmc_b* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
    );
/*--------------------------- ------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_b_consumer_signal_try_set_tmatch(
    bl_mpmc_b* q, bl_mpmc_b_ticket* expected, bl_mpmc_b_sig desired
    );
/*--------------------------- ------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCKMC_H__ */
