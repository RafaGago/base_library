#include <bl/cmocka_pre.h>

#include <bl/base/array_concat_dynarray.h>
#include <bl/base/default_allocator.h>
#include <bl/base/utility.h>
#include <bl/base/integer.h>

bl_define_ac_dynarray_types (bl_u32_acdarr, bl_u32)
bl_declare_ac_dynarray_funcs (bl_u32_acdarr, bl_u32)
/*---------------------------------------------------------------------------*/
static void ac_bl_dynarray_0_sized_array (void **state)
{
  bl_u32_acdarr d;
  bl_alloc_tbl t = bl_get_default_alloc();

  bl_u32_acdarr_init (&d, nullptr, 0);
  assert_true (bl_u32_acdarr_size (&d) == 0);
  bl_u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void ac_bl_dynarray_one_expand (void **state)
{
  bl_u32_acdarr d;
  bl_alloc_tbl t = bl_get_default_alloc();
  bl_u32 arr[4];
  bl_u32_acdarr_init (&d, arr, bl_arr_elems (arr));
  assert_true (bl_u32_acdarr_size (&d) == bl_arr_elems (arr));
  assert_int_equal(
    bl_u32_acdarr_resize (&d, bl_arr_elems (arr) * 2, &t).own, bl_ok
    );
  for (bl_uword i = 0; i < bl_u32_acdarr_size (&d); ++i) {
    *bl_u32_acdarr_at (&d, i) = i;
  }
  for (bl_uword i = 0; i < bl_arr_elems (arr); ++i) {
    assert_int_equal (arr[i], i);
  }
  for (bl_uword i = bl_arr_elems (arr); i < bl_u32_acdarr_size (&d); ++i) {
    assert_int_equal (*bl_u32_acdarr_at (&d, i), i);
  }
  bl_u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void ac_bl_dynarray_two_expand (void **state)
{
  bl_u32_acdarr d;
  bl_alloc_tbl t = bl_get_default_alloc();
  bl_u32 arr[4];
  bl_u32_acdarr_init (&d, arr, bl_arr_elems (arr));
  assert_true (bl_u32_acdarr_size (&d) == bl_arr_elems (arr));
  assert_int_equal(
    bl_u32_acdarr_resize (&d, bl_arr_elems (arr) * 2, &t).own, bl_ok
    );
  for (bl_uword i = 0; i < bl_u32_acdarr_size (&d); ++i) {
    *bl_u32_acdarr_at (&d, i) = i;
  }
  assert_int_equal(
    bl_u32_acdarr_resize (&d, bl_arr_elems (arr) * 3, &t).own, bl_ok
    );
  for (bl_uword i = bl_arr_elems (arr) * 2; i < bl_u32_acdarr_size (&d); ++i) {
    *bl_u32_acdarr_at (&d, i) = i;
  }
  for (bl_uword i = 0; i < bl_arr_elems (arr); ++i) {
    assert_int_equal (arr[i], i);
  }
  for (bl_uword i = bl_arr_elems (arr); i < bl_u32_acdarr_size (&d); ++i) {
    assert_int_equal (*bl_u32_acdarr_at (&d, i), i);
  }
  bl_u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void ac_bl_dynarray_one_shrink (void **state)
{
  bl_u32_acdarr d;
  bl_alloc_tbl t = bl_get_default_alloc();
  bl_u32 arr[4];
  bl_u32_acdarr_init (&d, arr, bl_arr_elems (arr));
  assert_true (bl_u32_acdarr_size (&d) == bl_arr_elems (arr));
  assert_int_equal(
    bl_u32_acdarr_resize (&d, bl_arr_elems (arr) * 2, &t).own, bl_ok
    );
  for (bl_uword i = 0; i < bl_u32_acdarr_size (&d); ++i) {
    *bl_u32_acdarr_at (&d, i) = i;
  }
  assert_int_equal(
    bl_u32_acdarr_resize (&d, (bl_arr_elems (arr) * 2) - 2, &t).own, bl_ok
    );
  for (bl_uword i = 0; i < bl_arr_elems (arr); ++i) {
    assert_int_equal (arr[i], i);
  }
  for (bl_uword i = bl_arr_elems (arr); i < bl_u32_acdarr_size (&d); ++i) {
    assert_int_equal (*bl_u32_acdarr_at (&d, i), i);
  }
  bl_u32_acdarr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (ac_bl_dynarray_0_sized_array),
  cmocka_unit_test (ac_bl_dynarray_one_expand),
  cmocka_unit_test (ac_bl_dynarray_two_expand),
  cmocka_unit_test (ac_bl_dynarray_one_shrink),
};
/*---------------------------------------------------------------------------*/
int ac_bl_dynarray_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
