#ifndef __BL_INTEGER_H__
#define __BL_INTEGER_H__

#include <stdint.h>
#include <bl/hdr/base/platform.h>

/*---------------------------------------------------------------------------*/
#ifndef BL_EXTERNAL_INTEGER_HEADER
  typedef uint8_t   u8;
  typedef int8_t    i8;
  typedef uint16_t  u16;
  typedef int16_t   i16;
#ifndef BL_NOINT32
  typedef uint32_t  u32;
  typedef int32_t   i32;
#endif
#ifndef BL_NOINT64
  typedef uint64_t  u64;
  typedef int64_t   i64;
#endif
/*---------------------------------------------------------------------------*/
#if defined (BL8)
  typedef u8 uword; 
  typedef i8 word;
/*----------------------------------------------------------------------------*/
#elif defined (BL16)
  typedef u16 uword; 
  typedef i16 word;
/*----------------------------------------------------------------------------*/
#elif defined (BL32)
  typedef u32 uword; 
  typedef i32 word;
/*----------------------------------------------------------------------------*/
#elif defined (BL64)
  typedef u64 uword; 
  typedef i64 word;
/*----------------------------------------------------------------------------*/
#else
  #error "unknown word size on this platform"
#endif
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




