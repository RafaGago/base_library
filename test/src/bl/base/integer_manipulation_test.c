#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/base/utility.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
static void bl_has_one_or_zero_bits_set_test (void** s)
{
  for (bl_uword i = 0; i < bl_type_bits (bl_u64); ++i) {
    assert_int_equal (bl_has_one_or_zero_bits_set ((bl_u64) 1 << i), true);
  }
  assert_int_equal (bl_has_one_or_zero_bits_set (0), true);
}
/*---------------------------------------------------------------------------*/
static void bl_u_bit_test (void** s)
{
  assert_int_equal (bl_u_bit (0), 1);
  assert_int_equal (bl_u_bit (1), 2);
  assert_int_equal (bl_u_bit (2), 4);
  assert_int_equal (bl_u_bit (3), 8);
}
/*---------------------------------------------------------------------------*/
static void bl_u_bitv_test (void** s)
{
  assert_int_equal (bl_u_bitv (1, 0), 1);
  assert_int_equal (bl_u_bitv (1, 1), 2);
  assert_int_equal (bl_u_bitv (1, 2), 4);
  assert_int_equal (bl_u_bitv (1, 3), 8);

  assert_int_equal (bl_u_bitv (0, 0), 0);
  assert_int_equal (bl_u_bitv (0, 1), 0);
  assert_int_equal (bl_u_bitv (0, 2), 0);
  assert_int_equal (bl_u_bitv (0, 3), 0);
}
/*---------------------------------------------------------------------------*/
static void bl_u_lsb_set_test (void** s)
{
  assert_int_equal (bl_u_lsb_set (0), 0);
  assert_int_equal (bl_u_lsb_set (1), 1);
  assert_int_equal (bl_u_lsb_set (2), (2 | 1));
  assert_int_equal (bl_u_lsb_set (3), (4 | 2 | 1));
  assert_int_equal (bl_u_lsb_set (4), (8 | 4 | 2 | 1));
  assert_int_equal (bl_u64_lsb_set (64), -1ull);
  assert_int_equal (bl_u64_lsb_set (66), -1ull);
}
/*---------------------------------------------------------------------------*/
static void bl_u_msb_set_test (void** s)
{
  assert_int_equal (bl_u16_msb_set (0), 0);
  assert_int_equal (bl_u16_msb_set (1), (1 << 15));
  assert_int_equal (bl_u16_msb_set (2), ((1 << 15) | (1 << 14)));
  assert_int_equal (bl_u16_msb_set (3), ((1 << 15) | (1 << 14) | (1 << 13)));
  assert_int_equal (bl_u64_msb_set (64), -1ull);
  assert_int_equal (bl_u64_msb_set (66), -1ull);
}
/*---------------------------------------------------------------------------*/
static void bl_u_lsb_set_shift_test (void** s)
{
  assert_int_equal (bl_u_lsb_set_shift (2, 3), ((1 << 3) | (1 << 4)));
  assert_int_equal(
    bl_u_lsb_set_shift (3, 5), ((1 << 5) | (1 << 6) | (1 << 7))
    );
}
/*---------------------------------------------------------------------------*/
static void bl_u_lsb_shift_test (void** s)
{
  assert_int_equal(
    bl_u_lsb_shift (  7, 5, 3), ((1 << 7) | (1 << 6) | (1 << 5))
    );
  assert_int_equal(
    bl_u_lsb_shift (  3, 5, 3), (           (1 << 6) | (1 << 5))
    );
  assert_int_equal(
    bl_u_lsb_shift (  1, 5, 3), (                      (1 << 5))
    );
  assert_int_equal(
    bl_u_lsb_shift (255, 5, 3), ((1 << 7) | (1 << 6) | (1 << 5))
    );
  assert_int_equal(
    bl_u_lsb_shift (  7, 4, 3), ((1 << 6) | (1 << 5) | (1 << 4))
    );
  assert_int_equal(
    bl_u_lsb_shift (  3, 4, 3), (           (1 << 5) | (1 << 4))
    );
  assert_int_equal(
    bl_u_lsb_shift (  1, 4, 3), (                      (1 << 4))
    );
  assert_int_equal(
    bl_u_lsb_shift (255, 4, 3), ((1 << 6) | (1 << 5) | (1 << 4)));

  assert_int_equal(bl_u_lsb_shift (  7, 5, 2), ((1 << 6) | (1 << 5)));
  assert_int_equal(bl_u_lsb_shift (  3, 5, 2), ((1 << 6) | (1 << 5)));
  assert_int_equal(bl_u_lsb_shift (  1, 5, 2), (           (1 << 5)));
  assert_int_equal(bl_u_lsb_shift (255, 5, 2), ((1 << 6) | (1 << 5)));

  assert_int_equal(bl_u_lsb_shift (255, 64, 2), 0);
  assert_int_equal(bl_u_lsb_shift (255, 65, 2), 0);
}
/*---------------------------------------------------------------------------*/
static void bl_u_get_bit_test (void** s)
{
  assert_int_equal (bl_u_get_bit (7, 0), 1);
  assert_int_equal (bl_u_get_bit (7, 1), 1);
  assert_int_equal (bl_u_get_bit (7, 2), 1);
  assert_int_equal (bl_u_get_bit (7, 3), 0);
  assert_int_equal (bl_u_get_bit (7, 4), 0);
  assert_int_equal (bl_u_get_bit (7, 5), 0);
  assert_int_equal (bl_u_get_bit (7, 33), 0);
  assert_int_equal (bl_u_get_bit (7, 65), 0);
}
/*---------------------------------------------------------------------------*/
static void bl_u_var_set_bitv_test (void** s)
{
  assert_int_equal (bl_u_var_set_bitv (0, 0, 1), 1);
  assert_int_equal (bl_u_var_set_bitv (1, 0, 1), 1);
  assert_int_equal (bl_u_var_set_bitv (0, 0, 0), 0);
  assert_int_equal (bl_u_var_set_bitv (0, 0, 0), 0);

  assert_int_equal (bl_u_var_set_bitv (0, 1, 1), 2);
  assert_int_equal (bl_u_var_set_bitv (2, 1, 1), 2);
  assert_int_equal (bl_u_var_set_bitv (2, 1, 0), 0);
  assert_int_equal (bl_u_var_set_bitv (0, 1, 0), 0);

  assert_int_equal (bl_u_var_set_bitv (0, 2, 1), 4);
  assert_int_equal (bl_u_var_set_bitv (4, 2, 1), 4);
  assert_int_equal (bl_u_var_set_bitv (4, 2, 0), 0);
  assert_int_equal (bl_u_var_set_bitv (0, 2, 0), 0);
}
/*---------------------------------------------------------------------------*/
static void bl_u_first_clear_lsb_mask_test (void** s)
{
  for (bl_uword i = 1; i < bl_type_bits (bl_u64); ++i) {
    assert_int_equal(
      bl_u64_first_clear_lsb_mask (bl_utype_max (bl_u64) >>  i),
          ((bl_u64) 1) << (bl_type_bits (bl_u64) - i)
      );
  }
  assert_int_equal (bl_u_first_clear_lsb_mask (2), 1);
  assert_int_equal (bl_u_first_clear_lsb_mask (4), 1);
  assert_int_equal (bl_u_first_clear_lsb_mask (6), 1);
  assert_int_equal (bl_u_first_clear_lsb_mask (8), 1);
  assert_int_equal (bl_u_first_clear_lsb_mask (bl_utype_max (bl_uword)), 0);
}
/*---------------------------------------------------------------------------*/
static void bl_u_first_set_lsb_mask_test (void** s)
{
  for (bl_uword i = 0; i < bl_type_bits (bl_u64); ++i) {
    assert_int_equal(
      bl_u64_first_set_lsb_mask (bl_utype_max (bl_u64) <<  i),
      (((bl_u64) 1ull) << i)
      );
  }
  assert_int_equal (bl_u64_first_set_lsb_mask (0), 0);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u64_be_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  assert_int_equal (bl_read_u64_be (arr), 0x0102030405060708);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u64_le_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  assert_int_equal (bl_read_u64_le (arr), 0x0807060504030201);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u64_be_test (void **s)
{
  bl_u8 arr[8];
  bl_u64 val = 0x0102030405060708;
  bl_write_u64_be (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
  assert_int_equal (arr[3], 4);
  assert_int_equal (arr[4], 5);
  assert_int_equal (arr[5], 6);
  assert_int_equal (arr[6], 7);
  assert_int_equal (arr[7], 8);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u64_le_test (void **s)
{
  bl_u8 arr[8];
  bl_u64 val = 0x0807060504030201;
  bl_write_u64_le (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
  assert_int_equal (arr[3], 4);
  assert_int_equal (arr[4], 5);
  assert_int_equal (arr[5], 6);
  assert_int_equal (arr[6], 7);
  assert_int_equal (arr[7], 8);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u48_be_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3, 4, 5, 6};
  assert_int_equal (bl_read_u48_be (arr), 0x010203040506);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u48_le_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3, 4, 5, 6};
  assert_int_equal (bl_read_u48_le (arr), 0x060504030201);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u48_be_test (void **s)
{
  bl_u8 arr[6];
  bl_u64 val = 0x010203040506;
  bl_write_u48_be (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
  assert_int_equal (arr[3], 4);
  assert_int_equal (arr[4], 5);
  assert_int_equal (arr[5], 6);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u48_le_test (void **s)
{
  bl_u8 arr[6];
  bl_u64 val = 0x060504030201;
  bl_write_u48_le (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
  assert_int_equal (arr[3], 4);
  assert_int_equal (arr[4], 5);
  assert_int_equal (arr[5], 6);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u32_be_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3, 4};
  assert_int_equal (bl_read_u32_be (arr), 0x01020304);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u32_le_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3, 4};
  assert_int_equal (bl_read_u32_le (arr), 0x04030201);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u32_be_test (void **s)
{
  bl_u8 arr[4];
  bl_u32 val = 0x01020304;
  bl_write_u32_be (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
  assert_int_equal (arr[3], 4);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u32_le_test (void **s)
{
  bl_u8 arr[4];
  bl_u32 val = 0x04030201;
  bl_write_u32_le (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
  assert_int_equal (arr[3], 4);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u24_be_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3};
  assert_int_equal (bl_read_u24_be (arr), 0x010203);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u24_le_test (void **s)
{
  bl_u8 arr[] = {1, 2, 3};
  assert_int_equal (bl_read_u24_le (arr), 0x030201);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u24_be_test (void **s)
{
  bl_u8 arr[3];
  bl_u32 val = 0x010203;
  bl_write_u24_be (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u24_le_test (void **s)
{
  bl_u8 arr[3];
  bl_u32 val = 0x030201;
  bl_write_u24_le (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
  assert_int_equal (arr[2], 3);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u16_be_test (void **s)
{
  bl_u8 arr[] = {1, 2};
  assert_int_equal (bl_read_u16_be (arr), 0x0102);
}
/*---------------------------------------------------------------------------*/
static void bl_read_u16_le_test (void **s)
{
  bl_u8 arr[] = {1, 2};
  assert_int_equal (bl_read_u16_le (arr), 0x0201);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u16_be_test (void **s)
{
  bl_u8 arr[2];
  bl_u16 val = 0x0102;
  bl_write_u16_be (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
}
/*---------------------------------------------------------------------------*/
static void bl_write_u16_le_test (void **s)
{
  bl_u8 arr[2];
  bl_u16 val = 0x0201;
  bl_write_u16_le (arr, val);
  assert_int_equal (arr[0], 1);
  assert_int_equal (arr[1], 2);
}
/*---------------------------------------------------------------------------*/
static void bl_u8_get_bit_idx_test (void **s)
{
  for (bl_uword i = 0; i < bl_type_bits (bl_u8); ++i) {
    assert_int_equal (bl_u8_get_bit_idx ((bl_u8) 1 << i), i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_u16_get_bit_idx_test (void **s)
{
  for (bl_uword i = 0; i < bl_type_bits (bl_u16); ++i) {
    assert_int_equal (bl_u16_get_bit_idx ((bl_u16) 1 << i), i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_u32_get_bit_idx_test (void **s)
{
  for (bl_uword i = 0; i < bl_type_bits (bl_u32); ++i) {
    assert_int_equal (bl_u32_get_bit_idx (((bl_u32) 1) << i), i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_u64_get_bit_idx_test (void **s)
{
  bl_u8 vals[64];
  for (bl_uword i = 0; i < bl_type_bits (bl_u64); ++i) {
    vals[i] = bl_u64_get_bit_idx ((bl_u64) 1 << i);
    assert_int_equal (vals[i], i);
  }
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (bl_has_one_or_zero_bits_set_test),
  cmocka_unit_test (bl_u_bit_test),
  cmocka_unit_test (bl_u_bitv_test),
  cmocka_unit_test (bl_u_lsb_set_test),
  cmocka_unit_test (bl_u_msb_set_test),
  cmocka_unit_test (bl_u_lsb_set_shift_test),
  cmocka_unit_test (bl_u_lsb_shift_test),
  cmocka_unit_test (bl_u_get_bit_test),
  cmocka_unit_test (bl_u_var_set_bitv_test),
  cmocka_unit_test (bl_u_first_clear_lsb_mask_test),
  cmocka_unit_test (bl_u_first_set_lsb_mask_test),
  cmocka_unit_test (bl_read_u64_be_test),
  cmocka_unit_test (bl_read_u64_le_test),
  cmocka_unit_test (bl_write_u64_be_test),
  cmocka_unit_test (bl_write_u64_le_test),
  cmocka_unit_test (bl_read_u48_be_test),
  cmocka_unit_test (bl_read_u48_le_test),
  cmocka_unit_test (bl_write_u48_be_test),
  cmocka_unit_test (bl_write_u48_le_test),
  cmocka_unit_test (bl_read_u32_be_test),
  cmocka_unit_test (bl_read_u32_le_test),
  cmocka_unit_test (bl_write_u32_be_test),
  cmocka_unit_test (bl_write_u32_le_test),
  cmocka_unit_test (bl_read_u24_be_test),
  cmocka_unit_test (bl_read_u24_le_test),
  cmocka_unit_test (bl_write_u24_be_test),
  cmocka_unit_test (bl_write_u24_le_test),
  cmocka_unit_test (bl_read_u16_be_test),
  cmocka_unit_test (bl_read_u16_le_test),
  cmocka_unit_test (bl_write_u16_be_test),
  cmocka_unit_test (bl_write_u16_le_test),
  cmocka_unit_test (bl_u8_get_bit_idx_test),
  cmocka_unit_test (bl_u16_get_bit_idx_test),
  cmocka_unit_test (bl_u32_get_bit_idx_test),
  cmocka_unit_test (bl_u64_get_bit_idx_test),
};
/*---------------------------------------------------------------------------*/
int integer_manipulation_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

