#ifndef BL_INTEGER_MANIPULATION_H
#define BL_INTEGER_MANIPULATION_H

#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>

#define has_one_or_zero_bits_set(x) (!((x) & ((x) - 1)))
/*---------------------------------------------------------------------------*/
#define utype_bit(lsb_offset, type)\
          (((type) 1) << ((type) (lsb_offset)))
#define utype_bitv(val, lsb_offset, type)\
          (((val) & ((type) 1)) << ((type) (lsb_offset)))
#define utype_lsb_set(bit_count, type)\
          ((((type) 1) << ((type) (bit_count))) - ((type) 1))
#define utype_msb_set(bit_count, type)\
          ((type) (~(utype_max (type) >> ((type) (bit_count)))))
#define utype_lsb_set_shift(bytes_set, from_lsb_shift_bits, type)\
          (utype_lsb_set ((bytes_set), type) << ((type) (from_lsb_shift_bits)))
#define utype_lsb_shift(val, lsb_offset, lsb_bit_count, type)\
          (((val) & utype_lsb_set ((lsb_bit_count), type)) <<\
              ((type) (lsb_offset)))
#define utype_get_bit(var, lsb_offset, type)\
          ((((type) var) >> (lsb_offset)) & ((type) 1))
#define utype_var_set_bitv(var, lsb_offset, val, type)\
          ((var) &\
           ~utype_bit ((lsb_offset), type) |\
           utype_bitv ((val), (lsb_offset), type))
/*the first LSB that is clear, e.g on 1111001 would return 0000010*/
#define utype_first_clear_lsb_mask(var, type)\
          ((type) (~((type) var) & (((type) var) + 1)))
/*the first LSB that is set, e.g on 1111001 would return 0000001*/
#define utype_first_set_lsb_mask(var, type)\
          (((type) var) & -((type) var))
/*---------------------------------------------------------------------------*/
#define u_bit(lsb_offset)\
          utype_bit ((lsb_offset), uword)
#define u_bitv(val, lsb_offset)\
          utype_bitv ((val), (lsb_offset), uword)
#define u_lsb_set(bit_count)\
          utype_lsb_set ((bit_count), uword)
#define u_msb_set(bit_count)\
          utype_msb_set ((bit_count), uword)
#define u_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), uword)
#define u_lsb_shift(val, lsb_offset, lsb_bit_count)\
          utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), uword)
#define u_get_bit(var, lsb_offset)\
          utype_get_bit ((var), (lsb_offset), uword)
#define u_var_set_bitv(var, lsb_offset, val)\
          utype_var_set_bitv ((var), (lsb_offset), (val), uword)
#define u_first_clear_lsb_mask(var)\
          utype_first_clear_lsb_mask ((var), uword)
#define u_first_set_lsb_mask(var)\
           utype_first_set_lsb_mask ((var), uword)
/*---------------------------------------------------------------------------*/
#define u8_bit(lsb_offset)\
          utype_bit ((lsb_offset), u8)
#define u8_bitv(val, lsb_offset)\
          utype_bitv ((val), (lsb_offset), u8)
#define u8_lsb_set(bit_count)\
          utype_lsb_set ((bit_count), u8)
#define u8_msb_set(bit_count)\
          utype_msb_set ((bit_count), u8)
#define u8_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), u8)
#define u8_lsb_shift(val, lsb_offset, lsb_bit_count)\
          utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), u8)
#define u8_get_bit(var, lsb_offset)\
          utype_get_bit ((var), (lsb_offset), u8)
#define u8_var_set_bitv(var, lsb_offset, val)\
          utype_var_set_bitv ((var), (lsb_offset), (val), u8)
#define u8_first_clear_lsb_mask(var)\
          utype_first_clear_lsb_mask ((var), u8)
#define u8_first_set_lsb_mask(var)\
           utype_first_set_lsb_mask ((var), u8)
/*---------------------------------------------------------------------------*/
#define u16_bit(lsb_offset)\
          utype_bit ((lsb_offset), u16)
#define u16_bitv(val, lsb_offset)\
          utype_bitv ((val), (lsb_offset), u16)
#define u16_lsb_set(bit_count)\
          utype_lsb_set ((bit_count), u16)
#define u16_msb_set(bit_count)\
          utype_msb_set ((bit_count), u16)
#define u16_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), u16)
#define u16_lsb_shift(val, lsb_offset, lsb_bit_count)\
          utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), u16)
#define u16_get_bit(var, lsb_offset)\
          utype_get_bit ((var), (lsb_offset), u16)
#define u16_var_set_bitv(var, lsb_offset, val)\
          utype_var_set_bitv ((var), (lsb_offset), (val), u16)
#define u16_first_clear_lsb_mask(var)\
          utype_first_clear_lsb_mask ((var), u16)
#define u16_first_set_lsb_mask(var)\
           utype_first_set_lsb_mask ((var), u16)
/*---------------------------------------------------------------------------*/
#define u32_bit(lsb_offset)\
          utype_bit ((lsb_offset), u32)
#define u32_bitv(val, lsb_offset)\
          utype_bitv ((val), (lsb_offset) u32)
#define u32_lsb_set(bit_count)\
          utype_lsb_set ((bit_count), u32)
#define u32_msb_set(bit_count)\
          utype_msb_set ((bit_count), u32)
#define u32_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), u32)
#define u32_lsb_shift(val, lsb_offset, lsb_bit_count)\
          utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), u32)
#define u32_get_bit(var, lsb_offset)\
          utype_get_bit ((var), (lsb_offset), u32)
#define u32_var_set_bitv(var, lsb_offset, val)\
          utype_var_set_bitv ((var), (lsb_offset), (val), u32)
#define u32_first_clear_lsb_mask(var)\
          utype_first_clear_lsb_mask ((var), u32)
#define u32_first_set_lsb_mask(var)\
           utype_first_set_lsb_mask ((var), u32)
/*---------------------------------------------------------------------------*/
#define u64_bit(lsb_offset)\
          utype_bit ((lsb_offset), u64)
#define u64_bitv(val, lsb_offset)\
          utype_bitv ((val), (lsb_offset), u64)
#define u64_lsb_set(bit_count)\
          utype_lsb_set ((bit_count), u64)
#define u64_msb_set(bit_count)\
          utype_msb_set ((bit_count), u64)
#define u64_lsb_set_shift(bytes_set, from_lsb_shift_bits)\
          utype_lsb_set_shift ((bytes_set), (from_lsb_shift_bits), u64)
#define u64_lsb_shift(val, lsb_offset, lsb_bit_count)\
          utype_lsb_shift ((val), (lsb_offset), (lsb_bit_count), u64)
#define u64_get_bit(var, lsb_offset)\
          utype_get_bit ((var), (lsb_offset), u64)
#define u64_var_set_bitv(var, lsb_offset, val)\
          utype_var_set_bitv ((var), (lsb_offset), (val), u64)
#define u64_first_clear_lsb_mask(var)\
          utype_first_clear_lsb_mask ((var), u64)
#define u64_first_set_lsb_mask(var)\
           utype_first_set_lsb_mask ((var), u64)
/*---------------------------------------------------------------------------*/
#define read_u32_low_mem_to_lsb(byte_ptr)\
  (\
    (((u32) (byte_ptr)[0]) <<  0) |\
    (((u32) (byte_ptr)[1]) <<  8) |\
    (((u32) (byte_ptr)[2]) << 16) |\
    (((u32) (byte_ptr)[3]) << 24)\
  )           
#define read_u32_low_mem_to_msb(byte_ptr)\
  (\
    (((u32) (byte_ptr)[0]) << 24)|\
    (((u32) (byte_ptr)[1]) << 16) |\
    (((u32) (byte_ptr)[2]) <<  8) |\
    (((u32) (byte_ptr)[3]) <<  0)\
  )
/*---------------------------------------------------------------------------*/
#define read_u24_low_mem_to_lsb(byte_ptr)\
  (\
    (((u32) (byte_ptr)[0]) <<  0) |\
    (((u32) (byte_ptr)[1]) <<  8) |\
    (((u32) (byte_ptr)[2]) << 16) \
  ) 
  
#define read_u24_low_mem_to_msb(byte_ptr)\
  (\
    (((u32) (byte_ptr)[0]) << 16) |\
    (((u32) (byte_ptr)[1]) <<  8) |\
    (((u32) (byte_ptr)[2]) <<  0)\
  )
/*---------------------------------------------------------------------------*/
#define read_u16_low_mem_to_lsb(byte_ptr)\
  (\
    (((u16) (byte_ptr)[0]) <<  0) |\
    (((u16) (byte_ptr)[1]) <<  8) \
  )
  
#define read_u16_low_mem_to_msb(byte_ptr)\
  (\
    (((u16) (byte_ptr)[0]) <<  8) |\
    (((u16) (byte_ptr)[1]) <<  0)\
  )
