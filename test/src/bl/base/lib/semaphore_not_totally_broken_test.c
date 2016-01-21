#include <bl/hdr/base/platform.h>

#ifdef BL_LINUX

#include <bl/base/cmocka_pre.h>
#include <bl/base/lib/semaphore_not_totally_broken_test.h>

#include <bl/hdr/base/thread.h>
#include <bl/lib/base/semaphore.h>
#include <bl/lib/base/semaphore_linux.c>
/*---------------------------------------------------------------------------*/
/* 
  A file to test that the timed semaphore hasn't too many issues unrelated to 
  multithreading.

  It was just used to run the debugger through after coding and seeing that
  everything is right. It uses its private to do checks.
*/
/*---------------------------------------------------------------------------*/
typedef struct sem_ntb_context {
  bl_tm_sem sem;
  bl_thread thr;
  bl_err    thr_err;
}
sem_ntb_context;
/*---------------------------------------------------------------------------*/
static int sem_ntb_test_setup (void **state)
{
  static sem_ntb_context c;
  *state = (void*) &c;
  assert_true (bl_tm_sem_init (&c.sem) == bl_ok);
  return 0;
}
/*---------------------------------------------------------------------------*/
static int sem_ntb_test_teardown (void **state)
{
  sem_ntb_context* c = (sem_ntb_context*) *state;
  assert_true (bl_tm_sem_destroy (&c->sem) == bl_ok);
  return 0;
}
/*---------------------------------------------------------------------------*/
static int sem_ntb_do_waiting (void* context)
{
  sem_ntb_context* c = (sem_ntb_context*) context;
  c->thr_err         = bl_tm_sem_wait (&c->sem, 1000000);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void sem_ntb_signal_then_wait (void **state)
{
  sem_ntb_context* c = (sem_ntb_context*) *state;
  assert_true (bl_tm_sem_signal (&c->sem) == bl_ok);
  assert_true (tm_sem_futex_get_sig (c->sem.sem) == 1);
  assert_true (tm_sem_futex_get_wait (c->sem.sem) == 0);

  assert_true (bl_tm_sem_wait (&c->sem, 1000) == bl_ok);
  assert_true (tm_sem_futex_get_sig (c->sem.sem) == 0);
  assert_true (tm_sem_futex_get_wait (c->sem.sem) == 0);
}
/*---------------------------------------------------------------------------*/
static void sem_ntb_wait_then_signal (void **state)
{
  sem_ntb_context* c = (sem_ntb_context*) *state;
  assert_true(
    bl_thread_init (&c->thr, sem_ntb_do_waiting, c) == bl_thread_success
    );
  while ((tm_sem_futex_get_wait (c->sem.sem) == 0)) {
    bl_thread_yield(); /*poor-man's synchronization*/
  }
  assert_true (bl_tm_sem_signal (&c->sem) == bl_ok);
  bl_thread_join (&c->thr);
  assert_true (c->thr_err == bl_ok);
}
/*---------------------------------------------------------------------------*/
static void sem_ntb_wait_timeout (void **state)
{
  sem_ntb_context* c = (sem_ntb_context*) *state;
  assert_true (bl_tm_sem_wait (&c->sem, 10000) == bl_timeout);
  assert_true (tm_sem_futex_get_sig (c->sem.sem) == 0);
  assert_true (tm_sem_futex_get_wait (c->sem.sem) == 0);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(
    sem_ntb_signal_then_wait, sem_ntb_test_setup, sem_ntb_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    sem_ntb_wait_then_signal, sem_ntb_test_setup, sem_ntb_test_teardown
    ),
  cmocka_unit_test_setup_teardown(
    sem_ntb_wait_timeout, sem_ntb_test_setup, sem_ntb_test_teardown
    ),
};
/*---------------------------------------------------------------------------*/
int semaphore_not_totally_broken_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
#else /*BL_LINUX*/
/*---------------------------------------------------------------------------*/
int semaphore_not_totally_broken_tests (void)
{
  return 0;
}
/*---------------------------------------------------------------------------*/
#endif /*BL_LINUX*/
