#include <stdio.h>
#include <ctype.h>

#include <bl/cmocka_pre.h>

#include <bl/base/platform.h>
#include <bl/base/utility.h>
#include <bl/base/default_allocator.h>
#include <bl/base/dynamic_string.h>

#define STRING1 "stringone123456789012345678901234567890_"
#define STRING2 "stringtwo12345678901234567890123456789012345678901234567890_"
#define STRING3 "stringthree123456789012345678901234567890_"
#define SPACES  " \t\n"

#define STRING1_LOWER STRING1
#define STRING2_LOWER STRING2

#define STRING1_UPPER "STRINGONE123456789012345678901234567890_"
#define STRING2_UPPER \
  "STRINGTWO12345678901234567890123456789012345678901234567890_"

#define STRINGL  "LONGLONG"
#define STRINGM  "MEDIUM"
#define STRINGS  "SMALL"
#define STRINGXS "*"
#define STRING_240 \
"_240 char filler ___________________________________________________"\
"_____________________________________________________________________"\
"_____________________________________________________________________"\
"_____________________________________________________________________"
alloc_tbl alloc;
/*----------------------------------------------------------------------------*/
static void dstrt_init (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  assert_int_equal (dstr_len (&s), 0);
  assert_string_equal (dstr_get (&s), "");
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_set (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_set_null (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, nullptr);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), 0);
  assert_string_equal (dstr_get (&s), "");
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
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
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
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
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_append (&s, STRING1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_head (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  err = dstr_insert (&s, 0, STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING1));
  assert_string_equal (dstr_get (&s), STRING2 STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_middle (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING3);
  err = dstr_insert (&s, lit_len (STRING1), STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_head_prealloc (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_capacity (&s, lit_len (STRING2 STRING1));
  assert_int_equal (err, bl_ok);
  err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  err = dstr_insert (&s, 0, STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING1));
  assert_string_equal (dstr_get (&s), STRING2 STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_middle_prealloc (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_capacity (&s, lit_len (STRING1 STRING2 STRING3));
  assert_int_equal (err, bl_ok);
  err = dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING3);
  err = dstr_insert (&s, lit_len (STRING1), STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_head_null (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  err = dstr_insert (&s, 0, nullptr);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_head_to_empty (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_insert (&s, 0, STRING1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_middle_to_empty (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_insert (&s, 99, STRING1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_set_va (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, "%s%s%s", STRING1, STRING2, STRING3);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append_va (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_append_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_append_va_to_empty (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_append_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_head_va (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_insert_va (&s, 0, "%s%s", STRING2, STRING3);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3 STRING1));
  assert_string_equal (dstr_get (&s), STRING2 STRING3 STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_middle_va (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err, bl_ok);
  err = dstr_insert_va (&s, lit_len (STRING1), "%s", STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_head_va_prealloc (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_capacity (&s, lit_len (STRING2 STRING3 STRING1));
  assert_int_equal (err, bl_ok);
  err = dstr_set (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_insert_va (&s, 0, "%s%s", STRING2, STRING3);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3 STRING1));
  assert_string_equal (dstr_get (&s), STRING2 STRING3 STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_middle_va_prealloc (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_capacity (&s, lit_len (STRING1 STRING2 STRING3));
  assert_int_equal (err, bl_ok);
  err = dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err, bl_ok);
  err = dstr_insert_va (&s, lit_len (STRING1), "%s", STRING2);
  assert_int_equal (dstr_len (&s), lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING1 STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_head_va_to_empty (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_insert_va (&s, 0, "%s%s", STRING2, STRING3);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_insert_middle_va_to_empty (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_insert_va (&s, 99, "%s%s", STRING2, STRING3);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING2 STRING3));
  assert_string_equal (dstr_get (&s), STRING2 STRING3);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_head (&s, lit_len (STRING1));
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING2));
  assert_string_equal (dstr_get (&s), STRING2);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_out_of_range (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_head (&s, lit_len (STRING1) + 1);
  assert_int_equal (err, bl_range);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_tail (&s, lit_len (STRING2));
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail_out_of_range (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_tail (&s, lit_len (STRING1) + 1);
  assert_int_equal (err, bl_range);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_while (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, SPACES STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_head_while (&s, isspace, 1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_while_negated (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1 SPACES);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_head_while (&s, isspace, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (SPACES));
  assert_string_equal (dstr_get (&s), SPACES);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail_while (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1 SPACES);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_tail_while (&s, isspace, 1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_tail_while_negated (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, SPACES STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_tail_while (&s, isspace, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (SPACES));
  assert_string_equal (dstr_get (&s), SPACES);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_tail_while (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, SPACES STRING1 SPACES);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_head_tail_while (&s, isspace, 1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_erase_head_tail_while_negated (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1 SPACES STRING3);
  assert_int_equal (err, bl_ok);
  err = dstr_erase_head_tail_while (&s, isspace, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (SPACES));
  assert_string_equal (dstr_get (&s), SPACES);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_all (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1_LOWER);
  assert_int_equal (err, bl_ok);
  err = dstr_apply_all (&s, toupper);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1_LOWER));
  assert_string_equal (dstr_get (&s), STRING1_UPPER);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1_LOWER STRING2_LOWER);
  assert_int_equal (err, bl_ok);
  err = dstr_apply(
    &s,
    toupper,
    lit_len (STRING1_LOWER),
    lit_len (STRING1_LOWER STRING2_LOWER)
    );
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1_LOWER STRING2_LOWER));
  assert_string_equal (dstr_get (&s), STRING1_LOWER STRING2_UPPER);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_erange_end (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_apply (&s, toupper, 0, lit_len (STRING1) + 1);
  assert_int_equal (err, bl_range);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_erange_start (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_apply(
    &s, toupper, (lit_len (STRING1) / 2) + 1, lit_len (STRING1) / 2
    );
  assert_int_equal (err, bl_range);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_apply_no_op (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_va (&s, STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_apply(
    &s, toupper, lit_len (STRING1) / 2, lit_len (STRING1) / 2
    );
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRING1));
  assert_string_equal (dstr_get (&s), STRING1);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstr_set_capacity_append_va (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  assert_int_equal (dstr_get_capacity (&s), 0);
  bl_err err = dstr_set_capacity (&s, lit_len (STRING1 STRING2));
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_get_capacity (&s), lit_len (STRING1 STRING2));
  err = dstr_append (&s, "1234");
  assert_int_equal (err, bl_ok);
  char const* ptr = dstr_get (&s);
  dstr_clear (&s);
  err = dstr_append_va (&s, "%s", STRING1);
  assert_int_equal (err, bl_ok);
  err = dstr_append_va (&s, "%s", STRING2);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_get_capacity (&s), dstr_len (&s));
  assert_string_equal (dstr_get (&s), STRING1 STRING2);
  assert_ptr_equal (dstr_get (&s), ptr);
  dstr_destroy(&s);
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_le_matchlen_no_match (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGM STRINGL STRINGM);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, "nomatch", STRINGS, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRINGL STRINGM STRINGL STRINGM));
  assert_string_equal (dstr_get (&s), STRINGL STRINGM STRINGL STRINGM);
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_le_matchlen_all (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGM STRINGL STRINGM STRINGL);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGM, STRINGS, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRINGS STRINGL STRINGS STRINGL));
  assert_string_equal (dstr_get (&s), STRINGS STRINGL STRINGS STRINGL);
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_le_matchlen_all_tail_match (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGM STRINGL STRINGM);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGM, STRINGS, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRINGL STRINGS STRINGL STRINGS));
  assert_string_equal (dstr_get (&s), STRINGL STRINGS STRINGL STRINGS);
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_le_matchlen_one (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGM STRINGL STRINGM);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGM, STRINGS, 1);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRINGL STRINGS STRINGL STRINGM));
  assert_string_equal (dstr_get (&s), STRINGL STRINGS STRINGL STRINGM);
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_gt_matchlen_no_match (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGS STRINGL STRINGS);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, "nomatch", STRINGM, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal (dstr_len (&s), lit_len (STRINGL STRINGS STRINGL STRINGS));
  assert_string_equal (dstr_get (&s), STRINGL STRINGS STRINGL STRINGS);
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_gt_matchlen_all (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGS STRINGL STRINGS STRINGL STRING_240);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGS, STRINGM, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal(
    dstr_len (&s), lit_len (STRINGM STRINGL STRINGM STRINGL STRING_240)
    );
  assert_string_equal(
    dstr_get (&s), STRINGM STRINGL STRINGM STRINGL STRING_240
    );
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_gt_matchlen_all_tail_match (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGS STRINGL STRINGS STRING_240);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGS, STRINGM, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal(
    dstr_len (&s), lit_len (STRINGL STRINGM STRINGL STRINGM STRING_240)
    );
  assert_string_equal(
    dstr_get (&s), STRINGL STRINGM STRINGL STRINGM STRING_240
    );
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_replacelen_gt_matchlen_one (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGS STRINGL STRINGS STRING_240);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGS, STRINGM, 1);
  assert_int_equal (err, bl_ok);
  assert_int_equal(
    dstr_len (&s), lit_len (STRINGL STRINGM STRINGL STRINGS STRING_240)
    );
  assert_string_equal(
    dstr_get (&s), STRINGL STRINGM STRINGL STRINGS STRING_240
    );
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_generic_no_match (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGXS STRINGL STRINGXS STRING_240);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, "nomatch", STRINGM, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal(
    dstr_len (&s), lit_len (STRINGL STRINGXS STRINGL STRINGXS STRING_240)
    );
  assert_string_equal(
    dstr_get (&s), STRINGL STRINGXS STRINGL STRINGXS STRING_240
    );
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_generic_all (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGXS STRINGL STRINGXS STRINGL STRING_240);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGXS, STRINGM, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal(
    dstr_len (&s), lit_len (STRINGM STRINGL STRINGM STRINGL STRING_240)
    );
  assert_string_equal(
    dstr_get (&s), STRINGM STRINGL STRINGM STRINGL STRING_240
    );
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_generic_all_tail_match (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGXS STRINGL STRINGXS STRING_240);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGXS, STRINGM, 0);
  assert_int_equal (err, bl_ok);
  assert_int_equal(
    dstr_len (&s), lit_len (STRINGL STRINGM STRINGL STRINGM STRING_240)
    );
  assert_string_equal(
    dstr_get (&s), STRINGL STRINGM STRINGL STRINGM STRING_240
    );
}
/*----------------------------------------------------------------------------*/
static void dstrt_replace_generic_one (void **state)
{
  dstr s = dstr_init_rv (&alloc);
  bl_err err = dstr_set_lit (&s, STRINGL STRINGXS STRINGL STRINGXS STRING_240);
  assert_int_equal (err, bl_ok);
  err = dstr_replace_lit (&s, STRINGXS, STRINGM, 1);
  assert_int_equal (err, bl_ok);
  assert_int_equal(
    dstr_len (&s), lit_len (STRINGL STRINGM STRINGL STRINGXS STRING_240)
    );
  assert_string_equal(
    dstr_get (&s), STRINGL STRINGM STRINGL STRINGXS STRING_240
    );
}
/*----------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (dstrt_init),
  cmocka_unit_test (dstrt_set),
  cmocka_unit_test (dstrt_set_null),
  cmocka_unit_test (dstrt_append),
  cmocka_unit_test (dstrt_append_null),
  cmocka_unit_test (dstrt_append_to_empty),
  cmocka_unit_test (dstrt_insert_head),
  cmocka_unit_test (dstrt_insert_middle),
  cmocka_unit_test (dstrt_insert_head_prealloc),
  cmocka_unit_test (dstrt_insert_middle_prealloc),
  cmocka_unit_test (dstrt_insert_head_null),
  cmocka_unit_test (dstrt_insert_head_to_empty),
  cmocka_unit_test (dstrt_insert_middle_to_empty),
  cmocka_unit_test (dstrt_set_va),
  cmocka_unit_test (dstrt_append_va),
  cmocka_unit_test (dstrt_append_va_to_empty),
  cmocka_unit_test (dstrt_insert_head_va),
  cmocka_unit_test (dstrt_insert_middle_va),
  cmocka_unit_test (dstrt_insert_head_va_prealloc),
  cmocka_unit_test (dstrt_insert_middle_va_prealloc),
  cmocka_unit_test (dstrt_insert_head_va_to_empty),
  cmocka_unit_test (dstrt_insert_middle_va_to_empty),
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
  cmocka_unit_test (dstr_set_capacity_append_va),
  cmocka_unit_test (dstrt_replace_replacelen_le_matchlen_no_match),
  cmocka_unit_test (dstrt_replace_replacelen_le_matchlen_all),
  cmocka_unit_test (dstrt_replace_replacelen_le_matchlen_all_tail_match),
  cmocka_unit_test (dstrt_replace_replacelen_le_matchlen_one),
  cmocka_unit_test (dstrt_replace_replacelen_gt_matchlen_no_match),
  cmocka_unit_test (dstrt_replace_replacelen_gt_matchlen_all),
  cmocka_unit_test (dstrt_replace_replacelen_gt_matchlen_all_tail_match),
  cmocka_unit_test (dstrt_replace_replacelen_gt_matchlen_one),
  cmocka_unit_test (dstrt_replace_generic_no_match),
  cmocka_unit_test (dstrt_replace_generic_all),
  cmocka_unit_test (dstrt_replace_generic_all_tail_match),
  cmocka_unit_test (dstrt_replace_generic_one),
};
/*----------------------------------------------------------------------------*/
int dstr_tests (void)
{
  alloc = get_default_alloc();
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*----------------------------------------------------------------------------*/
