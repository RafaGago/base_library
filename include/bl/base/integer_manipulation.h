#ifndef BL_INTEGER_MANIPULATION_H
#define BL_INTEGER_MANIPULATION_H

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>

#define bl_has_one_or_zero_bits_set(x) (!((x) & ((x) - 1)))
/*---------------------------------------------------------------------------*/
#define bl_utype_bit(lsb_offset, type)\
          (((type) 1) << ((type) (lsb_offset)))
#define bl_utype_bitv(val, lsb_offset, type)\
          (((val) & ((type) 1)) << ((type) (lsb_offset)))
#define bl_utype_lsb_set(bit_count, type)\
          ((((type) 1) << ((type) (bit_count))) - ((type) 1))
#define bl_utype_msb_set(bit_count, type)\
          ((type) (~(bl_utype_max (type) >> ((type) (bit_count)))))
#define bl_utype_lsb_set_shift(bytes_set, from_lsb_shift_bits, type)\
          (bl_utype_lsb_set \
            ((bytes_set), type) << ((type) (from_lsb_shift_bits)) \
            )
#define bl_utype_lsb_shift(val, lsb_offset, lsb_bit_count, type)\
          (((val) & bl_utype_lsb_set ((lsb_bit_count), type)) <<\
              ((type) (lsb_offset)))
#define bl_utype_get_bit(var, lsb_offset, type)\
          ((((type) var) >> (lsb_offset)) & ((type) 1))
#define bl_utype_var_set_bitv(var, lsb_offset, val, type)\
          (((var) & ~bl_utype_bit ((lsb_offset), type)) |\
           bl_utype_bitv ((val), (lsb_offset), type))
/*the first LSB that is clear, e.g on 1111001 would return 0000010*/
#define bl_utype_first_clear_lsb_mask(var, type)\
          ((type) (~((type) var) & (((type) var) + 1)))
/*the first LSB that is set, e.g on 1111001 would return 0000001*/
#define bl_utype_first_set_lsb_mask(var, type)\
          (((type) var) & -((type) var))
/*---------------------------------------------------------------------------*/
#define bl_u_bit(lsb_offset)\
          bl_utype_bit ((lsb_offset), bl_uword)
#define bl_u_bitv(val, lsb_offset)\
          bl_utype_bitv ((val), (lsb_offset), bl_uword)
#define bl_u_lsb_set(bit_count)\
          bl_utype_lsb_set ((bit_count), bl_uword)
#define bl_u_msb_set(bit_count)\
          bl_utype_msb_set ((bit_count), bl_uword)
#define bl_u_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          bl_utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), bl_uword)
#define bl_u_lsb_shift(val, lsb_offset, lsb_bit_count)\
          bl_utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), bl_uword)
#define bl_u_get_bit(var, lsb_offset)\
          bl_utype_get_bit ((var), (lsb_offset), bl_uword)
#define bl_u_var_set_bitv(var, lsb_offset, val)\
          bl_utype_var_set_bitv ((var), (lsb_offset), (val), bl_uword)
#define bl_u_first_clear_lsb_mask(var)\
          bl_utype_first_clear_lsb_mask ((var), bl_uword)
#define bl_u_first_set_lsb_mask(var)\
           bl_utype_first_set_lsb_mask ((var), bl_uword)
/*---------------------------------------------------------------------------*/
#define bl_u8_bit(lsb_offset)\
          bl_utype_bit ((lsb_offset), bl_u8)
#define bl_u8_bitv(val, lsb_offset)\
          bl_utype_bitv ((val), (lsb_offset), bl_u8)
#define bl_u8_lsb_set(bit_count)\
          bl_utype_lsb_set ((bit_count), bl_u8)
#define bl_u8_msb_set(bit_count)\
          bl_utype_msb_set ((bit_count), bl_u8)
#define bl_u8_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          bl_utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), bl_u8)
#define bl_u8_lsb_shift(val, lsb_offset, lsb_bit_count)\
          bl_utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), bl_u8)
#define bl_u8_get_bit(var, lsb_offset)\
          bl_utype_get_bit ((var), (lsb_offset), bl_u8)
#define bl_u8_var_set_bitv(var, lsb_offset, val)\
          bl_utype_var_set_bitv ((var), (lsb_offset), (val), bl_u8)