/*---------------------------------------------------------------------------*/
#define write_u32_low_mem_to_lsb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (u8) ((val32) >>  0);\
    (byte_ptr)[1] = (u8) ((val32) >>  8);\
    (byte_ptr)[2] = (u8) ((val32) >> 16);\
    (byte_ptr)[3] = (u8) ((val32) >> 24);\
  }\
  while (0)

#define init_u32_low_mem_to_lsb(val32)\
  (u8) ((val32) >>  0), (u8) ((val32) >>  8),\
  (u8) ((val32) >> 16), (u8) ((val32) >> 24)\
  
#define write_u32_low_mem_to_msb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (u8) ((val32) >> 24);\
    (byte_ptr)[1] = (u8) ((val32) >> 16);\
    (byte_ptr)[2] = (u8) ((val32) >>  8);\
    (byte_ptr)[3] = (u8) ((val32) >>  0);\
  }\
  while (0)
  
#define init_u32_low_mem_to_msb(val32)\
  (u8) ((val32) >> 24), (u8) ((val32) >> 16),\
  (u8) ((val32) >> 8), (u8) ((val32) >> 0)\
  
/*---------------------------------------------------------------------------*/
#define write_u24_low_mem_to_lsb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (u8) ((val32) >>  0);\
    (byte_ptr)[1] = (u8) ((val32) >>  8);\
    (byte_ptr)[2] = (u8) ((val32) >> 16);\
  }\
  while (0)
  
#define init_u24_low_mem_to_lsb(val32)\
  (u8) ((val32) >>  0), (u8) ((val32) >>  8), (u8) ((val32) >> 16)
  
#define write_u24_low_mem_to_msb(byte_ptr, val32)\
  do {\
    (byte_ptr)[0] = (u8) ((val32) >> 16);\
    (byte_ptr)[1] = (u8) ((val32) >>  8);\
    (byte_ptr)[2] = (u8) ((val32) >>  0);\
  }\
  while (0)
  
#define init_u24_low_mem_to_msb(val32)\
  (u8) ((val32) >> 16), (u8) ((val32) >> 8), (u8) ((val32) >> 0)
  
/*---------------------------------------------------------------------------*/
#define write_u16_low_mem_to_lsb(byte_ptr, val16)\
  do {\
    (byte_ptr)[0] = (u8) ((val16) >>  0);\
    (byte_ptr)[1] = (u8) ((val16) >>  8);\
  }\
  while (0)
  
#define init_u16_low_mem_to_lsb(val32)\
  (u8) ((val32) >>  0), (u8) ((val32) >>  8)
  
#define write_u16_low_mem_to_msb(byte_ptr, val16)\
  do {\
    (byte_ptr)[0] = (u8) ((val16) >>  8);\
    (byte_ptr)[1] = (u8) ((val16) >>  0);\
  }\
  while (0)
  
#define init_u16_low_mem_to_msb(val32)\
  (u8) ((val32) >> 8), (u8) ((val32) >> 0)
/*-----------------------------------------------------------------------------
I used this de Bruijn sequence generator: www.hakank.org/comb/debruijn_arb.cgi
/*-----------------------------------------------------------------------------*/
/*TODO: correct with BitScanForward on windows and the built-in functions on
  Linux*/
