#include <string.h>

#include <bl/cmocka_pre.h>
#include <bl/base/hdr/dynarray_test.h>

#include <bl/hdr/base/default_allocator.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/dynarray.h>
#include <bl/hdr/base/utility.h>

define_dynarray_types (u32_darr, u32)
declare_dynarray_funcs (u32_darr, u32, static inline)
define_dynarray_funcs (u32_darr, u32, static inline)
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
static void dynarray_resize (void **state)
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
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (dynarray_init),
  cmocka_unit_test (dynarray_init_non_0),
  cmocka_unit_test (dynarray_resize),
};
/*---------------------------------------------------------------------------*/
int dynarray_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
