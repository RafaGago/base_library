#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/base/ringbuffer.h>
#include <bl/base/utility.h>

define_ringb_funcs (ringbt, u32)
/*---------------------------------------------------------------------------*/
typedef struct ringbuffer_context {
  u32   buff[8];
  ringb r;
}
ringbuffer_context;
/*---------------------------------------------------------------------------*/
static int ringbuffer_test_setup (void **state)
{
  static ringbuffer_context c;
  *state = (void*) &c;
  memset (c.buff, 0, sizeof c.buff);
  ringbt_init_extern (&c.r, c.buff, arr_elems (c.buff));
  return 0;
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_init_extern_size_and_capacity (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbt_init_extern (&c->r, c->buff, arr_elems (c->buff));
  assert_true (ringbt_capacity (&c->r) == arr_elems (c->buff));
  assert_true (ringbt_size (&c->r) == 0);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_init_size_and_capacity (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  assert_true (ringbt_capacity (&c->r) == arr_elems (c->buff));
  assert_true (ringbt_size (&c->r) == 0);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_fill_with_consecutive (ringbuffer_context* c)
{
  u32 i = 0;
  while (ringbt_can_insert (&c->r)) {
    ++i;
    ringbt_insert_tail (&c->r, &i);
    assert_true (ringbt_size (&c->r) == i);
  }
  assert_true (i == arr_elems (c->buff));
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_fill_with_consecutive_inv (ringbuffer_context* c)
{
  u32 i = 0;
  while (ringbt_can_insert (&c->r)) {
    ++i;
    ringbt_insert_head (&c->r, &i);
    assert_true (ringbt_size (&c->r) == i);
  }
  assert_true (i == arr_elems (c->buff));
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_insert_tail_test (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_insert_head_test (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive_inv (c);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_head_test (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);

  uword sz = arr_elems (c->buff);
  u32 i  = 0;
  while (ringbt_size (&c->r)) {
    ++i;
    u32 v = *ringbt_at_head (&c->r);
    assert_true (v == i);
    ringbt_drop_head (&c->r);
    --sz;
    assert_true (ringbt_size (&c->r) == sz);
  }
  assert_true (i == arr_elems (c->buff));
  i = (u32) -1;
  ringbt_insert_tail (&c->r, &i);
  u32 j;
  j = *ringbt_at_head (&c->r);
  ringbt_drop_head (&c->r);
  assert_true (i == j);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_tail_test (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive_inv (c);

  uword sz = arr_elems (c->buff);
  u32 i  = 0;
  while (ringbt_size (&c->r)) {
    ++i;
    u32 v = *ringbt_at_tail (&c->r);
    assert_true (v == i);
    ringbt_drop_tail (&c->r);
    --sz;
    assert_true (ringbt_size (&c->r) == sz);
  }
  assert_true (i == arr_elems (c->buff));
  i = (u32) -1;
  ringbt_insert_tail (&c->r, &i);
  u32 j = *ringbt_at_tail (&c->r);
  ringbt_drop_tail (&c->r);
  assert_true (i == j);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_at_test (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  for (uword i = 0; i < ringbt_size (&c->r); ++i) {
    assert_true (*ringbt_at (&c->r, i) == i + 1);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_wrapped (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  u32 v , i = 1;
  v = *ringbt_at_head (&c->r);
  ringbt_drop_head (&c->r);
  assert_true (v == i);
  assert_true (ringbt_size (&c->r) == arr_elems (c->buff) - 1);
  v = arr_elems (c->buff) + 1;
  ringbt_insert_tail (&c->r, &v);
  while (ringbt_size (&c->r)) {
    ++i;
    v = *ringbt_at_head (&c->r);
    ringbt_drop_head (&c->r);
    assert_true (v == i);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_adjacent_elems_from (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  const uword half      = arr_elems (c->buff) / 2;
  ringbt_set_start_position (&c->r, 0);
  for (u32 i = half; i--; ) {
    ringbt_insert_head (&c->r, &i);
  }
  for (u32 i = half; i < arr_elems (c->buff); ++i) {
    ringbt_insert_tail (&c->r, &i);
  }
  uword res = ringbt_adjacent_elems_from (&c->r, 0, half);
  assert_true (res == half);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_insert (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  for (uword offset = 0; offset < ringbt_capacity (&c->r); ++offset) {
    for (u32 ins_pos = 0; ins_pos < ringbt_capacity (&c->r) - 1; ++ins_pos) {

      ringbt_drop_head_n (&c->r, ringbt_size (&c->r));
      ringbt_set_start_position (&c->r, offset);

      for (uword i = 0; i < ringbt_capacity (&c->r) - 1; ++i) {
        u32 ins = (i < ins_pos) ? i : i + 1;
        ringbt_insert_tail (&c->r, &ins);
      }
      ringbt_insert (&c->r, &ins_pos, ins_pos);
      for (u32 i = 0; i < ringbt_capacity (&c->r); ++i) {
        assert_int_equal (*ringbt_at (&c->r, i), i);
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  for (uword offset = 0; offset < ringbt_capacity (&c->r); ++offset) {
    for (u32 drop_pos = 0; drop_pos < ringbt_capacity (&c->r) - 1; ++drop_pos) {

      ringbt_drop_head_n (&c->r, ringbt_size (&c->r));
      ringbt_set_start_position (&c->r, offset);

      for (uword i = 0; i < ringbt_capacity (&c->r); ++i) {
        u32 ins = (i < drop_pos) ? i : i + -1;
        ringbt_insert_tail (&c->r, &ins);
      }
      ringbt_drop (&c->r, drop_pos);
      for (u32 i = 0; i < ringbt_size (&c->r); ++i) {
        assert_int_equal (*ringbt_at (&c->r, i), i);
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_range_lo (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  const u32 insert[arr_elems (c->buff)] =  {
    0, 1, 2, 3, 4, 5, 6, 7
  };
  const uword rmpos = 2;
  const uword rmsz  = 3;
  const u32 expected[arr_elems (c->buff) - 3] =  {
    0, 1, 5, 6, 7
  };
  for (uword i = 0; i < arr_elems (insert); ++i) {
    ringbt_insert_tail (&c->r, &insert[i]);
  }
  assert_true (!ringbt_can_insert (&c->r));
  ringbt_drop_range (&c->r, rmpos, rmsz);
  assert_true (ringbt_size (&c->r) == (ringbt_capacity (&c->r) - rmsz));
  for (uword i = 0; i < arr_elems (expected); ++i) {
    assert_true (*ringbt_at (&c->r, i) == expected[i]);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_range_hi (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  const u32 insert[arr_elems (c->buff)] =  {
    0, 1, 2, 3, 4, 5, 6, 7
  };
  const uword rmpos = 4;
  const uword rmsz  = 3;
  const u32 expected[arr_elems (c->buff) - 3] =  {
    0, 1, 2, 3, 7
  };
  for (uword i = 0; i < arr_elems (insert); ++i) {
    ringbt_insert_tail (&c->r, &insert[i]);
  }
  assert_true (!ringbt_can_insert (&c->r));
  ringbt_drop_range (&c->r, rmpos, rmsz);
  assert_true (ringbt_size (&c->r) == (ringbt_capacity (&c->r) - rmsz));
  for (uword i = 0; i < arr_elems (expected); ++i) {
    assert_true (*ringbt_at (&c->r, i) == expected[i]);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_size_1 (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbt_init_extern (&c->r, c->buff, 1);
  assert_true (ringbt_can_insert (&c->r));
  assert_true (ringbt_size (&c->r) == 0);
  assert_true (ringbt_capacity (&c->r) == 1);
  u32 val = 3;
  ringbt_insert_tail (&c->r, &val);
  assert_true (*ringbt_at (&c->r ,0) == val);
  ringbt_drop_head (&c->r);
  val = 33;
  ringbt_insert_tail (&c->r, &val);
  assert_true (*ringbt_at (&c->r ,0) == val);
  ringbt_drop_head (&c->r);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup(
    ringbuffer_init_size_and_capacity, ringbuffer_test_setup
    ),
  cmocka_unit_test_setup(
    ringbuffer_init_extern_size_and_capacity, ringbuffer_test_setup
    ),
  cmocka_unit_test_setup (ringbuffer_insert_tail_test, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_insert_head_test, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_head_test, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_tail_test, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_at_test, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_wrapped, ringbuffer_test_setup),
  cmocka_unit_test_setup(
    ringbuffer_adjacent_elems_from, ringbuffer_test_setup
    ),
  cmocka_unit_test_setup (ringbuffer_insert, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_range_lo, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_range_hi, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_size_1, ringbuffer_test_setup),
};
/*---------------------------------------------------------------------------*/
int ringb_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
