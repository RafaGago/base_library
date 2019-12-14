#ifndef BL_INTEGER_MATH_H
#define BL_INTEGER_MATH_H

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/utility.h>
#include <bl/base/static_integer_math.h>

/*TODO: C11 Type-generic expressions are very suitable here*/

/*---------------------------------------------------------------------------*/
static inline bl_u8 bl_msb_to_right_set_u8 (bl_u8 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  return x;
}
#if BL_WORDSIZE_MAX >= 16
static inline bl_u16 bl_msb_to_right_set_u16 (bl_u16 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  return x;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline bl_u32 bl_msb_to_right_set_u32 (bl_u32 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  return x;
}
#endif
#if BL_WORDSIZE_MAX >= 64
static inline bl_u64 bl_msb_to_right_set_u64 (bl_u64 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  x |= (x >> 32);
  return x;
}
#endif
/*---------------------------------------------------------------------------*/
#define bl_next_pow2_u8(x)  ((bl_u8)  bl_msb_to_right_set_u8 (x)  + 1)
#define bl_next_pow2_u16(x) ((bl_u16) bl_msb_to_right_set_u16 (x) + 1)
#define bl_next_pow2_u32(x) ((bl_u32) bl_msb_to_right_set_u32 (x) + 1)
#define bl_next_pow2_u64(x) ((bl_u64) bl_msb_to_right_set_u64 (x) + 1)
/*---------------------------------------------------------------------------*/
#define bl_round_next_pow2_u8(x) \
  (bl_is_pow2 (x) ? x : bl_next_pow2_u8 (x))
#define bl_round_next_pow2_u16(x) \
  (bl_is_pow2 (x) ? x : bl_next_pow2_u16 (x))
#define bl_round_next_pow2_u32(x) \
  (bl_is_pow2 (x) ? x : bl_next_pow2_u32 (x))
#define bl_round_next_pow2_u64(x) \
  (bl_is_pow2 (x) ? x : bl_next_pow2_u64 (x))
/*---------------------------------------------------------------------------*/
static inline bl_u8 bl_reverse_bits (bl_u8 val)
{
  bl_u64 v;
  v   = (bl_u64) val * 0x80200802ULL;
  v  &= 0x0884422110ULL;
  v  *= 0x0101010101ULL;
  v >>= 32;
  return (bl_u8) v;
}
/*---------------------------------------------------------------------------*/
#if BL_COMPILER_IS (GCC) || BL_COMPILER_IS (CLANG)
  #include <bl/base/impl/integer_math_gcc_clang.h>
#else
  #include <bl/base/impl/integer_math_generic.h>
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  #define bl_msb_to_right_set_u bl_msb_to_right_set_u8
  #define bl_next_pow2_u        bl_next_pow2_u8
  #define bl_round_next_pow2_u  bl_round_next_pow2_u8
  #define bl_popcount_u         bl_popcount_u8
  #define bl_log2_floor_u       bl_log2_floor_u8
  #define bl_log2_ceil_u        bl_log2_ceil_u8
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 16
  #define bl_msb_to_right_set_u bl_msb_to_right_set_u16
  #define bl_next_pow2_u        bl_next_pow2_u16
  #define bl_round_next_pow2_u  bl_round_next_pow2_u16
  #define bl_popcount_u         bl_popcount_u16
  #define bl_log2_floor_u       bl_log2_floor_u16
  #define bl_log2_ceil_u        bl_log2_ceil_u16
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 32
  #define bl_msb_to_right_set_u bl_msb_to_right_set_u32
  #define bl_next_pow2_u        bl_next_pow2_u32
  #define bl_round_next_pow2_u  bl_round_next_pow2_u32
  #define bl_popcount_u         bl_popcount_u32
  #define bl_log2_floor_u       bl_log2_floor_u32
  #define bl_log2_ceil_u        bl_log2_ceil_u32
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 64
  #define bl_msb_to_right_set_u bl_msb_to_right_set_u64
  #define bl_next_pow2_u        bl_next_pow2_u64
  #define bl_round_next_pow2_u  bl_round_next_pow2_u64
  #define bl_popcount_u         bl_popcount_u64
  #define bl_log2_floor_u       bl_log2_floor_u64
  #define bl_log2_ceil_u        bl_log2_ceil_u64
#endif
/*---------------------------------------------------------------------------*/

#endif /* BL_INTEGER_MATH_H */
