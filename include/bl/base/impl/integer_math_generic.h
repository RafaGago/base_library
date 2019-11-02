#ifndef BL_INTEGER_MATH_H
    #error "Don't include this directly"
#endif

/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_popcount_u64 (bl_u64 x)
{
    const bl_u64 m1  = 0x5555555555555555ULL;
    const bl_u64 m2  = 0x3333333333333333ULL;
    const bl_u64 m4  = 0x0f0f0f0f0f0f0f0fULL;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >>  8;
    x += x >> 16;
    x += x >> 32;
    return (int) (x & 0x7f);
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_popcount_u32 (bl_u32 x)
{
    const bl_u32 m1  = 0x55555555ULL; //binary: 0101...
    const bl_u32 m2  = 0x33333333ULL; //binary: 00110011..
    const bl_u32 m4  = 0x0f0f0f0fULL;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >>  8;
    x += x >> 16;
    return (int) (x & 0x3f);
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_popcount_u16 (bl_u16 x)
{
    const bl_u16 m1  = 0x5555ULL;
    const bl_u16 m2  = 0x3333ULL;
    const bl_u16 m4  = 0x0f0fULL;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    x += x >>  8;
    return (int) (x & 0x1f);
}
#endif
static inline int bl_popcount_u8 (bl_u8 x)
{
    const bl_u8 m1  = 0x55;
    const bl_u8 m2  = 0x33;
    const bl_u8 m4  = 0x0f;

    x -= (x >> 1) & m1;
    x = (x & m2) + ((x >> 2) & m2);
    x = (x + (x >> 4)) & m4;
    return (int) (x & 0x0f);
}
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_log2_floor_u64 (bl_u64 x) /* log_2 of 0 will return -1 */
{
    x = bl_msb_to_right_set_u64 (x);
    return bl_popcount_u64 (x) - 1;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_log2_floor_u32 (bl_u32 x) /* log_2 of 0 will return -1 */
{
    x = bl_msb_to_right_set_u32 (x);
    return bl_popcount_u32 (x) - 1;
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_log2_floor_u16 (bl_u16 x) /* log_2 of 0 will return -1 */
{
    x = bl_msb_to_right_set_u16 (x);
    return bl_popcount_u16 (x) - 1;
}
#endif
static inline int bl_log2_floor_u8 (bl_u8 x)  /* log_2 of 0 will return -1 */
{
    x = bl_msb_to_right_set_u8 (x);
    return bl_popcount_u8 (x) - 1;
}
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
static inline int bl_log2_ceil_u64 (bl_u64 x) /* log_2 of 0 will return -1 */
{
    bl_i64 y = x & (x - 1);
    y    |= -y;
    y   >>= bl_type_bits (y) - 1;

    x = bl_msb_to_right_set_u64 (x);
    return bl_popcount_u64 (x) - 1 - (int) y;
}
#endif
#if BL_WORDSIZE_MAX >= 32
static inline int bl_log2_ceil_u32 (bl_u32 x) /* log_2 of 0 will return -1 */
{
    bl_i32 y = x & (x - 1);
    y    |= -y;
    y   >>= bl_type_bits (y) - 1;

    x = bl_msb_to_right_set_u32 (x);
    return bl_popcount_u32 (x) - 1 - (int) y;
}
#endif
#if BL_WORDSIZE_MAX >= 16
static inline int bl_log2_ceil_u16 (bl_u16 x) /* log_2 of 0 will return -1 */
{
    bl_i16 y = x & (x - 1);
    y    |= -y;
    y   >>= bl_type_bits (y) - 1;

    x = bl_msb_to_right_set_u16 (x);
    return bl_popcount_u16 (x) - 1 - (int) y;
}
#endif
static inline int bl_log2_ceil_u8 (bl_u8 x) /* log_2 of 0 will return -1 */
{
    bl_i8 y = x & (x - 1);
    y   |= -y;
    y  >>= bl_type_bits (y) - 1;

    x = bl_msb_to_right_set_u8 (x);
    return bl_popcount_u8 (x) - 1 - (int) y;
}
/*---------------------------------------------------------------------------*/
/* to be used when the input is known to not be zero, the result for an input
   of zero is undefined */
#define bl_log2_floor_unsafe_u64 bl_log2_floor_u64
#define bl_log2_floor_unsafe_u32 bl_log2_floor_u32
#define bl_log2_floor_unsafe_u16 bl_log2_floor_u16
#define bl_log2_floor_unsafe_u8  bl_log2_floor_u8
#define bl_log2_ceil_unsafe_u64  bl_log2_ceil_u64
#define bl_log2_ceil_unsafe_u32  bl_log2_ceil_u32
#define bl_log2_ceil_unsafe_u16  bl_log2_ceil_u16
#define bl_log2_ceil_unsafe_u8   bl_log2_ceil_u8
/*---------------------------------------------------------------------------*/
