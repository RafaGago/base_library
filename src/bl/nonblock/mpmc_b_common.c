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
  mpmc_b_ticket r;
  mpmc_b_ticket w;
  bl_err err = bl_ok;
  r          = atomic_u32_load_rlx (dst);
  do {
    if (mpmc_b_sig_decode (r) != *expected) {
      err = bl_preconditions;
      break;
    }
    w = mpmc_b_ticket_encode (r, desired);
  }
  while (!atomic_u32_weak_cas_rlx (dst, &r, w));
  *expected = mpmc_b_sig_decode (r);
  return err;
}
/*----------------------------------------------------------------------------*/
bl_err mpmc_b_signal_try_set_tmatch(
  atomic_u32* dst, mpmc_b_ticket* expected, mpmc_b_sig desired
  )
{
  mpmc_b_ticket r;
  mpmc_b_ticket w;
  mpmc_b_ticket exp;

  bl_err err = bl_preconditions;
  exp        = mpmc_b_ticket_encode(
    *expected + 1, mpmc_b_sig_decode (*expected)
    );
  r = atomic_u32_load_rlx (dst);
  if (r != exp) {
    goto save_expected;
  }
  w = mpmc_b_ticket_encode (r, desired);
  if (atomic_u32_weak_cas_rlx (dst, &r, w)) {
    err = bl_ok;
  }
save_expected:
  *expected = mpmc_b_ticket_encode (r - 1, mpmc_b_sig_decode (r));
  return err;
}
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif
