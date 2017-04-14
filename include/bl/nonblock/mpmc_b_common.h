#ifndef __BL_NONBLOCK_MPMC_B_COMMON_H__
#define __BL_NONBLOCK_MPMC_B_COMMON_H__

#include <bl/base/platform.h>
#include <bl/base/integer_math.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/utility.h>
#include <bl/base/assert.h>

typedef u8  mpmc_b_sig;
typedef u32 mpmc_b_ticket;

#define mpmc_b_signal_bits 6
#define mpmc_b_ticket_bits \
  (type_bits (u32) - mpmc_b_signal_bits)

/* 1 bit is for regular queue wrapping (lap), the other one for blocking
   detection */
#define mpmc_b_max_slots (pow2_u32 (mpmc_b_ticket_bits - 2))
#define mpmc_b_unset_ticket u32_lsb_set (mpmc_b_ticket_bits)

typedef struct mpmc_b_ticket_up {
  mpmc_b_ticket value;
  mpmc_b_sig    sig;
}
mpmc_b_ticket_up;
/*----------------------------------------------------------------------------*/
static inline mpmc_b_ticket mpmc_b_ticket_decode (mpmc_b_ticket t)
{
  return t & u32_lsb_set (mpmc_b_ticket_bits);
}
/*----------------------------------------------------------------------------*/
static inline mpmc_b_sig mpmc_b_sig_decode (mpmc_b_ticket t)
{
  return t >> mpmc_b_ticket_bits;
}
/*----------------------------------------------------------------------------*/
static inline mpmc_b_ticket mpmc_b_ticket_encode (mpmc_b_ticket t, mpmc_b_sig s)
{
  bl_assert (s <= u32_lsb_set (mpmc_b_signal_bits));
  return (t & u32_lsb_set (mpmc_b_ticket_bits)) |
    ((u32) s) << mpmc_b_ticket_bits;
}
/*----------------------------------------------------------------------------*/
static inline mpmc_b_ticket_up mpmc_b_ticket_unpack (mpmc_b_ticket t)
{
  mpmc_b_ticket_up tu;
  tu.value = mpmc_b_ticket_decode (t);
  tu.sig   = mpmc_b_sig_decode (t);
  return tu;
}
/*----------------------------------------------------------------------------*/
static inline mpmc_b_ticket mpmc_b_ticket_pack (mpmc_b_ticket_up td)
{
  return mpmc_b_ticket_encode (td.value, td.sig);
}
/*----------------------------------------------------------------------------*/
static inline u32 mpmc_b_round_slot_size (u32 slot_size, u32 alignment)
{
  u32 al = bl_max (sizeof (u32), alignment);
  //fixes alignment to 4 bytes
  slot_size += al - 1;
  slot_size /= al;
  return slot_size * al;
}
/*----------------------------------------------------------------------------*/
static inline u32 mpmc_b_compute_payload (u32 rounded_slot_size, u32 alignment)
{
  return rounded_slot_size - bl_max (sizeof (u32), alignment);
}
/*----------------------------------------------------------------------------*/
static inline u32 mpmc_b_compute_slot_size (u32 payload, u32 alignment)
{
  return payload + bl_max (sizeof (u32), alignment);
}
/*----------------------------------------------------------------------------*/
#endif
