#ifndef __BL_INTEGER_SHORT_H__
#define __BL_INTEGER_SHORT_H__

#ifdef BL_COMPILATION
  #error "This is a user-only header that introduces prone-to-clash typedefs. This error is to avoid using these types on our interface."
#endif

#include <bl/base/integer.h>

typedef bl_u8  u8;
typedef bl_u16 u16;
typedef bl_u32 u32;
typedef bl_u64 u64;

typedef bl_i8  i8;
typedef bl_i16 i16;
typedef bl_i32 i32;
typedef bl_i64 i64;

typedef bl_uword uword;
typedef bl_word  word;
#if BL_HAS_WORD_D2 == 1
typedef bl_uword_d2 uword_d2;
typedef bl_word_d2  word_d2;
#endif
#if BL_HAS_WORD_D4 == 1
typedef bl_uword_d4 uword_d4;
typedef bl_word_d4  word_d4;
#endif

typedef bl_ubig ubig;
typedef bl_ibig ibig;
#if BL_HAS_BIG_D2 == 1
typedef bl_ubig_d2 ubig_d2;
typedef bl_big_d2  big_d2;
#endif
#if BL_HAS_BIG_D4 == 1
typedef bl_ubig_d4 ubig_d4;
typedef bl_big_d4  big_d4;
#endif

#endif /* __BL_INTEGER_SHORT_H__ */
