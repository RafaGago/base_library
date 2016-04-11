#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/base/ringbuffer.h>
#include <bl/base/utility.h>

define_ringb_types (ringb, u32)
declare_ringb_funcs (ringb, u32, static inline)
define_ringb_funcs (ringb, u32, static inline)
declare_ringb_extended_funcs (ringb, u32, static inline)
define_ringb_extended_funcs (ringb, u32, static inline)
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
  ringb_init_extern (&c.r, c.buff, arr_elems (c.buff));
  return 0;
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_init_extern_size_and_capacity (void **state)
{ 
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringb_init_extern (&c->r, c->buff, arr_elems (c->buff));
  assert_true (ringb_capacity (&c->r) == arr_elems (c->buff));
  assert_true (ringb_size (&c->r) == 0);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_init_size_and_capacity (void **state)
{  
  ringbuffer_context* c = (ringbuffer_context*) *state;
  assert_true (ringb_capacity (&c->r) == arr_elems (c->buff));
  assert_true (ringb_size (&c->r) == 0);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_fill_with_consecutive (ringbuffer_context* c)
{
  u32 i = 0;
  while (ringb_can_insert (&c->r)) {
    ++i;
    ringb_insert_tail (&c->r, &i);
    assert_true (ringb_size (&c->r) == i);
  }
  assert_true (i == arr_elems (c->buff));
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_fill_with_consecutive_inv (ringbuffer_context* c)
{
  u32 i = 0;
  while (ringb_can_insert (&c->r)) {
    ++i;
    ringb_insert_head (&c->r, &i);
    assert_true (ringb_size (&c->r) == i);
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
  uword i  = 0;
  while (ringb_size (&c->r)) {
    ++i;
    u32 v = *ringb_at_head (&c->r);
    assert_true (v == i);
    ringb_drop_head (&c->r);
    --sz;
    assert_true (ringb_size (&c->r) == sz);
  }
  assert_true (i == arr_elems (c->buff));
  i = (u32) -1;
  ringb_insert_tail (&c->r, &i);
  u32 j;
  j = *ringb_at_head (&c->r);
  ringb_drop_head (&c->r);
  assert_true (i == j);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_tail_test (void **state)
{ 
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive_inv (c);

  uword sz = arr_elems (c->buff);
  uword i  = 0;
  while (ringb_size (&c->r)) {
    ++i;
    u32 v = *ringb_at_tail (&c->r);
    assert_true (v == i);
    ringb_drop_tail (&c->r);
    --sz;
    assert_true (ringb_size (&c->r) == sz);
  }
  assert_true (i == arr_elems (c->buff));
  i = (u32) -1;
  ringb_insert_tail (&c->r, &i);
  u32 j = *ringb_at_tail (&c->r);
  ringb_drop_tail (&c->r);
  assert_true (i == j);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_at_test (void **state)
{ 
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  for (uword i = 0; i < ringb_size (&c->r); ++i) {
    assert_true (*ringb_at (&c->r, i) == i + 1);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_wrapped (void **state)
{ 
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  u32 v , i = 1;
  v = *ringb_at_head (&c->r);
  ringb_drop_head (&c->r);
  assert_true (v == i);
  assert_true (ringb_size (&c->r) == arr_elems (c->buff) - 1);
  v = arr_elems (c->buff) + 1;
  ringb_insert_tail (&c->r, &v);
  while (ringb_size (&c->r)) {
    ++i;
    v = *ringb_at_head (&c->r);
    ringb_drop_head (&c->r);
    assert_true (v == i);
  }  
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_adjacent_elems_from (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  const uword half      = arr_elems (c->buff) / 2;
  ringb_set_start_position (&c->r, 0);
  for (u32 i = half; i--; ) {
    ringb_insert_head (&c->r, &i);
  }
  for (u32 i = half; i < arr_elems (c->buff); ++i) {
    ringb_insert_tail (&c->r, &i);
  }
  uword res = ringb_adjacent_elems_from (&c->r, 0, half);
  assert_true (res == half);
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_insert_random_head (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  uword i;
  for (i = 1; i < ringb_capacity (&c->r); ++i) {
      ringb_insert_tail (&c->r, &i);
  }
  i = 0;
  ringb_insert (&c->r, &i, 0);
  assert_true (!ringb_can_insert (&c->r));
  for (i = 0; i < ringb_capacity (&c->r); ++i) {
    assert_true (*ringb_at (&c->r, i) == i);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_insert_random_tail (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  uword i;
  for (i = 0; i < ringb_capacity (&c->r) - 1; ++i) {
      ringb_insert_tail (&c->r, &i);
  }
  ringb_insert (&c->r, &i, i);
  assert_true (!ringb_can_insert (&c->r));
  for (i = 0; i < ringb_capacity (&c->r); ++i) {
    assert_true (*ringb_at (&c->r, i) == i);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_insert_random_lo (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  ringb_drop_tail (&c->r);
  uword ins = 2;
  ringb_insert (&c->r, &ins, ins);
  uword j = 1;
  assert_true (!ringb_can_insert (&c->r));
  for (uword i = 0; i < ringb_capacity (&c->r); ++i) {
    assert_true (*ringb_at (&c->r, i) == i + j);
    if (i + 1 == ins) {
      j = 0;
    }
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_insert_random_hi (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  ringb_drop_tail (&c->r);
  uword ins = 5;
  ringb_insert (&c->r, &ins, ins);
  uword j = 1;
  assert_true (!ringb_can_insert (&c->r));
  for (uword i = 0; i < ringb_capacity (&c->r); ++i) {
    assert_true (*ringb_at (&c->r, i) == i + j);
    if (i + 1 == ins) {
      j = 0;
    }
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_random_head (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  ringb_drop (&c->r, 0);
  for (uword i = 0; i < ringb_capacity (&c->r) - 1; ++i) {
    assert_true (*ringb_at (&c->r, i) == i + 2);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_random_tail (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  ringb_drop (&c->r, ringb_size (&c->r) - 1);
  for (uword i = 0; i < ringb_capacity (&c->r) - 1; ++i) {
    assert_true (*ringb_at (&c->r, i) == i + 1);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_random_lo (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  uword drop = 3;
  ringb_drop (&c->r, drop);
  uword j = 1;
  for (uword i = 0; i < ringb_capacity (&c->r) - 1; ++i) {
    if (i == drop) {
      j = 2;
    }
    assert_true (*ringb_at (&c->r, i) == i + j);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_drop_random_hi (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringbuffer_fill_with_consecutive (c);
  uword drop = 5;
  ringb_drop (&c->r, drop);
  uword j = 1;
  for (uword i = 0; i < ringb_capacity (&c->r) - 1; ++i) {
    if (i == drop) {
      j = 2;
    }
    assert_true (*ringb_at (&c->r, i) == i + j);
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
    ringb_insert_tail (&c->r, &insert[i]);
  }
  assert_true (!ringb_can_insert (&c->r));
  ringb_drop_range (&c->r, rmpos, rmsz);
  assert_true (ringb_size (&c->r) == (ringb_capacity (&c->r) - rmsz));
  for (uword i = 0; i < arr_elems (expected); ++i) {
    assert_true (*ringb_at (&c->r, i) == expected[i]);
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
    ringb_insert_tail (&c->r, &insert[i]);
  }
  assert_true (!ringb_can_insert (&c->r));
  ringb_drop_range (&c->r, rmpos, rmsz);
  assert_true (ringb_size (&c->r) == (ringb_capacity (&c->r) - rmsz));
  for (uword i = 0; i < arr_elems (expected); ++i) {
    assert_true (*ringb_at (&c->r, i) == expected[i]);
  }
}
/*---------------------------------------------------------------------------*/
static void ringbuffer_size_1 (void **state)
{
  ringbuffer_context* c = (ringbuffer_context*) *state;
  ringb_init_extern (&c->r, c->buff, 1);
  assert_true (ringb_can_insert (&c->r));
  assert_true (ringb_size (&c->r) == 0);
  assert_true (ringb_capacity (&c->r) == 1);
  u32 val = 3;
  ringb_insert_tail (&c->r, &val);
  assert_true (*ringb_at (&c->r ,0) == val);
  ringb_drop_head (&c->r);
  val = 33;
  ringb_insert_tail (&c->r, &val);
  assert_true (*ringb_at (&c->r ,0) == val);
  ringb_drop_head (&c->r);
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
  cmocka_unit_test_setup (ringbuffer_insert_random_head, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_insert_random_tail, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_insert_random_lo, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_insert_random_hi, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_random_head, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_random_tail, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_random_lo, ringbuffer_test_setup),
  cmocka_unit_test_setup (ringbuffer_drop_random_hi, ringbuffer_test_setup),
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
