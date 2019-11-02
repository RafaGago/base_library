#include <bl/nonblock/libexport.h>
#include <bl/nonblock/mpmc_b_common_priv.h>
#include <bl/base/error.h>
#include <bl/base/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
bl_err bl_mpmc_b_signal_try_set(
  bl_atomic_u32* dst, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  )
{
  bl_mpmc_b_op r;
  bl_mpmc_b_op w;
  bl_err err = bl_mkok();
  r          = bl_atomic_u32_load_rlx (dst);
  do {
    if (bl_mpmc_b_sig_decode (r) != *expected) {
      err = bl_mkerr (bl_preconditions);
      break;
    }
    w = bl_mpmc_b_op_encode (r, desired);
  }
  while (!bl_atomic_u32_weak_cas_rlx (dst, &r, w));
  *expected = bl_mpmc_b_sig_decode (r);
  return err;
}
/*----------------------------------------------------------------------------*/
bl_err bl_mpmc_b_signal_try_set_tmatch(
  bl_atomic_u32* dst, bl_mpmc_b_op* expected, bl_mpmc_b_sig desired
  )
{
  bl_mpmc_b_op r;
  bl_mpmc_b_op w;
  bl_mpmc_b_op exp;

  bl_err err = bl_mkerr (bl_preconditions);
  exp = bl_mpmc_b_op_encode (*expected + 1, bl_mpmc_b_sig_decode (*expected));
  r = bl_atomic_u32_load_rlx (dst);
  if (r != exp) {
    goto save_expected;
  }
  w = bl_mpmc_b_op_encode (r, desired);
  if (bl_atomic_u32_weak_cas_rlx (dst, &r, w)) {
    err = bl_mkok();
  }
save_expected:
  *expected = bl_mpmc_b_op_encode (r - 1, bl_mpmc_b_sig_decode (r));
  return err;
}
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif
