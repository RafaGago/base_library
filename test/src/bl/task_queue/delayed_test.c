#include <bl/cmocka_pre.h>
#include <bl/hdr/base/default_allocator.h>
/*---------------------------------------------------------------------------*/
#define BL_TIME_NO_TIMESTAMP 1
#include <bl/hdr/base/time.h>
static inline tstamp bl_get_tstamp (void)
{
  return (tstamp) mock();
}
static inline tstamp tstamp_usec_ceil (tstamp t)
{
  return t;
}
/*---------------------------------------------------------------------------*/
static inline tstamp bl_tstamp_offset_usec (toffset usec)
{
  return usec;
}
/*---------------------------------------------------------------------------*/
/* the conversion function ...*/

/*---------------------------------------------------------------------------*/
#include <bl/task_queue/delayed_test.h>
#include <bl/lib/task_queue/delayed.h>
#include <bl/lib/task_queue/delayed.c>
#include <bl/hdr/base/utility.h>
/*---------------------------------------------------------------------------*/
typedef struct delayed_context {
  alloc_tbl alloc;
  delayed   dl;
}
delayed_context;
/*---------------------------------------------------------------------------*/
#define delayed_test_elems 2
/*---------------------------------------------------------------------------*/
static int delayed_test_setup (void **state)
{
  static delayed_context c;
  *state = (void*) &c;
  will_return (bl_get_tstamp, 0);
  c.alloc    = get_default_alloc();
  bl_err err = delayed_init (&c.dl, &c.alloc, 2);
  assert_true (!err);
  return 0;
}
/*---------------------------------------------------------------------------*/
static int delayed_test_teardown (void **state)
{
  delayed_context* c = (delayed_context*) *state;
  delayed_destroy (&c->dl, &c->alloc);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void delayed_o1_regular_insertion (void **state)
{
  delayed_context* c = (delayed_context*) *state;
  delayed_entry e[2];

  static_assert_ns (arr_elems (e) == delayed_test_elems);

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[0].deadl, 1000);
  e[0].id           = 0;
  e[0].task.func    = (taskq_task_func) 0;
  e[0].task.context = (void*) 0;

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[1].deadl, 2000);
  e[1].id           = 1;
  e[1].task.func    = (taskq_task_func) 1;
  e[1].task.context = (void*) 1;

  bl_err err = delayed_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[0]);
  assert_true (err == bl_would_overflow);

  will_return (bl_get_tstamp, 999);
  const delayed_entry* v = delayed_get_head_if_expired (&c->dl);
  assert_true (v == nullptr);

  will_return (bl_get_tstamp, 1000);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v != nullptr);
  assert_true (v->id == 0);
  assert_true (v->task.func == (taskq_task_func) 0);
  assert_true (v->task.context == (void*) 0);
  delayed_drop_head (&c->dl);

  will_return (bl_get_tstamp, 1999);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v == nullptr);

  will_return (bl_get_tstamp, 2000);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  assert_true (v->task.func == (taskq_task_func) 1);
  assert_true (v->task.context == (void*) 1);
  delayed_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void delayed_o1_regular_insertion_wrap (void **state)
{
  delayed_context* c = (delayed_context*) *state;
  delayed_entry e[2];

  const tstamp near_wrap = utype_max (tstamp) - 1500;

  static_assert_ns (arr_elems (e) == delayed_test_elems);

  c->dl.time_offset = near_wrap;

  will_return (bl_get_tstamp, near_wrap);
  deadline_init (&e[0].deadl, 1000);
  e[0].id           = 0;
  e[0].task.func    = (taskq_task_func) 0;
  e[0].task.context = (void*) 0;

  will_return (bl_get_tstamp, near_wrap);
  deadline_init (&e[1].deadl, 2000);
  e[1].id           = 1;
  e[1].task.func    = (taskq_task_func) 1;
  e[1].task.context = (void*) 1;

  bl_err err = delayed_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[0]);
  assert_true (err == bl_would_overflow);

  will_return (bl_get_tstamp, near_wrap + 999);
  const delayed_entry* v = delayed_get_head_if_expired (&c->dl);
  assert_true (v == nullptr);

  will_return (bl_get_tstamp, near_wrap + 1000);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v != nullptr);
  assert_true (v->id == 0);
  assert_true (v->task.func == (taskq_task_func) 0);
  assert_true (v->task.context == (void*) 0);
  delayed_drop_head (&c->dl);

  will_return (bl_get_tstamp, near_wrap + 1999);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v == nullptr);

  will_return (bl_get_tstamp, near_wrap + 2000);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  assert_true (v->task.func == (taskq_task_func) 1);
  assert_true (v->task.context == (void*) 1);
  delayed_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void delayed_cancellation (void **state)
{
  delayed_context* c = (delayed_context*) *state;
  delayed_entry e[2];

  static_assert_ns (arr_elems (e) == delayed_test_elems);

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[0].deadl, 1000);
  e[0].id           = 0;
  e[0].task.func    = (taskq_task_func) 0;
  e[0].task.context = (void*) 0;

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[1].deadl, 2000);
  e[1].id           = 1;
  e[1].task.func    = (taskq_task_func) 1;
  e[1].task.context = (void*) 1;

  bl_err err = delayed_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[0]);
  assert_true (!err);

  taskq_task task;
  bool       res;
  res = delayed_try_get_task_and_drop (&c->dl, &task, e[0].deadl, e[0].id);
  assert_true (res);
  assert_true (task.func == e[0].task.func);
  assert_true (task.context == e[0].task.context);
  res = delayed_try_get_task_and_drop (&c->dl, &task, e[0].deadl, e[0].id);
  assert_true (!res);
  
  res = delayed_try_get_task_and_drop (&c->dl, &task, e[1].deadl, e[1].id);
  assert_true (res);
  assert_true (task.func == e[1].task.func);
  assert_true (task.context == e[1].task.context);
  res = delayed_try_get_task_and_drop (&c->dl, &task, e[1].deadl, e[1].id);
  assert_true (!res);
}
/*---------------------------------------------------------------------------*/
static void delayed_duplicate_deadline (void **state)
{
  delayed_context* c = (delayed_context*) *state;
  delayed_entry e[2];

  static_assert_ns (arr_elems (e) == delayed_test_elems);

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[0].deadl, 1000);
  e[0].id           = 0;
  e[0].task.func    = (taskq_task_func) 0;
  e[0].task.context = (void*) 0;

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[1].deadl, 1000);
  e[1].id           = 1;
  e[1].task.func    = (taskq_task_func) 1;
  e[1].task.context = (void*) 1;

  bl_err err = delayed_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[1]);
  assert_true (err == bl_would_overflow);

  will_return (bl_get_tstamp, 999);
  const delayed_entry* v = delayed_get_head_if_expired (&c->dl);
  assert_true (v == nullptr);

  will_return (bl_get_tstamp, 1000);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v != nullptr);
  assert_true (v->id == 0);
  assert_true (v->task.func == (taskq_task_func) 0);
  assert_true (v->task.context == (void*) 0);
  delayed_drop_head (&c->dl);

  /*will_return (bl_get_tstamp, 1000);*/
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  assert_true (v->task.func == (taskq_task_func) 1);
  assert_true (v->task.context == (void*) 1);
  delayed_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static void delayed_duplicate_deadline_deletion (void **state)
{
  delayed_context* c = (delayed_context*) *state;
  delayed_entry e[2];

  static_assert_ns (arr_elems (e) == delayed_test_elems);

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[0].deadl, 1000);
  e[0].id           = 0;
  e[0].task.func    = (taskq_task_func) 0;
  e[0].task.context = (void*) 0;

  will_return (bl_get_tstamp, 0);
  deadline_init (&e[1].deadl, 1000);
  e[1].id           = 1;
  e[1].task.func    = (taskq_task_func) 1;
  e[1].task.context = (void*) 1;

  bl_err err = delayed_insert (&c->dl, &e[1]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[0]);
  assert_true (!err);
  err = delayed_insert (&c->dl, &e[0]);
  assert_true (err == bl_would_overflow);

  will_return (bl_get_tstamp, 999);
  const delayed_entry* v = delayed_get_head_if_expired (&c->dl);
  assert_true (v == nullptr);

  taskq_task task;
  bool       res;
  res = delayed_try_get_task_and_drop (&c->dl, &task, e[0].deadl, e[0].id);
  assert_true (res);
  assert_true (task.func == e[0].task.func);
  assert_true (task.context == e[0].task.context);
  res = delayed_try_get_task_and_drop (&c->dl, &task, e[0].deadl, e[0].id);
  assert_true (!res); 

  will_return (bl_get_tstamp, 1000);
  v = delayed_get_head_if_expired (&c->dl);
  assert_true (v != nullptr);
  assert_true (v->id == 1);
  assert_true (v->task.func == (taskq_task_func) 1);
  assert_true (v->task.context == (void*) 1);
  delayed_drop_head (&c->dl);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(
    delayed_o1_regular_insertion,
    delayed_test_setup,
    delayed_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    delayed_o1_regular_insertion_wrap,
    delayed_test_setup,
    delayed_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    delayed_cancellation,
    delayed_test_setup,
    delayed_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    delayed_duplicate_deadline,
    delayed_test_setup,
    delayed_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    delayed_duplicate_deadline_deletion,
    delayed_test_setup,
    delayed_test_teardown
    ),
};
/*---------------------------------------------------------------------------*/
int delayed_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
