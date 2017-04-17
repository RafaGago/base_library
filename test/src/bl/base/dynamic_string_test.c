#include <stdio.h>
#include <ctype.h>

#include <bl/cmocka_pre.h>

#include <bl/base/platform.h>
#include <bl/base/utility.h>
#include <bl/base/default_allocator.h>
#include <bl/base/dynamic_string.h>

#define STRING1 "stringone"
#define STRING2 "stringtwo"
#define STRING3 "stringthree"
#define SPACES  " \t\n"

#define STRING1_LOWER STRING1
#define STRING2_LOWER STRING2

#define STRING1_UPPER "STRINGONE"
#define STRING2_UPPER "STRINGTWO"

alloc_tbl alloc;
/*----------------------------------------------------------------------------*/
static void dstrt_init (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  assert_int_equal (dstr_len (&s), 0);
  assert_string_equal (dstr_get (&s), "");
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_set (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, STRING1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_set_null (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, nullptr);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), 0);
  assert_string_equal (dstr_get (&s), "");
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, STRING1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  err = dstr_append (&s, STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2));
  assert_string_equal (dstr_get (&s), STRING1 STRING2);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append_null (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, STRING1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  err = dstr_append (&s, nullptr);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append_to_empty (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_append (&s, STRING1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_prepend (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, STRING1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  err = dstr_prepend (&s, STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING1));
  assert_string_equal (dstr_get (&s), STRING2 STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_prepend_null (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, STRING1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  err = dstr_prepend (&s, nullptr);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_prepend_to_empty (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_prepend (&s, STRING1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_set_va (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, "%s%s%s", STRING1, STRING2, STRING3);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append_va (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, STRING1);
  assert_int_equal (err, 0);
  err = dstr_append_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append_va_to_empty (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_append_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_prepend_va (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set (&s, STRING1);
  assert_int_equal (err, 0);
  err = dstr_prepend_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3 STRING1));
  assert_string_equal (dstr_get (&s), STRING2 STRING3 STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_prepend_va_to_empty (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_prepend_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err, 0);
  err = dstr_erase_head (&s, lit_len (STRING1));
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING2));
  assert_string_equal (dstr_get (&s), STRING2);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_out_of_range (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, 0);
  err = dstr_erase_head (&s, lit_len (STRING1) + 1);
  assert_int_equal (err, bl_range);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err, 0);
  err = dstr_erase_tail (&s, lit_len (STRING2));
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail_out_of_range (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, 0);
  err = dstr_erase_tail (&s, lit_len (STRING1) + 1);
  assert_int_equal (err, bl_range);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_while (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, SPACES STRING1);
  assert_int_equal (err, 0);
  err = dstr_erase_head_while (&s, isspace, 1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_while_negated (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1 SPACES);
  assert_int_equal (err, 0);
  err = dstr_erase_head_while (&s, isspace, 0);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (SPACES));
  assert_string_equal (dstr_get (&s), SPACES);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail_while (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1 SPACES);
  assert_int_equal (err, 0);
  err = dstr_erase_tail_while (&s, isspace, 1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail_while_negated (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, SPACES STRING1);
  assert_int_equal (err, 0);
  err = dstr_erase_tail_while (&s, isspace, 0);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (SPACES));
  assert_string_equal (dstr_get (&s), SPACES);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_tail_while (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, SPACES STRING1 SPACES);
  assert_int_equal (err, 0);
  err = dstr_erase_head_tail_while (&s, isspace, 1);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_tail_while_negated (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1 SPACES STRING3);
  assert_int_equal (err, 0);
  err = dstr_erase_head_tail_while (&s, isspace, 0);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (SPACES));
  assert_string_equal (dstr_get (&s), SPACES);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_all (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1_LOWER);
  assert_int_equal (err, 0);
  err = dstr_apply_all (&s, toupper);
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1_LOWER));
  assert_string_equal (dstr_get (&s), STRING1_UPPER);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1_LOWER STRING2_LOWER);
  assert_int_equal (err, 0);
  err = dstr_apply(
    &s,
    toupper,
    lit_len (STRING1_LOWER),
    lit_len (STRING1_LOWER STRING2_LOWER)
    );
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1_LOWER STRING2_LOWER));
  assert_string_equal (dstr_get (&s), STRING1_LOWER STRING2_UPPER);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_erange_end (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, 0);
  err = dstr_apply (&s, toupper, 0, lit_len (STRING1) + 1);
  assert_int_equal (err, bl_range);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_erange_start (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, 0);
  err = dstr_apply(
    &s, toupper, (lit_len (STRING1) / 2) + 1, lit_len (STRING1) / 2
    );
  assert_int_equal (err, bl_range);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_no_op (void **state)
{
  struct dstr s = dstr_init_rv (&alloc);
  int err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, 0);
  err = dstr_apply(
    &s, toupper, lit_len (STRING1) / 2, lit_len (STRING1) / 2
    );
  assert_int_equal (err, 0);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (dstrt_init),
  cmocka_unit_test (dstrt_set),
  cmocka_unit_test (dstrt_set_null),
  cmocka_unit_test (dstrt_append),
  cmocka_unit_test (dstrt_append_null),
  cmocka_unit_test (dstrt_append_to_empty),
  cmocka_unit_test (dstrt_prepend),
  cmocka_unit_test (dstrt_prepend_null),
  cmocka_unit_test (dstrt_prepend_to_empty),
  cmocka_unit_test (dstrt_set_va),
  cmocka_unit_test (dstrt_append_va),
  cmocka_unit_test (dstrt_append_va_to_empty),
  cmocka_unit_test (dstrt_prepend_va),
  cmocka_unit_test (dstrt_prepend_va_to_empty),
  cmocka_unit_test (dstrt_erase_head),
  cmocka_unit_test (dstrt_erase_head_out_of_range),
  cmocka_unit_test (dstrt_erase_tail),
  cmocka_unit_test (dstrt_erase_tail_out_of_range),
  cmocka_unit_test (dstrt_erase_head_while),
  cmocka_unit_test (dstrt_erase_head_while_negated),
  cmocka_unit_test (dstrt_erase_tail_while),
  cmocka_unit_test (dstrt_erase_tail_while_negated),
  cmocka_unit_test (dstrt_erase_head_tail_while),
  cmocka_unit_test (dstrt_erase_head_tail_while_negated),
  cmocka_unit_test (dstrt_apply_all),
  cmocka_unit_test (dstrt_apply),
  cmocka_unit_test (dstrt_apply_erange_end),
  cmocka_unit_test (dstrt_apply_erange_start),
  cmocka_unit_test (dstrt_apply_no_op),
};
/*----------------------------------------------------------------------------*/
int dstr_tests (void)
{
  alloc = get_default_alloc();
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*----------------------------------------------------------------------------*/
