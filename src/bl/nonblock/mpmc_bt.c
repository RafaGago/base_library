/*---------------------------------------------------------------------------*/
#include <bl/nonblock/libexport.h>
#include <bl/base/platform.h>

#include <bl/nonblock/mpmc_bt.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bt_produce_sig_fallback(
    mpmc_bt*       q,
    mpmc_b_ticket* ticket,
    void const*    data,
    bool           replace_sig,
    mpmc_b_sig     sig,
    mpmc_b_sig     sig_fmask,
    mpmc_b_sig     sig_fmatch
    )
{
  u8* slot;
  bl_err err = mpmc_b_produce_prepare_sig_fallback(
    &q->q, ticket, &slot, replace_sig, sig, sig_fmask, sig_fmatch
    );
  if (likely (!err)) {
    memcpy (slot, data, q->sizeof_type);
    mpmc_b_produce_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bt_produce_sp(
  mpmc_bt* q, mpmc_b_ticket* ticket, void const* data
  )
{
  u8* slot;
  bl_err err = mpmc_b_produce_prepare_sp (&q->q, ticket, &slot);
  if (likely (!err)) {
    memcpy (slot, data, q->sizeof_type);
    mpmc_b_produce_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bt_consume_sig_fallback(
  mpmc_bt*       q,
  mpmc_b_ticket* ticket,
  void*          data,
  bool           replace_sig,
  mpmc_b_sig     sig,
  mpmc_b_sig     sig_fmask,
  mpmc_b_sig     sig_fmatch
  )
{
  u8* slot;
  bl_err err = mpmc_b_consume_prepare_sig_fallback(
    &q->q, ticket, &slot, replace_sig, sig, sig_fmask, sig_fmatch
    );
  if (likely (!err)) {
    memcpy (data, slot, q->sizeof_type);
    mpmc_b_consume_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err mpmc_bt_consume_sc(
  mpmc_bt* q, mpmc_b_ticket* ticket, void* data
  )
{
  u8* slot;
  bl_err err = mpmc_b_consume_prepare_sc (&q->q, ticket, &slot);
  if (likely (!err)) {
    memcpy (data, slot, q->sizeof_type);
    mpmc_b_consume_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
