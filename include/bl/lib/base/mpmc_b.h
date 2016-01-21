/*---------------------------------------------------------------------------*/
#ifndef __BL_MPMC_B_H__
#define __BL_MPMC_B_H__
/*---------------------------------------------------------------------------*/
#include <assert.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/libexport.h>
#include <bl/hdr/base/cache.h>
#include <bl/hdr/base/alignment.h>
#include <bl/hdr/base/allocator.h>
#include <bl/hdr/base/error.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/atomic.h>
/*---------------------------------------------------------------------------*/
/*
  This is the Dimitry Dyukov bounded MPMC queue with some modifications:

    -Can be broken to MPMC SPMC MPSC. If you now that there just is one
      reader/writer there is no need for CAS.
    -Returns a transaction id (that was already available for free).
    -Takes 8 bytes from the enqueue/dequeue counter to use the CAS operation as
     a rudimentary external synchronization mechanism.
*/
/*---------------------------------------------------------------------------*/
#if defined(BL32)
  #define mpmc_b_info_signal_bits 8
#elif defined(BL64)
  #define mpmc_b_info_signal_bits 32
#endif

#define mpmc_b_info_transaction_bits\
  (type_bits (uword) - mpmc_b_info_signal_bits)
typedef u8 mpmc_b_sig;
/*---------------------------------------------------------------------------*/
typedef struct mpmc_b_info {
  uword transaction : mpmc_b_info_transaction_bits;
  uword signal      : mpmc_b_info_signal_bits;
}
mpmc_b_info;
/*---------------------------------------------------------------------------*/
/*mpmc_b = mpmc bounded*/
typedef struct mpmc_b {
  declare_cache_pad_member;
  u8*                       buffer;
  uword                     buffer_mask;
  uword                     join_size;
  uword                     contained_size;
  uword                     gate_offset;
  declare_cache_pad_member;
  atomic_uword              i_producer;
  declare_cache_pad_member;
  atomic_uword              i_consumer;
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
extern void BL_EXPORT mpmc_b_destroy (mpmc_b* q, const alloc_tbl* alloc);
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
extern bl_err BL_EXPORT mpmc_b_produce_sig_fallback(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  const void*  value,
  bool         replace_sig,
  mpmc_b_sig   sig_replacement,
  mpmc_b_sig   sig_fallback_mask,
  mpmc_b_sig   sig_fallback_match
  );
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_b_produce_sig(
  mpmc_b*      q,
  mpmc_b_info* info_out,
  const void*  value,
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
  const void*  value,
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
  const void*  value
  )
{
  return mpmc_b_produce_sig_fallback (q, info_out, value, false, 0, 0, 1);
}
/*-----------------------------------------------------------------------------
 Single producer (SP) mode insert. Remember that you can't mix single and 
 multiple producer modes on the same queue.
-----------------------------------------------------------------------------*/
extern bl_err BL_EXPORT mpmc_b_produce_single_p(
  mpmc_b* q, mpmc_b_info* info_out, const void* value
  );
/*------------------------------------------------------------------------------
 Inserts to the queue on multiple consumer (MC) mode.
 
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
------------------------------------------------------------------------------*/
extern bl_err BL_EXPORT mpmc_b_consume_sig_fallback(
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
extern bl_err BL_EXPORT mpmc_b_consume_single_c(
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
extern bl_err BL_EXPORT mpmc_b_producer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  );
/*-----------------------------------------------------------------------------
 Same as the above but the transaction has to match too
-----------------------------------------------------------------------------*/
extern bl_err BL_EXPORT mpmc_b_producer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_info* expected, mpmc_b_sig desired
  );
/*-----------------------------------------------------------------------------
  Same as above but for consumers
-----------------------------------------------------------------------------*/
extern bl_err BL_EXPORT mpmc_b_consumer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  );
/*--------------------------- ------------------------------------------------*/
extern bl_err BL_EXPORT mpmc_b_consumer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_info* expected, mpmc_b_sig desired
  );
/*--------------------------- ------------------------------------------------*/
extern bl_err BL_EXPORT mpmc_b_init_private(
  mpmc_b*          q,
  const alloc_tbl* alloc,
  uword            buffer_size,
  uword            join_size,
  uword            contained_size,
  uword            contained_offset
  );
/*----------------------------------------------------------------------------*/
#endif /* __BL_MPMC_H__ */
