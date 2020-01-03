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

/*---------------------------------------------------------------------------*/
static void bl_string_asnprintf_success_first_alloc (void **state)
{
  bl_dstrbuf b = bl_dstrbuf_init (nullptr, 0, 0, nullptr);
  /* exact hint */
  bl_err err   = bl_asnprintf (&b, bl_lit_len (EXPECTED), FORMAT, ARGS());
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (b.len, bl_lit_len (EXPECTED));
  assert_int_equal (b.maxlen, bl_lit_len (EXPECTED));
  bl_dealloc (&bl_default_alloc, b.str);
}
/*---------------------------------------------------------------------------*/
static void bl_string_asnprintf_success_second_alloc (void **state)
{
  bl_dstrbuf b = bl_dstrbuf_init (nullptr, 0, 0, nullptr);
  bl_err err   = bl_asnprintf (&b, 1, FORMAT, ARGS()); /* bad hint */
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (b.len, bl_lit_len (EXPECTED));
  assert_int_equal (b.maxlen, bl_lit_len (EXPECTED));
  bl_dealloc (&bl_default_alloc, b.str);
}
/*---------------------------------------------------------------------------*/
static void bl_string_asnprintf_success_first_alloc_buffer (void **state)
{
  char* buff = (char*) bl_alloc (&bl_default_alloc, sizeof EXPECTED);
  assert_true (buff != nullptr);
  bl_dstrbuf b = bl_dstrbuf_init (buff, 0, bl_lit_len (EXPECTED), nullptr);
  bl_err err   = bl_asnprintf (&b, 1, FORMAT, ARGS()); /* bad hint */
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (b.len, bl_lit_len (EXPECTED));
  assert_int_equal (b.maxlen, bl_lit_len (EXPECTED));
  bl_dealloc (&bl_default_alloc, b.str);
}
/*---------------------------------------------------------------------------*/
static void bl_string_asnprintf_success_second_alloc_buffer (void **state)
{
  char* buff = (char*) bl_alloc (&bl_default_alloc, sizeof EXPECTED / 2);
  assert_true (buff != nullptr);
  bl_dstrbuf b = bl_dstrbuf_init (buff, 0, (sizeof EXPECTED / 2) - 1, nullptr);
  bl_err err   = bl_asnprintf (&b, 1, FORMAT, ARGS()); /* bad hint */
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (b.len, bl_lit_len (EXPECTED));
  assert_int_equal (b.maxlen, bl_lit_len (EXPECTED));
  bl_dealloc (&bl_default_alloc, b.str);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (bl_string_asnprintf_success_first_alloc),
  cmocka_unit_test (bl_string_asnprintf_success_second_alloc),
  cmocka_unit_test (bl_string_asnprintf_success_first_alloc_buffer),
  cmocka_unit_test (bl_string_asnprintf_success_second_alloc_buffer),
};
/*---------------------------------------------------------------------------*/
int bl_string_tests (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
