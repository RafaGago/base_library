#ifndef BL_INTEGER_MATH_H
#define BL_INTEGER_MATH_H

#include <base_library/hdr/platform.h>
#include <base_library/hdr/integer.h>
#include <base_library/hdr/integer_manipulation.h>
#include <base_library/hdr/utility.h>
#include <base_library/hdr/static_integer_math.h>

/*TODO: C11 Type-generic expressions are very suitable here*/

/*---------------------------------------------------------------------------*/
static inline u8 msb_to_right_set_u8 (u8 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  return x;
}

static inline u16 msb_to_right_set_u16 (u16 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  return x;
}

static inline u32 msb_to_right_set_u32 (u32 x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  return x;
}

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
/* for gcc see __builtin_popcount, __builtin_popcountl, __builtin_popcountull*/
/*---------------------------------------------------------------------------*/
#ifdef __GNUC__

static inline int popcount_u64 (u64 x)
{
  return __builtin_popcountll (x);
}

static inline int popcount_u32 (u32 x)
{
  return __builtin_popcountl (x);
}

static inline int popcount_u16 (u16 x)
{
  return __builtin_popcount(x);
}

static inline int popcount_u8 (u8 x)
{
  return __builtin_popcount (x);
}

#else /*__GNUC__*/

static inline int popcount_u64 (u64 x)
{
    const u64 m1  = 0x5555555555555555ULL;
    const u64 m2  = 0x3333333333333333ULL;
    const u64 m4  = 0x0f0f0f0f0f0f0f0fULL;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >>  8;
    x += x >> 16;
    x += x >> 32;
    return (int) (x & 0x7f);
}

static inline int popcount_u32 (u32 x)
{
    const u32 m1  = 0x55555555ULL; //binary: 0101...
    const u32 m2  = 0x33333333ULL; //binary: 00110011..
    const u32 m4  = 0x0f0f0f0fULL;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >>  8;
    x += x >> 16;
    return (int) (x & 0x3f);
}
static inline int popcount_u16 (u16 x)
{
    const u16 m1  = 0x5555ULL;
    const u16 m2  = 0x3333ULL;
    const u16 m4  = 0x0f0fULL;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >>  8;
    return (int) (x & 0x1f);
}
static inline int popcount_u8 (u8 x)
{
    const u8 m1  = 0x55;
    const u8 m2  = 0x33;
    const u8 m4  = 0x0f;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    return (int) (x & 0x0f);
}

#endif /*__GNUC__*/
/*---------------------------------------------------------------------------*/
static inline int log2_floor_u64 (u64 x) /* log_2 of 0 will return -1 */
{
    x = msb_to_right_set_u64 (x);
    return popcount_u64 (x) - 1;
}

static inline int log2_floor_u32 (u32 x) /* log_2 of 0 will return -1 */
{
    x = msb_to_right_set_u32 (x);
    return popcount_u32 (x) - 1;
}

static inline int log2_floor_u16 (u16 x) /* log_2 of 0 will return -1 */
{
    x = msb_to_right_set_u16 (x);
    return popcount_u16 (x) - 1;
}

static inline int log2_floor_u8 (u8 x)  /* log_2 of 0 will return -1 */
{
    x = msb_to_right_set_u8 (x);
    return popcount_u8 (x) - 1;
}
/*---------------------------------------------------------------------------*/
static inline int log2_ceil_64 (u64 x) /* log_2 of 0 will return -1 */
{
    i64 y = x & (x - 1);
    y    |= -y;
    y   >>= type_bits (y) - 1;

    x = msb_to_right_set_u64 (x);
    return popcount_u64 (x) - 1 - (int) y;
}

static inline int log2_ceil_32 (u32 x) /* log_2 of 0 will return -1 */
{
    i32 y = x & (x - 1);
    y    |= -y;
    y   >>= type_bits (y) - 1;

    x = msb_to_right_set_u32 (x);
    return popcount_u32 (x) - 1 - (int) y;
}

static inline int log2_ceil_16 (u16 x) /* log_2 of 0 will return -1 */
{
    i16 y = x & (x - 1);
    y    |= -y;
    y   >>= type_bits (y) - 1;

    x = msb_to_right_set_u16 (x);
    return popcount_u16 (x) - 1 - (int) y;
}

static inline int log2_ceil_8 (u8 x) /* log_2 of 0 will return -1 */
{
    i8 y = x & (x - 1);
    y   |= -y;
    y  >>= type_bits (y) - 1;  

    x = msb_to_right_set_u8 (x);
    return popcount_u8 (x) - 1 - (int) y;
}
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
#ifdef BL8
  #define msb_to_right_set_u msb_to_right_set_u8
  #define next_pow2_u        next_pow2_u8
  #define round_next_pow2_u  round_next_pow2_u8
  #define popcount_u         popcount_u8
  #define log2_floor_u       log2_floor_u8
  #define log2_ceil_u        log2_ceil_u8
#endif
/*---------------------------------------------------------------------------*/
#ifdef BL16
  #define msb_to_right_set_u msb_to_right_set_u16
  #define next_pow2_u        next_pow2_u16
  #define round_next_pow2_u  round_next_pow2_u16
  #define popcount_u         popcount_u16
  #define log2_floor_u       log2_floor_u16
  #define log2_ceil_u        log2_ceil_u16
#endif
/*---------------------------------------------------------------------------*/
#ifdef BL32
  #define msb_to_right_set_u msb_to_right_set_u32
  #define next_pow2_u        next_pow2_u32
  #define round_next_pow2_u  round_next_pow2_u32
  #define popcount_u         popcount_u32
  #define log2_floor_u       log2_floor_u32
  #define log2_ceil_u        log2_ceil_u32
#endif
/*---------------------------------------------------------------------------*/
#ifdef BL64
  #define msb_to_right_set_u msb_to_right_set_u64
  #define next_pow2_u        next_pow2_u64
  #define round_next_pow2_u  round_next_pow2_u64
  #define popcount_u         popcount_u64
  #define log2_floor_u       log2_floor_u64
  #define log2_ceil_u        log2_ceil_u64
#endif
/*---------------------------------------------------------------------------*/

#endif /* BL_INTEGER_MATH_H */
