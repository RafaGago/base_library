#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/base/ordered_ringbuffer.h>
#include <bl/base/utility.h>
/*---------------------------------------------------------------------------*/
typedef struct content {
  bl_uword key;
  bl_uword value;
}
content;
/*---------------------------------------------------------------------------*/
bl_word cmp_func (void const* k1, void const* k2, void* cmp_context)
{
  return ((content*) k1)->key - ((content*) k2)->key;
}
/*---------------------------------------------------------------------------*/
define_bl_oringb_funcs (bl_oringbt, content, cmp_func)
/*---------------------------------------------------------------------------*/
typedef struct bl_oringb_context {
  content dat[8];
  bl_oringb  od;
}
bl_oringb_context;
/*---------------------------------------------------------------------------*/
static int bl_oringb_test_setup (void **state)
{
  static bl_oringb_context c;
  *state = (void*) &c;
  memset (c.dat, -1, sizeof c.dat);
  bl_oringbt_init_extern (&c.od, c.dat, bl_arr_elems (c.dat));
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_tail (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  content   next;

  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    next.key   = i;
    next.value = i;
    assert_true (bl_oringbt_can_insert (&c->od));
    bl_uword it = bl_oringbt_insert_lifo_duplicates (&c->od, &next, nullptr);
    assert_true (bl_oringbt_at (&c->od, it)->key == next.key);
    assert_true (bl_oringbt_at (&c->od, it)->value == next.value);
    assert_true (bl_oringbt_size (&c->od) == i + 1);
  }
  assert_true (!bl_oringbt_can_insert (&c->od));
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == i);
    assert_true (bl_oringbt_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_head (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  content   next;

  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    next.key   = bl_oringbt_capacity (&c->od) - i - 1;
    next.value = bl_oringbt_capacity (&c->od) - i - 1;
    assert_true (bl_oringbt_can_insert (&c->od));
    bl_uword it = bl_oringbt_insert_lifo_duplicates (&c->od, &next, nullptr);
    assert_true (bl_oringbt_at (&c->od, it)->key == next.key);
    assert_true (bl_oringbt_at (&c->od, it)->value == next.value);
    assert_true (bl_oringbt_size (&c->od) == i + 1);
  }
  assert_true (!bl_oringbt_can_insert (&c->od));
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == i);
    assert_true (bl_oringbt_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_random_1 (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
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
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_can_insert (&c->od));
    bl_uword it =
      bl_oringbt_insert_lifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (bl_oringbt_at (&c->od, it)->key == values[i].key);
    assert_true (bl_oringbt_at (&c->od, it)->value == values[i].value);
    assert_true (bl_oringbt_size (&c->od) == i + 1);
  }
  assert_true (!bl_oringbt_can_insert (&c->od));
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == i);
    assert_true (bl_oringbt_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_random_2 (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
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
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_can_insert (&c->od));
    bl_uword it =
      bl_oringbt_insert_lifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (bl_oringbt_at (&c->od, it)->key == values[i].key);
    assert_true (bl_oringbt_at (&c->od, it)->value == values[i].value);
    assert_true (bl_oringbt_size (&c->od) == i + 1);
  }
  assert_true (!bl_oringbt_can_insert (&c->od));
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == i);
    assert_true (bl_oringbt_at (&c->od, i)->value == i);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_head_fifo_duplicates (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  content next;

  next.key   = 1;
  next.value = bl_oringbt_capacity (&c->od) - 1;
  bl_oringbt_insert_fifo_duplicates (&c->od, &next, nullptr);

  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i) {
    next.key   = 0;
    next.value = i;
    bl_oringbt_insert_fifo_duplicates (&c->od, &next, nullptr);
  }
  bl_uword i;
  for (i = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == 0);
    assert_true (bl_oringbt_at (&c->od, i)->value == i);
  }
  assert_true (bl_oringbt_at (&c->od, i)->key == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_tail_fifo_duplicates (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  content next;

  next.key   = 0;
  next.value = bl_oringbt_capacity (&c->od) - 1;
  bl_oringbt_insert_fifo_duplicates (&c->od, &next, nullptr);

  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i) {
    next.key   = 1;
    next.value = i;
    bl_oringbt_insert_fifo_duplicates (&c->od, &next, nullptr);
  }
  assert_true (bl_oringbt_at (&c->od, 0)->key == 0);
  for (bl_uword i = 1; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == 1);
    assert_true (bl_oringbt_at (&c->od, i)->value == i - 1);
  }

}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_random_fifo_duplicates (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
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
  bl_oringbt_init_extern (&c->od, c->dat, bl_arr_elems (c->dat));

  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_can_insert (&c->od));
    bl_uword it =
      bl_oringbt_insert_fifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (bl_oringbt_at (&c->od, it)->key == values[i].key);
    assert_true (bl_oringbt_at (&c->od, it)->value == values[i].value);
    assert_true (bl_oringbt_size (&c->od) == i + 1);
  }
  assert_true (!bl_oringbt_can_insert (&c->od));

  assert_true (bl_oringbt_at (&c->od, 0)->key == 0);
  assert_true (bl_oringbt_at (&c->od, 0)->value == 0);

  for (bl_uword i = 1; i < (bl_oringbt_capacity (&c->od) - 1); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == 1);
    assert_true (bl_oringbt_at (&c->od, i)->value == i);
  }
  assert_true (bl_oringbt_at (&c->od, 7)->key == 7);
  assert_true (bl_oringbt_at (&c->od, 7)->value == 7);
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_head_lifo_duplicates (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  content next;

  next.key   = 1;
  next.value = bl_oringbt_capacity (&c->od) - 1;
  bl_oringbt_insert_lifo_duplicates (&c->od, &next, nullptr);

  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i) {
    next.key   = 0;
    next.value = i;
    bl_oringbt_insert_lifo_duplicates (&c->od, &next, nullptr);
  }
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == 0);
    assert_true(
      bl_oringbt_at (&c->od, i)->value == bl_oringbt_capacity (&c->od) - i - 2
      );
  }
  assert_true(
    bl_oringbt_at (&c->od, bl_oringbt_capacity (&c->od) - 1)->key == 1
    );
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_tail_lifo_duplicates (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  content next;

  next.key   = 0;
  next.value = bl_oringbt_capacity (&c->od) - 1;
  bl_oringbt_insert_lifo_duplicates (&c->od, &next, nullptr);

  for (bl_uword i = 1; i < bl_oringbt_capacity (&c->od); ++i) {
    next.key   = 1;
    next.value = i;
    bl_oringbt_insert_lifo_duplicates (&c->od, &next, nullptr);
  }
  assert_true (bl_oringbt_at (&c->od, 0)->key == 0);
  for (bl_uword i = 1; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == 1);
    assert_true(
      bl_oringbt_at (&c->od, i)->value == bl_oringbt_capacity (&c->od) - i
      );
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_insert_random_lifo_duplicates (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
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
  bl_oringbt_init_extern (&c->od, c->dat, bl_arr_elems (c->dat));

  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    assert_true (bl_oringbt_can_insert (&c->od));
    bl_uword it =
      bl_oringbt_insert_lifo_duplicates (&c->od, &values[i], nullptr);
    assert_true (bl_oringbt_at (&c->od, it)->key == values[i].key);
    assert_true (bl_oringbt_at (&c->od, it)->value == values[i].value);
    assert_true (bl_oringbt_size (&c->od) == i + 1);
  }
  assert_true (!bl_oringbt_can_insert (&c->od));

  assert_true (bl_oringbt_at (&c->od, 0)->key == 0);
  assert_true (bl_oringbt_at (&c->od, 0)->value == 0);

  for (bl_uword i = 1; i < (bl_oringbt_capacity (&c->od) - 1); ++i) {
    assert_true (bl_oringbt_at (&c->od, i)->key == 1);
    assert_true (bl_oringbt_at (&c->od, i)->value == i);
  }
  assert_true (bl_oringbt_at (&c->od, 7)->key == 7);
  assert_true (bl_oringbt_at (&c->od, 7)->value == 7);
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_regular_insert_no_duplicates (void **state)
{
  bl_oringb_insert_result ir;
  content              next;
  bl_oringb_context*      c = (bl_oringb_context*) *state;

  next.key   = 0;
  next.value = 0;
  ir = bl_oringbt_insert (&c->od, &next, nullptr);
  assert_true (ir.new_insertion);
  assert_true (bl_oringbt_at (&c->od, ir.idx)->value == next.value);

  next.key   = 1;
  next.value = 1;
  ir = bl_oringbt_insert (&c->od, &next, nullptr);
  assert_true (ir.new_insertion);
  assert_true (bl_oringbt_at (&c->od, ir.idx)->value == next.value);

  next.key   = 0;
  next.value = 1;
  ir = bl_oringbt_insert (&c->od, &next, nullptr);
  assert_true (!ir.new_insertion);
  assert_true (bl_oringbt_at (&c->od, ir.idx)->value == 0);
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_drop_head (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  const bl_uword rmkey = 0;
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
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    bl_oringbt_insert_lifo_duplicates (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  bl_uword rmit = bl_oringbt_find (&c->od, &rm, nullptr);
  assert_true (rmit < bl_oringbt_size (&c->od));
  bl_oringbt_drop (&c->od, rmit);

  for (bl_uword i = 0, j = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (bl_oringbt_at (&c->od, i)->key == j);
    assert_true (bl_oringbt_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_drop_tail (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  const bl_uword rmkey = 7;
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
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    bl_oringbt_insert_lifo_duplicates (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  bl_uword rmit = bl_oringbt_find (&c->od, &rm, nullptr);
  assert_true (rmit < bl_oringbt_size (&c->od));
  bl_oringbt_drop (&c->od, rmit);

  for (bl_uword i = 0, j = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (bl_oringbt_at (&c->od, i)->key == j);
    assert_true (bl_oringbt_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_drop_middle_lower (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  const bl_uword rmkey = 2;
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
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    bl_oringbt_insert (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  bl_uword rmit = bl_oringbt_find (&c->od, &rm, nullptr);
  assert_true (rmit < bl_oringbt_size (&c->od));
  bl_oringbt_drop (&c->od, rmit);

  for (bl_uword i = 0, j = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (bl_oringbt_at (&c->od, i)->key == j);
    assert_true (bl_oringbt_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_oringb_test_drop_middle_upper (void **state)
{
  bl_oringb_context* c = (bl_oringb_context*) *state;
  const bl_uword rmkey = 6;
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
  for (bl_uword i = 0; i < bl_oringbt_capacity (&c->od); ++i) {
    bl_oringbt_insert_lifo_duplicates (&c->od, &values[i], nullptr);
  }
  content rm;
  rm.key     = rmkey;
  bl_uword rmit = bl_oringbt_find (&c->od, &rm, nullptr);
  assert_true (rmit < bl_oringbt_size (&c->od));
  bl_oringbt_drop (&c->od, rmit);

  for (bl_uword i = 0, j = 0; i < bl_oringbt_capacity (&c->od) - 1; ++i, ++j) {
    if (i == rmkey) {
      ++j;
    }
    assert_true (bl_oringbt_at (&c->od, i)->key == j);
    assert_true (bl_oringbt_at (&c->od, i)->value == j);
  }
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (bl_oringb_test_insert_tail, bl_oringb_test_setup),
  cmocka_unit_test_setup (bl_oringb_test_insert_head, bl_oringb_test_setup),
  cmocka_unit_test_setup (bl_oringb_test_insert_random_1, bl_oringb_test_setup),
  cmocka_unit_test_setup (bl_oringb_test_insert_random_2, bl_oringb_test_setup),
  cmocka_unit_test_setup(
    bl_oringb_test_insert_head_fifo_duplicates, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_insert_tail_fifo_duplicates, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_insert_random_fifo_duplicates, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_insert_head_lifo_duplicates, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_insert_tail_lifo_duplicates, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_insert_random_lifo_duplicates, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_regular_insert_no_duplicates, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_drop_head, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_drop_tail, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_drop_middle_lower, bl_oringb_test_setup
    ),
  cmocka_unit_test_setup(
    bl_oringb_test_drop_middle_upper, bl_oringb_test_setup
    ),
};
/*---------------------------------------------------------------------------*/
int bl_oringb_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
