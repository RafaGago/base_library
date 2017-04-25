#ifndef BL_INTEGER_MATH_H
#define BL_INTEGER_MATH_H

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/utility.h>
#include <bl/base/static_integer_math.h>

/*TODO: C11 Type-generic expressions are very suitable here*/

/*---------------------------------------------------------------------------*/
static inline u8 msb_to_right_set_u8 (u8 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  return x;
}
#if BL_WORDSIZE_MAX >= 16
static inline u16 msb_to_right_set_u16 (u16 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  return x;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline u32 msb_to_right_set_u32 (u32 x)
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
static inline u64 msb_to_right_set_u64 (u64 x)
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
#define next_pow2_u8(x)  ((u8)  msb_to_right_set_u8 (x)  + 1)
#define next_pow2_u16(x) ((u16) msb_to_right_set_u16 (x) + 1)
#define next_pow2_u32(x) ((u32) msb_to_right_set_u32 (x) + 1)
#define next_pow2_u64(x) ((u64) msb_to_right_set_u64 (x) + 1)
/*---------------------------------------------------------------------------*/
#define round_next_pow2_u8(x)  (is_pow2 (x) ? x : next_pow2_u8 (x))
#define round_next_pow2_u16(x) (is_pow2 (x) ? x : next_pow2_u16 (x))
#define round_next_pow2_u32(x) (is_pow2 (x) ? x : next_pow2_u32 (x))
#define round_next_pow2_u64(x) (is_pow2 (x) ? x : next_pow2_u64 (x))
/*---------------------------------------------------------------------------*/
static inline u8 reverse_bits (u8 val)
{
  u64 v;
  v   = (u64) val * 0x80200802ULL;
  v  &= 0x0884422110ULL;
  v  *= 0x0101010101ULL;
  v >>= 32;
  return (u8) v;
}
/*---------------------------------------------------------------------------*/
#if defined (BL_GCC) || defined (BL_GCLANG)
  #include <bl/base/impl/integer_math_gcc_clang.h>
#else
  #include <bl/base/impl/integer_math_generic.h>
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  #define msb_to_right_set_u msb_to_right_set_u8
  #define next_pow2_u        next_pow2_u8
  #define round_next_pow2_u  round_next_pow2_u8
  #define popcount_u         popcount_u8
  #define log2_floor_u       log2_floor_u8
  #define log2_ceil_u        log2_ceil_u8
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 16
  #define msb_to_right_set_u msb_to_right_set_u16
  #define next_pow2_u        next_pow2_u16
  #define round_next_pow2_u  round_next_pow2_u16
  #define popcount_u         popcount_u16
  #define log2_floor_u       log2_floor_u16
  #define log2_ceil_u        log2_ceil_u16
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 32
  #define msb_to_right_set_u msb_to_right_set_u32
  #define next_pow2_u        next_pow2_u32
  #define round_next_pow2_u  round_next_pow2_u32
  #define popcount_u         popcount_u32
  #define log2_floor_u       log2_floor_u32
  #define log2_ceil_u        log2_ceil_u32
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 64
  #define msb_to_right_set_u msb_to_right_set_u64
  #define next_pow2_u        next_pow2_u64
  #define round_next_pow2_u  round_next_pow2_u64
  #define popcount_u         popcount_u64
  #define log2_floor_u       log2_floor_u64
  #define log2_ceil_u        log2_ceil_u64
#endif
/*---------------------------------------------------------------------------*/

#endif /* BL_INTEGER_MATH_H */
