#define BL_ASSERT_DEFAULT_BEHAVIOR 1 /*to avoid interfearing with cmocka */
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/cmocka_pre.h>
/*---------------------------------------------------------------------------*/
#define BL_TSTAMP_MOCK_FOR_TESTS 1
#include <bl/base/time.h>
static inline tstamp bl_get_tstamp (void)
{
  return (tstamp) mock();
}
/*---------------------------------------------------------------------------*/
#include <bl/base/default_allocator.h>
#include <bl/task_queue/task_queue.h>
#include <bl/task_queue/task_queue.c>
#include <bl/task_queue/delayed.c>
/*---------------------------------------------------------------------------*/
/* 
  A file to test that queue hasn't too many issues unrelated to
   multithreading.

  It was just used to run the debugger through after coding and seeing that
  everything is right.
*/
//-----------------------------------------------------------------------------
typedef struct taskq_test_context {
  taskq*    tq;
  alloc_tbl alloc;
  taskq_id  last_id;
  bl_err    last_err;
}
taskq_test_context;
/*---------------------------------------------------------------------------*/
void taskq_callback (bl_err err, taskq_id id, void* context)
{
  taskq_test_context* c = (taskq_test_context*) context;
  c->last_id            = id;
  c->last_err           = err;
}
/*---------------------------------------------------------------------------*/
static const uword     queue_size   = 4;
static const uword     delayed_size = 2;
static const taskq_id  unset_id     = utype_max (taskq_id);
static const bl_err    unset_err    = utype_max (bl_err);
/*---------------------------------------------------------------------------*/
static int taskq_test_setup (void **state)
{
  static taskq_test_context c;
  *state       = (void*) &c;
  c.alloc      = get_default_alloc();
  c.last_id    = unset_id;
  c.last_err   = unset_err;
  will_return (bl_get_tstamp, 0);
  bl_err err = taskq_init (&c.tq, &c.alloc, queue_size, delayed_size);
  assert_true (err == bl_ok);
  return 0;
}
/*---------------------------------------------------------------------------*/
static int taskq_test_teardown (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  taskq_destroy (c->tq, &c->alloc);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void taskq_test_try_run_nothing_to_do (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  bl_err err = taskq_try_run_one (c->tq);
  assert_true (err == bl_nothing_to_do);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_simple (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  taskq_id id;
  taskq_task task = taskq_task_rv (taskq_callback, c);
  bl_err      err = taskq_post (c->tq, &id, task);
  assert_true (!err);
  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_simple_delayed (void **state)
{
  taskq_id            id;
  tstamp              tp_cancel;
  taskq_test_context* c;
  taskq_task          task;
  bl_err              err;

  c   = (taskq_test_context*) *state;
  task = taskq_task_rv (taskq_callback, c);
  will_return (bl_get_tstamp, 0);
  err = taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err);
  will_return (bl_get_tstamp, 1000);
  err = taskq_try_run_one (c->tq);
  assert_true (err == bl_nothing_to_do);
  assert_true (c->last_err == unset_err);
  assert_true (c->last_id == unset_id);
  will_return (bl_get_tstamp, 1999);
  err = taskq_try_run_one (c->tq);
  assert_true (err == bl_nothing_to_do);
  assert_true (c->last_err == unset_err);
  assert_true (c->last_id == unset_id);
  will_return (bl_get_tstamp, 2000);
  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_two_delayed (void **state)
{
  taskq_id            id, id2;
  tstamp              tp_cancel;
  taskq_test_context* c;
  taskq_task          task;
  bl_err              err;

  c   = (taskq_test_context*) *state;
  task = taskq_task_rv (taskq_callback, c);
  will_return (bl_get_tstamp, 0);
  err = taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err);
  will_return (bl_get_tstamp, 1);
  err = taskq_try_run_one (c->tq);
  assert_true (err == bl_nothing_to_do);
  assert_true (c->last_err == unset_err);
  assert_true (c->last_id == unset_id);

  will_return (bl_get_tstamp, 2);
  err = taskq_post_delayed (c->tq, &id2, &tp_cancel, task, 1000);
  assert_true (!err);
  will_return (bl_get_tstamp, 3);
  will_return (bl_get_tstamp, 3);
  err = taskq_try_run_one (c->tq);
  assert_true (err == bl_nothing_to_do);
  assert_true (c->last_err == unset_err);
  assert_true (c->last_id == unset_id);

  will_return (bl_get_tstamp, 1999);
  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_ok);
  assert_true (c->last_id == id2);

  will_return (bl_get_tstamp, 2000);
  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_two_delayed_same_tp (void **state)
{
  taskq_id            id, id2;
  tstamp              tp_cancel;
  taskq_test_context* c;
  taskq_task          task;
  bl_err              err;

  c   = (taskq_test_context*) *state;
  task = taskq_task_rv (taskq_callback, c);
  will_return (bl_get_tstamp, 0);
  err = taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err);

  will_return (bl_get_tstamp, 1);
  err = taskq_post_delayed (c->tq, &id2, &tp_cancel, task, 1999);
  assert_true (!err);

  will_return (bl_get_tstamp, 1999);
  will_return (bl_get_tstamp, 1999);
  err = taskq_try_run_one (c->tq);
  assert_true (err == bl_nothing_to_do);
  assert_true (c->last_err == unset_err);
  assert_true (c->last_id == unset_id);

  will_return (bl_get_tstamp, 2000);
  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_ok);
  assert_true (c->last_id == id);

  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_ok);
  assert_true (c->last_id == id2);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_delayed_expired_on_first_run (void **state)
{
  taskq_id            id;
  tstamp              tp_cancel;
  taskq_test_context* c;
  taskq_task          task;
  bl_err              err;

  c   = (taskq_test_context*) *state;
  task = taskq_task_rv (taskq_callback, c);
  will_return (bl_get_tstamp, 0);
  err = taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err);
  will_return (bl_get_tstamp, 2000);
  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_queue_overflow (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  taskq_task task = taskq_task_rv (taskq_callback, c);
  taskq_id id;
  for (uword i = 0; i < queue_size; ++i) {
    bl_assert (taskq_post (c->tq, &id, task) == bl_ok);
  }
  bl_assert (taskq_post (c->tq, &id, task) == bl_would_overflow);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_delayed_queue_overflow (void **state)
{
  taskq_id            id[delayed_size + 1];
  tstamp              tp_cancel;
  taskq_test_context* c;
  taskq_task          task;
  bl_err              err;

  c   = (taskq_test_context*) *state;
  task = taskq_task_rv (taskq_callback, c);
  will_return (bl_get_tstamp, 0);
  will_return (bl_get_tstamp, 1);
  will_return (bl_get_tstamp, 2);

  for (uword i = 0; i < arr_elems (id); ++i) {
    err = taskq_post_delayed (c->tq, &id[i], &tp_cancel, task, 2000);
    assert_true (!err);
  }
  will_return (bl_get_tstamp, 3);
  will_return (bl_get_tstamp, 4);
  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  /*user is resposible of not overflowing the timer queue*/
  assert_true (c->last_err == bl_would_overflow);
  assert_true (c->last_id == id[delayed_size]);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_post_try_cancel_delayed (void **state)
{
  taskq_id            id;
  tstamp              tp_cancel;
  taskq_test_context* c;
  taskq_task          task;
  bl_err              err;

  will_return (bl_get_tstamp, 0);
  will_return (bl_get_tstamp, 1);

  c   = (taskq_test_context*) *state;
  task = taskq_task_rv (taskq_callback, c);
  err = taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err);
  assert_true (c->last_err == unset_err);
  assert_true (c->last_id == unset_id);

  taskq_post_try_cancel_delayed (c->tq, id, tp_cancel);
  assert_true (c->last_err == unset_err);
  assert_true (c->last_id == unset_id);

  err = taskq_try_run_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_cancelled);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void taskq_run_one_timeout (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  will_return (bl_get_tstamp, 0);
  will_return (bl_get_tstamp, 0);
  bl_err err = taskq_run_one (c->tq, BL_SCHED_TMIN_US + 1);
  assert_true (err == bl_timeout);
}
/*---------------------------------------------------------------------------*/
static void taskq_run_one_timeout_under_sched (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  will_return (bl_get_tstamp, 0);
  will_return (bl_get_tstamp, BL_SCHED_TMIN_US - 1);
  will_return (bl_get_tstamp, BL_SCHED_TMIN_US);
  bl_err err = taskq_run_one (c->tq, BL_SCHED_TMIN_US);
  assert_true (err == bl_timeout);
}
/*---------------------------------------------------------------------------*/
static void taskq_run_one_pending_delayed (void **state)
{
  taskq_id            id;  
  tstamp              tp_cancel;
  taskq_test_context* c;
  taskq_task          task;
  bl_err              err;

  c   = (taskq_test_context*) *state;
  task = taskq_task_rv (taskq_callback, c);

  will_return (bl_get_tstamp, 0); /*taskq_post_delayed*/
  will_return (bl_get_tstamp, 0); /*taskq_try_run_one*/
  will_return (bl_get_tstamp, 0); /*taskq_run_one, init expiration*/
  will_return (bl_get_tstamp, 1); /*taskq_run_one, calc sem timeout*/
  will_return (bl_get_tstamp, (BL_SCHED_TMIN_US * 2) - 1); /*taskq_try_run_one 
                                                           after sem wake up*/
  err = taskq_post_delayed (c->tq, &id, &tp_cancel, task, BL_SCHED_TMIN_US * 2);

  err = taskq_run_one (c->tq, BL_SCHED_TMIN_US + 2);
  assert_true (err == bl_timeout);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_try_cancel_one_nothing_to_do (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  bl_err err = taskq_try_cancel_one (c->tq);
  assert_true (err == bl_nothing_to_do);
}
/*---------------------------------------------------------------------------*/
static void taskq_test_try_cancel_one (void **state)
{
  taskq_test_context* c = (taskq_test_context*) *state;
  taskq_id   id;
  taskq_task task = taskq_task_rv (taskq_callback, c);
  bl_err  err     = taskq_post (c->tq, &id, task);
  bl_assert (!err);
  err = taskq_try_cancel_one (c->tq);
  assert_true (!err);
  assert_true (c->last_err == bl_cancelled);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(
    taskq_test_try_run_nothing_to_do, taskq_test_setup, taskq_test_teardown
	   ),
  cmocka_unit_test_setup_teardown(
	   taskq_test_simple, taskq_test_setup, taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
	   taskq_test_simple_delayed, taskq_test_setup, taskq_test_teardown
	   ),
  cmocka_unit_test_setup_teardown(
    taskq_test_two_delayed, taskq_test_setup, taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_test_two_delayed_same_tp, taskq_test_setup, taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_test_delayed_expired_on_first_run,
    taskq_test_setup,
    taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_test_queue_overflow, taskq_test_setup, taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_test_delayed_queue_overflow, taskq_test_setup, taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_test_post_try_cancel_delayed, taskq_test_setup, taskq_test_teardown
    ),  
  cmocka_unit_test_setup_teardown(
    taskq_run_one_timeout, taskq_test_setup, taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
      taskq_run_one_timeout_under_sched,
      taskq_test_setup,
      taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_run_one_pending_delayed, taskq_test_setup, taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_test_try_cancel_one_nothing_to_do,
    taskq_test_setup,
    taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    taskq_test_try_cancel_one, taskq_test_setup, taskq_test_teardown
    ),
};
/*---------------------------------------------------------------------------*/
int task_queue_not_totally_broken_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

