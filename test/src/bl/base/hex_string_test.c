#include <bl/cmocka_pre.h>

#include <bl/base/platform.h>
#include <bl/base/utility.h>
#include <bl/base/hex_string.c>
/*---------------------------------------------------------------------------*/
static void hex_string_to_bytes_even_string (void** s)
{
  static const char str[] = "000f0e101499";
  static const bl_u8   exp[] = {0x00, 0x0f, 0x0e, 0x10, 0x14, 0x99 };
  bl_u8 buff[bl_arr_elems (exp)];
  bl_word res = bl_hex_string_to_bytes (buff, bl_arr_elems (buff), str);
  assert_true (res == bl_arr_elems (exp));
  assert_memory_equal (buff, exp, res);
}
/*---------------------------------------------------------------------------*/
static void hex_string_to_bytes_odd_string (void** s)
{
  static const char str[] = "000f0e1014994";
  static const bl_u8   exp[] = {0x00, 0x0f, 0x0e, 0x10, 0x14, 0x99, 0x40 };
  bl_u8 buff[bl_arr_elems (exp)];
  bl_word res = bl_hex_string_to_bytes (buff, bl_arr_elems (buff), str);
  assert_true (res == bl_arr_elems (exp));
  assert_memory_equal (buff, exp, res);
}
/*---------------------------------------------------------------------------*/
static void hex_string_to_bytes_invalid_char (void** s)
{
  static const char str[] = "000f0eX014994";
  static const bl_u8   exp[] = {0x00, 0x0f, 0x0e, 0x10, 0x14, 0x99, 0x40 };
  bl_u8 buff[bl_arr_elems (exp)];
  bl_word res = bl_hex_string_to_bytes (buff, bl_arr_elems (buff), str);
  assert_true (-1 == res);
}
/*---------------------------------------------------------------------------*/
static void hex_string_to_bytes_not_enough_buffer (void** s)
{
  static const char str[] = "000f0eX014994";
  static const bl_u8   exp[] = {0x00, 0x0f, 0x0e, 0x10, 0x14, 0x99, 0x40 };
  bl_u8 buff[bl_arr_elems (exp)];
  bl_word res = bl_hex_string_to_bytes (buff, 2, str);
  assert_true (-1 == res);
}
/*---------------------------------------------------------------------------*/
static void bytes_to_hex_string_normal (void** s)
{
  static const char exp[] = "000f0e101499";
  static const bl_u8   bytes[] = {0x00, 0x0f, 0x0e, 0x10, 0x14, 0x99 };
  char buff[bl_arr_elems (exp)];
  bl_word res = bl_bytes_to_hex_string(
    buff, bl_arr_elems (buff), bytes, bl_arr_elems (bytes)
    );
  assert_true (res == bl_arr_elems (exp) - 1);
  assert_memory_equal (buff, exp, res);
}
/*---------------------------------------------------------------------------*/
static void bytes_to_hex_string_not_enough_buffer (void** s)
{
  static const char exp[] = "000f0e101499";
  static const bl_u8   bytes[] = {0x00, 0x0f, 0x0e, 0x10, 0x14, 0x99 };
  char buff[bl_arr_elems (exp)];
  bl_word res = bl_bytes_to_hex_string(
    buff, bl_arr_elems (exp) - 1, bytes, bl_arr_elems (bytes)
    );
  assert_true (-1 == res);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (hex_string_to_bytes_even_string),
  cmocka_unit_test (hex_string_to_bytes_odd_string),
  cmocka_unit_test (hex_string_to_bytes_invalid_char),
  cmocka_unit_test (hex_string_to_bytes_not_enough_buffer),
  cmocka_unit_test (bytes_to_hex_string_normal),
  cmocka_unit_test (bytes_to_hex_string_not_enough_buffer),
};
/*---------------------------------------------------------------------------*/
int hex_string_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

