/*---------------------------------------------------------------------------*/
#ifndef __BL_NONBLOCK_MPMC_B_H__
#define __BL_NONBLOCK_MPMC_B_H__
/*---------------------------------------------------------------------------*/
#include <bl/nonblock/libexport.h>
#include <bl/base/platform.h>
#include <bl/base/cache.h>
#include <bl/base/alignment.h>
#include <bl/base/allocator.h>
#include <bl/base/error.h>
#include <bl/base/utility.h>
#include <bl/base/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
/*
  This is the Dimitry Dyukov bounded MPMC queue with some modifications:

    -Can be broken to MPMC SPMC MPSC SPSC. If you now that there just is one
     reader/writer there is no need for CAS. This makes it a swiss army knife
     type of queue. 

    -Returns a transaction id (that was already available for free).
    -Takes 8 bytes from the enqueue/dequeue counter to use the CAS operation as
     a rudimentary external synchronization mechanism.
*/
/*---------------------------------------------------------------------------*/ 

#define mpmc_b_info_signal_bits 8
typedef u8 mpmc_b_sig;
#define mpmc_b_info_transaction_bits\
  (type_bits (u32) - mpmc_b_info_signal_bits)
/*---------------------------------------------------------------------------*/
typedef struct mpmc_b_info {
  u32 transaction : mpmc_b_info_transaction_bits;
  u32 signal      : mpmc_b_info_signal_bits;
}
mpmc_b_info;
/*---------------------------------------------------------------------------*/
/*mpmc_b = mpmc bounded*/
typedef struct mpmc_b {
  declare_cache_pad_member;
  u8*                       buffer;
  u32                       buffer_mask;
  u32                       join_size;
  u32                       contained_size;
  u32                       gate_offset;
  declare_cache_pad_member;
  atomic_u32                i_producer;
  declare_cache_pad_member;
  atomic_u32                i_consumer;
  declare_cache_pad_member;
}
mpmc_b;
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns (sizeof (atomic_uword) == sizeof (mpmc_b_info));
/*---------------------------------------------------------------------------*/
#define mpmc_b_init(mpmc_b_ptr, alloc_tbl_ptr, buffer_size, type_contained)\
  mpmc_b_init_private(\
    (mpmc_b_ptr),\
    (alloc_tbl_ptr),\
    (buffer_size),\
    align_combined_size (atomic_uword, type_contained),\
    sizeof (type_contained),\
    align_combined_type2_offset (type_contained, u32)\
    )
