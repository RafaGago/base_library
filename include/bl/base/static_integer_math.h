#ifndef BL_STATIC_INTEGER_MATH_H
#define BL_STATIC_INTEGER_MATH_H

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/integer_manipulation.h>
/*---------------------------------------------------------------------------*/
#define div_ceil(num, div)\
  (((num) + (div) - 1) / (div))
#define round_to_next_multiple(v, mult)\
  (div_ceil ((v), (mult)) * (mult))
#define is_pow2(x) (x != 0 && has_one_or_zero_bits_set(x))
#define unsigned_average(x, y)\
  (((x) & (y)) + (((x) ^ (y)) >> 1))
#define is_multiple_unsafe(candidate, base)\
  (((candidate) % (base)) == 0)
#define is_multiple(candidate, base)\
    ((candidate) && (base) && (is_multiple_unsafe ((candidate), (base))))
/*---------------------------------------------------------------------------*/
#define pow2_u8(val) (((u8) 1) << (val))
#define pow2_u16(val) (((u16) 1) << (val))
#define pow2_u32(val) (((u32) 1) << (val))
#define pow2_u64(val) (((u64) 1) << (val))
#define pow2_ubig(unsigned_type) (((ubig) 1) << (unsigned_type))
/*---------------------------------------------------------------------------*/
/* The most simple cases of fixed-point arithmetic (both operands have the same
   format, a bigger data type with the required precission is available and
   truncation is used) */
#define int_to_fixp(type, v, dec_bits)  (((type) (v)) << (dec_bits))
#define fixp_to_int(v, dec_bits)        ((v) >> (dec_bits))
#define fixp_mul(type, v1, v2, dec_bits)\
  (((type) (v1) * (type) (v2)) >> (dec_bits))
#define fixp_div(type, v1, v2, dec_bits)\
  ((((type) (v1)) << (dec_bits)) / (type) (v2))

/*fixed-point reminder:
  -just numbers of the same format can be added or subtracted.
  -when multiplying two numbers the format at each side format is the sum of
   both e.g. X(A.B) * Y(C.D) = (X * Y) (A + C. B + D)
  -when dividing two numbers the format at each side format is the diff of
   both e.g. X(A.B) / Y(C.D) = (X / Y) (A - C. B - D)
*/
/*---------------------------------------------------------------------------*/
#define static_msb_to_right_set_u8(x)\
  (((x) >> 0) | ((x) >> 1) | ((x) >> 2) | ((x) >> 3) |\
   ((x) >> 4) | ((x) >> 5) | ((x) >> 6) | ((x) >> 7)\
   )
#define static_msb_to_right_set_u16(x)\
  (static_msb_to_right_set_u8 (x) |\
   ((x) >> 8)  | ((x) >> 9)  | ((x) >> 10) | ((x) >> 11) |\
   ((x) >> 12) | ((x) >> 13) | ((x) >> 14) | ((x) >> 15)\
   )
#define static_msb_to_right_set_u32(x)\
  (static_msb_to_right_set_u16 (x) |\
   ((x) >> 16) | ((x) >> 17) | ((x) >> 18) | ((x) >> 19) |\
   ((x) >> 20) | ((x) >> 21) | ((x) >> 22) | ((x) >> 23) |\
   ((x) >> 24) | ((x) >> 25) | ((x) >> 26) | ((x) >> 27) |\
   ((x) >> 28) | ((x) >> 29) | ((x) >> 30) | ((x) >> 31)\
   )
#define static_msb_to_right_set_u64(x)\
  (static_msb_to_right_set_u32 (x) |\
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
#define static_next_pow2_u8(x)  ((u8)  static_msb_to_right_set_u8(x)  + 1)
#define static_next_pow2_u16(x) ((u16) static_msb_to_right_set_u16(x) + 1)
#define static_next_pow2_u32(x) ((u32) static_msb_to_right_set_u32(x) + 1)
#define static_next_pow2_u64(x) ((u64) static_msb_to_right_set_u64(x) + 1)
/*---------------------------------------------------------------------------*/
#define static_round_next_pow2_u8(x)\
  (is_pow2 (x) ? x : static_next_pow2_u8 (x))
#define static_round_next_pow2_u16(x)\
  (is_pow2 (x) ? x : static_next_pow2_u8 (x))
#define static_round_next_pow2_u32(x)\
  (is_pow2 (x) ? x : static_next_pow2_u8 (x))
#define static_round_next_pow2_u64(x)\
  (is_pow2 (x) ? x : static_next_pow2_u8 (x))
/*---------------------------------------------------------------------------*/
#define static_log2_ceil_u64(val)\
  (\
  ((u64)(val) >  ((u64) 1 << 63)) ? 64 :\
  ((u64)(val) >  ((u64) 1 << 62)) ? 63 :\
  ((u64)(val) >  ((u64) 1 << 61)) ? 62 :\
  ((u64)(val) >  ((u64) 1 << 60)) ? 61 :\
  ((u64)(val) >  ((u64) 1 << 59)) ? 60 :\
  ((u64)(val) >  ((u64) 1 << 58)) ? 59 :\
  ((u64)(val) >  ((u64) 1 << 57)) ? 58 :\
  ((u64)(val) >  ((u64) 1 << 56)) ? 57 :\
  ((u64)(val) >  ((u64) 1 << 55)) ? 56 :\
  ((u64)(val) >  ((u64) 1 << 54)) ? 55 :\
  ((u64)(val) >  ((u64) 1 << 53)) ? 54 :\
  ((u64)(val) >  ((u64) 1 << 52)) ? 53 :\
  ((u64)(val) >  ((u64) 1 << 51)) ? 52 :\
  ((u64)(val) >  ((u64) 1 << 50)) ? 51 :\
  ((u64)(val) >  ((u64) 1 << 49)) ? 50 :\
  ((u64)(val) >  ((u64) 1 << 48)) ? 49 :\
  ((u64)(val) >  ((u64) 1 << 47)) ? 48 :\
  ((u64)(val) >  ((u64) 1 << 46)) ? 47 :\
  ((u64)(val) >  ((u64) 1 << 45)) ? 46 :\
  ((u64)(val) >  ((u64) 1 << 44)) ? 45 :\
  ((u64)(val) >  ((u64) 1 << 43)) ? 44 :\
  ((u64)(val) >  ((u64) 1 << 42)) ? 43 :\
  ((u64)(val) >  ((u64) 1 << 41)) ? 42 :\
  ((u64)(val) >  ((u64) 1 << 40)) ? 41 :\
  ((u64)(val) >  ((u64) 1 << 39)) ? 40 :\
  ((u64)(val) >  ((u64) 1 << 38)) ? 39 :\
  ((u64)(val) >  ((u64) 1 << 37)) ? 38 :\
  ((u64)(val) >  ((u64) 1 << 36)) ? 37 :\
  ((u64)(val) >  ((u64) 1 << 35)) ? 36 :\
  ((u64)(val) >  ((u64) 1 << 34)) ? 35 :\
  ((u64)(val) >  ((u64) 1 << 33)) ? 34 :\
  ((u64)(val) >  ((u64) 1 << 32)) ? 33 :\
  ((u64)(val) >  ((u64) 1 << 31)) ? 32 :\
  ((u64)(val) >  ((u64) 1 << 30)) ? 31 :\
  ((u64)(val) >  ((u64) 1 << 29)) ? 30 :\
  ((u64)(val) >  ((u64) 1 << 28)) ? 29 :\
  ((u64)(val) >  ((u64) 1 << 27)) ? 28 :\
  ((u64)(val) >  ((u64) 1 << 26)) ? 27 :\
  ((u64)(val) >  ((u64) 1 << 25)) ? 26 :\
  ((u64)(val) >  ((u64) 1 << 24)) ? 25 :\
  ((u64)(val) >  ((u64) 1 << 23)) ? 24 :\
  ((u64)(val) >  ((u64) 1 << 22)) ? 23 :\
  ((u64)(val) >  ((u64) 1 << 21)) ? 22 :\
  ((u64)(val) >  ((u64) 1 << 20)) ? 21 :\
  ((u64)(val) >  ((u64) 1 << 19)) ? 20 :\
  ((u64)(val) >  ((u64) 1 << 18)) ? 19 :\
  ((u64)(val) >  ((u64) 1 << 17)) ? 18 :\
  ((u64)(val) >  ((u64) 1 << 16)) ? 17 :\
  ((u64)(val) >  ((u64) 1 << 15)) ? 16 :\
  ((u64)(val) >  ((u64) 1 << 14)) ? 15 :\
  ((u64)(val) >  ((u64) 1 << 13)) ? 14 :\
  ((u64)(val) >  ((u64) 1 << 12)) ? 13 :\
  ((u64)(val) >  ((u64) 1 << 11)) ? 12 :\
  ((u64)(val) >  ((u64) 1 << 10)) ? 11 :\
  ((u64)(val) >  ((u64) 1 <<  9)) ? 10 :\
  ((u64)(val) >  ((u64) 1 <<  8)) ?  9 :\
  ((u64)(val) >  ((u64) 1 <<  7)) ?  8 :\
  ((u64)(val) >  ((u64) 1 <<  6)) ?  7 :\
  ((u64)(val) >  ((u64) 1 <<  5)) ?  6 :\
  ((u64)(val) >  ((u64) 1 <<  4)) ?  5 :\
  ((u64)(val) >  ((u64) 1 <<  3)) ?  4 :\
  ((u64)(val) >  ((u64) 1 <<  2)) ?  3 :\
  ((u64)(val) >  ((u64) 1 <<  1)) ?  2 :\
  ((u64)(val) >  ((u64) 1 <<  0)) ?  1 :\
  ((u64)(val) == ((u64) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define static_log2_floor_u64(val)\
  (\
  ((u64)(val) >= ((u64) 1 << 63)) ? 63 :\
  ((u64)(val) >= ((u64) 1 << 62)) ? 62 :\
  ((u64)(val) >= ((u64) 1 << 61)) ? 61 :\
  ((u64)(val) >= ((u64) 1 << 60)) ? 60 :\
  ((u64)(val) >= ((u64) 1 << 59)) ? 59 :\
  ((u64)(val) >= ((u64) 1 << 58)) ? 58 :\
  ((u64)(val) >= ((u64) 1 << 57)) ? 57 :\
  ((u64)(val) >= ((u64) 1 << 56)) ? 56 :\
  ((u64)(val) >= ((u64) 1 << 55)) ? 55 :\
  ((u64)(val) >= ((u64) 1 << 54)) ? 54 :\
  ((u64)(val) >= ((u64) 1 << 53)) ? 53 :\
  ((u64)(val) >= ((u64) 1 << 52)) ? 52 :\
  ((u64)(val) >= ((u64) 1 << 51)) ? 51 :\
  ((u64)(val) >= ((u64) 1 << 50)) ? 50 :\
  ((u64)(val) >= ((u64) 1 << 49)) ? 49 :\
  ((u64)(val) >= ((u64) 1 << 48)) ? 48 :\
  ((u64)(val) >= ((u64) 1 << 47)) ? 47 :\
  ((u64)(val) >= ((u64) 1 << 46)) ? 46 :\
  ((u64)(val) >= ((u64) 1 << 45)) ? 45 :\
  ((u64)(val) >= ((u64) 1 << 44)) ? 44 :\
  ((u64)(val) >= ((u64) 1 << 43)) ? 43 :\
  ((u64)(val) >= ((u64) 1 << 42)) ? 42 :\
  ((u64)(val) >= ((u64) 1 << 41)) ? 41 :\
  ((u64)(val) >= ((u64) 1 << 40)) ? 40 :\
  ((u64)(val) >= ((u64) 1 << 39)) ? 39 :\
  ((u64)(val) >= ((u64) 1 << 38)) ? 38 :\
  ((u64)(val) >= ((u64) 1 << 37)) ? 37 :\
  ((u64)(val) >= ((u64) 1 << 36)) ? 36 :\
  ((u64)(val) >= ((u64) 1 << 35)) ? 35 :\
  ((u64)(val) >= ((u64) 1 << 34)) ? 34 :\
  ((u64)(val) >= ((u64) 1 << 33)) ? 33 :\
  ((u64)(val) >= ((u64) 1 << 32)) ? 32 :\
  ((u64)(val) >= ((u64) 1 << 31)) ? 31 :\
  ((u64)(val) >= ((u64) 1 << 30)) ? 30 :\
  ((u64)(val) >= ((u64) 1 << 29)) ? 29 :\
  ((u64)(val) >= ((u64) 1 << 28)) ? 28 :\
  ((u64)(val) >= ((u64) 1 << 27)) ? 27 :\
  ((u64)(val) >= ((u64) 1 << 26)) ? 26 :\
  ((u64)(val) >= ((u64) 1 << 25)) ? 25 :\
  ((u64)(val) >= ((u64) 1 << 24)) ? 24 :\
  ((u64)(val) >= ((u64) 1 << 23)) ? 23 :\
  ((u64)(val) >= ((u64) 1 << 22)) ? 22 :\
  ((u64)(val) >= ((u64) 1 << 21)) ? 21 :\
  ((u64)(val) >= ((u64) 1 << 20)) ? 20 :\
  ((u64)(val) >= ((u64) 1 << 19)) ? 19 :\
  ((u64)(val) >= ((u64) 1 << 18)) ? 18 :\
  ((u64)(val) >= ((u64) 1 << 17)) ? 17 :\
  ((u64)(val) >= ((u64) 1 << 16)) ? 16 :\
  ((u64)(val) >= ((u64) 1 << 15)) ? 15 :\
  ((u64)(val) >= ((u64) 1 << 14)) ? 14 :\
  ((u64)(val) >= ((u64) 1 << 13)) ? 13 :\
  ((u64)(val) >= ((u64) 1 << 12)) ? 12 :\
  ((u64)(val) >= ((u64) 1 << 11)) ? 11 :\
  ((u64)(val) >= ((u64) 1 << 10)) ? 10 :\
  ((u64)(val) >= ((u64) 1 <<  9)) ?  9 :\
  ((u64)(val) >= ((u64) 1 <<  8)) ?  8 :\
  ((u64)(val) >= ((u64) 1 <<  7)) ?  7 :\
  ((u64)(val) >= ((u64) 1 <<  6)) ?  6 :\
  ((u64)(val) >= ((u64) 1 <<  5)) ?  5 :\
  ((u64)(val) >= ((u64) 1 <<  4)) ?  4 :\
  ((u64)(val) >= ((u64) 1 <<  3)) ?  3 :\
  ((u64)(val) >= ((u64) 1 <<  2)) ?  2 :\
  ((u64)(val) >= ((u64) 1 <<  1)) ?  1 :\
  ((u64)(val) >= ((u64) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define static_log2_ceil_u32(val)\
  (\
  ((u32)(val) >  ((u32) 1 << 31)) ? 32 :\
  ((u32)(val) >  ((u32) 1 << 30)) ? 31 :\
  ((u32)(val) >  ((u32) 1 << 29)) ? 30 :\
  ((u32)(val) >  ((u32) 1 << 28)) ? 29 :\
  ((u32)(val) >  ((u32) 1 << 27)) ? 28 :\
  ((u32)(val) >  ((u32) 1 << 26)) ? 27 :\
  ((u32)(val) >  ((u32) 1 << 25)) ? 26 :\
  ((u32)(val) >  ((u32) 1 << 24)) ? 25 :\
  ((u32)(val) >  ((u32) 1 << 23)) ? 24 :\
  ((u32)(val) >  ((u32) 1 << 22)) ? 23 :\
  ((u32)(val) >  ((u32) 1 << 21)) ? 22 :\
  ((u32)(val) >  ((u32) 1 << 20)) ? 21 :\
  ((u32)(val) >  ((u32) 1 << 19)) ? 20 :\
  ((u32)(val) >  ((u32) 1 << 18)) ? 19 :\
  ((u32)(val) >  ((u32) 1 << 17)) ? 18 :\
  ((u32)(val) >  ((u32) 1 << 16)) ? 17 :\
  ((u32)(val) >  ((u32) 1 << 15)) ? 16 :\
  ((u32)(val) >  ((u32) 1 << 14)) ? 15 :\
  ((u32)(val) >  ((u32) 1 << 13)) ? 14 :\
  ((u32)(val) >  ((u32) 1 << 12)) ? 13 :\
  ((u32)(val) >  ((u32) 1 << 11)) ? 12 :\
  ((u32)(val) >  ((u32) 1 << 10)) ? 11 :\
  ((u32)(val) >  ((u32) 1 <<  9)) ? 10 :\
  ((u32)(val) >  ((u32) 1 <<  8)) ?  9 :\
  ((u32)(val) >  ((u32) 1 <<  7)) ?  8 :\
  ((u32)(val) >  ((u32) 1 <<  6)) ?  7 :\
  ((u32)(val) >  ((u32) 1 <<  5)) ?  6 :\
  ((u32)(val) >  ((u32) 1 <<  4)) ?  5 :\
  ((u32)(val) >  ((u32) 1 <<  3)) ?  4 :\
  ((u32)(val) >  ((u32) 1 <<  2)) ?  3 :\
  ((u32)(val) >  ((u32) 1 <<  1)) ?  2 :\
  ((u32)(val) >  ((u32) 1 <<  0)) ?  1 :\
  ((u32)(val) == ((u32) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define static_log2_floor_u32(val)\
  (\
  ((u32)(val) >= ((u32) 1 << 31)) ? 31 :\
  ((u32)(val) >= ((u32) 1 << 30)) ? 30 :\
  ((u32)(val) >= ((u32) 1 << 29)) ? 29 :\
  ((u32)(val) >= ((u32) 1 << 28)) ? 28 :\
  ((u32)(val) >= ((u32) 1 << 27)) ? 27 :\
  ((u32)(val) >= ((u32) 1 << 26)) ? 26 :\
  ((u32)(val) >= ((u32) 1 << 25)) ? 25 :\
  ((u32)(val) >= ((u32) 1 << 24)) ? 24 :\
  ((u32)(val) >= ((u32) 1 << 23)) ? 23 :\
  ((u32)(val) >= ((u32) 1 << 22)) ? 22 :\
  ((u32)(val) >= ((u32) 1 << 21)) ? 21 :\
  ((u32)(val) >= ((u32) 1 << 20)) ? 20 :\
  ((u32)(val) >= ((u32) 1 << 19)) ? 19 :\
  ((u32)(val) >= ((u32) 1 << 18)) ? 18 :\
  ((u32)(val) >= ((u32) 1 << 17)) ? 17 :\
  ((u32)(val) >= ((u32) 1 << 16)) ? 16 :\
  ((u32)(val) >= ((u32) 1 << 15)) ? 15 :\
  ((u32)(val) >= ((u32) 1 << 14)) ? 14 :\
  ((u32)(val) >= ((u32) 1 << 13)) ? 13 :\
  ((u32)(val) >= ((u32) 1 << 12)) ? 12 :\
  ((u32)(val) >= ((u32) 1 << 11)) ? 11 :\
  ((u32)(val) >= ((u32) 1 << 10)) ? 10 :\
  ((u32)(val) >= ((u32) 1 <<  9)) ?  9 :\
  ((u32)(val) >= ((u32) 1 <<  8)) ?  8 :\
  ((u32)(val) >= ((u32) 1 <<  7)) ?  7 :\
  ((u32)(val) >= ((u32) 1 <<  6)) ?  6 :\
  ((u32)(val) >= ((u32) 1 <<  5)) ?  5 :\
  ((u32)(val) >= ((u32) 1 <<  4)) ?  4 :\
  ((u32)(val) >= ((u32) 1 <<  3)) ?  3 :\
  ((u32)(val) >= ((u32) 1 <<  2)) ?  2 :\
  ((u32)(val) >= ((u32) 1 <<  1)) ?  1 :\
  ((u32)(val) >= ((u32) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define static_log2_ceil_u16(val)\
  (\
  ((u16)(val) >  ((u16) 1 << 15)) ? 16 :\
  ((u16)(val) >  ((u16) 1 << 14)) ? 15 :\
  ((u16)(val) >  ((u16) 1 << 13)) ? 14 :\
  ((u16)(val) >  ((u16) 1 << 12)) ? 13 :\
  ((u16)(val) >  ((u16) 1 << 11)) ? 12 :\
  ((u16)(val) >  ((u16) 1 << 10)) ? 11 :\
  ((u16)(val) >  ((u16) 1 <<  9)) ? 10 :\
  ((u16)(val) >  ((u16) 1 <<  8)) ?  9 :\
  ((u16)(val) >  ((u16) 1 <<  7)) ?  8 :\
  ((u16)(val) >  ((u16) 1 <<  6)) ?  7 :\
  ((u16)(val) >  ((u16) 1 <<  5)) ?  6 :\
  ((u16)(val) >  ((u16) 1 <<  4)) ?  5 :\
  ((u16)(val) >  ((u16) 1 <<  3)) ?  4 :\
  ((u16)(val) >  ((u16) 1 <<  2)) ?  3 :\
  ((u16)(val) >  ((u16) 1 <<  1)) ?  2 :\
  ((u16)(val) >  ((u16) 1 <<  0)) ?  1 :\
  ((u16)(val) == ((u16) 1 <<  0)) ?  0 :\
  -1\
  )
/*----------------------------------------------------------------------------*/
#define static_log2_floor_u16(val)\
  (\
  ((u16)(val) >= ((u16) 1 << 15)) ? 15 :\
  ((u16)(val) >= ((u16) 1 << 14)) ? 14 :\
  ((u16)(val) >= ((u16) 1 << 13)) ? 13 :\
  ((u16)(val) >= ((u16) 1 << 12)) ? 12 :\
  ((u16)(val) >= ((u16) 1 << 11)) ? 11 :\
  ((u16)(val) >= ((u16) 1 << 10)) ? 10 :\
  ((u16)(val) >= ((u16) 1 <<  9)) ?  9 :\
  ((u16)(val) >= ((u16) 1 <<  8)) ?  8 :\
  ((u16)(val) >= ((u16) 1 <<  7)) ?  7 :\
  ((u16)(val) >= ((u16) 1 <<  6)) ?  6 :\
  ((u16)(val) >= ((u16) 1 <<  5)) ?  5 :\
  ((u16)(val) >= ((u16) 1 <<  4)) ?  4 :\
  ((u16)(val) >= ((u16) 1 <<  3)) ?  3 :\
  ((u16)(val) >= ((u16) 1 <<  2)) ?  2 :\
  ((u16)(val) >= ((u16) 1 <<  1)) ?  1 :\
  ((u16)(val) >= ((u16) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define static_log2_ceil_u8(val)\
  (\
  ((u8)(val) >  ((u8) 1 <<  7)) ?  8 :\
  ((u8)(val) >  ((u8) 1 <<  6)) ?  7 :\
  ((u8)(val) >  ((u8) 1 <<  5)) ?  6 :\
  ((u8)(val) >  ((u8) 1 <<  4)) ?  5 :\
  ((u8)(val) >  ((u8) 1 <<  3)) ?  4 :\
  ((u8)(val) >  ((u8) 1 <<  2)) ?  3 :\
  ((u8)(val) >  ((u8) 1 <<  1)) ?  2 :\
  ((u8)(val) >  ((u8) 1 <<  0)) ?  1 :\
  ((u8)(val) == ((u8) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#define static_log2_floor_u8(val)\
  (\
  ((u8)(val) >= ((u8) 1 <<  7)) ?  7 :\
  ((u8)(val) >= ((u8) 1 <<  6)) ?  6 :\
  ((u8)(val) >= ((u8) 1 <<  5)) ?  5 :\
  ((u8)(val) >= ((u8) 1 <<  4)) ?  4 :\
  ((u8)(val) >= ((u8) 1 <<  3)) ?  3 :\
  ((u8)(val) >= ((u8) 1 <<  2)) ?  2 :\
  ((u8)(val) >= ((u8) 1 <<  1)) ?  1 :\
  ((u8)(val) >= ((u8) 1 <<  0)) ?  0 :\
  -1\
  )
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  #define static_msb_to_right_set_u static_msb_to_right_set_u8
  #define static_next_pow2_u        static_next_pow2_u8
  #define static_round_next_pow2_u  static_round_next_pow2_u8
  #define static_log2_floor_u       static_log2_floor_u8
  #define static_log2_ceil_u        static_log2_ceil_u8
  #define pow2_u                    pow2_u8
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 16
  #define static_msb_to_right_set_u static_msb_to_right_set_u16
  #define static_next_pow2_u        static_next_pow2_u16
  #define static_round_next_pow2_u  static_round_next_pow2_u16
  #define static_log2_floor_u       static_log2_floor_u16
  #define static_log2_ceil_u        static_log2_ceil_u16
  #define pow2_u                    pow2_u16
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 32
  #define static_msb_to_right_set_u static_msb_to_right_set_u32
  #define static_next_pow2_u        static_next_pow2_u32
  #define static_round_next_pow2_u  static_round_next_pow2_u32
  #define static_log2_floor_u       static_log2_floor_u32
  #define static_log2_ceil_u        static_log2_ceil_u32
  #define pow2_u                    pow2_u32
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 64
  #define static_msb_to_right_set_u static_msb_to_right_set_u64
  #define static_next_pow2_u        static_next_pow2_u64
  #define static_round_next_pow2_u  static_round_next_pow2_u64
  #define static_log2_floor_u       static_log2_floor_u64
  #define static_log2_ceil_u        static_log2_ceil_u64
  #define pow2_u                    pow2_u64
#endif
/*---------------------------------------------------------------------------*/

#endif /* BL_STATIC_INTEGER_MATH_H */
