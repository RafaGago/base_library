#ifndef BL_INTEGER_MATH_H
  #error "Don't include this directly"
#endif

#include <bl/base/utility.h>
#include <bl/base/assert.h>

/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_popcount_u64 (bl_u64 x)
{
  return __builtin_popcountll (x);
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_popcount_u32 (bl_u32 x)
{
  return __builtin_popcountl (x);
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_popcount_u16 (bl_u16 x)
{
  return __builtin_popcount (x);
}
#endif
static inline int bl_popcount_u8 (bl_u8 x)
{
  return __builtin_popcount (x);
}
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_log2_floor_unsafe_u64 (bl_u64 x) /* log_2 of 0 will return -1 */
{
  bl_assert (x != 0);
  return bl_type_bits (unsigned long long) - 1 - __builtin_clzll (x);
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_log2_floor_unsafe_u32 (bl_u32 x) /* log_2 of 0 will return -1 */
{
  bl_assert (x != 0);
  return bl_type_bits (unsigned long) - 1 - __builtin_clzl (x);
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_log2_floor_unsafe_u16 (bl_u16 x) /* log_2 of 0 will return -1 */
{
  bl_assert (x != 0);
  return bl_type_bits (unsigned int) - 1 - __builtin_clz (x);
}
#endif
static inline int bl_log2_floor_unsafe_u8 (bl_u8 x)  /* log_2 of 0 will return -1 */
{
  bl_assert (x != 0);
  return bl_type_bits (unsigned int) - 1 - __builtin_clz (x);
}
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_log2_ceil_unsafe_u64 (bl_u64 x) /* log_2 of 0 will return -1 */
{
  int guess = bl_log2_floor_unsafe_u64 (x);
  return guess + ((((bl_u64) 1) << guess) < x);
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_log2_ceil_unsafe_u32 (bl_u32 x) /* log_2 of 0 will return -1 */
{
  int guess = bl_log2_floor_unsafe_u32 (x);
  return guess + ((((bl_u32) 1) << guess) < x);
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_log2_ceil_unsafe_u16 (bl_u16 x) /* log_2 of 0 will return -1 */
{
  int guess = bl_log2_floor_unsafe_u16 (x);
  return guess + ((((bl_u16) 1) << guess) < x);
}
#endif
static inline int bl_log2_ceil_unsafe_u8 (bl_u8 x) /* log_2 of 0 will return -1 */
{
  int guess = bl_log2_floor_unsafe_u8 (x);
  return guess + ((((bl_u8) 1) << guess) < x);
}
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_log2_floor_u64 (bl_u64 x) /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_floor_unsafe_u64 (x) : -1;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_log2_floor_u32 (bl_u32 x) /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_floor_unsafe_u32 (x) : -1;
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_log2_floor_u16 (bl_u16 x) /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_floor_unsafe_u16 (x) : -1;
}
#endif
static inline int bl_log2_floor_u8 (bl_u8 x)  /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_floor_unsafe_u8 (x) : -1;
}
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_log2_ceil_u64 (bl_u64 x) /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_ceil_unsafe_u64 (x) : -1;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_log2_ceil_u32 (bl_u32 x) /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_ceil_unsafe_u64 (x) : -1;
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_log2_ceil_u16 (bl_u16 x) /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_ceil_unsafe_u64 (x) : -1;
}
#endif
static inline int bl_log2_ceil_u8 (bl_u8 x) /* log_2 of 0 will return -1 */
{
  return x ? bl_log2_ceil_unsafe_u64 (x) : -1;
}
/*----------------------------------------------------------------------------*/