#define bl_u8_first_clear_lsb_mask(var)\
          bl_utype_first_clear_lsb_mask ((var), bl_u8)
#define bl_u8_first_set_lsb_mask(var)\
           bl_utype_first_set_lsb_mask ((var), bl_u8)
/*---------------------------------------------------------------------------*/
#define bl_u16_bit(lsb_offset)\
          bl_utype_bit ((lsb_offset), bl_u16)
#define bl_u16_bitv(val, lsb_offset)\
          bl_utype_bitv ((val), (lsb_offset), bl_u16)
#define bl_u16_lsb_set(bit_count)\
          bl_utype_lsb_set ((bit_count), bl_u16)
#define bl_u16_msb_set(bit_count)\
          bl_utype_msb_set ((bit_count), bl_u16)
#define bl_u16_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          bl_utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), bl_u16)
#define bl_u16_lsb_shift(val, lsb_offset, lsb_bit_count)\
          bl_utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), bl_u16)
#define bl_u16_get_bit(var, lsb_offset)\
          bl_utype_get_bit ((var), (lsb_offset), bl_u16)
#define bl_u16_var_set_bitv(var, lsb_offset, val)\
          bl_utype_var_set_bitv ((var), (lsb_offset), (val), bl_u16)
#define bl_u16_first_clear_lsb_mask(var)\
          bl_utype_first_clear_lsb_mask ((var), bl_u16)
#define bl_u16_first_set_lsb_mask(var)\
           bl_utype_first_set_lsb_mask ((var), bl_u16)
/*---------------------------------------------------------------------------*/
#define bl_u32_bit(lsb_offset)\
          bl_utype_bit ((lsb_offset), bl_u32)
#define bl_u32_bitv(val, lsb_offset)\
          bl_utype_bitv ((val), (lsb_offset), bl_u32)
#define bl_u32_lsb_set(bit_count)\
          bl_utype_lsb_set ((bit_count), bl_u32)
#define bl_u32_msb_set(bit_count)\
          bl_utype_msb_set ((bit_count), bl_u32)
#define bl_u32_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          bl_utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), bl_u32)
#define bl_u32_lsb_shift(val, lsb_offset, lsb_bit_count)\
          bl_utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), bl_u32)
#define bl_u32_get_bit(var, lsb_offset)\
          bl_utype_get_bit ((var), (lsb_offset), bl_u32)
#define bl_u32_var_set_bitv(var, lsb_offset, val)\
          bl_utype_var_set_bitv ((var), (lsb_offset), (val), bl_u32)
#define bl_u32_first_clear_lsb_mask(var)\
          bl_utype_first_clear_lsb_mask ((var), bl_u32)
#define bl_u32_first_set_lsb_mask(var)\
           bl_utype_first_set_lsb_mask ((var), bl_u32)
/*---------------------------------------------------------------------------*/
#define bl_u64_bit(lsb_offset)\
          bl_utype_bit ((lsb_offset), bl_u64)
#define bl_u64_bitv(val, lsb_offset)\
          bl_utype_bitv ((val), (lsb_offset), bl_u64)
#define bl_u64_lsb_set(bit_count)\
          bl_utype_lsb_set ((bit_count), bl_u64)
#define bl_u64_msb_set(bit_count)\
          bl_utype_msb_set ((bit_count), bl_u64)
#define bl_u64_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          bl_utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), bl_u64)
#define bl_u64_lsb_shift(val, lsb_offset, lsb_bit_count)\
          bl_utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), bl_u64)
#define bl_u64_get_bit(var, lsb_offset)\
          bl_utype_get_bit ((var), (lsb_offset), bl_u64)
#define bl_u64_var_set_bitv(var, lsb_offset, val)\
          bl_utype_var_set_bitv ((var), (lsb_offset), (val), bl_u64)
#define bl_u64_first_clear_lsb_mask(var)\
          bl_utype_first_clear_lsb_mask ((var), bl_u64)
#define bl_u64_first_set_lsb_mask(var)\
           bl_utype_first_set_lsb_mask ((var), bl_u64)
/*---------------------------------------------------------------------------*/
#define bl_read_u64_low_mem_to_lsb(byte_ptr)\
  (\
    (((bl_u64) (byte_ptr)[0]) << (0 * 8)) |\
    (((bl_u64) (byte_ptr)[1]) << (1 * 8)) |\
    (((bl_u64) (byte_ptr)[2]) << (2 * 8)) |\
    (((bl_u64) (byte_ptr)[3]) << (3 * 8)) |\
    (((bl_u64) (byte_ptr)[4]) << (4 * 8)) |\
    (((bl_u64) (byte_ptr)[5]) << (5 * 8)) |\
    (((bl_u64) (byte_ptr)[6]) << (6 * 8)) |\
    (((bl_u64) (byte_ptr)[7]) << (7 * 8))\
  )
#define bl_read_u64_low_mem_to_msb(byte_ptr)\
  (\
    (((bl_u64) (byte_ptr)[0]) << (7 * 8)) |\
    (((bl_u64) (byte_ptr)[1]) << (6 * 8)) |\
    (((bl_u64) (byte_ptr)[2]) << (5 * 8)) |\
    (((bl_u64) (byte_ptr)[3]) << (4 * 8)) |\
    (((bl_u64) (byte_ptr)[4]) << (3 * 8)) |\
    (((bl_u64) (byte_ptr)[5]) << (2 * 8)) |\
    (((bl_u64) (byte_ptr)[6]) << (1 * 8)) |\
    (((bl_u64) (byte_ptr)[7]) << (0 * 8))\
  )
/*---------------------------------------------------------------------------*/
#define bl_read_u48_low_mem_to_lsb(byte_ptr)\
  (\
    (((bl_u64) (byte_ptr)[0]) << (0 * 8)) |\
    (((bl_u64) (byte_ptr)[1]) << (1 * 8)) |\
    (((bl_u64) (byte_ptr)[2]) << (2 * 8)) |\
    (((bl_u64) (byte_ptr)[3]) << (3 * 8)) |\
    (((bl_u64) (byte_ptr)[4]) << (4 * 8)) |\
    (((bl_u64) (byte_ptr)[5]) << (5 * 8))\
  )
#define bl_read_u48_low_mem_to_msb(byte_ptr)\
  (\
    (((bl_u64) (byte_ptr)[0]) << (5 * 8)) |\
    (((bl_u64) (byte_ptr)[1]) << (4 * 8)) |\
    (((bl_u64) (byte_ptr)[2]) << (3 * 8)) |\
    (((bl_u64) (byte_ptr)[3]) << (2 * 8)) |\
    (((bl_u64) (byte_ptr)[4]) << (1 * 8)) |\
    (((bl_u64) (byte_ptr)[5]) << (0 * 8))\
  )
/*---------------------------------------------------------------------------*/
#define bl_read_u32_low_mem_to_lsb(byte_ptr)\
  (\
    (((bl_u32) (byte_ptr)[0]) << (0 * 8)) |\
    (((bl_u32) (byte_ptr)[1]) << (1 * 8)) |\
    (((bl_u32) (byte_ptr)[2]) << (2 * 8)) |\
    (((bl_u32) (byte_ptr)[3]) << (3 * 8))\
  )
#define bl_read_u32_low_mem_to_msb(byte_ptr)\
  (\
    (((bl_u32) (byte_ptr)[0]) << (3 * 8)) |\
    (((bl_u32) (byte_ptr)[1]) << (2 * 8)) |\
    (((bl_u32) (byte_ptr)[2]) << (1 * 8)) |\
    (((bl_u32) (byte_ptr)[3]) << (0 * 8))\
  )
/*---------------------------------------------------------------------------*/
#define bl_read_u24_low_mem_to_lsb(byte_ptr)\
  (\
    (((bl_u32) (byte_ptr)[0]) << (0 * 8)) |\
    (((bl_u32) (byte_ptr)[1]) << (1 * 8)) |\
    (((bl_u32) (byte_ptr)[2]) << (2 * 8)) \
  )

#define bl_read_u24_low_mem_to_msb(byte_ptr)\
  (\
    (((bl_u32) (byte_ptr)[0]) << (2 * 8)) |\
    (((bl_u32) (byte_ptr)[1]) << (1 * 8)) |\
    (((bl_u32) (byte_ptr)[2]) << (0 * 8))\
  )
/*---------------------------------------------------------------------------*/
#define bl_read_u16_low_mem_to_lsb(byte_ptr)\
  (\
    (((bl_u16) (byte_ptr)[0]) << (0 * 8)) |\
    (((bl_u16) (byte_ptr)[1]) << (1 * 8)) \
  )

#define bl_read_u16_low_mem_to_msb(byte_ptr)\
  (\
    (((bl_u16) (byte_ptr)[0]) << (1 * 8)) |\
    (((bl_u16) (byte_ptr)[1]) << (0 * 8))\
  )
/*---------------------------------------------------------------------------*/
#define bl_read_u8(byte_ptr) ((byte_ptr)[0])
/*---------------------------------------------------------------------------*/
#define bl_write_u64_low_mem_to_lsb(byte_ptr, val64)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val64) >> (0 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val64) >> (1 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val64) >> (2 * 8));\
    (byte_ptr)[3] = (bl_u8) ((val64) >> (3 * 8));\
    (byte_ptr)[4] = (bl_u8) ((val64) >> (4 * 8));\
    (byte_ptr)[5] = (bl_u8) ((val64) >> (5 * 8));\
    (byte_ptr)[6] = (bl_u8) ((val64) >> (6 * 8));\
    (byte_ptr)[7] = (bl_u8) ((val64) >> (7 * 8));\
  }\
  while (0)

#define bl_init_u64_low_mem_to_lsb(val64)\
  (bl_u8) ((val64) >> (0 * 8)), (bl_u8) ((val64) >> (1 * 8)),\
  (bl_u8) ((val64) >> (2 * 8)), (bl_u8) ((val64) >> (3 * 8)),\
  (bl_u8) ((val64) >> (4 * 8)), (bl_u8) ((val64) >> (5 * 8)),\
  (bl_u8) ((val64) >> (6 * 8)), (bl_u8) ((val64) >> (7 * 8))

#define bl_write_u64_low_mem_to_msb(byte_ptr, val64)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val64) >> (7 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val64) >> (6 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val64) >> (5 * 8));\
    (byte_ptr)[3] = (bl_u8) ((val64) >> (4 * 8));\
    (byte_ptr)[4] = (bl_u8) ((val64) >> (3 * 8));\
    (byte_ptr)[5] = (bl_u8) ((val64) >> (2 * 8));\
    (byte_ptr)[6] = (bl_u8) ((val64) >> (1 * 8));\
    (byte_ptr)[7] = (bl_u8) ((val64) >> (0 * 8));\
  }\
  while (0)

#define bl_init_u64_low_mem_to_msb(val64)\
  (bl_u8) ((val64) >> (7 * 8)), (bl_u8) ((val64) >> (6 * 8)),\
  (bl_u8) ((val64) >> (5 * 8)), (bl_u8) ((val64) >> (4 * 8)),\
  (bl_u8) ((val64) >> (3 * 8)), (bl_u8) ((val64) >> (2 * 8)),\
  (bl_u8) ((val64) >> (1 * 8)), (bl_u8) ((val64) >> (0 * 8))
/*---------------------------------------------------------------------------*/
#define bl_write_u48_low_mem_to_lsb(byte_ptr, val48)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val48) >> (0 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val48) >> (1 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val48) >> (2 * 8));\
    (byte_ptr)[3] = (bl_u8) ((val48) >> (3 * 8));\
    (byte_ptr)[4] = (bl_u8) ((val48) >> (4 * 8));\
    (byte_ptr)[5] = (bl_u8) ((val48) >> (5 * 8));\
  }\
  while (0)

#define bl_init_u48_low_mem_to_lsb(val48)\
  (bl_u8) ((val48) >> (0 * 8)), (bl_u8) ((val48) >> (1 * 8)),\
  (bl_u8) ((val48) >> (2 * 8)), (bl_u8) ((val48) >> (3 * 8)),\
  (bl_u8) ((val48) >> (4 * 8)), (bl_u8) ((val48) >> (5 * 8))