static inline u8 u8_get_bit_idx (u8 non_zero_pow2)
{
  static const u8 bruijn_seq              = 0x17;
  static const u8 bruijn_seq_cycle_inv[8] = {
    0, 1, 2, 4, 7, 3, 6, 5,
  };
  bl_assert (non_zero_pow2 && has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((u8)(non_zero_pow2 * bruijn_seq)) >> 5];
}
/*-----------------------------------------------------------------------------*/
static inline u8 u16_get_bit_idx (u16 non_zero_pow2)
{
  static const u16 bruijn_seq               = 0x0d2f;
  static const u8  bruijn_seq_cycle_inv[16] = {
    0, 1, 8, 2, 6, 9, 3, 11, 15, 7, 5, 10, 14, 4, 13, 12,
  };
  bl_assert (non_zero_pow2 && has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((u16)(non_zero_pow2 * bruijn_seq)) >> 12];
}
/*-----------------------------------------------------------------------------*/
static inline u8 u32_get_bit_idx (u32 non_zero_pow2)
{
  static const u32 bruijn_seq               = 0x0563937d;
  static const u8  bruijn_seq_cycle_inv[32] = {
     0,  1,  2, 11, 17, 3, 20, 12, 30, 18,  4, 6,  8,  21, 13, 24, 
    31, 10, 16, 19, 29, 5,  7, 23,  9, 15, 28, 22, 14, 27, 26, 25,
  };
  bl_assert (non_zero_pow2 && has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((u32)(non_zero_pow2 * bruijn_seq)) >> 27];
}
/*-----------------------------------------------------------------------------*/
static inline u8 u64_get_bit_idx (u64 non_zero_pow2)
{
  static const u64 bruijn_seq               = 0x30b37894769fab9;
  static const u8  bruijn_seq_cycle_inv[64] = {
     0,  1,  8,  2, 25,  9,  3, 34, 61, 26, 29, 10,  4, 16, 35, 45,
    62, 32, 27, 43, 30, 53, 11, 55,  5, 13, 40, 17, 57, 36, 20, 46,
    63,  7, 24, 33, 60, 28, 15, 44, 31, 42, 52, 54, 12, 39, 56, 19,
     6, 23, 59, 14, 41, 51, 38, 18, 22, 58, 50, 37, 21, 49, 48, 47,
  };
  bl_assert (non_zero_pow2 && has_one_or_zero_bits_set (non_zero_pow2));
  return bruijn_seq_cycle_inv[((u64)(non_zero_pow2 * bruijn_seq)) >> 58];
}
/*---------------------------------------------------------------------------*/
#ifdef BL8
  #define u_get_bit_idx(v) u8_get_bit_idx(v)
#endif
#ifdef BL16
  #define u_get_bit_idx(v) u16_get_bit_idx(v)
#endif
#ifdef BL32
  #define u_get_bit_idx(v) u32_get_bit_idx(v)
#endif
#ifdef BL64
  #define u_get_bit_idx(v) u64_get_bit_idx(v)
#endif
/*---------------------------------------------------------------------------*/
/* "_be" suffix = big endian, "_le" suffix = little endian                   */ 
/*---------------------------------------------------------------------------*/

/*on gcc for non static calculations see __builtin_bswap16 32, ...*/

#ifdef BL_ARCH_BIG_ENDIAN
  #define read_u32_be  read_u32_low_mem_to_lsb
  #define read_u32_le  read_u32_low_mem_to_msb
  #define write_u32_be write_u32_low_mem_to_lsb
  #define write_u32_le write_u32_low_mem_to_msb
  #define init_u32_be  init_u32_low_mem_to_lsb
  #define init_u32_le  init_u32_low_mem_to_msb
  
  #define read_u24_be  read_u24_low_mem_to_lsb
  #define read_u24_le  read_u24_low_mem_to_msb
  #define write_u24_be write_u24_low_mem_to_lsb
  #define write_u24_le write_u24_low_mem_to_msb
  #define init_u24_be  init_u24_low_mem_to_lsb
  #define init_u24_le  init_u24_low_mem_to_msb
  
  #define read_u16_be  read_u16_low_mem_to_lsb
  #define read_u16_le  read_u16_low_mem_to_msb
  #define write_u16_be write_u16_low_mem_to_lsb
  #define write_u16_le write_u16_low_mem_to_msb
  #define init_u16_be  init_u16_low_mem_to_lsb
  #define init_u16_le  init_u16_low_mem_to_msb
#endif

#ifdef BL_ARCH_LITTLE_ENDIAN
  #define read_u32_be  read_u32_low_mem_to_msb
  #define read_u32_le  read_u32_low_mem_to_lsb
  #define write_u32_be write_u32_low_mem_to_msb
  #define write_u32_le write_u32_low_mem_to_lsb
  #define init_u32_be  init_u32_low_mem_to_msb
  #define init_u32_le  init_u32_low_mem_to_lsb
  
  #define read_u24_be  read_u24_low_mem_to_msb
  #define read_u24_le  read_u24_low_mem_to_lsb
  #define write_u24_be write_u24_low_mem_to_msb
  #define write_u24_le write_u24_low_mem_to_lsb
  #define init_u24_be  init_u24_low_mem_to_msb
  #define init_u24_le  init_u24_low_mem_to_lsb
  
  #define read_u16_be  read_u16_low_mem_to_msb
  #define read_u16_le  read_u16_low_mem_to_lsb
  #define write_u16_be write_u16_low_mem_to_msb
  #define write_u16_le write_u16_low_mem_to_lsb
  #define init_u16_be  init_u16_low_mem_to_msb
  #define init_u16_le  init_u16_low_mem_to_lsb
#endif
/*---------------------------------------------------------------------------*/
#endif /*LGC_INTEGER_MANIPULATION_H*/
