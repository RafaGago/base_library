/*---------------------------------------------------------------------------*/
#include <bl/nonblock/libexport.h>
#include <bl/base/platform.h>

#include <bl/nonblock/mpmc_bt.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bt_produce_sig_fallback(
    bl_mpmc_bt*       q,
    bl_mpmc_b_ticket* ticket,
    void const*    data,
    bool           replace_sig,
    bl_mpmc_b_sig     sig,
    bl_mpmc_b_sig     sig_fmask,
    bl_mpmc_b_sig     sig_fmatch
    )
{
  bl_u8* slot;
  bl_err err = bl_mpmc_b_produce_prepare_sig_fallback(
    &q->q, ticket, &slot, replace_sig, sig, sig_fmask, sig_fmatch
    );
  if (bl_likely (!err.bl)) {
    memcpy (slot, data, q->sizeof_type);
    bl_mpmc_b_produce_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bt_produce_sp(
  bl_mpmc_bt* q, bl_mpmc_b_ticket* ticket, void const* data
  )
{
  bl_u8* slot;
  bl_err err = bl_mpmc_b_produce_prepare_sp (&q->q, ticket, &slot);
  if (bl_likely (!err.bl)) {
    memcpy (slot, data, q->sizeof_type);
    bl_mpmc_b_produce_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bt_consume_sig_fallback(
  bl_mpmc_bt*       q,
  bl_mpmc_b_ticket* ticket,
  void*          data,
  bool           replace_sig,
  bl_mpmc_b_sig     sig,
  bl_mpmc_b_sig     sig_fmask,
  bl_mpmc_b_sig     sig_fmatch
  )
{
  bl_u8* slot;
  bl_err err = bl_mpmc_b_consume_prepare_sig_fallback(
    &q->q, ticket, &slot, replace_sig, sig, sig_fmask, sig_fmatch
    );
  if (bl_likely (!err.bl)) {
    memcpy (data, slot, q->sizeof_type);
    bl_mpmc_b_consume_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpmc_bt_consume_sc(
  bl_mpmc_bt* q, bl_mpmc_b_ticket* ticket, void* data
  )
{
  bl_u8* slot;
  bl_err err = bl_mpmc_b_consume_prepare_sc (&q->q, ticket, &slot);
  if (bl_likely (!err.bl)) {
    memcpy (data, slot, q->sizeof_type);
    bl_mpmc_b_consume_commit (&q->q, *ticket);
  }
  return err;
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
