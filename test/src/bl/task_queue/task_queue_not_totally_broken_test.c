#define BL_ASSERT_DEFAULT_BEHAVIOR 1 /*to avoid interfering with cmocka */
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/cmocka_pre.h>
/*---------------------------------------------------------------------------*/
#define BL_TPOINT_MOCK_FOR_TESTS 1
#include <bl/base/time.h>
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_get (void)
{
  return (bl_timept32) mock();
}
/*---------------------------------------------------------------------------*/
static inline void timept_will_return (unsigned v)
{
  will_return (bl_timept32_get, bl_usec_to_timept32 (v));
}
/*---------------------------------------------------------------------------*/
/* these are just to make deadline compile and silence warnings */
#define bl_timept64_get() 0
#define bl_timept_get() 0

#define bl_sysclock64_get() 0
#define bl_sysclock32_get() 0
#define bl_sysclock_get() 0
/*---------------------------------------------------------------------------*/
#include <bl/base/default_allocator.h>
#include <bl/base/flat_deadlines.c> /* rebuild here to link with the mock */
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
typedef struct bl_taskq_test_context {
  bl_taskq*    tq;
  bl_alloc_tbl alloc;
  bl_taskq_id  last_id;
  bl_err       last_err;
}
bl_taskq_test_context;
/*---------------------------------------------------------------------------*/
void bl_taskq_callback (bl_err err, bl_taskq_id id, void* context)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) context;
  c->last_id            = id;
  c->last_err           = err;
}
/*---------------------------------------------------------------------------*/
static const bl_uword       queue_size   = 4;
static const bl_uword       delayed_size = 2;
static const bl_taskq_id    unset_id     = bl_utype_max (bl_taskq_id);
static const bl_err_uint unset_err       = bl_utype_max (bl_err_uint);
/*---------------------------------------------------------------------------*/
static int bl_taskq_test_setup (void **state)
{
  static bl_taskq_test_context c;
  *state        = (void*) &c;
  c.alloc       = bl_get_default_alloc();
  c.last_id     = unset_id;
  c.last_err.own = unset_err;
  timept_will_return (0);
  bl_err err = bl_taskq_init (&c.tq, &c.alloc, queue_size, delayed_size);
  assert_true (err.own == bl_ok);
  return 0;
}
/*---------------------------------------------------------------------------*/
static int bl_taskq_test_teardown (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  bl_taskq_destroy (c->tq, &c->alloc);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_try_run_nothing_to_do (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  bl_err err = bl_taskq_try_run_one (c->tq);
  assert_true (err.own == bl_nothing_to_do);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_simple (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  bl_taskq_id id = 0;
  bl_taskq_task task = bl_taskq_task_rv (bl_taskq_callback, c);
  bl_err      err = bl_taskq_post (c->tq, &id, task);
  assert_true (!err.own);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_simple_delayed (void **state)
{
  bl_taskq_id            id = 0;
  bl_timept32            tp_cancel;
  bl_taskq_test_context* c;
  bl_taskq_task          task;
  bl_err                 err;

  c   = (bl_taskq_test_context*) *state;
  task = bl_taskq_task_rv (bl_taskq_callback, c);
  timept_will_return (0);
  err = bl_taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err.own);
  timept_will_return (1000);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (err.own == bl_nothing_to_do);
  assert_true (c->last_err.own == unset_err);
  assert_true (c->last_id == unset_id);
  timept_will_return (1999);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (err.own == bl_nothing_to_do);
  assert_true (c->last_err.own == unset_err);
  assert_true (c->last_id == unset_id);
  timept_will_return (2000);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_two_delayed (void **state)
{
  bl_taskq_id            id = 0, id2 = 0;
  bl_timept32            tp_cancel;
  bl_taskq_test_context* c;
  bl_taskq_task          task;
  bl_err                 err;

  c   = (bl_taskq_test_context*) *state;
  task = bl_taskq_task_rv (bl_taskq_callback, c);
  timept_will_return (0);
  err = bl_taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err.own);
  timept_will_return (1);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (err.own == bl_nothing_to_do);
  assert_true (c->last_err.own == unset_err);
  assert_true (c->last_id == unset_id);

  timept_will_return (2);
  err = bl_taskq_post_delayed (c->tq, &id2, &tp_cancel, task, 1000);
  assert_true (!err.own);
  timept_will_return (3);
  timept_will_return (3);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (err.own == bl_nothing_to_do);
  assert_true (c->last_err.own == unset_err);
  assert_true (c->last_id == unset_id);

  timept_will_return (1999);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_ok);
  assert_true (c->last_id == id2);

  timept_will_return (2000);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_two_delayed_same_tp (void **state)
{
  bl_taskq_id            id  = 0, id2 = 0;
  bl_timept32            tp_cancel;
  bl_taskq_test_context* c;
  bl_taskq_task          task;
  bl_err                 err;

  c   = (bl_taskq_test_context*) *state;
  task = bl_taskq_task_rv (bl_taskq_callback, c);

  timept_will_return (0);
  err = bl_taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err.own);

  timept_will_return (1);
  err = bl_taskq_post_delayed (c->tq, &id2, &tp_cancel, task, 1999);
  assert_true (!err.own);

  timept_will_return (1999);
  timept_will_return (1999);

  err = bl_taskq_try_run_one (c->tq);
  assert_true (err.own == bl_nothing_to_do);
  assert_true (c->last_err.own == unset_err);
  assert_true (c->last_id == unset_id);

  timept_will_return (2000);
  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_ok);
  assert_true (c->last_id == id);

  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_ok);
  assert_true (c->last_id == id2);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_delayed_expired_on_first_run (void **state)
{
  bl_taskq_id            id = 0;
  bl_timept32            tp_cancel;
  bl_taskq_test_context* c;
  bl_taskq_task          task;
  bl_err                 err;

  c   = (bl_taskq_test_context*) *state;
  task = bl_taskq_task_rv (bl_taskq_callback, c);

  timept_will_return (0);
  err = bl_taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err.own);
  timept_will_return (2000);

  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_ok);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_queue_overflow (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  bl_taskq_task task = bl_taskq_task_rv (bl_taskq_callback, c);
  bl_taskq_id id;
  for (bl_uword i = 0; i < queue_size; ++i) {
    bl_assert (bl_taskq_post (c->tq, &id, task).own == bl_ok);
  }
  bl_assert (bl_taskq_post (c->tq, &id, task).own == bl_would_overflow);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_delayed_queue_overflow (void **state)
{
  bl_taskq_id            id[delayed_size + 1];
  bl_timept32            tp_cancel;
  bl_taskq_test_context* c;
  bl_taskq_task          task;
  bl_err                 err;

  c   = (bl_taskq_test_context*) *state;
  task = bl_taskq_task_rv (bl_taskq_callback, c);
  timept_will_return (0);
  timept_will_return (1);
  timept_will_return (2);

  for (bl_uword i = 0; i < bl_arr_elems (id); ++i) {
    err = bl_taskq_post_delayed (c->tq, &id[i], &tp_cancel, task, 2000);
    assert_true (!err.own);
  }
  timept_will_return (3);
  timept_will_return (4);

  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  /*user is resposible of not overflowing the timer queue*/
  assert_true (c->last_err.own == bl_would_overflow);
  assert_true (c->last_id == id[delayed_size]);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_post_try_cancel_delayed (void **state)
{
  bl_taskq_id            id = 0;
  bl_timept32            tp_cancel;
  bl_taskq_test_context* c;
  bl_taskq_task          task;
  bl_err                 err;

  timept_will_return (0);
  timept_will_return (1);

  c   = (bl_taskq_test_context*) *state;
  task = bl_taskq_task_rv (bl_taskq_callback, c);
  err = bl_taskq_post_delayed (c->tq, &id, &tp_cancel, task, 2000);
  assert_true (!err.own);
  assert_true (c->last_err.own == unset_err);
  assert_true (c->last_id == unset_id);

  bl_taskq_post_try_cancel_delayed (c->tq, id, tp_cancel);
  assert_true (c->last_err.own == unset_err);
  assert_true (c->last_id == unset_id);

  err = bl_taskq_try_run_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_cancelled);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_run_one_timeout (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  timept_will_return (0);
  timept_will_return (0);
  bl_err err = bl_taskq_run_one (c->tq, BL_SCHED_TMIN_US + 1);
  assert_true (err.own == bl_timeout);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_run_one_timeout_under_sched (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  timept_will_return (0);
  timept_will_return (BL_SCHED_TMIN_US - 1);
  timept_will_return (BL_SCHED_TMIN_US);

  bl_err err = bl_taskq_run_one (c->tq, BL_SCHED_TMIN_US);
  assert_true (err.own == bl_timeout);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_run_one_pending_delayed (void **state)
{
  bl_taskq_id            id;
  bl_timept32            tp_cancel;
  bl_taskq_test_context* c;
  bl_taskq_task          task;
  bl_err                 err;

  c   = (bl_taskq_test_context*) *state;
  task = bl_taskq_task_rv (bl_taskq_callback, c);

  timept_will_return (0); /*bl_taskq_post_delayed*/
  timept_will_return (0); /*bl_taskq_try_run_one*/
  timept_will_return (0); /*bl_taskq_run_one, init expiration*/
  timept_will_return (1); /*bl_taskq_run_one, calc sem timeout*/
  timept_will_return ((BL_SCHED_TMIN_US * 2) - 1); /*bl_taskq_try_run_one
                                                           after sem wake up*/
  err = bl_taskq_post_delayed(
    c->tq, &id, &tp_cancel, task, BL_SCHED_TMIN_US * 2
    );

  err = bl_taskq_run_one (c->tq, BL_SCHED_TMIN_US + 2);
  assert_true (err.own == bl_timeout);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_try_cancel_one_nothing_to_do (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  bl_err err = bl_taskq_try_cancel_one (c->tq);
  assert_true (err.own == bl_nothing_to_do);
}
/*---------------------------------------------------------------------------*/
static void bl_taskq_test_try_cancel_one (void **state)
{
  bl_taskq_test_context* c = (bl_taskq_test_context*) *state;
  bl_taskq_id   id;
  bl_taskq_task task = bl_taskq_task_rv (bl_taskq_callback, c);
  bl_err  err     = bl_taskq_post (c->tq, &id, task);
  bl_assert (!err.own);
  err = bl_taskq_try_cancel_one (c->tq);
  assert_true (!err.own);
  assert_true (c->last_err.own == bl_cancelled);
  assert_true (c->last_id == id);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_try_run_nothing_to_do,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_simple, bl_taskq_test_setup, bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_simple_delayed, bl_taskq_test_setup, bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_two_delayed, bl_taskq_test_setup, bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_two_delayed_same_tp,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_delayed_expired_on_first_run,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_queue_overflow, bl_taskq_test_setup, bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_delayed_queue_overflow,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_post_try_cancel_delayed,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_run_one_timeout, bl_taskq_test_setup, bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_run_one_timeout_under_sched,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_run_one_pending_delayed,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_try_cancel_one_nothing_to_do,
    bl_taskq_test_setup,
    bl_taskq_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    bl_taskq_test_try_cancel_one, bl_taskq_test_setup, bl_taskq_test_teardown
    ),
};
/*---------------------------------------------------------------------------*/
int task_queue_not_totally_broken_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

