#ifndef __BL_INTEGER_H__
#define __BL_INTEGER_H__

#include <stdint.h>
/*---------------------------------------------------------------------------*/
#ifndef BL_EXTERNAL_INTEGER_HEADER
  typedef uint8_t   u8;
  typedef int8_t    i8;
  typedef uint16_t  u16;
  typedef int16_t   i16;
  typedef uint32_t  u32;
  typedef int32_t   i32;
  typedef uint64_t  u64;
  typedef int64_t   i64;

  typedef intptr_t  word;
  typedef uintptr_t uword;
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
#endif 

/* __INTEGER_H__ */




