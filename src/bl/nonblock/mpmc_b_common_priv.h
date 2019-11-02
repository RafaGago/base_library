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
static inline bl_u8*
  slot_addr (bl_u8* mem, bl_u32 slots, bl_u32 slot_size, bl_mpmc_b_ticket t)
{
  return &mem[(t & (slots - 1)) * slot_size];
}
/*----------------------------------------------------------------------------*/
extern bl_err bl_mpmc_b_signal_try_set(
  bl_atomic_u32* dst, bl_mpmc_b_sig* expected, bl_mpmc_b_sig desired
  );
/*----------------------------------------------------------------------------*/
extern bl_err bl_mpmc_b_signal_try_set_tmatch(
  bl_atomic_u32* dst, bl_mpmc_b_ticket* expected, bl_mpmc_b_sig desired
  );
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif