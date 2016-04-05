#include <bl/cmocka_pre.h>
#include <bl/base/integer_manipulation_test.h>

#include <bl/base/integer.h>
#include <bl/base/utility.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
static void has_one_or_zero_bits_set_test (void** s)
{
  for (uword i = 0; i < type_bits (u64); ++i) {
    assert_true (has_one_or_zero_bits_set ((u64) 1 << i) == true);
  }
  assert_true (has_one_or_zero_bits_set (0) == true);
}
/*---------------------------------------------------------------------------*/
static void u_bit_test (void** s)
{
  assert_true (u_bit (0) == 1);
  assert_true (u_bit (1) == 2);
  assert_true (u_bit (2) == 4);
  assert_true (u_bit (3) == 8);
}
/*---------------------------------------------------------------------------*/
static void u_bitv_test (void** s)
{
  assert_true (u_bitv (1, 0) == 1);
  assert_true (u_bitv (1, 1) == 2);
  assert_true (u_bitv (1, 2) == 4);
  assert_true (u_bitv (1, 3) == 8);

  assert_true (u_bitv (0, 0) == 0);
  assert_true (u_bitv (0, 1) == 0);
  assert_true (u_bitv (0, 2) == 0);
  assert_true (u_bitv (0, 3) == 0);
}
/*---------------------------------------------------------------------------*/
static void u_lsb_set_test (void** s)
{
  assert_true (u_lsb_set (1) == 1);
  assert_true (u_lsb_set (2) == (2 | 1));
  assert_true (u_lsb_set (3) == (4 | 2 | 1));
  assert_true (u_lsb_set (4) == (8 | 4 | 2 | 1));
}
/*---------------------------------------------------------------------------*/
static void u_msb_set_test (void** s)
{
  assert_true (u16_msb_set (0) == 0);
  assert_true (u16_msb_set (1) == (1 << 15));
  assert_true (u16_msb_set (2) == ((1 << 15) | (1 << 14)));
  assert_true (u16_msb_set (3) == ((1 << 15) | (1 << 14) | (1 << 13)));
}
/*---------------------------------------------------------------------------*/
static void u_lsb_set_shift_test (void** s)
{
  assert_true (u_lsb_set_shift (2, 3) == ((1 << 3) | (1 << 4)));
  assert_true (u_lsb_set_shift (3, 5) == ((1 << 5) | (1 << 6) | (1 << 7)));
}
/*---------------------------------------------------------------------------*/
static void u_lsb_shift_test (void** s)
{
  assert_true (u_lsb_shift (  7, 5, 3) == ((1 << 7) | (1 << 6) | (1 << 5)));
  assert_true (u_lsb_shift (  3, 5, 3) == (           (1 << 6) | (1 << 5)));
  assert_true (u_lsb_shift (  1, 5, 3) == (                      (1 << 5)));
  assert_true (u_lsb_shift (255, 5, 3) == ((1 << 7) | (1 << 6) | (1 << 5)));

  assert_true (u_lsb_shift (  7, 4, 3) == ((1 << 6) | (1 << 5) | (1 << 4)));
  assert_true (u_lsb_shift (  3, 4, 3) == (           (1 << 5) | (1 << 4)));
  assert_true (u_lsb_shift (  1, 4, 3) == (                      (1 << 4)));
  assert_true (u_lsb_shift (255, 4, 3) == ((1 << 6) | (1 << 5) | (1 << 4)));

  assert_true (u_lsb_shift (  7, 5, 2) == ((1 << 6) | (1 << 5)));
  assert_true (u_lsb_shift (  3, 5, 2) == ((1 << 6) | (1 << 5)));
  assert_true (u_lsb_shift (  1, 5, 2) == (           (1 << 5)));
  assert_true (u_lsb_shift (255, 5, 2) == ((1 << 6) | (1 << 5)));
}
/*---------------------------------------------------------------------------*/
static void u_get_bit_test (void** s)
{
  assert_true (u_get_bit (7, 0) == 1);
  assert_true (u_get_bit (7, 1) == 1);
  assert_true (u_get_bit (7, 2) == 1);
  assert_true (u_get_bit (7, 3) == 0);
  assert_true (u_get_bit (7, 4) == 0);
  assert_true (u_get_bit (7, 5) == 0);
}
/*---------------------------------------------------------------------------*/
static void u_var_set_bitv_test (void** s)
{
  assert_true (u_var_set_bitv (0, 0, 1) == 1);
  assert_true (u_var_set_bitv (1, 0, 1) == 1);
  assert_true (u_var_set_bitv (0, 0, 0) == 0);
  assert_true (u_var_set_bitv (0, 0, 0) == 0);

  assert_true (u_var_set_bitv (0, 1, 1) == 2);
  assert_true (u_var_set_bitv (2, 1, 1) == 2);
  assert_true (u_var_set_bitv (2, 1, 0) == 0);
  assert_true (u_var_set_bitv (0, 1, 0) == 0);

  assert_true (u_var_set_bitv (0, 2, 1) == 4);
  assert_true (u_var_set_bitv (4, 2, 1) == 4);
  assert_true (u_var_set_bitv (4, 2, 0) == 0);
  assert_true (u_var_set_bitv (0, 2, 0) == 0);
}
/*---------------------------------------------------------------------------*/
static void u_first_clear_lsb_mask_test (void** s)
{
  for (uword i = 1; i < type_bits (u64); ++i) {
    assert_true(
      u64_first_clear_lsb_mask (utype_max (u64) >>  i) ==
          ((u64) 1) << (type_bits (u64) - i)
      );
  }
  assert_true (u_first_clear_lsb_mask (2) == 1);
  assert_true (u_first_clear_lsb_mask (4) == 1);
  assert_true (u_first_clear_lsb_mask (6) == 1);
  assert_true (u_first_clear_lsb_mask (8) == 1);
  assert_true (u_first_clear_lsb_mask (utype_max (uword)) == 0);
}
/*---------------------------------------------------------------------------*/
static void u_first_set_lsb_mask_test (void** s)
{
  for (uword i = 0; i < type_bits (u64); ++i) {
    assert_true(
      u64_first_set_lsb_mask (utype_max (u64) <<  i) == (((u64) 1) << i)
      );
  }
  assert_true (u64_first_set_lsb_mask (0) == 0);
}
/*---------------------------------------------------------------------------*/
static void read_u64_be_test (void **s)
{
  u8 arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  assert_true (read_u64_be (arr) == 0x0102030405060708);
}
/*---------------------------------------------------------------------------*/
static void read_u64_le_test (void **s)
{ 
  u8 arr[] = {1, 2, 3, 4, 5, 6, 7, 8};  
  assert_true (read_u64_le (arr) == 0x0807060504030201);
}
/*---------------------------------------------------------------------------*/
static void write_u64_be_test (void **s)
{
  u8 arr[8];
  u64 val = 0x0102030405060708;
  write_u64_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
  assert_true (arr[3] = 4);
  assert_true (arr[4] = 5);
  assert_true (arr[5] = 6);
  assert_true (arr[6] = 7);
  assert_true (arr[7] = 8);
}
/*---------------------------------------------------------------------------*/
static void write_u64_le_test (void **s)
{
  u8 arr[8];
  u64 val = 0x0807060504030201;
  write_u64_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
  assert_true (arr[3] = 4);
  assert_true (arr[4] = 5);
  assert_true (arr[5] = 6);
  assert_true (arr[6] = 7);
  assert_true (arr[7] = 8);
}
/*---------------------------------------------------------------------------*/
static void read_u48_be_test (void **s)
{
  u8 arr[] = {1, 2, 3, 4, 5, 6};
  assert_true (read_u48_be (arr) == 0x010203040506);
}
/*---------------------------------------------------------------------------*/
static void read_u48_le_test (void **s)
{ 
  u8 arr[] = {1, 2, 3, 4, 5, 6};  
  assert_true (read_u48_le (arr) == 0x060504030201);
}
/*---------------------------------------------------------------------------*/
static void write_u48_be_test (void **s)
{
  u8 arr[6];
  u64 val = 0x010203040506;
  write_u48_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
  assert_true (arr[3] = 4);
  assert_true (arr[4] = 5);
  assert_true (arr[5] = 6);
}
/*---------------------------------------------------------------------------*/
static void write_u48_le_test (void **s)
{
  u8 arr[6];
  u64 val = 0x060504030201;
  write_u48_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
  assert_true (arr[3] = 4);
  assert_true (arr[4] = 5);
  assert_true (arr[5] = 6);
}
/*---------------------------------------------------------------------------*/
static void read_u32_be_test (void **s)
{
  u8 arr[] = {1, 2, 3, 4};
  assert_true (read_u32_be (arr) == 0x01020304);
}
/*---------------------------------------------------------------------------*/
static void read_u32_le_test (void **s)
{ 
  u8 arr[] = {1, 2, 3, 4};  
  assert_true (read_u32_le (arr) == 0x04030201);
}
/*---------------------------------------------------------------------------*/
static void write_u32_be_test (void **s)
{
  u8 arr[4];
  u32 val = 0x01020304;
  write_u32_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
  assert_true (arr[3] = 4);
}
/*---------------------------------------------------------------------------*/
static void write_u32_le_test (void **s)
{
  u8 arr[4];
  u32 val = 0x04030201;
  write_u32_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
  assert_true (arr[3] = 4);
}
/*---------------------------------------------------------------------------*/
static void read_u24_be_test (void **s)
{
  u8 arr[] = {1, 2, 3};
  assert_true (read_u24_be (arr) == 0x010203);
}
/*---------------------------------------------------------------------------*/
static void read_u24_le_test (void **s)
{ 
  u8 arr[] = {1, 2, 3};  
  assert_true (read_u24_le (arr) == 0x030201);
}
/*---------------------------------------------------------------------------*/
static void write_u24_be_test (void **s)
{
  u8 arr[3];
  u32 val = 0x010203;
  write_u24_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
}
/*---------------------------------------------------------------------------*/
static void write_u24_le_test (void **s)
{
  u8 arr[3];
  u32 val = 0x030201;
  write_u24_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
  assert_true (arr[2] = 3);
}
/*---------------------------------------------------------------------------*/
static void read_u16_be_test (void **s)
{
  u8 arr[] = {1, 2};
  assert_true (read_u16_be (arr) == 0x0102);
}
/*---------------------------------------------------------------------------*/
static void read_u16_le_test (void **s)
{ 
  u8 arr[] = {1, 2};  
  assert_true (read_u16_le (arr) == 0x0201);
}
/*---------------------------------------------------------------------------*/
static void write_u16_be_test (void **s)
{
  u8 arr[2];
  u16 val = 0x0102;
  write_u16_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
}
/*---------------------------------------------------------------------------*/
static void write_u16_le_test (void **s)
{
  u8 arr[2];
  u16 val = 0x0201;
  write_u16_le (arr, val);
  assert_true (arr[0] = 1);
  assert_true (arr[1] = 2);
}
/*---------------------------------------------------------------------------*/
static void u8_get_bit_idx_test (void **s)
{
  for (uword i = 0; i < type_bits (u8); ++i) {
    assert_true (u8_get_bit_idx ((u8) 1 << i) == i);
  }
}
/*---------------------------------------------------------------------------*/
static void u16_get_bit_idx_test (void **s)
{
  for (uword i = 0; i < type_bits (u16); ++i) {
    assert_true (u16_get_bit_idx ((u16) 1 << i) == i);
  }
}
/*---------------------------------------------------------------------------*/
static void u32_get_bit_idx_test (void **s)
{
  for (uword i = 0; i < type_bits (u32); ++i) {
    assert_true (u32_get_bit_idx (((u32) 1) << i) == i);
  }
}
/*---------------------------------------------------------------------------*/
static void u64_get_bit_idx_test (void **s)
{
  u8 vals[64];
  for (uword i = 0; i < type_bits (u64); ++i) {
    vals[i] = u64_get_bit_idx ((u64) 1 << i);
  }
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (has_one_or_zero_bits_set_test),
  cmocka_unit_test (u_bit_test),
  cmocka_unit_test (u_bitv_test),
  cmocka_unit_test (u_lsb_set_test),
  cmocka_unit_test (u_msb_set_test),
  cmocka_unit_test (u_lsb_set_shift_test),
  cmocka_unit_test (u_lsb_shift_test),
  cmocka_unit_test (u_get_bit_test),
  cmocka_unit_test (u_var_set_bitv_test),
  cmocka_unit_test (u_first_clear_lsb_mask_test),
  cmocka_unit_test (u_first_set_lsb_mask_test),
  cmocka_unit_test (read_u64_be_test),
  cmocka_unit_test (read_u64_le_test),
  cmocka_unit_test (write_u64_be_test),
  cmocka_unit_test (write_u64_le_test),
  cmocka_unit_test (read_u48_be_test),
  cmocka_unit_test (read_u48_le_test),
  cmocka_unit_test (write_u48_be_test),
  cmocka_unit_test (write_u48_le_test),
  cmocka_unit_test (read_u32_be_test),
  cmocka_unit_test (read_u32_le_test),
  cmocka_unit_test (write_u32_be_test),
  cmocka_unit_test (write_u32_le_test),
  cmocka_unit_test (read_u24_be_test),
  cmocka_unit_test (read_u24_le_test),
  cmocka_unit_test (write_u24_be_test),
  cmocka_unit_test (write_u24_le_test),
  cmocka_unit_test (read_u16_be_test),
  cmocka_unit_test (read_u16_le_test),
  cmocka_unit_test (write_u16_be_test),
  cmocka_unit_test (write_u16_le_test),
  cmocka_unit_test (u8_get_bit_idx_test),
  cmocka_unit_test (u16_get_bit_idx_test),
  cmocka_unit_test (u32_get_bit_idx_test),
  cmocka_unit_test (u64_get_bit_idx_test),
};
/*---------------------------------------------------------------------------*/
int integer_manipulation_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

