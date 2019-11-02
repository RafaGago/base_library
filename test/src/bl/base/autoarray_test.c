#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/default_allocator.h>
#include <bl/base/to_type_containing.h>
#include <bl/base/integer.h>
#include <bl/base/autoarray.h>
#include <bl/base/autoarray.c>
#include <bl/base/utility.h>

bl_define_autoarray_types (bl_u32_darr, bl_u32)
bl_declare_autoarray_funcs (bl_u32_darr, bl_u32)
/*---------------------------------------------------------------------------*/
typedef struct alloc_data {
  bl_u32       buff[64];
  bl_alloc_tbl alloc;
  bl_uword     realloc_count;
  bl_uword     dealloc_count;
  bl_uword     do_fail_realloc;
}
alloc_data;
/*---------------------------------------------------------------------------*/
static void* alloc_func (size_t bytes, bl_alloc_tbl const* invoker)
{
  assert_true (false);
  return nullptr;
}
/*---------------------------------------------------------------------------*/
static void* realloc_func(
  void* mem, size_t new_size, bl_alloc_tbl const* invoker
  )
{
  alloc_data* d = bl_to_type_containing (invoker, alloc, alloc_data);
  ++d->realloc_count;
  if (d->do_fail_realloc == 0) {
    return d->buff;
  }
  --d->do_fail_realloc;
  return nullptr;
}
/*---------------------------------------------------------------------------*/
static void dealloc_func (void const* mem, bl_alloc_tbl const* invoker)
{
  alloc_data* d = bl_to_type_containing (invoker, alloc, alloc_data);
  ++d->dealloc_count;
}
/*---------------------------------------------------------------------------*/
static inline alloc_data get_alloc()
{
  alloc_data d;
  d.alloc.alloc     = alloc_func;
  d.alloc.realloc   = realloc_func;
  d.alloc.dealloc   = dealloc_func;
  d.realloc_count   = 0;
  d.dealloc_count   = 0;
  d.do_fail_realloc = 0;
  return d;
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_init (void **state)
{
  bl_u32_darr d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (a.realloc_count == 0);
  assert_true (a.dealloc_count == 1);
  assert_true (bl_u32_darr_capacity (&d) == 0);
  bl_u32_darr_destroy (&d, &a.alloc);
  assert_true (a.dealloc_count == 2);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_init_non_0 (void **state)
{
  bl_u32_darr d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 4, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) >= 4);
  bl_u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_basic (void **state)
{
  const bl_u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  bl_u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) == 0);

  for (bl_u32 i = 0; i < bl_arr_elems (data); ++i) {
    assert_true (bl_u32_darr_insert_tail (&d, &data[i], &a.alloc).bl == bl_ok);
  }
  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (data));
  assert_true (bl_u32_darr_capacity (&d) >= bl_arr_elems (data));
  assert_true (a.realloc_count >= 1);

  assert_memory_equal (bl_u32_darr_beg (&d), data, bl_arr_elems (data));
  assert_true (a.dealloc_count == 1);
  bl_u32_darr_destroy (&d, &a.alloc);
  assert_true (a.dealloc_count == 2);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_tail_insert (void **state)
{
  const bl_u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const bl_u32 tail[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  bl_u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) == 0);

  for (bl_u32 i = 0; i < bl_arr_elems (data); ++i) {
    assert_true (bl_u32_darr_insert_tail (&d, &data[i], &a.alloc).bl == bl_ok);
  }
  assert_true(
    bl_u32_darr_insert_tail_n (&d, tail, bl_arr_elems (tail), &a.alloc).bl == bl_ok
    );
  assert_true(
    bl_u32_darr_size (&d) == bl_arr_elems (data) + bl_arr_elems (tail)
    );
  assert_true(
    bl_u32_darr_capacity (&d) >= bl_arr_elems (data) + bl_arr_elems (tail)
    );
  assert_true (a.realloc_count >= 1);

  assert_memory_equal (bl_u32_darr_beg (&d), data, bl_arr_elems (data));
  assert_memory_equal(
    bl_u32_darr_at (&d, bl_arr_elems (data)), tail, bl_arr_elems (tail)
    );
  assert_true (a.dealloc_count == 1);
  bl_u32_darr_destroy (&d, &a.alloc);
  assert_true (a.dealloc_count == 2);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_middle_insert (void **state)
{
  const bl_u32 data[]     = { 0, 1, 2, 3, 7, 8, 9, 10 };
  const bl_u32 new_data[] = { 4, 5, 6 };
  const bl_u32 expected[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  bl_u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) == 0);

  assert_true(
    bl_u32_darr_insert_tail_n(
      &d, data, bl_arr_elems (data), &a.alloc
      ).bl == bl_ok
    );
  assert_true (bl_u32_darr_insert_n(
    &d, new_data[0], new_data, bl_arr_elems (new_data), &a.alloc
    ).bl == bl_ok);

  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (expected));
  assert_memory_equal (bl_u32_darr_beg (&d), expected, bl_arr_elems (expected));
  bl_u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_middle_drop (void **state)
{
  const bl_u32 data[]     = { 0, 1, 2, 3, 4, 4, 4, 4, 4, 5, 6, 7, 8, 9, 10 };
  const bl_u32 expected[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  bl_u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) == 0);

  assert_true(
    bl_u32_darr_insert_tail_n(
      &d, data, bl_arr_elems (data), &a.alloc
      ).bl == bl_ok
    );
  bl_u32_darr_drop_n (&d ,4, 4);

  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (expected));
  assert_memory_equal (bl_u32_darr_beg (&d), expected, bl_arr_elems (expected));
  bl_u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_tail_drop (void **state)
{
  const bl_u32 data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const bl_u32 expected[] = { 0, 1, 2, 3, 4, 5, 6 };
  bl_u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) == 0);

  assert_true(
    bl_u32_darr_insert_tail_n(
      &d, data, bl_arr_elems (data), &a.alloc
      ).bl == bl_ok
    );
  bl_u32_darr_drop_tail_n (&d ,4);

  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (expected));
  assert_memory_equal (bl_u32_darr_beg (&d), expected, bl_arr_elems (expected));
  bl_u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_insert_alloc_failure (void **state)
{
  const bl_u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  bl_u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) == 0);

  assert_true(
    bl_u32_darr_insert_tail_n(
      &d, data, bl_arr_elems (data), &a.alloc
      ).bl == bl_ok
    );
  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (data));
  bl_uword capacity = bl_u32_darr_capacity (&d);

  a.do_fail_realloc = 1;
  assert_true(
    bl_u32_darr_insert_n(
      &d, 2, data, bl_arr_elems (data), &a.alloc
      ).bl == bl_alloc
    );
  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (data));
  assert_true (bl_u32_darr_capacity (&d) == capacity);

  bl_u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_autoarray_insert_tail_alloc_failure (void **state)
{
  const bl_u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  bl_u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (bl_u32_darr_init (&d, 0, &a.alloc).bl == bl_ok);
  assert_true (bl_u32_darr_capacity (&d) == 0);

  assert_true(
    bl_u32_darr_insert_tail_n(
      &d, data, bl_arr_elems (data), &a.alloc
      ).bl == bl_ok
    );
  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (data));
  bl_uword capacity = bl_u32_darr_capacity (&d);

  a.do_fail_realloc = 1;
  assert_true(
    bl_u32_darr_insert_tail_n(
      &d, data, bl_arr_elems (data), &a.alloc
      ).bl == bl_alloc
    );
  assert_true (bl_u32_darr_size (&d) == bl_arr_elems (data));
  assert_true (bl_u32_darr_capacity (&d) == capacity);

  bl_u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (bl_autoarray_init),
  cmocka_unit_test (bl_autoarray_init_non_0),
  cmocka_unit_test (bl_autoarray_basic),
  cmocka_unit_test (bl_autoarray_tail_insert),
  cmocka_unit_test (bl_autoarray_middle_insert),
  cmocka_unit_test (bl_autoarray_middle_drop),
  cmocka_unit_test (bl_autoarray_tail_drop),
  cmocka_unit_test (bl_autoarray_insert_alloc_failure),
  cmocka_unit_test (bl_autoarray_insert_tail_alloc_failure),
};
/*---------------------------------------------------------------------------*/
int bl_autoarray_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
