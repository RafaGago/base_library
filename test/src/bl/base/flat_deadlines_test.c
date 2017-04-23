#include <bl/cmocka_pre.h>
#include <bl/base/default_allocator.h>
#include <bl/base/time.h>
#include <bl/base/deadline.h>
#include <bl/base/flat_deadlines.h>
#include <bl/base/utility.h>
/*---------------------------------------------------------------------------*/
typedef struct flat_deadlines_content {
  int id;
}
flat_deadlines_content;
/*---------------------------------------------------------------------------*/
static inline int fdlc_cmpeq_func(
    flat_deadlines_content const* a, flat_deadlines_content const* b
  )
{
  return a->id - b->id;
}
/*---------------------------------------------------------------------------*/
define_flat_deadlines_types(
  flat_deadlines, flat_deadlines_content
  )
declare_flat_deadlines_funcs(
  flat_deadlines, flat_deadlines_content, static inline
  )
define_flat_deadlines_funcs(
    flat_deadlines, flat_deadlines_content, fdlc_cmpeq_func, static inline
  )
/*---------------------------------------------------------------------------*/
typedef struct flat_deadlines_context {
  alloc_tbl      alloc;
  flat_deadlines dl;
}
flat_deadlines_context;
/*---------------------------------------------------------------------------*/
#define flat_deadlines_test_elems 2
/*---------------------------------------------------------------------------*/
static int flat_deadlines_test_setup (void **state)
{
  static flat_deadlines_context c;
  *state = (void*) &c;
  c.alloc    = get_default_alloc();
  bl_err err = flat_deadlines_init_explicit (&c.dl, 0, 2, &c.alloc);
  assert_true (!err);
  return 0;
}
/*---------------------------------------------------------------------------*/
static int flat_deadlines_test_teardown (void **state)
{
  flat_deadlines_context* c = (flat_deadlines_context*) *state;
  flat_deadlines_destroy (&c->dl, &c->alloc);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void flat_deadlines_o1_regular_insertion (void **state)
{
  flat_deadlines_context* c = (flat_deadlines_context*) *state;
  flat_deadlines_entry e[2];

  static_assert_ns (arr_elems (e) == flat_deadlines_test_elems);

  deadline_init_explicit (&e[0].key, 0, 1000);
  e[0].value.id = 0;

  deadline_init_explicit (&e[1].key, 0, 2000);
  e[1].value.id = 1;

  bl_err err = flat_deadlines_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (err == bl_would_overflow);

  const flat_deadlines_entry* v =
    flat_deadlines_get_head_if_expired_explicit (&c->dl, 999);
  assert_true (v == nullptr);

  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, 1000);
  assert_true (v != nullptr);
  assert_true (v->value.id == 0);
  flat_deadlines_drop_head (&c->dl);

  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, 1999);
  assert_true (v == nullptr);

  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, 2000);
  assert_true (v != nullptr);
  assert_true (v->value.id == 1);
  flat_deadlines_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void flat_deadlines_o1_regular_insertion_wrap (void **state)
{
  flat_deadlines_context* c = (flat_deadlines_context*) *state;
  flat_deadlines_entry e[2];

  const tstamp near_wrap = utype_max (tstamp) - 1500;

  static_assert_ns (arr_elems (e) == flat_deadlines_test_elems);

  c->dl.time_offset = near_wrap;

  deadline_init_explicit (&e[0].key, near_wrap, 1000);
  e[0].value.id = 0;

  deadline_init_explicit (&e[1].key, near_wrap, 2000);
  e[1].value.id = 1;

  bl_err err = flat_deadlines_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (err == bl_would_overflow);

  const flat_deadlines_entry* v =
    flat_deadlines_get_head_if_expired_explicit (&c->dl, near_wrap + 999);
  assert_true (v == nullptr);

  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, near_wrap + 1000);
  assert_true (v != nullptr);
  assert_true (v->value.id == 0);
  flat_deadlines_drop_head (&c->dl);

  /*deliberate tstamp overflow*/
  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, near_wrap + 1999);
  assert_true (v == nullptr);

  /*deliberate tstamp overflow*/
  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, near_wrap + 2000);
  assert_true (v != nullptr);
  assert_true (v->value.id == 1);
  flat_deadlines_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void flat_deadlines_cancellation (void **state)
{
  flat_deadlines_context* c = (flat_deadlines_context*) *state;
  flat_deadlines_entry e[2];

  static_assert_ns (arr_elems (e) == flat_deadlines_test_elems);

  deadline_init_explicit (&e[0].key, 0, 1000);
  e[0].value.id = 0;

  deadline_init_explicit (&e[1].key, 0, 2000);
  e[1].value.id = 1;

  bl_err err = flat_deadlines_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (!err);

  flat_deadlines_content cancel = { 0 };
  bool                   res;
  res = flat_deadlines_try_get_and_drop(
    &c->dl, &cancel, e[0].key, &e[0].value
    );
  assert_true (res);
  assert_true (cancel.id == e[0].value.id);
  res = flat_deadlines_try_get_and_drop(
    &c->dl, &cancel, e[0].key, &e[0].value
    );
  assert_true (!res);

  res = flat_deadlines_try_get_and_drop(
    &c->dl, &cancel, e[1].key, &e[1].value
    );
  assert_true (res);
  assert_true (cancel.id == e[1].value.id);
  res = flat_deadlines_try_get_and_drop(
    &c->dl, &cancel, e[1].key, &e[1].value
    );
  assert_true (!res);
}
/*---------------------------------------------------------------------------*/
static void flat_deadlines_duplicate_deadline (void **state)
{
  flat_deadlines_context* c = (flat_deadlines_context*) *state;
  flat_deadlines_entry e[2];

  static_assert_ns (arr_elems (e) == flat_deadlines_test_elems);

  deadline_init_explicit (&e[0].key, 0, 1000);
  e[0].value.id = 0;

  deadline_init_explicit (&e[1].key, 0, 1000);
  e[1].value.id = 1;

  bl_err err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[1]);
  assert_true (err == bl_would_overflow);

  const flat_deadlines_entry* v =
    flat_deadlines_get_head_if_expired_explicit (&c->dl, 999);
  assert_true (v == nullptr);

  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, 1000);
  assert_true (v != nullptr);
  assert_true (v->value.id == 0);
  flat_deadlines_drop_head (&c->dl);

  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, 1000);
  assert_true (v != nullptr);
  assert_true (v->value.id == 1);
  flat_deadlines_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void flat_deadlines_duplicate_deadline_deletion (void **state)
{
  flat_deadlines_context* c = (flat_deadlines_context*) *state;
  flat_deadlines_entry e[2];

  static_assert_ns (arr_elems (e) == flat_deadlines_test_elems);

  deadline_init_explicit (&e[0].key, 0, 1000);
  e[0].value.id = 0;

  deadline_init_explicit (&e[1].key, 0, 1000);
  e[1].value.id = 1;

  bl_err err = flat_deadlines_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = flat_deadlines_insert (&c->dl, &e[0]);
  assert_true (err == bl_would_overflow);

  const flat_deadlines_entry* v =
    flat_deadlines_get_head_if_expired_explicit (&c->dl, 999);
  assert_true (v == nullptr);

  flat_deadlines_content cancel = { 0 };
  bool                   res;
  res = flat_deadlines_try_get_and_drop(
    &c->dl, &cancel, e[0].key, &e[0].value
    );
  assert_true (res);
  assert_true (cancel.id == e[0].value.id);
  res = flat_deadlines_try_get_and_drop(
    &c->dl, &cancel, e[0].key, &e[0].value
    );
  assert_true (!res);

  v = flat_deadlines_get_head_if_expired_explicit (&c->dl, 1000);
  assert_true (v != nullptr);
  assert_true (v->value.id == 1);
  flat_deadlines_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(
    flat_deadlines_o1_regular_insertion,
    flat_deadlines_test_setup,
    flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    flat_deadlines_o1_regular_insertion_wrap,
    flat_deadlines_test_setup,
    flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    flat_deadlines_cancellation,
    flat_deadlines_test_setup,
    flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    flat_deadlines_duplicate_deadline,
    flat_deadlines_test_setup,
    flat_deadlines_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    flat_deadlines_duplicate_deadline_deletion,
    flat_deadlines_test_setup,
    flat_deadlines_test_teardown
    ),
};
/*---------------------------------------------------------------------------*/
int flat_deadlines_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
