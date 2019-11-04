#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/base/ringbuffer.h>
#include <bl/base/utility.h>

bl_define_ringb_funcs (bl_ringbt, bl_u32)
/*---------------------------------------------------------------------------*/
typedef struct bl_ringbuffer_context {
  bl_u32   buff[8];
  bl_ringb r;
}
bl_ringbuffer_context;
/*---------------------------------------------------------------------------*/
static int bl_ringbuffer_test_setup (void **state)
{
  static bl_ringbuffer_context c;
  *state = (void*) &c;
  memset (c.buff, 0, sizeof c.buff);
  bl_ringbt_init_extern (&c.r, c.buff, bl_arr_elems (c.buff));
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_init_extern_size_and_capacity (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbt_init_extern (&c->r, c->buff, bl_arr_elems (c->buff));
  assert_true (bl_ringbt_capacity (&c->r) == bl_arr_elems (c->buff));
  assert_true (bl_ringbt_size (&c->r) == 0);
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_init_size_and_capacity (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  assert_true (bl_ringbt_capacity (&c->r) == bl_arr_elems (c->buff));
  assert_true (bl_ringbt_size (&c->r) == 0);
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_fill_with_consecutive (bl_ringbuffer_context* c)
{
  bl_u32 i = 0;
  while (bl_ringbt_can_insert (&c->r)) {
    ++i;
    bl_ringbt_insert_tail (&c->r, &i);
    assert_true (bl_ringbt_size (&c->r) == i);
  }
  assert_true (i == bl_arr_elems (c->buff));
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_fill_with_consecutive_inv (bl_ringbuffer_context* c)
{
  bl_u32 i = 0;
  while (bl_ringbt_can_insert (&c->r)) {
    ++i;
    bl_ringbt_insert_head (&c->r, &i);
    assert_true (bl_ringbt_size (&c->r) == i);
  }
  assert_true (i == bl_arr_elems (c->buff));
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_insert_tail_test (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbuffer_fill_with_consecutive (c);
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_insert_head_test (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbuffer_fill_with_consecutive_inv (c);
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_drop_head_test (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbuffer_fill_with_consecutive (c);

  bl_uword sz = bl_arr_elems (c->buff);
  bl_u32 i  = 0;
  while (bl_ringbt_size (&c->r)) {
    ++i;
    bl_u32 v = *bl_ringbt_at_head (&c->r);
    assert_true (v == i);
    bl_ringbt_drop_head (&c->r);
    --sz;
    assert_true (bl_ringbt_size (&c->r) == sz);
  }
  assert_true (i == bl_arr_elems (c->buff));
  i = (bl_u32) -1;
  bl_ringbt_insert_tail (&c->r, &i);
  bl_u32 j;
  j = *bl_ringbt_at_head (&c->r);
  bl_ringbt_drop_head (&c->r);
  assert_true (i == j);
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_drop_tail_test (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbuffer_fill_with_consecutive_inv (c);

  bl_uword sz = bl_arr_elems (c->buff);
  bl_u32 i  = 0;
  while (bl_ringbt_size (&c->r)) {
    ++i;
    bl_u32 v = *bl_ringbt_at_tail (&c->r);
    assert_true (v == i);
    bl_ringbt_drop_tail (&c->r);
    --sz;
    assert_true (bl_ringbt_size (&c->r) == sz);
  }
  assert_true (i == bl_arr_elems (c->buff));
  i = (bl_u32) -1;
  bl_ringbt_insert_tail (&c->r, &i);
  bl_u32 j = *bl_ringbt_at_tail (&c->r);
  bl_ringbt_drop_tail (&c->r);
  assert_true (i == j);
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_at_test (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbuffer_fill_with_consecutive (c);
  for (bl_uword i = 0; i < bl_ringbt_size (&c->r); ++i) {
    assert_true (*bl_ringbt_at (&c->r, i) == i + 1);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_wrapped (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbuffer_fill_with_consecutive (c);
  bl_u32 v , i = 1;
  v = *bl_ringbt_at_head (&c->r);
  bl_ringbt_drop_head (&c->r);
  assert_true (v == i);
  assert_true (bl_ringbt_size (&c->r) == bl_arr_elems (c->buff) - 1);
  v = bl_arr_elems (c->buff) + 1;
  bl_ringbt_insert_tail (&c->r, &v);
  while (bl_ringbt_size (&c->r)) {
    ++i;
    v = *bl_ringbt_at_head (&c->r);
    bl_ringbt_drop_head (&c->r);
    assert_true (v == i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_adjacent_elems_from (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  const bl_uword half      = bl_arr_elems (c->buff) / 2;
  bl_ringbt_set_start_position (&c->r, 0);
  for (bl_u32 i = half; i--; ) {
    bl_ringbt_insert_head (&c->r, &i);
  }
  for (bl_u32 i = half; i < bl_arr_elems (c->buff); ++i) {
    bl_ringbt_insert_tail (&c->r, &i);
  }
  bl_uword res = bl_ringbt_adjacent_elems_from (&c->r, 0, half);
  assert_true (res == half);
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_insert (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  for (bl_uword offset = 0; offset < bl_ringbt_capacity (&c->r); ++offset) {
    for(
      bl_u32 ins_pos = 0;
      ins_pos < bl_ringbt_capacity (&c->r) - 1;
      ++ins_pos
      ) {

      bl_ringbt_drop_head_n (&c->r, bl_ringbt_size (&c->r));
      bl_ringbt_set_start_position (&c->r, offset);

      for (bl_uword i = 0; i < bl_ringbt_capacity (&c->r) - 1; ++i) {
        bl_u32 ins = (i < ins_pos) ? i : i + 1;
        bl_ringbt_insert_tail (&c->r, &ins);
      }
      bl_ringbt_insert (&c->r, &ins_pos, ins_pos);
      for (bl_u32 i = 0; i < bl_ringbt_capacity (&c->r); ++i) {
        assert_int_equal (*bl_ringbt_at (&c->r, i), i);
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_drop (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  for (bl_uword offset = 0; offset < bl_ringbt_capacity (&c->r); ++offset) {
    for(
      bl_u32 drop_pos = 0;
      drop_pos < bl_ringbt_capacity (&c->r) - 1;
      ++drop_pos
      ) {

      bl_ringbt_drop_head_n (&c->r, bl_ringbt_size (&c->r));
      bl_ringbt_set_start_position (&c->r, offset);

      for (bl_uword i = 0; i < bl_ringbt_capacity (&c->r); ++i) {
        bl_u32 ins = (i < drop_pos) ? i : i + -1;
        bl_ringbt_insert_tail (&c->r, &ins);
      }
      bl_ringbt_drop (&c->r, drop_pos);
      for (bl_u32 i = 0; i < bl_ringbt_size (&c->r); ++i) {
        assert_int_equal (*bl_ringbt_at (&c->r, i), i);
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_drop_range_lo (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  const bl_u32 insert[bl_arr_elems (c->buff)] =  {
    0, 1, 2, 3, 4, 5, 6, 7
  };
  const bl_uword rmpos = 2;
  const bl_uword rmsz  = 3;
  const bl_u32 expected[bl_arr_elems (c->buff) - 3] =  {
    0, 1, 5, 6, 7
  };
  for (bl_uword i = 0; i < bl_arr_elems (insert); ++i) {
    bl_ringbt_insert_tail (&c->r, &insert[i]);
  }
  assert_true (!bl_ringbt_can_insert (&c->r));
  bl_ringbt_drop_range (&c->r, rmpos, rmsz);
  assert_true (bl_ringbt_size (&c->r) == (bl_ringbt_capacity (&c->r) - rmsz));
  for (bl_uword i = 0; i < bl_arr_elems (expected); ++i) {
    assert_true (*bl_ringbt_at (&c->r, i) == expected[i]);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_drop_range_hi (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  const bl_u32 insert[bl_arr_elems (c->buff)] =  {
    0, 1, 2, 3, 4, 5, 6, 7
  };
  const bl_uword rmpos = 4;
  const bl_uword rmsz  = 3;
  const bl_u32 expected[bl_arr_elems (c->buff) - 3] =  {
    0, 1, 2, 3, 7
  };
  for (bl_uword i = 0; i < bl_arr_elems (insert); ++i) {
    bl_ringbt_insert_tail (&c->r, &insert[i]);
  }
  assert_true (!bl_ringbt_can_insert (&c->r));
  bl_ringbt_drop_range (&c->r, rmpos, rmsz);
  assert_true (bl_ringbt_size (&c->r) == (bl_ringbt_capacity (&c->r) - rmsz));
  for (bl_uword i = 0; i < bl_arr_elems (expected); ++i) {
    assert_true (*bl_ringbt_at (&c->r, i) == expected[i]);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_ringbuffer_size_1 (void **state)
{
  bl_ringbuffer_context* c = (bl_ringbuffer_context*) *state;
  bl_ringbt_init_extern (&c->r, c->buff, 1);
  assert_true (bl_ringbt_can_insert (&c->r));
  assert_true (bl_ringbt_size (&c->r) == 0);
  assert_true (bl_ringbt_capacity (&c->r) == 1);
  bl_u32 val = 3;
  bl_ringbt_insert_tail (&c->r, &val);
  assert_true (*bl_ringbt_at (&c->r ,0) == val);
  bl_ringbt_drop_head (&c->r);
  val = 33;
  bl_ringbt_insert_tail (&c->r, &val);
  assert_true (*bl_ringbt_at (&c->r ,0) == val);
  bl_ringbt_drop_head (&c->r);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup(
    bl_ringbuffer_init_size_and_capacity, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_init_extern_size_and_capacity, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_insert_tail_test, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_insert_head_test, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_drop_head_test, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_drop_tail_test, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_at_test, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_wrapped, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_adjacent_elems_from, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup (bl_ringbuffer_insert, bl_ringbuffer_test_setup),
  cmocka_unit_test_setup (bl_ringbuffer_drop, bl_ringbuffer_test_setup),
  cmocka_unit_test_setup(
    bl_ringbuffer_drop_range_lo, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    bl_ringbuffer_drop_range_hi, bl_ringbuffer_test_setup
    ),
  cmocka_unit_test_setup (bl_ringbuffer_size_1, bl_ringbuffer_test_setup),
};
/*---------------------------------------------------------------------------*/
int bl_ringb_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
