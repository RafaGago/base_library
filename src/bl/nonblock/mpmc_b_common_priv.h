#ifndef __BL_NONBLOCK_MPMC_B_COMMON_PRIV_H__
#define __BL_NONBLOCK_MPMC_B_COMMON_PRIV_H__

#include <bl/base/atomic.h>
#include <bl/base/integer.h>
#include <bl/base/error.h>
#include <bl/nonblock/mpmc_b_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
static inline u8* slot_addr (u8* mem, u32 slots, u32 slot_size, mpmc_b_ticket t)
{
  return &mem[(t & (slots - 1)) * slot_size];
}
/*----------------------------------------------------------------------------*/
extern bl_err mpmc_b_signal_try_set(
  atomic_u32* dst, mpmc_b_sig* expected, mpmc_b_sig desired
  );
/*----------------------------------------------------------------------------*/
extern bl_err mpmc_b_signal_try_set_tmatch(
  atomic_u32* dst, mpmc_b_ticket* expected, mpmc_b_sig desired
  );
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif