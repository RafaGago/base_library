#ifndef __BL_INTEGER_SHORT_H__
#define __BL_INTEGER_SHORT_H__

#ifdef BL_COMPILATION
  #error "This is a user-only header that introduces prone-to-clash typedefs. This error is to avoid using these types on our interface."
#endif

#include <bl/base/integer.h>

typedef u8  bl_u8;
typedef u16 bl_u16;
typedef u32 bl_u32;
typedef u64 bl_u64;

typedef i8  bl_i8;
typedef i16 bl_i16;
typedef i32 bl_i32;
typedef i64 bl_i64;

typedef uword bl_uword;
typedef word  bl_word;
#if BL_HAS_WORD_D2 == 1
typedef uword_d2 bl_uword_d2;
typedef word_d2  bl_word_d2;
#endif
#if BL_HAS_WORD_D4 == 1
typedef uword_d4 bl_uword_d4;
typedef word_d4  bl_word_d4;
#endif

typedef ubig bl_ubig;
typedef ibig bl_ibig;
#if BL_HAS_BIG_D2 == 1
typedef ubig_d2 bl_ubig_d2;
typedef big_d2  bl_big_d2;
#endif
#if BL_HAS_BIG_D4 == 1
typedef ubig_d4 bl_ubig_d4;
typedef big_d4  bl_big_d4;
#endif

#endif /* __BL_INTEGER_SHORT_H__ */
