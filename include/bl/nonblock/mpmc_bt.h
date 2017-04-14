/*---------------------------------------------------------------------------*/
#ifndef __BL_NONBLOCKMC_BT_H__
#define __BL_NONBLOCKMC_BT_H__
/*---------------------------------------------------------------------------*/
#include <bl/nonblock/libexport.h>
#include <bl/base/platform.h>

#include <bl/nonblock/mpmc_b.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
/* mpmc_b but for types. The "high-level" version */
/*----------------------------------------------------------------------------*/
typedef struct mpmc_bt {
  u32    sizeof_type;
  mpmc_b q;
}
mpmc_bt;
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_init(
  mpmc_bt*         q,
  alloc_tbl const* alloc,
  u32              slot_count,
  u32              data_size,     /* use: sizeof (your type)*/
  u32              data_alignment /* use: bl_alignof (your type)*/
  )
{
  q->sizeof_type = data_size;
  return mpmc_b_init (&q->q, alloc, slot_count, data_size, data_alignment);
}
/*----------------------------------------------------------------------------*/
static inline void mpmc_bt_destroy (mpmc_bt* q, alloc_tbl const* alloc)
{
  mpmc_b_destroy (&q->q, alloc);
}
/*------------------------------------------------------------------------------
 Inserts to the queue on multiple producer (MP) mode.
op: when there is no error will contain the current ticket id
  and the signal that was present on the producer's "channel". On return of
  the "bl_preconditions" error code the signal will be the signal that was
  present on the channel but the ticket will be the ticket of the
  next insertion (which should be ignored). On return of another error code
  the value will be left untouched.
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
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bt_produce_sig_fallback(
    mpmc_bt*    q,
    mpmc_b_op*  op,
    void const* data,
    bool        replace_sig,
    mpmc_b_sig  sig_replacement,
    mpmc_b_sig  sig_fallback_mask,
    mpmc_b_sig  sig_fallback_match
    );
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_produce_sig(
  mpmc_bt*    q,
  mpmc_b_op*  op,
  void const* data,
  mpmc_b_sig  sig_replacement
  )
{
  return mpmc_bt_produce_sig_fallback(
    q, op, data, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_produce_fallback(
  mpmc_bt*    q,
  mpmc_b_op*  op,
  void const* data,
  mpmc_b_sig  sig_fallback_mask,
  mpmc_b_sig  sig_fallback_match
  )
{
  return mpmc_bt_produce_sig_fallback(
    q, op, data, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_produce(
  mpmc_bt* q, mpmc_b_op* op, void const* data
  )
{
  return mpmc_bt_produce_sig_fallback (q, op, data, false, 0, 0, 1);
}
/*------------------------------------------------------------------------------
 Single producer (SP) mode insert. Remember that you can't mix single and
 multiple producer modes on the same queue.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bt_produce_sp (mpmc_bt* q, mpmc_b_op* op, void const* data);
/*------------------------------------------------------------------------------
 Comsumes from the queue on multiple consumer (MC) mode.
 op: when there is no error will contain the current ticket id
  and the signal that was present on the producer's "channel". On return of
  the "bl_preconditions" error code the signal will be the signal that was
  present on the channel but the ticket will be the ticket of the
  next insertion (which should be ignored). On return of another error code
  the value will be left untouched.
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
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bt_consume_sig_fallback(
    mpmc_bt*   q,
    mpmc_b_op* op,
    void*      data,
    bool       replace_sig,
    mpmc_b_sig sig_replacement,
    mpmc_b_sig sig_fallback_mask,
    mpmc_b_sig sig_fallback_match
    );
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_consume_fallback(
  mpmc_bt*   q,
  mpmc_b_op* op,
  void*      data,
  mpmc_b_sig sig_fallback_mask,
  mpmc_b_sig sig_fallback_match
  )
{
  return mpmc_bt_consume_sig_fallback(
    q, op, data, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_consume_sig(
  mpmc_bt*   q,
  mpmc_b_op* op,
  void*      data,
  mpmc_b_sig sig_replacement
  )
{
  return mpmc_bt_consume_sig_fallback(
    q, op, data, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_consume(
  mpmc_bt*   q,
  mpmc_b_op* op,
  void*      data
  )
{
  return mpmc_bt_consume_sig_fallback (q, op, data, false, 0, 0, 1);
}
/*------------------------------------------------------------------------------
 Single consumer (SC) mode insert. Remember that you can't mix single and
 multiple consumer modes on the same queue.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err mpmc_bt_consume_sc (mpmc_bt* q, mpmc_b_op* op, void* data);
/*------------------------------------------------------------------------------
  Can only be used on multiple producer mode. The interface is similar to the
  CAS interface on the C11 atomic library. The data is only changed if expected
  matches. On return expected always has the previous data. If expected didn't
  match the error code "bl_preconditions" is returned.
  The error "bl_preconditions" will be returned in case that "expected" didn't
  match. It is up to the user to retry or not.
------------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_producer_signal_try_set(
  mpmc_bt* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_producer_signal_try_set (&q->q, expected, desired);
}
/*------------------------------------------------------------------------------
 Same as the above but the last successful ticket has to match too. If
 the queue is unitialized the last successful ticket is uint_max;
------------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_producer_signal_try_set_tmatch(
  mpmc_bt* q, mpmc_b_op* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_producer_signal_try_set_tmatch (&q->q, expected, desired);
}
/*-----------------------------------------------------------------------------
  Same as above but for consumers
------------------------------------------------------------------------------*/
static inline bl_err mpmc_bt_consumer_signal_try_set(
  mpmc_bt* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_consumer_signal_try_set (&q->q, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
static inline bl_err mpmc_bt_consumer_signal_try_set_tmatch(
    mpmc_bt* q, mpmc_b_op* expected, mpmc_b_sig desired
    )
{
  return mpmc_b_consumer_signal_try_set_tmatch (&q->q, expected, desired);
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCKMC_BT_H__ */