#define bl_write_u48_low_mem_to_msb(byte_ptr, val48)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val48) >> (5 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val48) >> (4 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val48) >> (3 * 8));\
    (byte_ptr)[3] = (bl_u8) ((val48) >> (2 * 8));\
    (byte_ptr)[4] = (bl_u8) ((val48) >> (1 * 8));\
    (byte_ptr)[5] = (bl_u8) ((val48) >> (0 * 8));\
  }\
  while (0)

#define bl_init_u48_low_mem_to_msb(val48)\
  (bl_u8) ((val48) >> (5 * 8)), (bl_u8) ((val48) >> (4 * 8)),\
  (bl_u8) ((val48) >> (3 * 8)), (bl_u8) ((val48) >> (2 * 8)),\
  (bl_u8) ((val48) >> (1 * 8)), (bl_u8) ((val48) >> (0 * 8))
/*---------------------------------------------------------------------------*/
#define bl_write_u32_low_mem_to_lsb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val32) >> (0 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val32) >> (1 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val32) >> (2 * 8));\
    (byte_ptr)[3] = (bl_u8) ((val32) >> (3 * 8));\
  }\
  while (0)

#define bl_init_u32_low_mem_to_lsb(val32)\
  (bl_u8) ((val32) >> (0 * 8)), (bl_u8) ((val32) >> (1 * 8)),\
  (bl_u8) ((val32) >> (2 * 8)), (bl_u8) ((val32) >> (3 * 8))

#define bl_write_u32_low_mem_to_msb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val32) >> (3 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val32) >> (2 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val32) >> (1 * 8));\
    (byte_ptr)[3] = (bl_u8) ((val32) >> (0 * 8));\
  }\
  while (0)

#define bl_init_u32_low_mem_to_msb(val32)\
  (bl_u8) ((val32) >> (3 * 8)), (bl_u8) ((val32) >> (2 * 8)),\
  (bl_u8) ((val32) >> (1 * 8)), (bl_u8) ((val32) >> (0 * 8))

/*---------------------------------------------------------------------------*/
#define bl_write_u24_low_mem_to_lsb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val32) >> (0 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val32) >> (1 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val32) >> (2 * 8));\
  }\
  while (0)

#define bl_init_u24_low_mem_to_lsb(val32)\
  (bl_u8) ((val32) >> (0 * 8)), (bl_u8) ((val32) >> (1 * 8)), \
  (bl_u8) ((val32) >> (2 * 8))

#define bl_write_u24_low_mem_to_msb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val32) >> (2 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val32) >> (1 * 8));\
    (byte_ptr)[2] = (bl_u8) ((val32) >> (0 * 8));\
  }\
  while (0)

#define bl_init_u24_low_mem_to_msb(val32)\
  (bl_u8) ((val32) >> (2 * 8)), (bl_u8) ((val32) >> 8), (bl_u8) ((val32) >> 0)

/*---------------------------------------------------------------------------*/
#define bl_write_u16_low_mem_to_lsb(byte_ptr, val16)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val16) >> (0 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val16) >> (1 * 8));\
  }\
  while (0)

#define bl_init_u16_low_mem_to_lsb(val32)\
  (bl_u8) ((val32) >> (0 * 8)), (bl_u8) ((val32) >> (1 * 8))

#define bl_write_u16_low_mem_to_msb(byte_ptr, val16)\
  do {\
    (byte_ptr)[0] = (bl_u8) ((val16) >> (1 * 8));\
    (byte_ptr)[1] = (bl_u8) ((val16) >> (0 * 8));\
  }\
  while (0)

#define bl_init_u16_low_mem_to_msb(val32)\
  (bl_u8) ((val32) >> (1 * 8)), (bl_u8) ((val32) >> (0 * 8))
/*---------------------------------------------------------------------------*/
#define bl_write_u8(byte_ptr, val8) (byte_ptr)[0] = val8;

#define bl_init_u8(val8) (bl_u8) (val8)
/*-----------------------------------------------------------------------------
I used this de Bruijn sequence generator: www.hakank.org/comb/debruijn_arb.cgi
------------------------------------------------------------------------------*/
/*TODO: correct with BitScanForward on windows and the built-in functions on
  Linux*/
