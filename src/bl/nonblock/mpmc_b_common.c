#include <bl/nonblock/libexport.h>
#include <bl/nonblock/mpmc_b_common_priv.h>
#include <bl/base/error.h>
#include <bl/base/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
bl_err mpmc_b_signal_try_set(
  atomic_u32* dst, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  mpmc_b_op r;
  mpmc_b_op w;
  bl_err err = bl_mkok();
  r          = atomic_u32_load_rlx (dst);
  do {
    if (mpmc_b_sig_decode (r) != *expected) {
      err = bl_mkerr (bl_preconditions);
      break;
    }
    w = mpmc_b_op_encode (r, desired);
  }
  while (!atomic_u32_weak_cas_rlx (dst, &r, w));
  *expected = mpmc_b_sig_decode (r);
  return err;
}
/*----------------------------------------------------------------------------*/
bl_err mpmc_b_signal_try_set_tmatch(
  atomic_u32* dst, mpmc_b_op* expected, mpmc_b_sig desired
  )
{
  mpmc_b_op r;
  mpmc_b_op w;
  mpmc_b_op exp;

  bl_err err = bl_mkerr (bl_preconditions);
  exp        = mpmc_b_op_encode (*expected + 1, mpmc_b_sig_decode (*expected));
  r = atomic_u32_load_rlx (dst);
  if (r != exp) {
    goto save_expected;
  }
  w = mpmc_b_op_encode (r, desired);
  if (atomic_u32_weak_cas_rlx (dst, &r, w)) {
    err = bl_mkok();
  }
save_expected:
  *expected = mpmc_b_op_encode (r - 1, mpmc_b_sig_decode (r));
  return err;
}
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif
