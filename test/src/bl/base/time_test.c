#include <string.h>

#include <bl/cmocka_pre.h>
#include <bl/base/platform.h>
#include <bl/base/time.h>
#include <bl/base/thread.h>

#include <bl/base/thread_micro_sleep.c>

/*A quick check that the timepoint calculations aren't very broken by
  sleeping the thread*/

/*---------------------------------------------------------------------------*/
static void timept32_not_very_broken (void **state)
{
  enum {
    sleep_us = 70000,
    ubound   = 500000,
  };
  bl_timept32 start    = bl_timept32_get();
  bl_thread_usleep (sleep_us);
  bl_timept32 finish   = bl_timept32_get();
  bl_timept32diff diff = bl_timept32_get_diff (finish, start);
  assert_true (diff > 0);
  bl_timeoft32 us      = bl_timept32_to_usec ((bl_timept) diff);
  assert_true (us >= sleep_us);
  /* this test could fail on some situations depending on the system load*/
  assert_true (us <= ubound);
}
/*---------------------------------------------------------------------------*/
static void timept32_maxes (void **state)
{
  bl_timept32 v;
  bl_timeoft32 max;

  max = bl_sec_to_timept32_max();
  v   = bl_sec_to_timept32 (max);
  assert_true (v <= bl_timept32_max_safe_add_sub);

  max = bl_msec_to_timept32_max();
  v   = bl_msec_to_timept32 (max);
  assert_true (v <= bl_timept32_max_safe_add_sub);

  max = bl_usec_to_timept32_max();
  v   = bl_usec_to_timept32 (max);
  assert_true (v <= bl_timept32_max_safe_add_sub);

  max = bl_nsec_to_timept32_max();
  v   = bl_nsec_to_timept32 (max);
  assert_true (v <= bl_timept32_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static void timept64_not_very_broken (void **state)
{
  enum {
    sleep_us = 70000,
    ubound   = 500000,
  };
  bl_timept64 start    = bl_timept64_get();
  bl_thread_usleep (sleep_us);
  bl_timept64 finish   = bl_timept64_get();
  bl_timept64diff diff = bl_timept64_get_diff (finish, start);
  assert_true (diff > 0);
  bl_timeoft64 us      = bl_timept64_to_usec ((bl_timept) diff);
  assert_true (us >= sleep_us);
  /* this test could fail on some situations depending on the system load*/
  assert_true (us <= ubound);
}
/*---------------------------------------------------------------------------*/
static void timept64_maxes (void **state)
{
  bl_timept64 v;
  bl_timeoft64 max;

  max = bl_sec_to_timept64_max();
  v   = bl_sec_to_timept64 (max);
  assert_true (v <= bl_timept64_max_safe_add_sub);

  max = bl_msec_to_timept64_max();
  v   = bl_msec_to_timept64 (max);
  assert_true (v <= bl_timept64_max_safe_add_sub);

  max = bl_usec_to_timept64_max();
  v   = bl_usec_to_timept64 (max);
  assert_true (v <= bl_timept64_max_safe_add_sub);

  max = bl_nsec_to_timept64_max();
  v   = bl_nsec_to_timept64 (max);
  assert_true (v <= bl_timept64_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (timept32_not_very_broken),
  cmocka_unit_test (timept32_maxes),
  cmocka_unit_test (timept64_not_very_broken),
  cmocka_unit_test (timept64_maxes),
};
/*---------------------------------------------------------------------------*/
int time_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
