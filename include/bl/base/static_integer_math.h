#ifndef BL_STATIC_INTEGER_MATH_H
#define BL_STATIC_INTEGER_MATH_H

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/integer_manipulation.h>
/*---------------------------------------------------------------------------*/
#define bl_div_ceil(num, div)\
  (((num) + (div) - 1) / (div))
#define bl_round_to_next_multiple(v, mult)\
  (bl_div_ceil ((v), (mult)) * (mult))
#define bl_is_pow2(x) (x != 0 && bl_has_one_or_zero_bits_set(x))
#define bl_unsigned_average(x, y)\
  (((x) & (y)) + (((x) ^ (y)) >> 1))
#define bl_is_multiple_unsafe(candidate, base)\
  (((candidate) % (base)) == 0)
#define bl_is_multiple(candidate, base)\
    ((candidate) && (base) && (bl_is_multiple_unsafe ((candidate), (base))))
/* only well defined for signed types, as it can wrap */
#define bl_two_complement_u(v) ((~(v)) + 1)
/*---------------------------------------------------------------------------*/
#define bl_pow2_u8(val) (((bl_u8) 1) << (val))
#define bl_pow2_u16(val) (((bl_u16) 1) << (val))
#define bl_pow2_u32(val) (((bl_u32) 1) << (val))
#define bl_pow2_u64(val) (((bl_u64) 1) << (val))
#define bl_pow2_ubig(unsigned_type) (((bl_ubig) 1) << (unsigned_type))
/*---------------------------------------------------------------------------*/
/* The most simple cases of fixed-point arithmetic (both operands have the same
   format, a bigger data type with the required precission is available and
   truncation is used) */
#define bl_int_to_fixp(type, v, dec_bits)  (((type) (v)) << (dec_bits))
#define bl_fixp_to_int(v, dec_bits)        ((v) >> (dec_bits))
#define bl_fixp_mul(type, v1, v2, dec_bits)\
  (((type) (v1) * (type) (v2)) >> (dec_bits))
#define bl_fixp_div(type, v1, v2, dec_bits)\
  ((((type) (v1)) << (dec_bits)) / (type) (v2))

/*fixed-point reminder:
  -just numbers of the same format can be added or subtracted.
  -when multiplying two numbers the format at each side format is the sum of
   both e.g. X(A.B) * Y(C.D) = (X * Y) (A + C. B + D)
  -when dividing two numbers the format at each side format is the diff of
   both e.g. X(A.B) / Y(C.D) = (X / Y) (A - C. B - D)
*/
/*---------------------------------------------------------------------------*/
#define bl_static_msb_to_right_set_u8(x)\
  (((x) >> 0) | ((x) >> 1) | ((x) >> 2) | ((x) >> 3) |\
   ((x) >> 4) | ((x) >> 5) | ((x) >> 6) | ((x) >> 7)\
   )
#define bl_static_msb_to_right_set_u16(x)\
  (bl_static_msb_to_right_set_u8 (x) |\
   ((x) >> 8)  | ((x) >> 9)  | ((x) >> 10) | ((x) >> 11) |\
   ((x) >> 12) | ((x) >> 13) | ((x) >> 14) | ((x) >> 15)\
   )
#define bl_static_msb_to_right_set_u32(x)\
  (bl_static_msb_to_right_set_u16 (x) |\
   ((x) >> 16) | ((x) >> 17) | ((x) >> 18) | ((x) >> 19) |\
   ((x) >> 20) | ((x) >> 21) | ((x) >> 22) | ((x) >> 23) |\
   ((x) >> 24) | ((x) >> 25) | ((x) >> 26) | ((x) >> 27) |\
   ((x) >> 28) | ((x) >> 29) | ((x) >> 30) | ((x) >> 31)\
   )
#define bl_static_msb_to_right_set_u64(x)\
  (bl_static_msb_to_right_set_u32 (x) |\
   ((x) >> 32) | ((x) >> 33) | ((x) >> 34) | ((x) >> 35) |\
   ((x) >> 36) | ((x) >> 37) | ((x) >> 38) | ((x) >> 39) |\
   ((x) >> 40) | ((x) >> 41) | ((x) >> 42) | ((x) >> 43) |\
   ((x) >> 44) | ((x) >> 45) | ((x) >> 46) | ((x) >> 47) |\
   ((x) >> 48) | ((x) >> 49) | ((x) >> 50) | ((x) >> 51) |\
   ((x) >> 52) | ((x) >> 53) | ((x) >> 54) | ((x) >> 55) |\
   ((x) >> 56) | ((x) >> 57) | ((x) >> 58) | ((x) >> 59) |\
   ((x) >> 60) | ((x) >> 61) | ((x) >> 62) | ((x) >> 63)\
   )
/*---------------------------------------------------------------------------*/
#define bl_static_next_pow2_u8(x) \
  ((bl_u8)  bl_static_msb_to_right_set_u8(x)  + 1)
#define bl_static_next_pow2_u16(x) \
  ((bl_u16) bl_static_msb_to_right_set_u16(x) + 1)
#define bl_static_next_pow2_u32(x) \
  ((bl_u32) bl_static_msb_to_right_set_u32(x) + 1)
#define bl_static_next_pow2_u64(x) \
  ((bl_u64) bl_static_msb_to_right_set_u64(x) + 1)
/*---------------------------------------------------------------------------*/
#define bl_static_round_next_pow2_u8(x)\
  (bl_is_pow2 (x) ? x : bl_static_next_pow2_u8 (x))
#define bl_static_round_next_pow2_u16(x)\
  (bl_is_pow2 (x) ? x : bl_static_next_pow2_u8 (x))
#define bl_static_round_next_pow2_u32(x)\
  (bl_is_pow2 (x) ? x : bl_static_next_pow2_u8 (x))
#define bl_static_round_next_pow2_u64(x)\
  (bl_is_pow2 (x) ? x : bl_static_next_pow2_u8 (x))
