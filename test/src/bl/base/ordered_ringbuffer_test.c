#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/base/ordered_ringbuffer.h>
#include <bl/base/utility.h>
/*---------------------------------------------------------------------------*/
typedef struct content {
  uword key;
  uword value;
}
content;
/*---------------------------------------------------------------------------*/
word cmp_func (void const* k1, void const* k2, void* cmp_context)
{
  return ((content*) k1)->key - ((content*) k2)->key;
}
/*---------------------------------------------------------------------------*/
define_oringb_types (oringb, content)
declare_oringb_funcs (oringb, content, static inline)
define_oringb_funcs (oringb, content, cmp_func, static inline)
/*---------------------------------------------------------------------------*/
typedef struct oringb_context {
  content dat[8];
  oringb  od;
}
oringb_context;
/*---------------------------------------------------------------------------*/
static int oringb_test_setup (void **state)
{
  static oringb_context c;
  *state = (void*) &c;
  memset (c.dat, -1, sizeof c.dat);
  oringb_init_extern (&c.od, c.dat, arr_elems (c.dat));
  return 0;
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_tail (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content   next;

  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    next.key   = i;
    next.value = i;
    assert_true (oringb_can_insert (&c->od));
    uword it = oringb_insert_lifo_duplicates (&c->od, &next, nullptr);
    assert_true (oringb_at (&c->od, it)->key == next.key);
    assert_true (oringb_at (&c->od, it)->value == next.value);
    assert_true (oringb_size (&c->od) == i + 1);
  }
  assert_true (!oringb_can_insert (&c->od));
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_at (&c->od, i)->key == i);
    assert_true (oringb_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_head (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content   next;

  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    next.key   = oringb_capacity (&c->od) - i - 1;
    next.value = oringb_capacity (&c->od) - i - 1;
    assert_true (oringb_can_insert (&c->od));
    uword it = oringb_insert_lifo_duplicates (&c->od, &next, nullptr);
    assert_true (oringb_at (&c->od, it)->key == next.key);
    assert_true (oringb_at (&c->od, it)->value == next.value);
    assert_true (oringb_size (&c->od) == i + 1);
  }
  assert_true (!oringb_can_insert (&c->od));
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_at (&c->od, i)->key == i);
    assert_true (oringb_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_random_1 (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content values[8] = {
    { 0, 0 },
    { 7, 7 },
    { 3, 3 },
    { 4, 4 },
    { 1, 1 },
    { 6, 6 },
    { 2, 2 },
    { 5, 5 },
  };
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_can_insert (&c->od));
    uword it = oringb_insert_lifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (oringb_at (&c->od, it)->key == values[i].key);
    assert_true (oringb_at (&c->od, it)->value == values[i].value);
    assert_true (oringb_size (&c->od) == i + 1);
  }
  assert_true (!oringb_can_insert (&c->od));
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_at (&c->od, i)->key == i);
    assert_true (oringb_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_random_2 (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content values[8] = {
    { 7, 7 },
    { 0, 0 },
    { 4, 4 },
    { 3, 3 },
    { 6, 6 },
    { 1, 1 },
    { 5, 5 },
    { 2, 2 },
  };
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_can_insert (&c->od));
    uword it = oringb_insert_lifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (oringb_at (&c->od, it)->key == values[i].key);
    assert_true (oringb_at (&c->od, it)->value == values[i].value);
    assert_true (oringb_size (&c->od) == i + 1);
  }
  assert_true (!oringb_can_insert (&c->od));
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_at (&c->od, i)->key == i);
    assert_true (oringb_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_head_fifo_duplicates (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content next;

  next.key   = 1;
  next.value = oringb_capacity (&c->od) - 1;
  oringb_insert_fifo_duplicates (&c->od, &next, nullptr);

  for (uword i = 0; i < oringb_capacity (&c->od) - 1; ++i) {
    next.key   = 0;
    next.value = i;
    oringb_insert_fifo_duplicates (&c->od, &next, nullptr);
  }
  uword i;
  for (i = 0; i < oringb_capacity (&c->od) - 1; ++i) {
    assert_true (oringb_at (&c->od, i)->key == 0);
    assert_true (oringb_at (&c->od, i)->value == i);
  }
  assert_true (oringb_at (&c->od, i)->key == 1);
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_tail_fifo_duplicates (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content next;

  next.key   = 0;
  next.value = oringb_capacity (&c->od) - 1;
  oringb_insert_fifo_duplicates (&c->od, &next, nullptr);

  for (uword i = 0; i < oringb_capacity (&c->od) - 1; ++i) {
    next.key   = 1;
    next.value = i;
    oringb_insert_fifo_duplicates (&c->od, &next, nullptr);
  }
  assert_true (oringb_at (&c->od, 0)->key == 0);
  for (uword i = 1; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_at (&c->od, i)->key == 1);
    assert_true (oringb_at (&c->od, i)->value == i - 1);
  }

}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_random_fifo_duplicates (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content values[8] = {
    { 0, 0 },
    { 7, 7 },
    { 1, 1 },
    { 1, 2 },
    { 1, 3 },
    { 1, 4 },
    { 1, 5 },
    { 1, 6 },
  };
  memset (c->dat, -1, sizeof c->dat);
  oringb_init_extern (&c->od, c->dat, arr_elems (c->dat));

  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_can_insert (&c->od));
    uword it = oringb_insert_fifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (oringb_at (&c->od, it)->key == values[i].key);
    assert_true (oringb_at (&c->od, it)->value == values[i].value);
    assert_true (oringb_size (&c->od) == i + 1);
  }
  assert_true (!oringb_can_insert (&c->od));

  assert_true (oringb_at (&c->od, 0)->key == 0);
  assert_true (oringb_at (&c->od, 0)->value == 0);

  for (uword i = 1; i < (oringb_capacity (&c->od) - 1); ++i) {
    assert_true (oringb_at (&c->od, i)->key == 1);
    assert_true (oringb_at (&c->od, i)->value == i);
  }
  assert_true (oringb_at (&c->od, 7)->key == 7);
  assert_true (oringb_at (&c->od, 7)->value == 7);
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_head_lifo_duplicates (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content next;

  next.key   = 1;
  next.value = oringb_capacity (&c->od) - 1;
  oringb_insert_lifo_duplicates (&c->od, &next, nullptr);

  for (uword i = 0; i < oringb_capacity (&c->od) - 1; ++i) {
    next.key   = 0;
    next.value = i;
    oringb_insert_lifo_duplicates (&c->od, &next, nullptr);
  }
  for (uword i = 0; i < oringb_capacity (&c->od) - 1; ++i) {
    assert_true (oringb_at (&c->od, i)->key == 0);
    assert_true(
      oringb_at (&c->od, i)->value == oringb_capacity (&c->od) - i - 2
      );
  }
  assert_true (oringb_at (&c->od, oringb_capacity (&c->od) - 1)->key == 1);
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_tail_lifo_duplicates (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content next;

  next.key   = 0;
  next.value = oringb_capacity (&c->od) - 1;
  oringb_insert_lifo_duplicates (&c->od, &next, nullptr);

  for (uword i = 1; i < oringb_capacity (&c->od); ++i) {
    next.key   = 1;
    next.value = i;
    oringb_insert_lifo_duplicates (&c->od, &next, nullptr);
  }
  assert_true (oringb_at (&c->od, 0)->key == 0);
  for (uword i = 1; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_at (&c->od, i)->key == 1);
    assert_true(
      oringb_at (&c->od, i)->value == oringb_capacity (&c->od) - i
      );
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_insert_random_lifo_duplicates (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  content values[8] = {
    { 0, 0 },
    { 7, 7 },
    { 1, 6 },
    { 1, 5 },
    { 1, 4 },
    { 1, 3 },
    { 1, 2 },
    { 1, 1 },
  };
  memset (c->dat, -1, sizeof c->dat);
  oringb_init_extern (&c->od, c->dat, arr_elems (c->dat));

  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    assert_true (oringb_can_insert (&c->od));
    uword it = oringb_insert_lifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (oringb_at (&c->od, it)->key == values[i].key);
    assert_true (oringb_at (&c->od, it)->value == values[i].value);
    assert_true (oringb_size (&c->od) == i + 1);
  }
  assert_true (!oringb_can_insert (&c->od));

  assert_true (oringb_at (&c->od, 0)->key == 0);
  assert_true (oringb_at (&c->od, 0)->value == 0);

  for (uword i = 1; i < (oringb_capacity (&c->od) - 1); ++i) {
    assert_true (oringb_at (&c->od, i)->key == 1);
    assert_true (oringb_at (&c->od, i)->value == i);
  }
  assert_true (oringb_at (&c->od, 7)->key == 7);
  assert_true (oringb_at (&c->od, 7)->value == 7);
}
/*---------------------------------------------------------------------------*/
static void oringb_regular_insert_no_duplicates (void **state)
{
  oringb_insert_result ir;
  content              next;
  oringb_context*      c = (oringb_context*) *state;

  next.key   = 0;
  next.value = 0;
  ir = oringb_insert (&c->od, &next, nullptr);
  assert_true (ir.new_insertion);
  assert_true (oringb_at (&c->od, ir.idx)->value == next.value);

  next.key   = 1;
  next.value = 1;
  ir = oringb_insert (&c->od, &next, nullptr);
  assert_true (ir.new_insertion);
  assert_true (oringb_at (&c->od, ir.idx)->value == next.value);

  next.key   = 0;
  next.value = 1;
  ir = oringb_insert (&c->od, &next, nullptr);
  assert_true (!ir.new_insertion);
  assert_true (oringb_at (&c->od, ir.idx)->value == 0);
}
/*---------------------------------------------------------------------------*/
static void oringb_test_drop_head (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  const uword rmkey = 0;
  content values[8] = {
    { 0, 0 },
    { 7, 7 },
    { 3, 3 },
    { 4, 4 },
    { 1, 1 },
    { 6, 6 },
    { 2, 2 },
    { 5, 5 },
  };
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    oringb_insert_lifo_duplicates (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  uword rmit = oringb_find (&c->od, &rm, nullptr);
  assert_true (rmit < oringb_size (&c->od));
  oringb_drop (&c->od, rmit);

  for (uword i = 0, j = 0; i < oringb_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (oringb_at (&c->od, i)->key == j);
    assert_true (oringb_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_drop_tail (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  const uword rmkey = 7;
  content values[8] = {
    { 0, 0 },
    { 7, 7 },
    { 3, 3 },
    { 4, 4 },
    { 1, 1 },
    { 6, 6 },
    { 2, 2 },
    { 5, 5 },
  };
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    oringb_insert_lifo_duplicates (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  uword rmit = oringb_find (&c->od, &rm, nullptr);
  assert_true (rmit < oringb_size (&c->od));
  oringb_drop (&c->od, rmit);

  for (uword i = 0, j = 0; i < oringb_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (oringb_at (&c->od, i)->key == j);
    assert_true (oringb_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_drop_middle_lower (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  const uword rmkey = 2;
  content values[8] = {
    { 0, 0 },
    { 7, 7 },
    { 3, 3 },
    { 4, 4 },
    { 1, 1 },
    { 6, 6 },
    { 2, 2 },
    { 5, 5 },
  };
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    oringb_insert (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  uword rmit = oringb_find (&c->od, &rm, nullptr);
  assert_true (rmit < oringb_size (&c->od));
  oringb_drop (&c->od, rmit);

  for (uword i = 0, j = 0; i < oringb_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (oringb_at (&c->od, i)->key == j);
    assert_true (oringb_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static void oringb_test_drop_middle_upper (void **state)
{
  oringb_context* c = (oringb_context*) *state;
  const uword rmkey = 6;
  content values[8] = {
    { 0, 0 },
    { 7, 7 },
    { 3, 3 },
    { 4, 4 },
    { 1, 1 },
    { 6, 6 },
    { 2, 2 },
    { 5, 5 },
  };
  for (uword i = 0; i < oringb_capacity (&c->od); ++i) {
    oringb_insert_lifo_duplicates (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  uword rmit = oringb_find (&c->od, &rm, nullptr);
  assert_true (rmit < oringb_size (&c->od));
  oringb_drop (&c->od, rmit);

  for (uword i = 0, j = 0; i < oringb_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (oringb_at (&c->od, i)->key == j);
    assert_true (oringb_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (oringb_test_insert_tail, oringb_test_setup),
  cmocka_unit_test_setup (oringb_test_insert_head, oringb_test_setup),
  cmocka_unit_test_setup (oringb_test_insert_random_1, oringb_test_setup),
  cmocka_unit_test_setup (oringb_test_insert_random_2, oringb_test_setup),
  cmocka_unit_test_setup(
    oringb_test_insert_head_fifo_duplicates, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_insert_tail_fifo_duplicates, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_insert_random_fifo_duplicates, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_insert_head_lifo_duplicates, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_insert_tail_lifo_duplicates, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_insert_random_lifo_duplicates, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_regular_insert_no_duplicates, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_drop_head, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_drop_tail, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_drop_middle_lower, oringb_test_setup
    ),
  cmocka_unit_test_setup(
    oringb_test_drop_middle_upper, oringb_test_setup
    ),
};
/*---------------------------------------------------------------------------*/
int oringb_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