static inline bl_u8 bl_u8_get_bit_idx (bl_u8 non_zero_pow2)
{
  static const bl_u8 bruijn_seq              = 0x17;
  static const bl_u8 bruijn_seq_cycle_inv[8] = {
    0, 1, 2, 4, 7, 3, 6, 5,
  };
  bl_assert (non_zero_pow2 && bl_has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((bl_u8)(non_zero_pow2 * bruijn_seq)) >> 5];
}
/*-----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 16
static inline bl_u8 bl_u16_get_bit_idx (bl_u16 non_zero_pow2)
{
  static const bl_u16 bruijn_seq               = 0x0d2f;
  static const bl_u8  bruijn_seq_cycle_inv[16] = {
    0, 1, 8, 2, 6, 9, 3, 11, 15, 7, 5, 10, 14, 4, 13, 12,
  };
  bl_assert (non_zero_pow2 && bl_has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((bl_u16)(non_zero_pow2 * bruijn_seq)) >> 12];
}
#endif
/*-----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 32
static inline bl_u8 bl_u32_get_bit_idx (bl_u32 non_zero_pow2)
{
  static const bl_u32 bruijn_seq               = 0x0563937d;
  static const bl_u8  bruijn_seq_cycle_inv[32] = {
     0,  1,  2, 11, 17, 3, 20, 12, 30, 18,  4, 6,  8,  21, 13, 24,
    31, 10, 16, 19, 29, 5,  7, 23,  9, 15, 28, 22, 14, 27, 26, 25,
  };
  bl_assert (non_zero_pow2 && bl_has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((bl_u32)(non_zero_pow2 * bruijn_seq)) >> 27];
}
#endif
/*-----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline bl_u8 bl_u64_get_bit_idx (bl_u64 non_zero_pow2)
{
  static const bl_u64 bruijn_seq               = 0x30b37894769fab9;
  static const bl_u8  bruijn_seq_cycle_inv[64] = {
     0,  1,  8,  2, 25,  9,  3, 34, 61, 26, 29, 10,  4, 16, 35, 45,
    62, 32, 27, 43, 30, 53, 11, 55,  5, 13, 40, 17, 57, 36, 20, 46,
    63,  7, 24, 33, 60, 28, 15, 44, 31, 42, 52, 54, 12, 39, 56, 19,
     6, 23, 59, 14, 41, 51, 38, 18, 22, 58, 50, 37, 21, 49, 48, 47,
  };
  bl_assert (non_zero_pow2 && bl_has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((bl_u64)(non_zero_pow2 * bruijn_seq)) >> 58];
}
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  #define bl_u_get_bit_idx(v) bl_u8_get_bit_idx(v)
#endif
#if BL_WORDSIZE == 16
  #define bl_u_get_bit_idx(v) bl_u16_get_bit_idx(v)
#endif
#if BL_WORDSIZE == 32
  #define bl_u_get_bit_idx(v) bl_u32_get_bit_idx(v)
#endif
#if BL_WORDSIZE == 64
  #define bl_u_get_bit_idx(v) bl_u64_get_bit_idx(v)
#endif
/*---------------------------------------------------------------------------*/
/* "_be" suffix = big endian, "_le" suffix = little endian                   */
/*---------------------------------------------------------------------------*/

/*on gcc for non static calculations see __builtin_bswap16 32, ...*/

