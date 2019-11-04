#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/default_allocator.h>
#include <bl/base/integer.h>
#include <bl/base/dynarray.h>
#include <bl/base/dynarray.c>
#include <bl/base/utility.h>

bl_define_dynarray_types (bl_u32_darr, bl_u32)
bl_declare_dynarray_funcs (bl_u32_darr, bl_u32)
bl_define_dynarray_types (bl_u8_darr, bl_u8)
bl_declare_dynarray_funcs (bl_u8_darr, bl_u8)

#define WRLIT "xxxxxxxxxxxxxx"

/*---------------------------------------------------------------------------*/
static void bl_dynarray_init (void **state)
{
  bl_u32_darr  d;
  bl_alloc_tbl t = bl_get_default_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &t).bl == bl_ok);
  assert_true (bl_u32_darr_size (&d) == 0);
  bl_u32_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void bl_dynarray_init_non_0 (void **state)
{
  bl_u32_darr  d;
  bl_alloc_tbl t = bl_get_default_alloc();
  assert_true (bl_u32_darr_init (&d, 4, &t).bl == bl_ok);
  assert_true (bl_u32_darr_size (&d) == 4);
  bl_u32_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void bl_dynarray_resize_test (void **state)
{
  const bl_uword sz = 4;
  bl_u32_darr    d;
  bl_alloc_tbl t = bl_get_default_alloc();
  assert_true (bl_u32_darr_init (&d, sz, &t).bl == bl_ok);
  assert_true (bl_u32_darr_size (&d) == sz);

  for (bl_u32 i = 0; i < sz; ++i) {
    *bl_u32_darr_at (&d, i) = i;
  }
  assert_true (bl_u32_darr_resize (&d, sz + 1, &t).bl == bl_ok);
  assert_true (bl_u32_darr_size (&d) == sz + 1);
  for (bl_u32 i = 0; i < sz; ++i) {
    assert_true (*bl_u32_darr_at (&d, i) == i);
  }
  bl_u32_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static void bl_dynarray_from_file_test (void **state)
{
  static const char* fname = "testfile.bin";
  const bl_uword sz = 4;
  bl_u8_darr d;
  bl_alloc_tbl t = bl_get_default_alloc();
  assert_true (bl_u8_darr_init (&d, sz, &t).bl == bl_ok);
  memset (bl_u8_darr_at (&d, 0), 0, sz);

  FILE* f = fopen (fname, "w");
  assert_non_null (f);
  assert_int_equal(
    bl_lit_len (WRLIT), fwrite (WRLIT, 1, bl_lit_len (WRLIT), f)
    );
  fclose(f);

  f = fopen (fname, "r");
  assert_non_null (f);
  bl_uword wr;
  assert_int_equal(
    bl_ok,
    bl_u8_darr_from_file (&d, &wr, sz - 1, 7, f, 0, &t).bl
    );
  fclose (f);
  remove(fname);

  assert_int_equal (wr, bl_lit_len (WRLIT));
  for (bl_uword i = 0; i < sz - 1; ++i) {
    assert_int_equal (0, *bl_u8_darr_at (&d, i));
  }
  for (bl_uword i = sz - 1; i < (sz - 1 + bl_lit_len (WRLIT)); ++i) {
    assert_int_equal ('x', *bl_u8_darr_at (&d, i));
  }
  assert_int_equal(
    bl_round_to_next_multiple(sz - 1 + bl_lit_len (WRLIT) + 7, 1),
    bl_u8_darr_size (&d)
    );
  bl_u8_darr_destroy (&d, &t);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (bl_dynarray_init),
  cmocka_unit_test (bl_dynarray_init_non_0),
  cmocka_unit_test (bl_dynarray_resize_test),
  cmocka_unit_test (bl_dynarray_from_file_test),
};
/*---------------------------------------------------------------------------*/
int bl_dynarray_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
