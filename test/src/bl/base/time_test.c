#ifndef BL_TIMESTAMP_64BIT
#  define BL_TIMESTAMP_64BIT
#endif

#include <string.h>

#include <bl/cmocka_pre.h>
#include <bl/base/platform.h>
#include <bl/base/time.h>
#include <bl/base/thread.h>

#include <bl/base/thread_micro_sleep.c>

/*A quick check that the timestamp calculations aren't very broken by
  sleeping the thread*/

/*---------------------------------------------------------------------------*/
static void timestamp_not_very_broken (void **state)
{
  enum {
    sleep_us = (bl_uword) BL_SCHED_TMIN_US * 20,
    ubound   = (bl_uword) sleep_us * 2,
  };
  bl_tstamp start    = bl_get_tstamp();
  bl_thread_usleep (sleep_us);
  bl_tstamp finish   = bl_get_tstamp();
  bl_tstampdiff diff = bl_tstamp_get_diff (finish, start);
  assert_true (diff > 0);
  bl_toffset us      = bl_tstamp_to_usec ((bl_tstamp) diff);
  assert_true (us >= sleep_us && us <= ubound);
}
/*---------------------------------------------------------------------------*/
static void timestamp_maxes (void **state)
{
  bl_tstamp v;
  bl_toffset max;

  max = bl_sec_to_tstamp_max();
  v   = bl_sec_to_tstamp (max);
  assert_true (v <= bl_tstamp_max_safe_add_sub);

  max = bl_msec_to_tstamp_max();
  v   = bl_msec_to_tstamp (max);
  assert_true (v <= bl_tstamp_max_safe_add_sub);

  max = bl_usec_to_tstamp_max();
  v   = bl_usec_to_tstamp (max);
  assert_true (v <= bl_tstamp_max_safe_add_sub);

  max = bl_nsec_to_tstamp_max();
  v   = bl_nsec_to_tstamp (max);
  assert_true (v <= bl_tstamp_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (timestamp_not_very_broken),
  cmocka_unit_test (timestamp_maxes),
};
/*---------------------------------------------------------------------------*/
int time_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
