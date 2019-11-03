#include <bl/cmocka_pre.h>

#include <bl/base/thread.h>
#include <bl/base/atomic.h>
#include <bl/time_extras/time_extras.h>

/*---------------------------------------------------------------------------*/
/* This is unfortunately just very rough smoke testing. */
/*---------------------------------------------------------------------------*/
static int setup (void** state)
{
  return bl_time_extras_init().bl;
}
/*---------------------------------------------------------------------------*/
static int teardown (void** state)
{
  bl_time_extras_destroy();
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_timept_to_sysclock_diff_test (void **state)
{
  /* Assuming no clock changes and a machine with right date. This is written
  in 2019, epoch was in 1970, hence the 49 years */
  double ns_49y_epoch = 49. * 12. * 30.41 * 24. * 60. * 60. * 1000000000.;
  double ns_80y_epoch = 80. * 12. * 30.41 * 24. * 60. * 60. * 1000000000.;

  bl_timeoft diff = bl_timept_to_sysclock_diff_ns();
  assert_true (diff > (bl_u64) ns_49y_epoch);
  assert_true (diff < (bl_u64) ns_80y_epoch);
  bl_timeoft diff2 = bl_timept_to_sysclock_diff_ns();
  /*drifting less than 1ms between calls*/
  assert_int_equal (diff / bl_nsec_in_msec, diff2 / bl_nsec_in_msec);
}
/*---------------------------------------------------------------------------*/
#ifdef BL_HAS_CPU_TSTAMP
/*---------------------------------------------------------------------------*/
static void bl_cpu_timept_vs_timept_test (void **state)
{
  bl_uword ok = 0;

  bl_thread_yield(); /* context switching in-between minimization attempt.*/
  bl_timept tprev = bl_timept_get();
  bl_timept cprev = bl_cpu_timept_get();
  atomic_sigfence (bl_mo_acq_rel);

  for (bl_uword i = 0; i < 20; ++i) {
    bl_thread_yield(); /* context switching in-between minimization attempt.*/
    bl_timept t = bl_timept_get();
    /* this smoke test may fail spuriosly if the scheduler preempts this thread
    here too many times. The 15 value is conservative enough.*/
    bl_timept c = bl_cpu_timept_get();
    double ratio = (double) bl_cpu_timept_to_nsec (c - cprev);
    ratio       /= (double) bl_timept_to_nsec (t - tprev);
    tprev = t;
    cprev = c;
    ratio = 1. - ratio;
    ok   += (ratio * ratio) < (0.01 * 0.01) ? 1 : 0;
  }
  assert_true (ok >= 15);
}
/*---------------------------------------------------------------------------*/
static void bl_cpu_timept_to_sysclock_diff_test (void **state)
{
  /* Assuming no clock changes and a machine with right date. This is written
  in 2019, epoch was in 1970, hence the 49 years */
  double ns_49y_epoch = 49. * 12. * 30.41 * 24. * 60. * 60. * 1000000000.;
  double ns_80y_epoch = 80. * 12. * 30.41 * 24. * 60. * 60. * 1000000000.;

  bl_timeoft diff = bl_cpu_timept_to_sysclock_diff_ns();
  assert_true (diff > (bl_u64) ns_49y_epoch);
  assert_true (diff < (bl_u64) ns_80y_epoch);
  bl_timeoft diff2 = bl_cpu_timept_to_sysclock_diff_ns();
  /*drifting less than 1ms between calls*/
  assert_int_equal (diff / bl_nsec_in_msec, diff2 / bl_nsec_in_msec);
}
#endif /* #ifdef BL_HAS_CPU_TSTAMP */
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test(bl_timept_to_sysclock_diff_test),
#ifdef BL_HAS_CPU_TSTAMP
  cmocka_unit_test(bl_cpu_timept_vs_timept_test),
  cmocka_unit_test(bl_cpu_timept_to_sysclock_diff_test),
#endif
};
/*---------------------------------------------------------------------------*/
int bl_time_extras_test (void)
{
  return cmocka_run_group_tests (tests, setup, teardown);
}
/*---------------------------------------------------------------------------*/
