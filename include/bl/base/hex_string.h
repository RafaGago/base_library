#ifndef __BL_HEX_STRING_H__
#define __BL_HEX_STRING_H__

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/libexport.h>

/*---------------------------------------------------------------------------*/
/*returns -1 on error, positive byte number if succeeds*/
extern BL_EXPORT word bl_hex_string_to_bytes(
	u8* bytes, uword bytes_capacity, char const* str
	);
/*---------------------------------------------------------------------------*/
/*returns -1 on error, positive string length if succeeds (trailing null
  not included on the count)*/
extern BL_EXPORT word bl_bytes_to_hex_string(
	char* str, uword str_capacity, u8 const* bytes, uword bytes_size
	);
/*---------------------------------------------------------------------------*/

#endif /*__BL_HEX_STRING_H__*/
