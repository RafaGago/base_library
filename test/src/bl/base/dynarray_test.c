#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/default_allocator.h>
#include <bl/base/integer.h>
#include <bl/base/dynarray.h>
#include <bl/base/dynarray.c>
#include <bl/base/utility.h>

define_dynarray_types (u32_darr, u32)
declare_dynarray_funcs (u32_darr, u32)
define_dynarray_types (u8_darr, u8)
declare_dynarray_funcs (u8_darr, u8)

#define WRLIT "xxxxxxxxxxxxxx"

/*---------------------------------------------------------------------------*/
static void dynarray_init (void **state)
{
  u32_darr  d;
  alloc_tbl t = get_default_alloc();
  assert_true (u32_darr_init (&d, 0, &t) == bl_ok);
  assert_true (u32_darr_size (&d) == 0);
  u32_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void dynarray_init_non_0 (void **state)
{
  u32_darr  d;
  alloc_tbl t = get_default_alloc();
  assert_true (u32_darr_init (&d, 4, &t) == bl_ok);
  assert_true (u32_darr_size (&d) == 4);
  u32_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void dynarray_resize_test (void **state)
{
  const uword sz = 4;
  u32_darr    d;
  alloc_tbl t = get_default_alloc();
  assert_true (u32_darr_init (&d, sz, &t) == bl_ok);
  assert_true (u32_darr_size (&d) == sz);

  for (u32 i = 0; i < sz; ++i) {
    *u32_darr_at (&d, i) = i;
  }
  assert_true (u32_darr_resize (&d, sz + 1, &t) == bl_ok);
  assert_true (u32_darr_size (&d) == sz + 1);
  for (u32 i = 0; i < sz; ++i) {
    assert_true (*u32_darr_at (&d, i) == i);
  }
  u32_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void dynarray_from_file_test (void **state)
{
  static const char* fname = "testfile.bin";
  const uword sz = 4;
  u8_darr d;
  alloc_tbl t = get_default_alloc();
  assert_true (u8_darr_init (&d, sz, &t) == bl_ok);
  memset (u8_darr_at (&d, 0), 0, sz);

  FILE* f = fopen (fname, "w");
  assert_non_null (f);
  assert_int_equal (lit_len (WRLIT), fwrite (WRLIT, 1, lit_len (WRLIT), f));
  fclose(f);

  f = fopen (fname, "r");
  assert_non_null (f);
  uword wr;
  assert_int_equal(
    bl_ok,
    u8_darr_from_file (&d, &wr, sz - 1, 7, f, 0, &t)
    );
  fclose (f);
  remove(fname);

  assert_int_equal (wr, lit_len (WRLIT));
  for (uword i = 0; i < sz - 1; ++i) {
    assert_int_equal (0, *u8_darr_at (&d, i));
  }
  for (uword i = sz - 1; i < (sz - 1 + lit_len (WRLIT)); ++i) {
    assert_int_equal ('x', *u8_darr_at (&d, i));
  }
  assert_int_equal(
    round_to_next_multiple (sz - 1 + lit_len (WRLIT) + 7, 1), u8_darr_size (&d)
    );
  u8_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (dynarray_init),
  cmocka_unit_test (dynarray_init_non_0),
  cmocka_unit_test (dynarray_resize_test),
  cmocka_unit_test (dynarray_from_file_test),
};
/*---------------------------------------------------------------------------*/
int dynarray_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
