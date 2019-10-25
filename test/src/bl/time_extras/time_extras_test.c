#include <bl/cmocka_pre.h>

#include <bl/time_extras/time_extras.h>

/*---------------------------------------------------------------------------*/
/* This is unfortunately just very rough smoke testing. */
/*---------------------------------------------------------------------------*/
static int setup (void** state)
{
  return bl_time_extras_init().bl == 0;
}
/*---------------------------------------------------------------------------*/
static int teardown (void** state)
{
  bl_time_extras_destroy();
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_tstamp_to_sysclock_diff_test (void **state)
{
  /* Assuming no clock changes and a machine with right date. This is written
  in 2019, epoch was in 1970, hence the 49 years */
  double ns_49y_epoch = 49. * 12. * 30.41 * 24. * 60. * 60. * 1000000000.;
  double ns_80y_epoch = 80. * 12. * 30.41 * 24. * 60. * 60. * 1000000000.;

  toffset diff = bl_tstamp_to_sysclock_diff_ns();
  assert_true (diff > (u64) ns_49y_epoch);
  assert_true (diff < (u64) ns_80y_epoch);
  toffset diff2 = bl_tstamp_to_sysclock_diff_ns();
  /*drifting less than 1ms between calls*/
  assert_int_equal (diff / nsec_in_msec, diff2 / nsec_in_msec);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test(bl_tstamp_to_sysclock_diff_test),
};
/*---------------------------------------------------------------------------*/
int bl_time_extras_test (void)
{
  return cmocka_run_group_tests (tests, setup, teardown);
}
/*---------------------------------------------------------------------------*/
