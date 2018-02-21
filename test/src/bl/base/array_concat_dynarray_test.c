#include <bl/cmocka_pre.h>

#include <bl/base/array_concat_dynarray.h>
#include <bl/base/default_allocator.h>
#include <bl/base/utility.h>
#include <bl/base/integer.h>

define_ac_dynarray_types (u32_acdarr, u32)
declare_ac_dynarray_funcs (u32_acdarr, u32)
/*---------------------------------------------------------------------------*/
static void ac_dynarray_0_sized_array (void **state)
{
  u32_acdarr d;
  alloc_tbl t = get_default_alloc();

  u32_acdarr_init (&d, nullptr, 0);
  assert_true (u32_acdarr_size (&d) == 0);
  u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void ac_dynarray_one_expand (void **state)
{
  u32_acdarr d;
  alloc_tbl t = get_default_alloc();
  u32 arr[4];
  u32_acdarr_init (&d, arr, arr_elems (arr));
  assert_true (u32_acdarr_size (&d) == arr_elems (arr));
  assert_int_equal (u32_acdarr_resize (&d, arr_elems (arr) * 2, &t).bl, bl_ok);
  for (uword i = 0; i < u32_acdarr_size (&d); ++i) {
    *u32_acdarr_at (&d, i) = i;
  }
  for (uword i = 0; i < arr_elems (arr); ++i) {
    assert_int_equal (arr[i], i);
  }
  for (uword i = arr_elems (arr); i < u32_acdarr_size (&d); ++i) {
    assert_int_equal (*u32_acdarr_at (&d, i), i);
  }
  u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void ac_dynarray_two_expand (void **state)
{
  u32_acdarr d;
  alloc_tbl t = get_default_alloc();
  u32 arr[4];
  u32_acdarr_init (&d, arr, arr_elems (arr));
  assert_true (u32_acdarr_size (&d) == arr_elems (arr));
  assert_int_equal (u32_acdarr_resize (&d, arr_elems (arr) * 2, &t).bl, bl_ok);
  for (uword i = 0; i < u32_acdarr_size (&d); ++i) {
    *u32_acdarr_at (&d, i) = i;
  }
  assert_int_equal (u32_acdarr_resize (&d, arr_elems (arr) * 3, &t).bl, bl_ok);
  for (uword i = arr_elems (arr) * 2; i < u32_acdarr_size (&d); ++i) {
    *u32_acdarr_at (&d, i) = i;
  }
  for (uword i = 0; i < arr_elems (arr); ++i) {
    assert_int_equal (arr[i], i);
  }
  for (uword i = arr_elems (arr); i < u32_acdarr_size (&d); ++i) {
    assert_int_equal (*u32_acdarr_at (&d, i), i);
  }
  u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void ac_dynarray_one_shrink (void **state)
{
  u32_acdarr d;
  alloc_tbl t = get_default_alloc();
  u32 arr[4];
  u32_acdarr_init (&d, arr, arr_elems (arr));
  assert_true (u32_acdarr_size (&d) == arr_elems (arr));
  assert_int_equal (u32_acdarr_resize (&d, arr_elems (arr) * 2, &t).bl, bl_ok);
  for (uword i = 0; i < u32_acdarr_size (&d); ++i) {
    *u32_acdarr_at (&d, i) = i;
  }
  assert_int_equal(
    u32_acdarr_resize (&d, (arr_elems (arr) * 2) - 2, &t).bl, bl_ok
    );
  for (uword i = 0; i < arr_elems (arr); ++i) {
    assert_int_equal (arr[i], i);
  }
  for (uword i = arr_elems (arr); i < u32_acdarr_size (&d); ++i) {
    assert_int_equal (*u32_acdarr_at (&d, i), i);
  }
  u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (ac_dynarray_0_sized_array),
  cmocka_unit_test (ac_dynarray_one_expand),
  cmocka_unit_test (ac_dynarray_two_expand),
  cmocka_unit_test (ac_dynarray_one_shrink),
};
/*---------------------------------------------------------------------------*/
int ac_dynarray_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
