#ifndef __BL_INTEGER_H__
#define __BL_INTEGER_H__

#include <stdint.h>
#include <bl/base/platform.h>

/*---------------------------------------------------------------------------*/
typedef uint8_t  bl_u8;
typedef int8_t   bl_i8;
#if BL_WORDSIZE_MAX >= 16
  typedef uint16_t bl_u16;
  typedef int16_t  bl_i16;
#endif
#if BL_WORDSIZE_MAX >= 32
  typedef uint32_t bl_u32;
  typedef int32_t  bl_i32;
#endif
#if BL_WORDSIZE_MAX >= 64
  typedef uint64_t bl_u64;
  typedef int64_t  bl_i64;
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  typedef bl_u8 bl_uword;
  typedef bl_i8 bl_word;
  #define BL_HAS_WORD_D2 0
  #define BL_HAS_WORD_D4 0
/*----------------------------------------------------------------------------*/
#elif BL_WORDSIZE == 16
  typedef bl_u16 bl_uword;
  typedef bl_i16 bl_word;
  typedef bl_u8  bl_uword_d2;
  typedef bl_i8  bl_word_d2;
  #define BL_HAS_WORD_D2 1
  #define BL_HAS_WORD_D4 0
  /*----------------------------------------------------------------------------*/
#elif BL_WORDSIZE == 32
  typedef bl_u32 bl_uword;
  typedef bl_i32 bl_word;
  typedef bl_u16 bl_uword_d2;
  typedef bl_i16 bl_word_d2;
  typedef bl_u8  bl_uword_d4;
  typedef bl_i8  bl_word_d4;
  #define BL_HAS_WORD_D2 1
  #define BL_HAS_WORD_D4 1
/*----------------------------------------------------------------------------*/
#elif BL_WORDSIZE == 64
  typedef bl_u64 bl_uword;
  typedef bl_i64 bl_word;
  typedef bl_u32 bl_uword_d2;
  typedef bl_i32 bl_word_d2;
  typedef bl_u16 bl_uword_d4;
  typedef bl_i16 bl_word_d4;
  #define BL_HAS_WORD_D2 1
  #define BL_HAS_WORD_D4 1
/*----------------------------------------------------------------------------*/
#else
  #error "unknown bl_word size on this platform"
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
  typedef bl_u64 bl_ubig;
  typedef bl_i64 bl_ibig;
  typedef bl_u32 bl_ubig_d2;
  typedef bl_i32 bl_ibig_d2;
  typedef bl_u16 bl_ubig_d4;
  typedef bl_i16 bl_ibig_d4;
  #define BL_HAS_BIG_D2 1
  #define BL_HAS_BIG_D4 1
#elif BL_WORDSIZE_MAX >= 32
  typedef bl_u32 bl_ubig;
  typedef bl_i32 bl_ibig;
  typedef bl_u16 bl_ubig_d2;
  typedef bl_i16 bl_ibig_d2;
  typedef bl_u8  bl_ubig_d4;
  typedef bl_i8  bl_ibig_d4;
  #define BL_HAS_BIG_D2 1
  #define BL_HAS_BIG_D4 1
#elif BL_WORDSIZE_MAX >= 16
  typedef bl_u16 bl_ubig;
  typedef bl_i16 bl_ibig;
  typedef bl_u8  bl_ubig_d2;
  typedef bl_i8  bl_ibig_d2;
  #define BL_HAS_BIG_D2 1
  #define BL_HAS_BIG_D4 0
#else
  typedef bl_u8 bl_ubig;
  typedef bl_i8 bl_ibig;
  #define BL_HAS_BIG_D2 0
  #define BL_HAS_BIG_D4 0
#endif
/*---------------------------------------------------------------------------*/
#define bl_utype_max(type) ((type) (~((type) 0)))
/*---------------------------------------------------------------------------*/
#define bl_itype_max(type) (bl_utype_max (type) >> (type) 1)
/*---------------------------------------------------------------------------*/
#define bl_has_two_comp_arithmetic (bl_utype_max (unsigned) == (unsigned) -1)
/*---------------------------------------------------------------------------*/
#define bl_itype_min(type) bl_has_two_comp_arithmetic ?\
  (-bl_itype_max (type) - 1) : (-bl_itype_max (type))

#endif /* __INTEGER_H__ */
