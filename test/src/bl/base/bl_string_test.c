#include <bl/cmocka_pre.h>
#include <stdio.h>

#include <bl/base/string.h>
#include <bl/base/default_allocator.h>
#include <bl/base/integer.h>
#include <bl/base/utility.h>

#define EXPECTED "this is the result, a: 1000, b: 1001"
#define FORMAT "this is the result, a: %d, b: %d"
#define COMMA() ,
#define ARGS() 1000 COMMA() 1001

static bl_alloc_tbl alloc;
/*---------------------------------------------------------------------------*/
static int run_vasnprintf_ex(
  char**      str,
  int         str_size,
  int         str_offset,
  char const* format,
  ...
  )
{
  va_list args;
  va_start (args, format);
  int ret = bl_vasprintf_ext(
    str, str_size, str_offset, 0, &alloc, format, args
    );
  va_end (args);
  return ret;
}
/*---------------------------------------------------------------------------*/
static void bl_string_vasnprintf_ex_success_first_alloc (void **state)
{
  char* str = nullptr;
  int ret   = run_vasnprintf_ex (&str, sizeof EXPECTED, 0, FORMAT, ARGS());
  assert_int_equal (ret, bl_lit_len (EXPECTED));
  assert_string_equal (str, EXPECTED);
  bl_dealloc (&alloc, str);
}
/*---------------------------------------------------------------------------*/
static void bl_string_vasnprintf_ex_success_second_alloc (void **state)
{
  char* str = nullptr;
  int ret   = run_vasnprintf_ex(
    &str, bl_lit_len (EXPECTED), 0, FORMAT, ARGS()
    );
  assert_int_equal (ret, bl_lit_len (EXPECTED));
  assert_string_equal (str, EXPECTED);
  bl_dealloc (&alloc, str);
}
/*---------------------------------------------------------------------------*/
static void bl_string_vasnprintf_ex_success_first_alloc_buffer (void **state)
{
  char buff[sizeof EXPECTED];
  char* str = buff;
  int ret   = run_vasnprintf_ex (&str, sizeof buff, 0, FORMAT, ARGS());
  assert_int_equal (ret, bl_lit_len (EXPECTED));
  assert_string_equal (str, EXPECTED);
  assert_ptr_equal (str, buff);
}
/*---------------------------------------------------------------------------*/
static void bl_string_vasnprintf_ex_success_second_alloc_buffer (void **state)
{
  char buff[bl_lit_len (EXPECTED)];
  char* str = buff;
  int ret   = run_vasnprintf_ex (&str, sizeof buff, 0, FORMAT, ARGS());
  assert_int_equal (ret, bl_lit_len (EXPECTED));
  assert_string_equal (str, EXPECTED);
  assert_ptr_not_equal (str, buff);
  bl_dealloc (&alloc, str);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (bl_string_vasnprintf_ex_success_first_alloc),
  cmocka_unit_test (bl_string_vasnprintf_ex_success_second_alloc),
  cmocka_unit_test (bl_string_vasnprintf_ex_success_first_alloc_buffer),
  cmocka_unit_test (bl_string_vasnprintf_ex_success_second_alloc_buffer),
};
/*---------------------------------------------------------------------------*/
int bl_string_tests (void)
{
  alloc = bl_get_default_alloc();
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
