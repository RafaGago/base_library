#include <inttypes.h> /* llabs */

#include <bl/cmocka_pre.h>

#include <bl/base/thread.h>
#include <bl/base/atomic.h>
#include <bl/time_extras/time_extras.h>

/*---------------------------------------------------------------------------*/
/* This is unfortunately just very rough smoke testing. */
/*---------------------------------------------------------------------------*/
static int setup (void** state)
{
  return bl_time_extras_init().own;
}
/*---------------------------------------------------------------------------*/
static int teardown (void** state)
{
  bl_time_extras_destroy();
  return 0;
}
#include <stdio.h>
/*---------------------------------------------------------------------------*/
#define BL_TEST_YEARS(y) \
  (((bl_u64) (12. * 30.41 * 24. * 60. * 60. * 1000000000.)) * y );
/*---------------------------------------------------------------------------*/
static void bl_timept_to_sysclock_diff_test (void **state)
{
  /* Assuming no clock changes and a machine with right date. This is written
  in 2019, epoch was in 1970, hence the 49 years */
  bl_u64 ns_49y_epoch = BL_TEST_YEARS (49);
  bl_u64 ns_80y_epoch = BL_TEST_YEARS (80);

  bl_timeoft64 diff = bl_timept64_to_sysclock64_diff_ns();
  diff = bl_sysclock64_to_epoch (diff);
  assert_true (diff > ns_49y_epoch);
  assert_true (diff < ns_80y_epoch);
  bl_timeoft64 diff2 = bl_timept64_to_sysclock64_diff_ns();
  diff2 = bl_sysclock64_to_epoch (diff2);
  /*drifting less than 10ms between calls*/
  assert_true (llabs (diff2 - diff) <= (10 * bl_nsec_in_msec));
}
/*---------------------------------------------------------------------------*/
#ifdef BL_HAS_CPU_TIMEPT
/*---------------------------------------------------------------------------*/
static void bl_cpu_timept_vs_timept_test (void **state)
{
  bl_uword ok = 0;

  bl_thread_yield(); /* context switching in-between minimization attempt.*/
  bl_timept tprev = bl_timept_get();
  bl_timept cprev = bl_cpu_timept_get();
  atomic_sigfence (bl_mo_acq_rel);

  for (bl_uword i = 0; i < 20; ++i) {
    bl_thread_usleep (5000);
    bl_timept t = bl_timept_get();
    /* this smoke test may fail spuriosly if the scheduler preempts this thread
    here too many times. The 15 value is conservative enough to don't fail
    often.*/
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
  bl_u64 ns_49y_epoch = BL_TEST_YEARS (49);
  bl_u64 ns_80y_epoch = BL_TEST_YEARS (80);

  bl_timeoft64 diff = bl_cpu_timept_to_sysclock64_diff_ns();
  diff = bl_sysclock64_to_epoch (diff);
  assert_true (diff > ns_49y_epoch);
  assert_true (diff < ns_80y_epoch);
  bl_timeoft64 diff2 = bl_cpu_timept_to_sysclock64_diff_ns();
  diff2 = bl_sysclock64_to_epoch (diff2);
  /*drifting less than 10ms between calls*/
  assert_true (llabs (diff2 - diff) <= (10 * bl_nsec_in_msec));
}
#endif /* #ifdef BL_HAS_CPU_TIMEPT */
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test(bl_timept_to_sysclock_diff_test),
#ifdef BL_HAS_CPU_TIMEPT
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
