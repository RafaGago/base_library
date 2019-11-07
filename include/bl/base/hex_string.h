#ifndef __BL_HEX_STRING_H__
#define __BL_HEX_STRING_H__

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/libexport.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
/*returns -1 on error, positive byte number if succeeds*/
extern BL_EXPORT bl_word bl_hex_string_to_bytes(
  bl_u8* bytes, bl_uword bytes_capacity, char const* str
  );
/*---------------------------------------------------------------------------*/
/*returns -1 on error, positive string length if succeeds (trailing null
  not included on the count)*/
extern BL_EXPORT bl_word bl_bytes_to_hex_string(
  char* str, bl_uword str_capacity, bl_u8 const* bytes, bl_uword bytes_size
  );
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} //extern "C" {
#endif

#endif /*__BL_HEX_STRING_H__*/
