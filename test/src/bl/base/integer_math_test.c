#include <bl/cmocka_pre.h>

#include <bl/base/integer_math.h>
#include <bl/base/utility.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
static void bl_msb_to_right_set_u8_test (void** s)
{
  assert_true (bl_msb_to_right_set_u8 (0) == 0);
  assert_true (bl_msb_to_right_set_u8 (128) == 255);
  assert_true(
    bl_msb_to_right_set_u8(bl_u8_bit (bl_type_bits (bl_u8) - 1))
    == (bl_u8) -1ULL
  );
}
/*---------------------------------------------------------------------------*/
static void bl_msb_to_right_set_u16_test (void** s)
{
  assert_true (bl_msb_to_right_set_u16 (0) == 0);
  assert_true (bl_msb_to_right_set_u16 (128) == 255);
  assert_true(
    bl_msb_to_right_set_u16 (bl_u16_bit (bl_type_bits (bl_u16) - 1))
    == (bl_u16) -1ULL
    );
}
/*---------------------------------------------------------------------------*/
static void bl_msb_to_right_set_u32_test (void** s)
{
  assert_true (bl_msb_to_right_set_u32 (0) == 0);
  assert_true (bl_msb_to_right_set_u32 (128) == 255);
  assert_true(
    bl_msb_to_right_set_u32(bl_u32_bit (bl_type_bits (bl_u32) - 1))
    == (bl_u32) -1ULL
    );
}
/*---------------------------------------------------------------------------*/
static void bl_msb_to_right_set_u64_test (void** s)
{
  assert_true (bl_msb_to_right_set_u64 (0) == 0);
  assert_true (bl_msb_to_right_set_u64 (128) == 255);
  assert_true(
    bl_msb_to_right_set_u64(bl_u64_bit (bl_type_bits (bl_u64) - 1))
    == (bl_u64) -1ULL
    );
}
/*---------------------------------------------------------------------------*/
static void bl_reverse_bits_test (void** s)
{
  assert_true (bl_reverse_bits (0) == 0);
  assert_true (bl_reverse_bits (bl_u8_bit (0)) == bl_u8_bit (7));
  assert_true (bl_reverse_bits (bl_u8_bit (1)) == bl_u8_bit (6));
  assert_true(
    bl_reverse_bits(bl_u8_bit (0) | bl_u8_bit (1))
    == (bl_u8_bit (6) | bl_u8_bit (7))
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_floor_u8_test (void** s)
{
  assert_true (bl_log2_floor_u8 (0) == -1);
  assert_int_equal (bl_log2_floor_u8 (bl_u8_bit (1)), 1);
  assert_true (bl_log2_floor_u8 (bl_u8_bit (1)) == 1);
  assert_true (bl_log2_floor_u8 (bl_u8_bit (6) | bl_u8_bit (5)) == 6);
  assert_true(
    bl_log2_floor_u8(bl_u8_bit (bl_type_bits (bl_u8) - 1))
    == bl_type_bits (bl_u8) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_floor_u16_test (void** s)
{
  assert_true (bl_log2_floor_u16 (0) == -1);
  assert_true (bl_log2_floor_u16 (bl_u16_bit (1)) == 1);
  assert_true (bl_log2_floor_u16 (bl_u16_bit (6)) == 6);
  assert_true (bl_log2_floor_u16 (bl_u16_bit (6) | bl_u16_bit (5)) == 6);
  assert_true(
    bl_log2_floor_u16 (bl_u16_bit (bl_type_bits (bl_u16) - 1))
    == bl_type_bits (bl_u16) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_floor_u32_test (void** s)
{
  assert_true (bl_log2_floor_u32 (0) == -1);
  assert_true (bl_log2_floor_u32 (bl_u32_bit (1)) == 1);
  assert_true (bl_log2_floor_u32 (bl_u32_bit (6) | bl_u32_bit (5)) == 6);
  assert_true(
    bl_log2_floor_u32 (bl_u32_bit (bl_type_bits (bl_u32) - 1))
    == bl_type_bits (bl_u32) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_floor_u64_test (void** s)
{
  assert_true (bl_log2_floor_u64 (0) == -1);
  assert_true (bl_log2_floor_u64 (bl_u64_bit (1)) == 1);
  assert_true (bl_log2_floor_u64 (bl_u64_bit (6) | bl_u64_bit (5)) == 6);
  assert_true(
    bl_log2_floor_u64 (bl_u64_bit (bl_type_bits (bl_u64) - 1))
    == bl_type_bits (bl_u64) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_ceil_u8_test (void** s)
{
  assert_true (bl_log2_ceil_u8 (0) == -1);
  assert_true (bl_log2_ceil_u8 (bl_u8_bit (1)) == 1);
  assert_true (bl_log2_ceil_u8 (bl_u8_bit (6) | bl_u8_bit (5)) == 7);
  assert_true(
    bl_log2_ceil_u8 (bl_u8_bit (bl_type_bits (bl_u8) - 1))
    == bl_type_bits (bl_u8) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_ceil_u16_test (void** s)
{
  assert_true (bl_log2_ceil_u16 (0) == -1);
  assert_true (bl_log2_ceil_u16 (bl_u16_bit (1)) == 1);
  assert_true (bl_log2_ceil_u16 (bl_u16_bit (6)) == 6);
  assert_true (bl_log2_ceil_u16 (bl_u16_bit (6) | bl_u16_bit (5)) == 7);
  assert_true(
    bl_log2_ceil_u16 (bl_u16_bit (bl_type_bits (bl_u16) - 1))
    == bl_type_bits (bl_u16) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_ceil_u32_test (void** s)
{
  assert_true (bl_log2_ceil_u32 (0) == -1);
  assert_true (bl_log2_ceil_u32 (bl_u32_bit (1)) == 1);
  assert_true (bl_log2_ceil_u32 (bl_u32_bit (6) | bl_u32_bit (5)) == 7);
  assert_true(
    bl_log2_ceil_u32 (bl_u32_bit (bl_type_bits (bl_u32) - 1))
    == bl_type_bits (bl_u32) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_log2_ceil_u64_test (void** s)
{
  assert_true (bl_log2_ceil_u64 (0) == -1);
  assert_true (bl_log2_ceil_u64 (bl_u64_bit (1)) == 1);
  assert_true (bl_log2_ceil_u64 (bl_u64_bit (6) | bl_u64_bit (5)) == 7);
  assert_true(
    bl_log2_ceil_u64 (bl_u64_bit (bl_type_bits (bl_u64) - 1))
    == bl_type_bits (bl_u64) - 1
    );
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (bl_msb_to_right_set_u8_test),
  cmocka_unit_test (bl_msb_to_right_set_u16_test),
  cmocka_unit_test (bl_msb_to_right_set_u32_test),
  cmocka_unit_test (bl_msb_to_right_set_u64_test),
  cmocka_unit_test (bl_reverse_bits_test),
  cmocka_unit_test (bl_log2_floor_u8_test),
  cmocka_unit_test (bl_log2_floor_u16_test),
  cmocka_unit_test (bl_log2_floor_u32_test),
  cmocka_unit_test (bl_log2_floor_u64_test),
  cmocka_unit_test (bl_log2_ceil_u8_test),
  cmocka_unit_test (bl_log2_ceil_u16_test),
  cmocka_unit_test (bl_log2_ceil_u32_test),
  cmocka_unit_test (bl_log2_ceil_u64_test),
};
/*---------------------------------------------------------------------------*/
int integer_math_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

