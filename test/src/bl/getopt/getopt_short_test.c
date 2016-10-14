#include <bl/base/utility.h>
#include <bl/base/platform.h>
#include <bl/cmocka_pre.h>
#include <bl/getopt/getopt.h>

/*---------------------------------------------------------------------------*/
static void getopt_short_empty (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe"};
  assert_true (-1 == bl_getopt (&s, arr_elems (argv), argv, "abc"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_simple (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a"};
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "a"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_unrecognized (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a"};
  assert_true ('?' == bl_getopt (&s, arr_elems (argv), argv, "cde"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_only_nonoptions (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "nonoption"};
  assert_true (-1 == bl_getopt (&s, arr_elems (argv), argv, "cde"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_consume_subset (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-f", "-g", "-h"};
  assert_true ('f' == bl_getopt (&s, arr_elems (argv), argv, "fgh"));
  assert_true ('g' == bl_getopt (&s, arr_elems (argv), argv, "fgh"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_multiple_options_diff_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-b", "-a"};
  assert_true ('b' == bl_getopt (&s, arr_elems (argv), argv, "ab"));
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "ab"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_multiple_options_same_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-ba"};
  assert_true ('b' == bl_getopt (&s, arr_elems (argv), argv, "ab"));
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "ab"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_embedded_nonoptions (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a", "nonoption", "-b"};
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "ab"));
  /* The non-option argument "nonoption1" terminates the scan */
  assert_true (-1 == bl_getopt (&s, arr_elems (argv), argv, "ab"));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_argument_same_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-aarg"};
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "a:"));
  assert_string_equal ("arg", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_argument_diff_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a", "arg"};
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "a:"));
  assert_string_equal ("arg", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_required_arg_missing (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a"};
  assert_true ('?' == bl_getopt (&s, arr_elems (argv), argv, "a:"));
  assert_true ('a' == bl_getopt_optopt (&s));
  assert_true (nullptr == bl_getopt_optarg (&s));
  assert_true (3 == bl_getopt_optind (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_optstring_starts_w_colon (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a"};
  assert_true (':' == bl_getopt (&s, arr_elems (argv), argv, ":a:"));
  assert_true ('a' == bl_getopt_optopt (&s));
  assert_true (nullptr == bl_getopt_optarg (&s));
  assert_true (3 == bl_getopt_optind (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_optional_arg_present (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-aarg"};
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "a::"));
  assert_string_equal ("arg", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_optional_arg_missing (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a"};
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "a::"));
  assert_true (nullptr == bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_short_test_optopt (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[] = {"foo.exe", "-a", "-b"};
  assert_true ('a' == bl_getopt (&s, arr_elems (argv), argv, "a"));
  assert_true ('a' == bl_getopt_optopt (&s));

  assert_true ('?' == bl_getopt (&s, arr_elems (argv), argv, "a"));
  assert_true ('b' == bl_getopt_optopt (&s));
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (getopt_short_empty),
  cmocka_unit_test (getopt_short_simple),
  cmocka_unit_test (getopt_short_unrecognized),
  cmocka_unit_test (getopt_short_only_nonoptions),
  cmocka_unit_test (getopt_short_consume_subset),
  cmocka_unit_test (getopt_short_multiple_options_diff_argv),
  cmocka_unit_test (getopt_short_multiple_options_same_argv),
  cmocka_unit_test (getopt_short_embedded_nonoptions),
  cmocka_unit_test (getopt_short_argument_same_argv),
  cmocka_unit_test (getopt_short_argument_diff_argv),
  cmocka_unit_test (getopt_short_required_arg_missing),
  cmocka_unit_test (getopt_short_optstring_starts_w_colon),
  cmocka_unit_test (getopt_short_optional_arg_present),
  cmocka_unit_test (getopt_short_optional_arg_missing),
  cmocka_unit_test (getopt_short_test_optopt),
};
/*---------------------------------------------------------------------------*/
int bl_getopt_short_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

