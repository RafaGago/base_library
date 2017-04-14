#include <bl/base/utility.h>
#include <bl/base/platform.h>
#include <bl/cmocka_pre.h>
#include <bl/getopt/getopt.h>
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_empty (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe"};
  bl_arg_option o[] = {{ 0 },};
  assert_true (
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, "abc", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_simple (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "a", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_unrecognized (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    '?' == bl_getopt_long (&s, arr_elems (argv), argv, "cde", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_only_nonoptions (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "nonoption"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, "cde", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_consume_subset (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-f", "-g", "-h"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'f' == bl_getopt_long (&s, arr_elems (argv), argv, "fgh", o, nullptr)
    );
  assert_true(
    'g' == bl_getopt_long (&s, arr_elems (argv), argv, "fgh", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_multiple_options_diff_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-b", "-a"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, "ab", o, nullptr)
    );
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "ab", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_multiple_options_same_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-ba"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, "ab", o, nullptr)
    );
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "ab", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_embedded_nonoptions (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a", "nonoption", "-b"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "ab", o, nullptr)
    );
  /* The non-option argument "nonoption1" terminates the scan */
  assert_true(
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, "ab", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_argument_same_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-aarg"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "a:", o, nullptr)
    );
  assert_string_equal ("arg", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_argument_diff_argv (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a", "arg"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "a:", o, nullptr)
    );
  assert_string_equal ("arg", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_required_arg_missing (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    '?' == bl_getopt_long (&s, arr_elems (argv), argv, "a:", o, nullptr)
    );
  assert_true ('a' == bl_getopt_optopt (&s));
  assert_true (nullptr == bl_getopt_optarg (&s));
  assert_true (3 == bl_getopt_optind (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_optstring_starts_w_colon (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    ':' == bl_getopt_long (&s, arr_elems (argv), argv, ":a:", o, nullptr)
    );
  assert_true ('a' == bl_getopt_optopt (&s));
  assert_true (nullptr == bl_getopt_optarg (&s));
  assert_true (3 == bl_getopt_optind (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_optional_arg_present (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-aarg"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "a::", o, nullptr)
    );
  assert_string_equal ("arg", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_optional_arg_missing (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "a::", o, nullptr)
    );
  assert_true (nullptr == bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_no_long_arg_test_optopt (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-a", "-b"};
  bl_arg_option o[] = {{ 0 },};
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "a", o, nullptr)
    );
  assert_true ('a' == bl_getopt_optopt (&s));

  assert_true(
    '?' == bl_getopt_long (&s, arr_elems (argv), argv, "a", o, nullptr)
    );
  assert_true ('b' == bl_getopt_optopt (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_mixed_options (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa", "-a"};
  bl_arg_option o[] = {{ "aa", no_argument, nullptr, 'x' }, { 0 }, };
  assert_true(
    'x' == bl_getopt_long (&s, arr_elems (argv), argv, "a", o, nullptr)
    );
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, "a", o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_only_long_options (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa", "--bb"};
  bl_arg_option o[] = {
    { "aa", no_argument, nullptr, 'a' },
    { "bb", no_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_wrong_long_option (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "-bb"};
  bl_arg_option o[] = {
    { "aa", no_argument, nullptr, 'a' },
    { "bb", no_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_wrong_long_abbreviation (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--b", "--a"};
  bl_arg_option o[] = {
    { "aa", no_argument, nullptr, 'a' },
    { "bb", no_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_true(
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_wrong_long_param_abbreviation_eq (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--b=123", "--a=321"};
  bl_arg_option o[] = {
    { "aa", required_argument, nullptr, 'a' },
    { "bb", required_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_string_equal ("123", bl_getopt_optarg (&s));
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_string_equal ("321", bl_getopt_optarg (&s));
  assert_true(
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_wrong_long_param_abbreviation_diff_arg (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--b", "123", "--a", "321"};
  bl_arg_option o[] = {
    { "aa", required_argument, nullptr, 'a' },
    { "bb", required_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_string_equal ("123", bl_getopt_optarg (&s));
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_string_equal ("321", bl_getopt_optarg (&s));
  assert_true(
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_wrong_long_ambiguous_abbrev (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--a"};
  bl_arg_option o[] = {
    { "aa", no_argument, nullptr, 'a' },
    { "ab", no_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    '?' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_using_longindex (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--cc", "--aa", "--bb"};
  bl_arg_option o[] = {
    { "aa", no_argument, nullptr, 'a' },
    { "bb", no_argument, nullptr, 'b' },
    { "cc", no_argument, nullptr, 'c' },
    { 0 },
  };
  int longindex = -1;
  assert_true(
    'c' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, &longindex)
    );
  assert_true (2 == longindex);
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, &longindex)
    );
  assert_true (0 == longindex);
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, &longindex)
    );
  assert_true (1 == longindex);
  assert_true(
    -1 == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, &longindex)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_using_long_flag (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa", "--bb"};
  int flag          = 99;
  bl_arg_option o[] = {
    { "aa", no_argument, &flag, 'a' },
    { "bb", no_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    0 == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_true ('a' == flag);
  assert_true(
    'b' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
}
/*---------------------------------------------------------------------------*/
static void getopt_long_unknown (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--wtf"};
  int flag          = 99;
  bl_arg_option o[] = {
    { "aa", no_argument, &flag, 'a' },
    { "bb", no_argument, nullptr, 'b' },
    { 0 },
  };
  assert_true(
    '?' == bl_getopt_long (&s, arr_elems (argv), argv, "ab", o, nullptr)
    );
  assert_true (2 == bl_getopt_optind (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_missing_required_arg (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa"};
  bl_arg_option o[] = {
    { "aa", required_argument, nullptr, 'a' },
    { 0 },
  };
  assert_true(
    ':' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_true (3 == bl_getopt_optind (&s));
  assert_true (nullptr == bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_missing_required_arg_by_equal (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa="};
  bl_arg_option o[] = {
    { "aa", required_argument, nullptr, 'a' },
    { 0 },
  };
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_string_equal ("", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_optional_arg (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa", "123"};
  bl_arg_option o[] = {
    { "aa", optional_argument, nullptr, 'a' },
    { 0 },
  };
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_true (nullptr == bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_optional_arg_by_equal (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa=123"};
  bl_arg_option o[] = {
    { "aa", optional_argument, nullptr, 'a' },
    { 0 },
  };
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_string_equal ("123", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_missing_optional_arg (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa"};
  bl_arg_option o[] = {
    { "aa", optional_argument, nullptr, 'a' },
    { 0 },
  };
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_true (nullptr == bl_getopt_optarg (&s));
  assert_true (2 == bl_getopt_optind (&s));
}
/*---------------------------------------------------------------------------*/
static void getopt_long_empty_optional_arg_by_equal (void **state)
{
  bl_getopt_state s = BL_GETOPT_STATE_INITIALIZER;
  char* argv[]      = {"foo.exe", "--aa="};
  bl_arg_option o[] = {
    { "aa", optional_argument, nullptr, 'a' },
    { 0 },
  };
  assert_true(
    'a' == bl_getopt_long (&s, arr_elems (argv), argv, nullptr, o, nullptr)
    );
  assert_string_equal ("", bl_getopt_optarg (&s));
}
/*---------------------------------------------------------------------------*/
#if defined (BL_POSIX) && 0
/*Check some weird behaviors*/

#undef no_argument
#undef optional_argument
#undef required_argument

#include <getopt.h>

/*---------------------------------------------------------------------------*/
static void real_getopt_long_missing_required_arg (void **state)
{
  char* argv[]      = {"foo.exe", "--aa"};
  struct option o[] = {
    { "aa", required_argument, nullptr, 'a' },
    { 0 },
  };
  optind  = 1;
  int res = getopt_long (arr_elems (argv), argv, "a", o, nullptr);
  assert_true ((':' == res && 3 == optind)|| ('?' == res && 2 == optind));
  assert_true (nullptr == optarg);
}
/*---------------------------------------------------------------------------*/
static void real_getopt_long_missing_required_arg_by_equal (void **state)
{
  char* argv[]      = {"foo.exe", "--aa="};
  struct option o[] = {
    { "aa", required_argument, nullptr, 'a' },
    { 0 },
  };
  optind = 1;
  assert_true(
    'a' == getopt_long (arr_elems (argv), argv, "a", o, nullptr)
    );
  assert_string_equal ("", optarg);
}
/*---------------------------------------------------------------------------*/
static void real_getopt_long_missing_optional_arg (void **state)
{
  char* argv[]      = {"foo.exe", "--aa"};
  struct option o[] = {
    { "aa", optional_argument, nullptr, 'a' },
    { 0 },
  };
  optind = 1;
  assert_true(
    'a' == getopt_long (arr_elems (argv), argv, "a", o, nullptr)
    );
  assert_true (nullptr == optarg);
  assert_true (2 == optind);
}
/*---------------------------------------------------------------------------*/
static void real_getopt_long_empty_optional_arg_by_equal (void **state)
{
  char* argv[]      = {"foo.exe", "--aa="};
  struct option o[] = {
    { "aa", optional_argument, nullptr, 'a' },
    { 0 },
  };
  optind = 1;
  assert_true(
    'a' == getopt_long (arr_elems (argv), argv, "a", o, nullptr)
    );
  assert_string_equal ("", optarg);
}
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (getopt_long_no_long_arg_empty),
  cmocka_unit_test (getopt_long_no_long_arg_simple),
  cmocka_unit_test (getopt_long_no_long_arg_unrecognized),
  cmocka_unit_test (getopt_long_no_long_arg_only_nonoptions),
  cmocka_unit_test (getopt_long_no_long_arg_consume_subset),
  cmocka_unit_test (getopt_long_no_long_arg_multiple_options_diff_argv),
  cmocka_unit_test (getopt_long_no_long_arg_multiple_options_same_argv),
  cmocka_unit_test (getopt_long_no_long_arg_embedded_nonoptions),
  cmocka_unit_test (getopt_long_no_long_arg_argument_same_argv),
  cmocka_unit_test (getopt_long_no_long_arg_argument_diff_argv),
  cmocka_unit_test (getopt_long_no_long_arg_required_arg_missing),
  cmocka_unit_test (getopt_long_no_long_arg_optstring_starts_w_colon),
  cmocka_unit_test (getopt_long_no_long_arg_optional_arg_present),
  cmocka_unit_test (getopt_long_no_long_arg_optional_arg_missing),
  cmocka_unit_test (getopt_long_no_long_arg_test_optopt),
  cmocka_unit_test (getopt_long_mixed_options),
  cmocka_unit_test (getopt_long_only_long_options),
  cmocka_unit_test (getopt_long_wrong_long_option),
  cmocka_unit_test (getopt_long_wrong_long_abbreviation),
  cmocka_unit_test (getopt_long_wrong_long_param_abbreviation_eq),
  cmocka_unit_test (getopt_long_wrong_long_param_abbreviation_diff_arg),
  cmocka_unit_test (getopt_long_wrong_long_ambiguous_abbrev),
  cmocka_unit_test (getopt_long_using_longindex),
  cmocka_unit_test (getopt_long_using_long_flag),
  cmocka_unit_test (getopt_long_unknown),
  cmocka_unit_test (getopt_long_missing_required_arg),
  cmocka_unit_test (getopt_long_missing_required_arg_by_equal),
  cmocka_unit_test (getopt_long_optional_arg),
  cmocka_unit_test (getopt_long_optional_arg_by_equal),
  cmocka_unit_test (getopt_long_missing_optional_arg),
  cmocka_unit_test (getopt_long_empty_optional_arg_by_equal),
#if defined (BL_POSIX) && 0
  cmocka_unit_test (real_getopt_long_missing_required_arg),
  cmocka_unit_test (real_getopt_long_missing_required_arg_by_equal),
  cmocka_unit_test (real_getopt_long_missing_optional_arg),
  cmocka_unit_test (real_getopt_long_empty_optional_arg_by_equal),
#endif
};
/*---------------------------------------------------------------------------*/
int bl_getopt_long_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/