/*---------------------------------------------------------------------------*/
extern void NONBLOCK_EXPORT mpmc_b_destroy (mpmc_b* q, alloc_tbl const* alloc);
/*------------------------------------------------------------------------------
 Inserts to the queue on multiple producer (MP) mode.
 
 info_out: will contain the current transaction id and the signal that was 
  present on the producer's "channel". On return of the "bl_preconditions"
  error code the signal will be the signal that was present on the channel but
  the transaction will be invalid.

 replace_sigl + sig_replacement: If "replace_signal" is "true" the signal
  value will be replaced on success with "signal_replacement". If 
  "replace_signal" is false "signal_replacement" is ignored.

 sig_fallback_mask + sig_fallback_match: The call will early fail with
  the "bl_preconditions" error code if signal present on the channel now
  matches the condition below:

 ("signal_on_channel_now" & sig_fallback_mask) == sig_fallback_match

 This can be used e.g. on termination contexts, the signal is set to a value
 that represents "on termination" and with an according mask + match the 
 producers will be blocked.
------------------------------------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_produce_sig_fallback(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void const*  value,
  bool         replace_sig,
  mpmc_b_sig   sig_replacement,
  mpmc_b_sig   sig_fallback_mask,
  mpmc_b_sig   sig_fallback_match
  );
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_b_produce_sig(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void const*  value,
  mpmc_b_sig   sig_replacement
  )
{
  return mpmc_b_produce_sig_fallback(
          q, info_out, value, true, sig_replacement, 0, 1
          );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_b_produce_fallback(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void const*  value,
  mpmc_b_sig   sig_fallback_mask,
  mpmc_b_sig   sig_fallback_match
  )
{
  return mpmc_b_produce_sig_fallback(
          q, info_out, value, false, 0, sig_fallback_mask, sig_fallback_match
          );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_b_produce(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void const*  value
  )
{
  return mpmc_b_produce_sig_fallback (q, info_out, value, false, 0, 0, 1);
}
/*-----------------------------------------------------------------------------
 Single producer (SP) mode insert. Remember that you can't mix single and 
 multiple producer modes on the same queue.
-----------------------------------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_produce_single_p(
  mpmc_b* q, mpmc_b_info* info_out, void const* value
  );
/*------------------------------------------------------------------------------
 Comsumes from the queue on multiple consumer (MC) mode.
 
 info_out: will contain the current transaction id and the signal that was 
  present on the consumer's "channel". On return of the "bl_preconditions"
  error code the signal will be the signal that was present on the channel but
  the transaction will be invalid.

 replace_sig + sig_replacement: If "replace_signal" is "true" the signal
  value will be replaced on success with "signal_replacement". If 
  "replace_signal" is false "signal_replacement" is ignored.

 sig_fallback_mask + sig_fallback_match: The call will early fail with
  the "bl_preconditions" error code if signal present on the channel now
  matches the condition below:

 ("signal_on_channel_now" & sig_fallback_mask) == sig_fallback_match

 This can be used e.g. on termination contexts, the signal is set to a value
 that represents "on termination" and with an according mask + match the 
 consumers will be blocked.

 The "bl_empty" error code will be returned when no data is available.
------------------------------------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_consume_sig_fallback(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void*        value,
  bool         replace_sig,
  mpmc_b_sig   sig_replacement,
  mpmc_b_sig   sig_fallback_mask,
  mpmc_b_sig   sig_fallback_match
  );
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_b_consume_fallback(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void*        value,
  mpmc_b_sig   sig_fallback_mask,
  mpmc_b_sig   sig_fallback_match
  )
{
  return mpmc_b_consume_sig_fallback(
          q, info_out, value, false, 0, sig_fallback_mask, sig_fallback_match
          );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_b_consume_sig(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void*        value,
  mpmc_b_sig   sig_replacement
  )
{
  return mpmc_b_consume_sig_fallback(
          q, info_out, value, true, sig_replacement, 0, 1
          );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_b_consume(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  void*        value
  )
{
  return mpmc_b_consume_sig_fallback (q, info_out, value, false, 0, 0, 1);
}
/*-----------------------------------------------------------------------------
 Single consumer (SC) mode insert. Remember that you can't mix single and 
 multiple consumer modes on the same queue.
-----------------------------------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_consume_single_c(
  mpmc_b* q, mpmc_b_info* info_out, void* value
  );
/*-----------------------------------------------------------------------------
  Can only be used on multiple producer mode. The interface is similar to the
  CAS interface on the C11 atomic library. The value is only changed if expected
  matches. On return expected always has the previous value. If expected didn't
  match the error code "bl_preconditions" is returned.

  The error "bl_preconditions" will be returned in case that "expected" didn't
  match. It is up to the user to retry or not.
-----------------------------------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_producer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  );
/*-----------------------------------------------------------------------------
 Same as the above but the transaction has to match too
-----------------------------------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_producer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_info* expected, mpmc_b_sig desired
  );
/*-----------------------------------------------------------------------------
  Same as above but for consumers
-----------------------------------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_consumer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  );
/*--------------------------- ------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_consumer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_info* expected, mpmc_b_sig desired
  );
/*--------------------------- ------------------------------------------------*/
extern bl_err NONBLOCK_EXPORT mpmc_b_init_private(
  mpmc_b*          q,
  alloc_tbl const* alloc,
  u32              buffer_size,
  u32              join_size,
  u32              contained_size,
  u32              contained_offset
  );
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCK_MPMC_H__ */
