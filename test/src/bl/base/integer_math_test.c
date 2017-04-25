#include <bl/cmocka_pre.h>

#include <bl/base/integer_math.h>
#include <bl/base/utility.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
static void msb_to_right_set_u8_test (void** s)
{
  assert_true (msb_to_right_set_u8 (0) == 0);
  assert_true (msb_to_right_set_u8 (128) == 255);
  assert_true (msb_to_right_set_u8 (u8_bit (type_bits (u8) - 1)) == (u8) -1ULL);
}
/*---------------------------------------------------------------------------*/
static void msb_to_right_set_u16_test (void** s)
{
  assert_true (msb_to_right_set_u16 (0) == 0);
  assert_true (msb_to_right_set_u16 (128) == 255);
  assert_true(
    msb_to_right_set_u16 (u16_bit (type_bits (u16) - 1)) == (u16) -1ULL
    );
}
/*---------------------------------------------------------------------------*/
static void msb_to_right_set_u32_test (void** s)
{
  assert_true (msb_to_right_set_u32 (0) == 0);
  assert_true (msb_to_right_set_u32 (128) == 255);
  assert_true(
    msb_to_right_set_u32 (u32_bit (type_bits (u32) - 1)) == (u32) -1ULL
    );
}
/*---------------------------------------------------------------------------*/
static void msb_to_right_set_u64_test (void** s)
{
  assert_true (msb_to_right_set_u64 (0) == 0);
  assert_true (msb_to_right_set_u64 (128) == 255);
  assert_true(
    msb_to_right_set_u64 (u64_bit (type_bits (u64) - 1)) == (u64) -1ULL
    );
}
/*---------------------------------------------------------------------------*/
static void reverse_bits_test (void** s)
{
  assert_true (reverse_bits (0) == 0);
  assert_true (reverse_bits (u8_bit (0)) == u8_bit (7));
  assert_true (reverse_bits (u8_bit (1)) == u8_bit (6));
  assert_true(
    reverse_bits (u8_bit (0) | u8_bit (1)) == (u8_bit (6) | u8_bit (7))
    );
}
/*---------------------------------------------------------------------------*/
static void log2_floor_u8_test (void** s)
{
  assert_true (log2_floor_u8 (0) == -1);
  assert_int_equal (log2_floor_u8 (u8_bit (1)), 1);
  assert_true (log2_floor_u8 (u8_bit (1)) == 1);
  assert_true (log2_floor_u8 (u8_bit (6) | u8_bit (5)) == 6);
  assert_true(
    log2_floor_u8 (u8_bit (type_bits (u8) - 1)) == type_bits (u8) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void log2_floor_u16_test (void** s)
{
  assert_true (log2_floor_u16 (0) == -1);
  assert_true (log2_floor_u16 (u16_bit (1)) == 1);
  assert_true (log2_floor_u16 (u16_bit (6)) == 6);
  assert_true (log2_floor_u16 (u16_bit (6) | u16_bit (5)) == 6);
  assert_true(
    log2_floor_u16 (u16_bit (type_bits (u16) - 1)) == type_bits (u16) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void log2_floor_u32_test (void** s)
{
  assert_true (log2_floor_u32 (0) == -1);
  assert_true (log2_floor_u32 (u32_bit (1)) == 1);
  assert_true (log2_floor_u32 (u32_bit (6) | u32_bit (5)) == 6);
  assert_true(
    log2_floor_u32 (u32_bit (type_bits (u32) - 1)) == type_bits (u32) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void log2_floor_u64_test (void** s)
{
  assert_true (log2_floor_u64 (0) == -1);
  assert_true (log2_floor_u64 (u64_bit (1)) == 1);
  assert_true (log2_floor_u64 (u64_bit (6) | u64_bit (5)) == 6);
  assert_true(
    log2_floor_u64 (u64_bit (type_bits (u64) - 1)) == type_bits (u64) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void log2_ceil_u8_test (void** s)
{
  assert_true (log2_ceil_u8 (0) == -1);
  assert_true (log2_ceil_u8 (u8_bit (1)) == 1);
  assert_true (log2_ceil_u8 (u8_bit (6) | u8_bit (5)) == 7);
  assert_true(
    log2_ceil_u8 (u8_bit (type_bits (u8) - 1)) == type_bits (u8) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void log2_ceil_u16_test (void** s)
{
  assert_true (log2_ceil_u16 (0) == -1);
  assert_true (log2_ceil_u16 (u16_bit (1)) == 1);
  assert_true (log2_ceil_u16 (u16_bit (6)) == 6);
  assert_true (log2_ceil_u16 (u16_bit (6) | u16_bit (5)) == 7);
  assert_true(
    log2_ceil_u16 (u16_bit (type_bits (u16) - 1)) == type_bits (u16) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void log2_ceil_u32_test (void** s)
{
  assert_true (log2_ceil_u32 (0) == -1);
  assert_true (log2_ceil_u32 (u32_bit (1)) == 1);
  assert_true (log2_ceil_u32 (u32_bit (6) | u32_bit (5)) == 7);
  assert_true(
    log2_ceil_u32 (u32_bit (type_bits (u32) - 1)) == type_bits (u32) - 1
    );
}
/*---------------------------------------------------------------------------*/
static void log2_ceil_u64_test (void** s)
{
  assert_true (log2_ceil_u64 (0) == -1);
  assert_true (log2_ceil_u64 (u64_bit (1)) == 1);
  assert_true (log2_ceil_u64 (u64_bit (6) | u64_bit (5)) == 7);
  assert_true(
    log2_ceil_u64 (u64_bit (type_bits (u64) - 1)) == type_bits (u64) - 1
    );
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (msb_to_right_set_u8_test),
  cmocka_unit_test (msb_to_right_set_u16_test),
  cmocka_unit_test (msb_to_right_set_u32_test),
  cmocka_unit_test (msb_to_right_set_u64_test),
  cmocka_unit_test (reverse_bits_test),
  cmocka_unit_test (log2_floor_u8_test),
  cmocka_unit_test (log2_floor_u16_test),
  cmocka_unit_test (log2_floor_u32_test),
  cmocka_unit_test (log2_floor_u64_test),
  cmocka_unit_test (log2_ceil_u8_test),
  cmocka_unit_test (log2_ceil_u16_test),
  cmocka_unit_test (log2_ceil_u32_test),
  cmocka_unit_test (log2_ceil_u64_test),
};
/*---------------------------------------------------------------------------*/
int integer_math_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

