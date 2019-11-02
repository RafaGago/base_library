#include <bl/cmocka_pre.h>
#include <bl/base/default_allocator.h>
#include <bl/base/time.h>
#include <bl/base/deadline.h>
#include <bl/base/flat_deadlines.h>
#include <bl/base/utility.h>
/*---------------------------------------------------------------------------*/
typedef struct bl_flat_deadlines_content {
  bl_tstamp time;
  int    id;
}
bl_flat_deadlines_content;
/*---------------------------------------------------------------------------*/
static bl_word fdl_cmpeq_func (void const* a, void const* b)
{
  return ((bl_flat_deadlines_content const*) a)->id -
    ((bl_flat_deadlines_content const*) b)->id;
}
/*---------------------------------------------------------------------------*/
define_bl_flat_deadlines_funcs (fdl, bl_flat_deadlines_content, fdl_cmpeq_func)
/*---------------------------------------------------------------------------*/
typedef struct bl_flat_deadlines_context {
  bl_alloc_tbl      alloc;
  bl_flat_deadlines dl;
}
bl_flat_deadlines_context;
/*---------------------------------------------------------------------------*/
#define bl_flat_deadlines_test_elems 2
/*---------------------------------------------------------------------------*/
static int bl_flat_deadlines_test_setup (void **state)
{
  static bl_flat_deadlines_context c;
  *state = (void*) &c;
  c.alloc    = bl_get_default_alloc();
  bl_err err = fdl_init (&c.dl, 0, 2, &c.alloc);
  assert_true (!err.bl);
  return 0;
}
/*---------------------------------------------------------------------------*/
static int bl_flat_deadlines_test_teardown (void **state)
{
  bl_flat_deadlines_context* c = (bl_flat_deadlines_context*) *state;
  fdl_destroy (&c->dl, &c->alloc);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_flat_deadlines_o1_regular_insertion (void **state)
{
  bl_flat_deadlines_context* c = (bl_flat_deadlines_context*) *state;
  bl_flat_deadlines_content e[2];

  bl_static_assert_ns (bl_arr_elems (e) == bl_flat_deadlines_test_elems);

  bl_deadline_init_explicit (&e[0].time, 0, 1000);
  e[0].id = 0;

  bl_deadline_init_explicit (&e[1].time, 0, 2000);
  e[1].id = 1;

  bl_err err = fdl_insert (&c->dl, &e[1]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[0]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[0]);
  assert_true (err.bl == bl_would_overflow);

  const bl_flat_deadlines_content* v =
    fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (999));
  assert_true (v == nullptr);

  v = fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (1000));
  assert_true (v != nullptr);
  assert_true (v->id == 0);
  fdl_drop_head (&c->dl);

  v = fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (1999));
  assert_true (v == nullptr);

  v = fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (2000));
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  fdl_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void bl_flat_deadlines_o1_regular_insertion_wrap (void **state)
{
  bl_flat_deadlines_context* c = (bl_flat_deadlines_context*) *state;
  bl_flat_deadlines_content e[2];

  const bl_tstamp near_wrap =
    bl_utype_max (bl_tstamp) - bl_usec_to_bl_tstamp (1500);

  bl_static_assert_ns (bl_arr_elems (e) == bl_flat_deadlines_test_elems);

  c->dl.time_offset = near_wrap;

  bl_deadline_init_explicit (&e[0].time, near_wrap, 1000);
  e[0].id = 0;

  bl_deadline_init_explicit (&e[1].time, near_wrap, 2000); /*integer wrap*/
  e[1].id = 1;

  bl_err err = fdl_insert (&c->dl, &e[1]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[0]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[0]);
  assert_true (err.bl == bl_would_overflow);

  const bl_flat_deadlines_content* v =
    fdl_get_head_if_expired(
      &c->dl, true, near_wrap + bl_usec_to_bl_tstamp (999)
      );
  assert_true (v == nullptr);

  v = fdl_get_head_if_expired(
    &c->dl, true, near_wrap + bl_usec_to_bl_tstamp (1000)
    );
  assert_true (v != nullptr);
  assert_true (v->id == 0);
  fdl_drop_head (&c->dl);

  /*deliberate bl_tstamp overflow*/
  v = fdl_get_head_if_expired(
    &c->dl, true, near_wrap + bl_usec_to_bl_tstamp (1999)
    );
  assert_true (v == nullptr);

  /*deliberate bl_tstamp overflow*/
  v = fdl_get_head_if_expired(
    &c->dl, true, near_wrap + bl_usec_to_bl_tstamp (2000)
    );
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  fdl_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void bl_flat_deadlines_cancellation (void **state)
{
  bl_flat_deadlines_context* c = (bl_flat_deadlines_context*) *state;
  bl_flat_deadlines_content e[2];

  bl_static_assert_ns (bl_arr_elems (e) == bl_flat_deadlines_test_elems);

  bl_deadline_init_explicit (&e[0].time, 0, 1000);
  e[0].id = 0;

  bl_deadline_init_explicit (&e[1].time, 0, 2000);
  e[1].id = 1;

  bl_err err = fdl_insert (&c->dl, &e[1]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[0]);
  assert_true (!err.bl);

  bl_flat_deadlines_content cancel = { 0 };
  bool                   res;
  res = fdl_try_get_and_drop (&c->dl, &cancel, &e[0]);
  assert_true (res);
  assert_true (cancel.id == e[0].id);
  res = fdl_try_get_and_drop (&c->dl, &cancel, &e[0]);
  assert_true (!res);

  res = fdl_try_get_and_drop (&c->dl, &cancel, &e[1]);
  assert_true (res);
  assert_true (cancel.id == e[1].id);
  res = fdl_try_get_and_drop (&c->dl, &cancel, &e[1]);
  assert_true (!res);
}
/*---------------------------------------------------------------------------*/
static void bl_flat_deadlines_duplicate_deadline (void **state)
{
  bl_flat_deadlines_context* c = (bl_flat_deadlines_context*) *state;
  bl_flat_deadlines_content e[2];

  bl_static_assert_ns (bl_arr_elems (e) == bl_flat_deadlines_test_elems);

  bl_deadline_init_explicit (&e[0].time, 0, 1000);
  e[0].id = 0;

  bl_deadline_init_explicit (&e[1].time, 0, 1000);
  e[1].id = 1;

  bl_err err = fdl_insert (&c->dl, &e[0]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[1]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[1]);
  assert_true (err.bl == bl_would_overflow);

  const bl_flat_deadlines_content* v =
    fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (999));
  assert_true (v == nullptr);

  v = fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (1000));
  assert_true (v != nullptr);
  assert_true (v->id == 0);
  fdl_drop_head (&c->dl);

  v = fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (1000));
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  fdl_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void bl_flat_deadlines_duplicate_bl_deadline_deletion (void **state)
{
  bl_flat_deadlines_context* c = (bl_flat_deadlines_context*) *state;
  bl_flat_deadlines_content e[2];

  bl_static_assert_ns (bl_arr_elems (e) == bl_flat_deadlines_test_elems);

  bl_deadline_init_explicit (&e[0].time, 0, 1000);
  e[0].id = 0;

  bl_deadline_init_explicit (&e[1].time, 0, 1000);
  e[1].id = 1;

  bl_err err = fdl_insert (&c->dl, &e[1]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[0]);
  assert_true (!err.bl);
  err = fdl_insert (&c->dl, &e[0]);
  assert_true (err.bl == bl_would_overflow);

  const bl_flat_deadlines_content* v = fdl_get_head_if_expired(
    &c->dl, true, bl_usec_to_bl_tstamp (999)
    );
  assert_true (v == nullptr);

  bl_flat_deadlines_content cancel = { 0 };
  bool                   res;
  res = fdl_try_get_and_drop (&c->dl, &cancel, &e[0]);
  assert_true (res);
  assert_true (cancel.id == e[0].id);
  res = fdl_try_get_and_drop (&c->dl, &cancel, &e[0]);
  assert_true (!res);

  v = fdl_get_head_if_expired (&c->dl, true, bl_usec_to_bl_tstamp (1000));
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  fdl_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(
    bl_flat_deadlines_o1_regular_insertion,
    bl_flat_deadlines_test_setup,
    bl_flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_flat_deadlines_o1_regular_insertion_wrap,
    bl_flat_deadlines_test_setup,
    bl_flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_flat_deadlines_cancellation,
    bl_flat_deadlines_test_setup,
    bl_flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_flat_deadlines_duplicate_deadline,
    bl_flat_deadlines_test_setup,
    bl_flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_flat_deadlines_duplicate_bl_deadline_deletion,
    bl_flat_deadlines_test_setup,
    bl_flat_deadlines_test_teardown
    ),
};
/*---------------------------------------------------------------------------*/
int bl_flat_deadlines_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
