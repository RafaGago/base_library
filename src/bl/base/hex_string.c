#include <bl/base/assert.h>
#include <bl/base/hex_string.h>

/*---------------------------------------------------------------------------*/
BL_EXPORT bl_word bl_hex_string_to_bytes(
  bl_u8* bytes, bl_uword bytes_capacity, char const* str
  )
{
  bl_word char_count = 0;
  bl_assert (str && bytes);
  bl_assert (bytes_capacity > 0);

  while (*str != '\0') {
    char c = *str;
    bl_u8 nibble;
    if (c >= '0' && c <= '9') {
      nibble = c - '0';
    }
    else if (c >= 'a' && c <= 'f') {
      nibble = c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F') {
      nibble = c - 'A' + 10;
    }
    else {
      return -1;
    }
    ++char_count;
    ++str;
    if ((char_count & 1) == 1) {
      if (char_count > (bl_word) bytes_capacity * 2) {
        return -1;
      }
      *bytes = nibble << 4;
    }
    else {
      *bytes |= nibble;
      ++bytes;
    }
  }
  return (char_count / 2) + (char_count & 1);
}
/*---------------------------------------------------------------------------*/
BL_EXPORT bl_word bl_bytes_to_hex_string(
  char* str, bl_uword str_capacity, bl_u8 const* bytes, bl_uword bytes_size
  )
{
  static const bl_u8 hex_lut[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f'
  };
  bl_assert (str && bytes);
  if ((str_capacity == 0 && bytes_size > 0) ||
    str_capacity - 1 < bytes_size * 2) {
    return -1;
  }
  bl_uword i;
  for (i = 0; i < bytes_size; ++i, ++bytes, str += 2) {
    str[0] = hex_lut[*bytes >> 4];
    str[1] = hex_lut[*bytes & 15];
  }
  *str = '\0';
  return (bl_word) (i * 2);
}
/*---------------------------------------------------------------------------*/
