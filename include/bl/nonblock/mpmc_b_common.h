#ifndef __BL_NONBLOCK_MPMC_B_COMMON_H__
#define __BL_NONBLOCK_MPMC_B_COMMON_H__

#include <bl/base/platform.h>
#include <bl/base/integer_math.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/utility.h>
#include <bl/base/assert.h>

typedef bl_u8  bl_mpmc_b_sig;
typedef bl_u32 bl_mpmc_b_ticket;
typedef bl_u32 bl_mpmc_b_op;

#define bl_mpmc_b_signal_bits 6
#define bl_mpmc_b_ticket_bits \
  (bl_type_bits (bl_u32) - bl_mpmc_b_signal_bits)

/* 1 bit is for regular queue wrapping (lap), the other one for blocking
   detection */
#define bl_mpmc_b_max_slots (bl_pow2_u32 (bl_mpmc_b_ticket_bits - 2))
/* this is the op value that will allow using "tmatch" suffixed functions on a
   newly initialized bl_mpmc_b queue */
#define bl_mpmc_b_first_op bl_u32_lsb_set (bl_mpmc_b_ticket_bits)

typedef struct bl_mpmc_b_op_up {
  bl_mpmc_b_ticket ticket;
  bl_mpmc_b_sig    sig;
}
bl_mpmc_b_op_up;
/*----------------------------------------------------------------------------*/
static inline bl_mpmc_b_ticket bl_mpmc_b_ticket_decode (bl_mpmc_b_op op)
{
  return (bl_mpmc_b_ticket) (op & bl_u32_lsb_set (bl_mpmc_b_ticket_bits));
}
/*----------------------------------------------------------------------------*/
static inline bl_mpmc_b_sig bl_mpmc_b_sig_decode (bl_mpmc_b_op op)
{
  return (bl_mpmc_b_sig) (op >> bl_mpmc_b_ticket_bits);
}
/*----------------------------------------------------------------------------*/
static inline bl_mpmc_b_op
  bl_mpmc_b_op_encode (bl_mpmc_b_ticket t, bl_mpmc_b_sig s)
{
  bl_assert (s <= bl_u32_lsb_set (bl_mpmc_b_signal_bits));
  return (t & bl_u32_lsb_set (bl_mpmc_b_ticket_bits)) |
    ((bl_u32) s) << bl_mpmc_b_ticket_bits;
}
/*----------------------------------------------------------------------------*/
static inline bl_mpmc_b_op_up bl_mpmc_b_op_unpack (bl_mpmc_b_op op)
{
  bl_mpmc_b_op_up ou;
  ou.ticket = bl_mpmc_b_ticket_decode (op);
  ou.sig    = bl_mpmc_b_sig_decode (op);
  return ou;
}
/*----------------------------------------------------------------------------*/
static inline bl_mpmc_b_op bl_mpmc_b_op_pack (bl_mpmc_b_op_up td)
{
  return bl_mpmc_b_op_encode (td.ticket, td.sig);
}
/*----------------------------------------------------------------------------*/
static inline bl_u32 bl_mpmc_b_round_slot_size (bl_u32 slot_size, bl_u32 alignment)
{
  bl_u32 al = bl_max (sizeof (bl_u32), alignment);
  //fixes alignment to 4 bytes
  slot_size += al - 1;
  slot_size /= al;
  return slot_size * al;
}
/*----------------------------------------------------------------------------*/
static inline bl_u32
  bl_mpmc_b_compute_payload (bl_u32 rounded_slot_size, bl_u32 alignment)
{
  return rounded_slot_size - bl_max (sizeof (bl_u32), alignment);
}
/*----------------------------------------------------------------------------*/
static inline bl_u32
  bl_mpmc_b_compute_slot_size (bl_u32 payload, bl_u32 alignment)
{
  return payload + bl_max (sizeof (bl_u32), alignment);
}
/*----------------------------------------------------------------------------*/
#endif
