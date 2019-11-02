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
/* bl_mpmc_b but for types. The "high-level" version */
/*----------------------------------------------------------------------------*/
typedef struct bl_mpmc_bt {
  bl_u32    sizeof_type;
  bl_mpmc_b q;
}
bl_mpmc_bt;
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_init(
  bl_mpmc_bt*         q,
  bl_alloc_tbl const* alloc,
  bl_u32              slot_count,
  bl_u32              data_size,     /* use: sizeof (your type)*/
  bl_u32              data_alignment /* use: bl_alignof (your type)*/
  )
{
  q->sizeof_type = data_size;
  return bl_mpmc_b_init (&q->q, alloc, slot_count, data_size, data_alignment);
}
/*----------------------------------------------------------------------------*/
static inline void bl_mpmc_bt_destroy (bl_mpmc_bt* q, bl_alloc_tbl const* alloc)
{
  bl_mpmc_b_destroy (&q->q, alloc);
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
  bl_err bl_mpmc_bt_produce_sig_fallback(
    bl_mpmc_bt*    q,
    bl_mpmc_b_op*  op,
    void const*    data,
    bool           replace_sig,
    bl_mpmc_b_sig  sig_replacement,
    bl_mpmc_b_sig  sig_fallback_mask,
    bl_mpmc_b_sig  sig_fallback_match
    );
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_produce_sig(
  bl_mpmc_bt*    q,
  bl_mpmc_b_op*  op,
  void const*    data,
  bl_mpmc_b_sig  sig_replacement
  )
{
  return bl_mpmc_bt_produce_sig_fallback(
    q, op, data, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_produce_fallback(
  bl_mpmc_bt*    q,
  bl_mpmc_b_op*  op,
  void const*    data,
  bl_mpmc_b_sig  sig_fallback_mask,
  bl_mpmc_b_sig  sig_fallback_match
  )
{
  return bl_mpmc_bt_produce_sig_fallback(
    q, op, data, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_produce(
  bl_mpmc_bt* q, bl_mpmc_b_op* op, void const* data
  )
{
  return bl_mpmc_bt_produce_sig_fallback (q, op, data, false, 0, 0, 1);
}
/*------------------------------------------------------------------------------
 Single producer (SP) mode insert. Remember that you can't mix single and
 multiple producer modes on the same queue.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_bt_produce_sp (bl_mpmc_bt* q, bl_mpmc_b_op* op, void const* data);
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
  bl_err bl_mpmc_bt_consume_sig_fallback(
    bl_mpmc_bt*   q,
    bl_mpmc_b_op* op,
    void*         data,
    bool          replace_sig,
    bl_mpmc_b_sig sig_replacement,
    bl_mpmc_b_sig sig_fallback_mask,
    bl_mpmc_b_sig sig_fallback_match
    );
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_consume_fallback(
  bl_mpmc_bt*   q,
  bl_mpmc_b_op* op,
  void*         data,
  bl_mpmc_b_sig sig_fallback_mask,
  bl_mpmc_b_sig sig_fallback_match
  )
{
  return bl_mpmc_bt_consume_sig_fallback(
    q, op, data, false, 0, sig_fallback_mask, sig_fallback_match
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_consume_sig(
  bl_mpmc_bt*   q,
  bl_mpmc_b_op* op,
  void*         data,
  bl_mpmc_b_sig sig_replacement
  )
{
  return bl_mpmc_bt_consume_sig_fallback(
    q, op, data, true, sig_replacement, 0, 1
    );
}
/*----------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_consume(
  bl_mpmc_bt*   q,
  bl_mpmc_b_op* op,
  void*         data
  )
{
  return bl_mpmc_bt_consume_sig_fallback (q, op, data, false, 0, 0, 1);
}
/*------------------------------------------------------------------------------
 Single consumer (SC) mode insert. Remember that you can't mix single and
 multiple consumer modes on the same queue.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT
  bl_err bl_mpmc_bt_consume_sc (bl_mpmc_bt* q, bl_mpmc_b_op* op, void* data);
/*------------------------------------------------------------------------------
  Can only be used on multiple producer mode. The interface is similar to the
  CAS interface on the C11 atomic library. The data is only changed if expected
  matches. On return expected always has the previous data. If expected didn't
  match the error code "bl_preconditions" is returned.
  The error "bl_preconditions" will be returned in case that "expected" didn't
  match. It is up to the user to retry or not.
------------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_producer_signal_try_set(
  bl_mpmc_bt* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_producer_signal_try_set (&q->q, expected, desired);
}
/*------------------------------------------------------------------------------
 Same as the above but the last successful ticket has to match too. If
 the queue is unitialized the last successful ticket is uint_max;
------------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_producer_signal_try_set_tmatch(
  bl_mpmc_bt* q, bl_mpmc_b_op* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_producer_signal_try_set_tmatch (&q->q, expected, desired);
}
/*-----------------------------------------------------------------------------
  Same as above but for consumers
------------------------------------------------------------------------------*/
static inline bl_err bl_mpmc_bt_consumer_signal_try_set(
  bl_mpmc_bt* q, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  )
{
  return bl_mpmc_b_consumer_signal_try_set (&q->q, expected, desired);
}
/*--------------------------- ------------------------------------------------*/
static inline bl_err bl_mpmc_bt_consumer_signal_try_set_tmatch(
    bl_mpmc_bt* q, bl_mpmc_b_op* expected, bl_mpmc_b_sig desired
    )
{
  return bl_mpmc_b_consumer_signal_try_set_tmatch (&q->q, expected, desired);
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCKMC_BT_H__ */

