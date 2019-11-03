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
static void timestamp32_not_very_broken (void **state)
{
  enum {
    sleep_us = (bl_uword) BL_SCHED_TMIN_US * 20,
    ubound   = (bl_uword) sleep_us * 2,
  };
  bl_tstamp32 start    = bl_tstamp32_get();
  bl_thread_usleep (sleep_us);
  bl_tstamp32 finish   = bl_tstamp32_get();
  bl_tstamp32diff diff = bl_tstamp32_get_diff (finish, start);
  assert_true (diff > 0);
  bl_toffset32 us      = bl_tstamp32_to_usec ((bl_tstamp) diff);
  assert_true (us >= sleep_us && us <= ubound);
}
/*---------------------------------------------------------------------------*/
static void timestamp32_maxes (void **state)
{
  bl_tstamp32 v;
  bl_toffset32 max;

  max = bl_sec_to_tstamp32_max();
  v   = bl_sec_to_tstamp32 (max);
  assert_true (v <= bl_tstamp32_max_safe_add_sub);

  max = bl_msec_to_tstamp32_max();
  v   = bl_msec_to_tstamp32 (max);
  assert_true (v <= bl_tstamp32_max_safe_add_sub);

  max = bl_usec_to_tstamp32_max();
  v   = bl_usec_to_tstamp32 (max);
  assert_true (v <= bl_tstamp32_max_safe_add_sub);

  max = bl_nsec_to_tstamp32_max();
  v   = bl_nsec_to_tstamp32 (max);
  assert_true (v <= bl_tstamp32_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static void timestamp64_not_very_broken (void **state)
{
  enum {
    sleep_us = (bl_uword) BL_SCHED_TMIN_US * 20,
    ubound   = (bl_uword) sleep_us * 2,
  };
  bl_tstamp64 start    = bl_tstamp64_get();
  bl_thread_usleep (sleep_us);
  bl_tstamp64 finish   = bl_tstamp64_get();
  bl_tstamp64diff diff = bl_tstamp64_get_diff (finish, start);
  assert_true (diff > 0);
  bl_toffset64 us      = bl_tstamp64_to_usec ((bl_tstamp) diff);
  assert_true (us >= sleep_us && us <= ubound);
}
/*---------------------------------------------------------------------------*/
static void timestamp64_maxes (void **state)
{
  bl_tstamp64 v;
  bl_toffset64 max;

  max = bl_sec_to_tstamp64_max();
  v   = bl_sec_to_tstamp64 (max);
  assert_true (v <= bl_tstamp64_max_safe_add_sub);

  max = bl_msec_to_tstamp64_max();
  v   = bl_msec_to_tstamp64 (max);
  assert_true (v <= bl_tstamp64_max_safe_add_sub);

  max = bl_usec_to_tstamp64_max();
  v   = bl_usec_to_tstamp64 (max);
  assert_true (v <= bl_tstamp64_max_safe_add_sub);

  max = bl_nsec_to_tstamp64_max();
  v   = bl_nsec_to_tstamp64 (max);
  assert_true (v <= bl_tstamp64_max_safe_add_sub);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (timestamp32_not_very_broken),
  cmocka_unit_test (timestamp32_maxes),
  cmocka_unit_test (timestamp64_not_very_broken),
  cmocka_unit_test (timestamp64_maxes),
};
/*---------------------------------------------------------------------------*/
int time_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