/*---------------------------------------------------------------------------*/
#define bl_static_log2_ceil_u64(val)\
  (\
  ((bl_u64)(val) >  ((bl_u64) 1 << 63)) ? 64 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 62)) ? 63 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 61)) ? 62 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 60)) ? 61 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 59)) ? 60 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 58)) ? 59 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 57)) ? 58 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 56)) ? 57 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 55)) ? 56 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 54)) ? 55 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 53)) ? 54 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 52)) ? 53 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 51)) ? 52 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 50)) ? 51 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 49)) ? 50 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 48)) ? 49 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 47)) ? 48 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 46)) ? 47 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 45)) ? 46 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 44)) ? 45 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 43)) ? 44 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 42)) ? 43 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 41)) ? 42 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 40)) ? 41 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 39)) ? 40 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 38)) ? 39 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 37)) ? 38 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 36)) ? 37 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 35)) ? 36 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 34)) ? 35 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 33)) ? 34 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 32)) ? 33 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 31)) ? 32 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 30)) ? 31 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 29)) ? 30 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 28)) ? 29 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 27)) ? 28 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 26)) ? 27 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 25)) ? 26 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 24)) ? 25 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 23)) ? 24 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 22)) ? 23 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 21)) ? 22 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 20)) ? 21 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 19)) ? 20 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 18)) ? 19 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 17)) ? 18 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 16)) ? 17 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 15)) ? 16 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 14)) ? 15 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 13)) ? 14 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 12)) ? 13 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 11)) ? 12 :\
  ((bl_u64)(val) >  ((bl_u64) 1 << 10)) ? 11 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  9)) ? 10 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  8)) ?  9 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  7)) ?  8 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  6)) ?  7 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  5)) ?  6 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  4)) ?  5 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  3)) ?  4 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  2)) ?  3 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  1)) ?  2 :\
  ((bl_u64)(val) >  ((bl_u64) 1 <<  0)) ?  1 :\
  ((bl_u64)(val) == ((bl_u64) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define bl_static_log2_floor_u64(val)\
  (\
  ((bl_u64)(val) >= ((bl_u64) 1 << 63)) ? 63 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 62)) ? 62 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 61)) ? 61 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 60)) ? 60 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 59)) ? 59 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 58)) ? 58 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 57)) ? 57 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 56)) ? 56 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 55)) ? 55 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 54)) ? 54 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 53)) ? 53 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 52)) ? 52 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 51)) ? 51 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 50)) ? 50 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 49)) ? 49 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 48)) ? 48 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 47)) ? 47 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 46)) ? 46 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 45)) ? 45 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 44)) ? 44 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 43)) ? 43 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 42)) ? 42 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 41)) ? 41 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 40)) ? 40 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 39)) ? 39 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 38)) ? 38 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 37)) ? 37 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 36)) ? 36 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 35)) ? 35 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 34)) ? 34 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 33)) ? 33 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 32)) ? 32 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 31)) ? 31 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 30)) ? 30 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 29)) ? 29 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 28)) ? 28 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 27)) ? 27 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 26)) ? 26 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 25)) ? 25 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 24)) ? 24 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 23)) ? 23 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 22)) ? 22 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 21)) ? 21 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 20)) ? 20 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 19)) ? 19 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 18)) ? 18 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 17)) ? 17 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 16)) ? 16 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 15)) ? 15 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 14)) ? 14 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 13)) ? 13 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 12)) ? 12 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 11)) ? 11 :\
  ((bl_u64)(val) >= ((bl_u64) 1 << 10)) ? 10 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  9)) ?  9 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  8)) ?  8 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  7)) ?  7 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  6)) ?  6 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  5)) ?  5 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  4)) ?  4 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  3)) ?  3 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  2)) ?  2 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  1)) ?  1 :\
  ((bl_u64)(val) >= ((bl_u64) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define bl_static_log2_ceil_u32(val)\
  (\
  ((bl_u32)(val) >  ((bl_u32) 1 << 31)) ? 32 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 30)) ? 31 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 29)) ? 30 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 28)) ? 29 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 27)) ? 28 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 26)) ? 27 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 25)) ? 26 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 24)) ? 25 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 23)) ? 24 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 22)) ? 23 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 21)) ? 22 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 20)) ? 21 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 19)) ? 20 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 18)) ? 19 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 17)) ? 18 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 16)) ? 17 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 15)) ? 16 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 14)) ? 15 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 13)) ? 14 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 12)) ? 13 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 11)) ? 12 :\
  ((bl_u32)(val) >  ((bl_u32) 1 << 10)) ? 11 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  9)) ? 10 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  8)) ?  9 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  7)) ?  8 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  6)) ?  7 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  5)) ?  6 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  4)) ?  5 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  3)) ?  4 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  2)) ?  3 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  1)) ?  2 :\
  ((bl_u32)(val) >  ((bl_u32) 1 <<  0)) ?  1 :\
  ((bl_u32)(val) == ((bl_u32) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define bl_static_log2_floor_u32(val)\
  (\
  ((bl_u32)(val) >= ((bl_u32) 1 << 31)) ? 31 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 30)) ? 30 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 29)) ? 29 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 28)) ? 28 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 27)) ? 27 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 26)) ? 26 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 25)) ? 25 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 24)) ? 24 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 23)) ? 23 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 22)) ? 22 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 21)) ? 21 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 20)) ? 20 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 19)) ? 19 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 18)) ? 18 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 17)) ? 17 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 16)) ? 16 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 15)) ? 15 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 14)) ? 14 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 13)) ? 13 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 12)) ? 12 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 11)) ? 11 :\
  ((bl_u32)(val) >= ((bl_u32) 1 << 10)) ? 10 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  9)) ?  9 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  8)) ?  8 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  7)) ?  7 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  6)) ?  6 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  5)) ?  5 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  4)) ?  4 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  3)) ?  3 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  2)) ?  2 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  1)) ?  1 :\
  ((bl_u32)(val) >= ((bl_u32) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define bl_static_log2_ceil_u16(val)\
  (\
  ((bl_u16)(val) >  ((bl_u16) 1 << 15)) ? 16 :\
  ((bl_u16)(val) >  ((bl_u16) 1 << 14)) ? 15 :\
  ((bl_u16)(val) >  ((bl_u16) 1 << 13)) ? 14 :\
  ((bl_u16)(val) >  ((bl_u16) 1 << 12)) ? 13 :\
  ((bl_u16)(val) >  ((bl_u16) 1 << 11)) ? 12 :\
  ((bl_u16)(val) >  ((bl_u16) 1 << 10)) ? 11 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  9)) ? 10 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  8)) ?  9 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  7)) ?  8 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  6)) ?  7 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  5)) ?  6 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  4)) ?  5 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  3)) ?  4 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  2)) ?  3 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  1)) ?  2 :\
  ((bl_u16)(val) >  ((bl_u16) 1 <<  0)) ?  1 :\
  ((bl_u16)(val) == ((bl_u16) 1 <<  0)) ?  0 :\
  -1\
  )
/*----------------------------------------------------------------------------*/
#define bl_static_log2_floor_u16(val)\
  (\
  ((bl_u16)(val) >= ((bl_u16) 1 << 15)) ? 15 :\
  ((bl_u16)(val) >= ((bl_u16) 1 << 14)) ? 14 :\
  ((bl_u16)(val) >= ((bl_u16) 1 << 13)) ? 13 :\
  ((bl_u16)(val) >= ((bl_u16) 1 << 12)) ? 12 :\
  ((bl_u16)(val) >= ((bl_u16) 1 << 11)) ? 11 :\
  ((bl_u16)(val) >= ((bl_u16) 1 << 10)) ? 10 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  9)) ?  9 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  8)) ?  8 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  7)) ?  7 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  6)) ?  6 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  5)) ?  5 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  4)) ?  4 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  3)) ?  3 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  2)) ?  2 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  1)) ?  1 :\
  ((bl_u16)(val) >= ((bl_u16) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define bl_static_log2_ceil_u8(val)\
  (\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  7)) ?  8 :\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  6)) ?  7 :\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  5)) ?  6 :\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  4)) ?  5 :\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  3)) ?  4 :\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  2)) ?  3 :\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  1)) ?  2 :\
  ((bl_u8)(val) >  ((bl_u8) 1 <<  0)) ?  1 :\
  ((bl_u8)(val) == ((bl_u8) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define bl_static_log2_floor_u8(val)\
  (\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  7)) ?  7 :\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  6)) ?  6 :\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  5)) ?  5 :\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  4)) ?  4 :\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  3)) ?  3 :\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  2)) ?  2 :\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  1)) ?  1 :\
  ((bl_u8)(val) >= ((bl_u8) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  #define bl_static_msb_to_right_set_u bl_static_msb_to_right_set_u8
  #define bl_static_next_pow2_u        bl_static_next_pow2_u8
  #define bl_static_round_next_pow2_u  bl_static_round_next_pow2_u8
  #define bl_static_log2_floor_u       bl_static_log2_floor_u8
  #define bl_static_log2_ceil_u        bl_static_log2_ceil_u8
  #define bl_pow2_u                    bl_pow2_u8
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 16
  #define bl_static_msb_to_right_set_u bl_static_msb_to_right_set_u16
  #define bl_static_next_pow2_u        bl_static_next_pow2_u16
  #define bl_static_round_next_pow2_u  bl_static_round_next_pow2_u16
  #define bl_static_log2_floor_u       bl_static_log2_floor_u16
  #define bl_static_log2_ceil_u        bl_static_log2_ceil_u16
  #define bl_pow2_u                    bl_pow2_u16
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 32
  #define bl_static_msb_to_right_set_u bl_static_msb_to_right_set_u32
  #define bl_static_next_pow2_u        bl_static_next_pow2_u32
  #define bl_static_round_next_pow2_u  bl_static_round_next_pow2_u32
  #define bl_static_log2_floor_u       bl_static_log2_floor_u32
  #define bl_static_log2_ceil_u        bl_static_log2_ceil_u32
  #define bl_pow2_u                    bl_pow2_u32
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 64
  #define bl_static_msb_to_right_set_u bl_static_msb_to_right_set_u64
  #define bl_static_next_pow2_u        bl_static_next_pow2_u64
  #define bl_static_round_next_pow2_u  bl_static_round_next_pow2_u64
  #define bl_static_log2_floor_u       bl_static_log2_floor_u64
  #define bl_static_log2_ceil_u        bl_static_log2_ceil_u64
  #define bl_pow2_u                    bl_pow2_u64
#endif
/*---------------------------------------------------------------------------*/

#endif /* BL_STATIC_INTEGER_MATH_H */
