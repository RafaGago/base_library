#ifndef BL_INTEGER_MATH_H
    #error "Don't include this directly"
#endif

#include <bl/base/utility.h>

/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int popcount_u64 (u64 x)
{
  return __builtin_popcountll (x);
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int popcount_u32 (u32 x)
{
  return __builtin_popcountl (x);
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int popcount_u16 (u16 x)
{
  return __builtin_popcount (x);
}
#endif
static inline int popcount_u8 (u8 x)
{
  return __builtin_popcount (x);
}
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int log2_floor_u64 (u64 x) /* log_2 of 0 will return -1 */
{
    return x ? type_bits (unsigned long long) - 1 - __builtin_clzll (x) : -1;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int log2_floor_u32 (u32 x) /* log_2 of 0 will return -1 */
{
    return x ? type_bits (unsigned long) - 1 - __builtin_clzl (x) : -1;
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int log2_floor_u16 (u16 x) /* log_2 of 0 will return -1 */
{
    return x ? type_bits (unsigned int) - 1 - __builtin_clz (x) : -1;
}
#endif
static inline int log2_floor_u8 (u8 x)  /* log_2 of 0 will return -1 */
{
    return x ? type_bits (unsigned int) - 1 - __builtin_clz (x) : -1;
}
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int log2_ceil_u64 (u64 x) /* log_2 of 0 will return -1 */
{
    int guess = log2_floor_u64 (x);
    return x != 0 && x - (((u64) 1) << guess) ? guess + 1: guess;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int log2_ceil_u32 (u32 x) /* log_2 of 0 will return -1 */
{
    int guess = log2_floor_u32 (x);
    return x != 0 && x - (((u32) 1) << guess) ? guess + 1: guess;
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int log2_ceil_u16 (u16 x) /* log_2 of 0 will return -1 */
{
    int guess = log2_floor_u16 (x);
    return x != 0 && x - (((u16) 1) << guess) ? guess + 1: guess;
}
#endif
static inline int log2_ceil_u8 (u8 x) /* log_2 of 0 will return -1 */
{
    int guess = log2_floor_u8 (x);
    return x != 0 && x - (((u8) 1) << guess) ? guess + 1: guess;
}
/*----------------------------------------------------------------------------*/
