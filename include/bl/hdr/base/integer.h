#ifndef __BL_INTEGER_H__
#define __BL_INTEGER_H__

#include <stdint.h>
#include <bl/hdr/base/platform.h>

/*---------------------------------------------------------------------------*/
#ifndef BL_EXTERNAL_INTEGER_HEADER
  typedef uint8_t  u8;
  typedef int8_t   i8;
#if BL_WORDSIZE_MAX >= 16
  typedef uint16_t u16;
  typedef int16_t  i16;
#endif
#if BL_WORDSIZE_MAX >= 32
  typedef uint32_t u32;
  typedef int32_t  i32;
#endif
#if BL_WORDSIZE_MAX >= 64
  typedef uint64_t u64;
  typedef int64_t  i64;
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  typedef u8 uword; 
  typedef i8 word;
/*----------------------------------------------------------------------------*/
#elif BL_WORDSIZE == 16
  typedef u16 uword; 
  typedef i16 word;
  typedef u8  uword_d2; 
  typedef i8  word_d2;
  /*----------------------------------------------------------------------------*/
#elif BL_WORDSIZE == 32
  typedef u32 uword; 
  typedef i32 word;
  typedef u16 uword_d2; 
  typedef i16 word_d2;
  typedef u8  uword_d4; 
  typedef i8  word_d4;
/*----------------------------------------------------------------------------*/
#elif BL_WORDSIZE == 64
  typedef u64 uword; 
  typedef i64 word;
  typedef u32 uword_d2; 
  typedef i32 word_d2;
  typedef u16 uword_d4; 
  typedef i16 word_d4;
/*----------------------------------------------------------------------------*/
#else
  #error "unknown word size on this platform"
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE_MAX >= 64
  typedef u64 ubig;
  typedef i64 ibig;
  typedef u32 ubig_d2;
  typedef i32 ibig_d2;
  typedef u16 ubig_d4;
  typedef i16 ibig_d4;
#elif BL_WORDSIZE_MAX >= 32
  typedef u32 ubig;
  typedef i32 ibig;
  typedef u16 ubig_d2;
  typedef i16 ibig_d2;
  typedef u8  ubig_d4;
  typedef i8  ibig_d4;
#elif BL_WORDSIZE_MAX >= 16
  typedef u16 ubig;
  typedef i16 ibig;
  typedef u8  ubig_d2;
  typedef i8  ibig_d2;
#else
  typedef u8 ubig;
  typedef i8 ibig;
#endif
/*---------------------------------------------------------------------------*/
#else
  #include BL_EXTERNAL_INTEGER_HEADER
#endif
/*---------------------------------------------------------------------------*/
#define utype_max(type) ((type) (~((type) 0)))
/*---------------------------------------------------------------------------*/
#define itype_max(type) (utype_max (type) >> (type) 1)
/*---------------------------------------------------------------------------*/
#define bl_has_two_comp_arithmetic (utype_max (unsigned) == (unsigned) -1)
/*---------------------------------------------------------------------------*/
#define itype_min(type) bl_has_two_comp_arithmetic ?\
  (-itype_max (type) - 1) : (-itype_max (type))
/*---------------------------------------------------------------------------*/
#endif /* __INTEGER_H__ */