#ifdef BL_ARCH_BIG_ENDIAN
  #define bl_read_u64_be  bl_read_u64_low_mem_to_lsb
  #define bl_read_u64_le  bl_read_u64_low_mem_to_msb
  #define bl_write_u64_be bl_write_u64_low_mem_to_lsb
  #define bl_write_u64_le bl_write_u64_low_mem_to_msb
  #define bl_init_u64_be  bl_init_u64_low_mem_to_lsb
  #define bl_init_u64_le  bl_init_u64_low_mem_to_msb

  #define bl_read_u48_be  bl_read_u48_low_mem_to_lsb
  #define bl_read_u48_le  bl_read_u48_low_mem_to_msb
  #define bl_write_u48_be bl_write_u48_low_mem_to_lsb
  #define bl_write_u48_le bl_write_u48_low_mem_to_msb
  #define bl_init_u48_be  bl_init_u48_low_mem_to_lsb
  #define bl_init_u48_le  bl_init_u48_low_mem_to_msb

  #define bl_read_u32_be  bl_read_u32_low_mem_to_lsb
  #define bl_read_u32_le  bl_read_u32_low_mem_to_msb
  #define bl_write_u32_be bl_write_u32_low_mem_to_lsb
  #define bl_write_u32_le bl_write_u32_low_mem_to_msb
  #define bl_init_u32_be  bl_init_u32_low_mem_to_lsb
  #define bl_init_u32_le  bl_init_u32_low_mem_to_msb

  #define bl_read_u24_be  bl_read_u24_low_mem_to_lsb
  #define bl_read_u24_le  bl_read_u24_low_mem_to_msb
  #define bl_write_u24_be bl_write_u24_low_mem_to_lsb
  #define bl_write_u24_le bl_write_u24_low_mem_to_msb
  #define bl_init_u24_be  bl_init_u24_low_mem_to_lsb
  #define bl_init_u24_le  bl_init_u24_low_mem_to_msb

  #define bl_read_u16_be  bl_read_u16_low_mem_to_lsb
  #define bl_read_u16_le  bl_read_u16_low_mem_to_msb
  #define bl_write_u16_be bl_write_u16_low_mem_to_lsb
  #define bl_write_u16_le bl_write_u16_low_mem_to_msb
  #define bl_init_u16_be  bl_init_u16_low_mem_to_lsb
  #define bl_init_u16_le  bl_init_u16_low_mem_to_msb
#endif

#ifdef BL_ARCH_LITTLE_ENDIAN
  #define bl_read_u64_be  bl_read_u64_low_mem_to_msb
  #define bl_read_u64_le  bl_read_u64_low_mem_to_lsb
  #define bl_write_u64_be bl_write_u64_low_mem_to_msb
  #define bl_write_u64_le bl_write_u64_low_mem_to_lsb
  #define bl_init_u64_be  bl_init_u64_low_mem_to_msb
  #define bl_init_u64_le  bl_init_u64_low_mem_to_lsb

  #define bl_read_u48_be  bl_read_u48_low_mem_to_msb
  #define bl_read_u48_le  bl_read_u48_low_mem_to_lsb
  #define bl_write_u48_be bl_write_u48_low_mem_to_msb
  #define bl_write_u48_le bl_write_u48_low_mem_to_lsb
  #define bl_init_u48_be  bl_init_u48_low_mem_to_msb
  #define bl_init_u48_le  bl_init_u48_low_mem_to_lsb

  #define bl_read_u32_be  bl_read_u32_low_mem_to_msb
  #define bl_read_u32_le  bl_read_u32_low_mem_to_lsb
  #define bl_write_u32_be bl_write_u32_low_mem_to_msb
  #define bl_write_u32_le bl_write_u32_low_mem_to_lsb
  #define bl_init_u32_be  bl_init_u32_low_mem_to_msb
  #define bl_init_u32_le  bl_init_u32_low_mem_to_lsb

  #define bl_read_u24_be  bl_read_u24_low_mem_to_msb
  #define bl_read_u24_le  bl_read_u24_low_mem_to_lsb
  #define bl_write_u24_be bl_write_u24_low_mem_to_msb
  #define bl_write_u24_le bl_write_u24_low_mem_to_lsb
  #define bl_init_u24_be  bl_init_u24_low_mem_to_msb
  #define bl_init_u24_le  bl_init_u24_low_mem_to_lsb

  #define bl_read_u16_be  bl_read_u16_low_mem_to_msb
  #define bl_read_u16_le  bl_read_u16_low_mem_to_lsb
  #define bl_write_u16_be bl_write_u16_low_mem_to_msb
  #define bl_write_u16_le bl_write_u16_low_mem_to_lsb
  #define bl_init_u16_be  bl_init_u16_low_mem_to_msb
  #define bl_init_u16_le  bl_init_u16_low_mem_to_lsb
#endif

  /*these are just to keep the syntax when refactoring someting from or to bl_u8*/
  #define bl_read_u8_be  bl_read_u8
  #define bl_read_u8_le  bl_read_u8
  #define bl_write_u8_be bl_write_u8
  #define bl_write_u8_le bl_write_u8
  #define bl_init_u8_be  bl_init_u8
  #define bl_init_u8_le  bl_init_u8

/*---------------------------------------------------------------------------*/
#endif /*LGC_INTEGER_MANIPULATION_H*/
