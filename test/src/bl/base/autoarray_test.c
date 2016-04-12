#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/default_allocator.h>
#include <bl/base/to_type_containing.h>
#include <bl/base/integer.h>
#include <bl/base/autoarray.h>
#include <bl/base/autoarray.c>
#include <bl/base/utility.h>

define_autoarray_types (u32_darr, u32)
declare_autoarray_funcs (u32_darr, u32)
/*---------------------------------------------------------------------------*/
typedef struct alloc_data {
  u32       buff[64];
  alloc_tbl alloc;
  uword     realloc_count;
  uword     dealloc_count;
  uword     do_fail_realloc;
} 
alloc_data;
/*---------------------------------------------------------------------------*/
static void* alloc_func (size_t bytes, alloc_tbl const* invoker)
{
  assert_true (false);
  return nullptr;
}
/*---------------------------------------------------------------------------*/
static void* realloc_func(
  void* mem, size_t new_size, alloc_tbl const* invoker
  )
{
  alloc_data* d = to_type_containing (invoker, alloc, alloc_data);
  ++d->realloc_count;
  if (d->do_fail_realloc == 0) {
    return d->buff;    
  }
  --d->do_fail_realloc;
  return nullptr;
}
/*---------------------------------------------------------------------------*/
static void dealloc_func (void const* mem, alloc_tbl const* invoker)
{
  alloc_data* d = to_type_containing (invoker, alloc, alloc_data);
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
static void autoarray_init (void **state)
{ 
  u32_darr d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (a.realloc_count == 0);
  assert_true (a.dealloc_count == 1);
  assert_true (u32_darr_capacity (&d) == 0);
  u32_darr_destroy (&d, &a.alloc);
  assert_true (a.dealloc_count == 2);
}
/*---------------------------------------------------------------------------*/
static void autoarray_init_non_0 (void **state)
{ 
  u32_darr d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 4, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) >= 4);  
  u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void autoarray_basic (void **state)
{ 
  const u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) == 0);

  for (u32 i = 0; i < arr_elems (data); ++i) {
    assert_true (u32_darr_insert_tail (&d, &data[i], &a.alloc) == bl_ok);
  }
  assert_true (u32_darr_size (&d) == arr_elems (data));
  assert_true (u32_darr_capacity (&d) >= arr_elems (data));
  assert_true (a.realloc_count >= 1);

  assert_memory_equal (u32_darr_beg (&d), data, arr_elems (data));
  assert_true (a.dealloc_count == 1);
  u32_darr_destroy (&d, &a.alloc);
  assert_true (a.dealloc_count == 2);
}
/*---------------------------------------------------------------------------*/
static void autoarray_tail_insert (void **state)
{ 
  const u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const u32 tail[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) == 0);

  for (u32 i = 0; i < arr_elems (data); ++i) {
    assert_true (u32_darr_insert_tail (&d, &data[i], &a.alloc) == bl_ok);
  }
  assert_true(
    u32_darr_insert_tail_n (&d, tail, arr_elems (tail), &a.alloc) == bl_ok
    );
  assert_true (u32_darr_size (&d) == arr_elems (data) + arr_elems (tail));
  assert_true (u32_darr_capacity (&d) >= arr_elems (data) + arr_elems (tail));
  assert_true (a.realloc_count >= 1);

  assert_memory_equal (u32_darr_beg (&d), data, arr_elems (data));
  assert_memory_equal(
    u32_darr_at (&d, arr_elems (data)), tail, arr_elems (tail)
    );
  assert_true (a.dealloc_count == 1);
  u32_darr_destroy (&d, &a.alloc);
  assert_true (a.dealloc_count == 2);
}
/*---------------------------------------------------------------------------*/
static void autoarray_middle_insert (void **state)
{
  const u32 data[]     = { 0, 1, 2, 3, 7, 8, 9, 10 };
  const u32 new_data[] = { 4, 5, 6 };
  const u32 expected[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) == 0);

  assert_true(
    u32_darr_insert_tail_n (&d, data, arr_elems (data), &a.alloc) == bl_ok
    );
  assert_true (u32_darr_insert_n(
    &d, new_data[0], new_data, arr_elems (new_data), &a.alloc
    ) == bl_ok);

  assert_true (u32_darr_size (&d) == arr_elems (expected));
  assert_memory_equal (u32_darr_beg (&d), expected, arr_elems (expected));
  u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void autoarray_middle_drop (void **state)
{
  const u32 data[]     = { 0, 1, 2, 3, 4, 4, 4, 4, 4, 5, 6, 7, 8, 9, 10 };
  const u32 expected[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) == 0);

  assert_true(
    u32_darr_insert_tail_n (&d, data, arr_elems (data), &a.alloc) == bl_ok
    );
  u32_darr_drop_n (&d ,4, 4);

  assert_true (u32_darr_size (&d) == arr_elems (expected));
  assert_memory_equal (u32_darr_beg (&d), expected, arr_elems (expected));
  u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void autoarray_tail_drop (void **state)
{
  const u32 data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const u32 expected[] = { 0, 1, 2, 3, 4, 5, 6 };
  u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) == 0);

  assert_true(
    u32_darr_insert_tail_n (&d, data, arr_elems (data), &a.alloc) == bl_ok
    );
  u32_darr_drop_tail_n (&d ,4);

  assert_true (u32_darr_size (&d) == arr_elems (expected));
  assert_memory_equal (u32_darr_beg (&d), expected, arr_elems (expected));
  u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void autoarray_insert_alloc_failure (void **state)
{
  const u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) == 0);

  assert_true(
    u32_darr_insert_tail_n (&d, data, arr_elems (data), &a.alloc) == bl_ok
    );
  assert_true (u32_darr_size (&d) == arr_elems (data));
  uword capacity = u32_darr_capacity (&d);

  a.do_fail_realloc = 1;
  assert_true(
    u32_darr_insert_n (&d, 2, data, arr_elems (data), &a.alloc) == bl_alloc
    );
  assert_true (u32_darr_size (&d) == arr_elems (data));
  assert_true (u32_darr_capacity (&d) == capacity);

  u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static void autoarray_insert_tail_alloc_failure (void **state)
{
  const u32 data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  u32_darr    d;
  alloc_data a = get_alloc();
  assert_true (u32_darr_init (&d, 0, &a.alloc) == bl_ok);
  assert_true (u32_darr_capacity (&d) == 0);

  assert_true(
    u32_darr_insert_tail_n (&d, data, arr_elems (data), &a.alloc) == bl_ok
    );
  assert_true (u32_darr_size (&d) == arr_elems (data));
  uword capacity = u32_darr_capacity (&d);

  a.do_fail_realloc = 1;
  assert_true(
    u32_darr_insert_tail_n (&d, data, arr_elems (data), &a.alloc) == bl_alloc
    );
  assert_true (u32_darr_size (&d) == arr_elems (data));
  assert_true (u32_darr_capacity (&d) == capacity);

  u32_darr_destroy (&d, &a.alloc);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (autoarray_init),
  cmocka_unit_test (autoarray_init_non_0),
  cmocka_unit_test (autoarray_basic),
  cmocka_unit_test (autoarray_tail_insert),
  cmocka_unit_test (autoarray_middle_insert),
  cmocka_unit_test (autoarray_middle_drop),
  cmocka_unit_test (autoarray_tail_drop),
  cmocka_unit_test (autoarray_insert_alloc_failure),
  cmocka_unit_test (autoarray_insert_tail_alloc_failure),
};
/*---------------------------------------------------------------------------*/
int autoarray_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
