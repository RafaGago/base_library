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
bl_alloc_tbl alloc;
/*----------------------------------------------------------------------------*/
static void bl_dstrt_init (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  assert_int_equal (bl_dstr_len (&s), 0);
  assert_string_equal (bl_dstr_get (&s), "");
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_set (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_set_null (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, nullptr);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), 0);
  assert_string_equal (bl_dstr_get (&s), "");
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_append (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  err = bl_dstr_append (&s, STRING2);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_append_null (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  err = bl_dstr_append (&s, nullptr);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_append_to_empty (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_append (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_head (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  err = bl_dstr_insert (&s, 0, STRING2);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2 STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING2 STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_middle (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING3);
  err = bl_dstr_insert (&s, bl_lit_len (STRING1), STRING2);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_head_prealloc (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_capacity (&s, bl_lit_len (STRING2 STRING1));
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  err = bl_dstr_insert (&s, 0, STRING2);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2 STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING2 STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_middle_prealloc (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_capacity (&s, bl_lit_len (STRING1 STRING2 STRING3));
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING3);
  err = bl_dstr_insert (&s, bl_lit_len (STRING1), STRING2);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_head_null (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  err = bl_dstr_insert (&s, 0, nullptr);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_head_to_empty (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_insert (&s, 0, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_middle_to_empty (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_insert (&s, 99, STRING1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_set_va (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, "%s%s%s", STRING1, STRING2, STRING3);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_append_va (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_append_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_append_va_to_empty (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_append_va (&s, "%s%s", STRING2, STRING3);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_head_va (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_insert_va (&s, 0, "%s%s", STRING2, STRING3);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2 STRING3 STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING2 STRING3 STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_middle_va (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_insert_va (&s, bl_lit_len (STRING1), "%s", STRING2);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_head_va_prealloc (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_capacity (&s, bl_lit_len (STRING2 STRING3 STRING1));
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_set (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_insert_va (&s, 0, "%s%s", STRING2, STRING3);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2 STRING3 STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING2 STRING3 STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_middle_va_prealloc (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_capacity (&s, bl_lit_len (STRING1 STRING2 STRING3));
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_set (&s, STRING1 STRING3);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_insert_va (&s, bl_lit_len (STRING1), "%s", STRING2);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_head_va_to_empty (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_insert_va (&s, 0, "%s%s", STRING2, STRING3);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_insert_middle_va_to_empty (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_insert_va (&s, 99, "%s%s", STRING2, STRING3);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2 STRING3));
  assert_string_equal (bl_dstr_get (&s), STRING2 STRING3);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_head (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_head (&s, bl_lit_len (STRING1));
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING2));
  assert_string_equal (bl_dstr_get (&s), STRING2);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_head_out_of_range (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_head (&s, bl_lit_len (STRING1) + 1);
  assert_int_equal (err.own, bl_range);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_tail (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_tail (&s, bl_lit_len (STRING2));
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_tail_out_of_range (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_tail (&s, bl_lit_len (STRING1) + 1);
  assert_int_equal (err.own, bl_range);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase (&s, bl_lit_len (STRING1), bl_lit_len (STRING2));
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_out_of_range (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1 STRING2);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase (&s, bl_lit_len (STRING1), bl_lit_len (STRING2) + 1);
  assert_int_equal (err.own, bl_range);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1 STRING2));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_head_while (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, SPACES STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_head_while (&s, isspace, 1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_head_while_negated (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1 SPACES);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_head_while (&s, isspace, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (SPACES));
  assert_string_equal (bl_dstr_get (&s), SPACES);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_tail_while (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1 SPACES);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_tail_while (&s, isspace, 1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_tail_while_negated (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, SPACES STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_tail_while (&s, isspace, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (SPACES));
  assert_string_equal (bl_dstr_get (&s), SPACES);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_head_tail_while (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, SPACES STRING1 SPACES);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_head_tail_while (&s, isspace, 1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_erase_head_tail_while_negated (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1 SPACES STRING3);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_erase_head_tail_while (&s, isspace, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (SPACES));
  assert_string_equal (bl_dstr_get (&s), SPACES);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_apply_all (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1_LOWER);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_apply_all (&s, toupper);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1_LOWER));
  assert_string_equal (bl_dstr_get (&s), STRING1_UPPER);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_apply (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1_LOWER STRING2_LOWER);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_apply(
    &s,
    toupper,
    bl_lit_len (STRING1_LOWER),
    bl_lit_len (STRING1_LOWER STRING2_LOWER)
    );
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1_LOWER STRING2_LOWER));
  assert_string_equal (bl_dstr_get (&s), STRING1_LOWER STRING2_UPPER);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_apply_erange_end (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_apply (&s, toupper, 0, bl_lit_len (STRING1) + 1);
  assert_int_equal (err.own, bl_range);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_apply_erange_start (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_apply(
    &s, toupper, (bl_lit_len (STRING1) / 2) + 1, bl_lit_len (STRING1) / 2
    );
  assert_int_equal (err.own, bl_range);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_apply_no_op (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_va (&s, STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_apply(
    &s, toupper, bl_lit_len (STRING1) / 2, bl_lit_len (STRING1) / 2
    );
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_len (&s), bl_lit_len (STRING1));
  assert_string_equal (bl_dstr_get (&s), STRING1);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_set_capacity_append_va (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  assert_int_equal (bl_dstr_get_capacity (&s), 0);
  bl_err err = bl_dstr_set_capacity (&s, bl_lit_len (STRING1 STRING2));
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_get_capacity (&s), bl_lit_len (STRING1 STRING2));
  err = bl_dstr_append (&s, "1234");
  assert_int_equal (err.own, bl_ok);
  char const* ptr = bl_dstr_get (&s);
  bl_dstr_clear (&s);
  err = bl_dstr_append_va (&s, "%s", STRING1);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_append_va (&s, "%s", STRING2);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal (bl_dstr_get_capacity (&s), bl_dstr_len (&s));
  assert_string_equal (bl_dstr_get (&s), STRING1 STRING2);
  assert_ptr_equal (bl_dstr_get (&s), ptr);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_find (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGS STRINGM STRINGS STRINGS);
  assert_int_equal (err.own, bl_ok);

  bl_uword f = 0;
  f = bl_dstr_find_lit (&s, STRINGS, f);
  assert_int_equal (f, 0);
  f += bl_lit_len (STRINGS);

  f = bl_dstr_find_lit (&s, STRINGS, f);
  assert_int_equal (f, bl_lit_len (STRINGS STRINGM));
  f += bl_lit_len (STRINGS);

  f = bl_dstr_find_lit (&s, STRINGS, f);
  assert_int_equal (f, bl_lit_len (STRINGS STRINGM STRINGS));
  f += bl_lit_len (STRINGS);

  f = bl_dstr_find_lit (&s, STRINGS, f);
  assert_int_equal (f, bl_lit_len (STRINGS STRINGM STRINGS STRINGS));
  assert_int_equal (f, bl_dstr_len (&s));
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_le_matchlen_no_match (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGL STRINGM STRINGL STRINGM);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, "nomatch", STRINGS, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGM STRINGL STRINGM)
    );
  assert_string_equal (bl_dstr_get (&s), STRINGL STRINGM STRINGL STRINGM);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_le_matchlen_all (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGM STRINGL STRINGM STRINGL);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGM, STRINGS, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGS STRINGL STRINGS STRINGL)
    );
  assert_string_equal (bl_dstr_get (&s), STRINGS STRINGL STRINGS STRINGL);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_le_matchlen_all_offset_1 (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGM STRINGL STRINGM STRINGL);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGM, STRINGS, 1, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGM STRINGL STRINGS STRINGL)
    );
  assert_string_equal (bl_dstr_get (&s), STRINGM STRINGL STRINGS STRINGL);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void
  bl_dstrt_replace_replacelen_le_matchlen_all_tail_match (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGL STRINGM STRINGL STRINGM);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGM, STRINGS, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGS STRINGL STRINGS)
    );
  assert_string_equal (bl_dstr_get (&s), STRINGL STRINGS STRINGL STRINGS);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_le_matchlen_one (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGL STRINGM STRINGL STRINGM);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGM, STRINGS, 0, 1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGS STRINGL STRINGM)
    );
  assert_string_equal (bl_dstr_get (&s), STRINGL STRINGS STRINGL STRINGM);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_gt_matchlen_no_match (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGL STRINGS STRINGL STRINGS);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, "nomatch", STRINGM, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGS STRINGL STRINGS)
    );
  assert_string_equal (bl_dstr_get (&s), STRINGL STRINGS STRINGL STRINGS);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_gt_matchlen_all (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGS STRINGL STRINGS STRINGL STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGS, STRINGM, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGM STRINGL STRINGM STRINGL STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGM STRINGL STRINGM STRINGL STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_gt_matchlen_all_offset_1 (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGS STRINGL STRINGS STRINGL STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGS, STRINGM, 1, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGS STRINGL STRINGM STRINGL STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGS STRINGL STRINGM STRINGL STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void
  bl_dstrt_replace_replacelen_gt_matchlen_all_tail_match (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGL STRINGS STRINGL STRINGS STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGS, STRINGM, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGM STRINGL STRINGM STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGL STRINGM STRINGL STRINGM STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_replacelen_gt_matchlen_one (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err = bl_dstr_set_lit (&s, STRINGL STRINGS STRINGL STRINGS STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGS, STRINGM, 0, 1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGM STRINGL STRINGS STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGL STRINGM STRINGL STRINGS STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_generic_no_match (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err =
    bl_dstr_set_lit (&s, STRINGL STRINGXS STRINGL STRINGXS STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, "nomatch", STRINGM, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGXS STRINGL STRINGXS STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGL STRINGXS STRINGL STRINGXS STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_generic_all (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err =
    bl_dstr_set_lit (&s, STRINGXS STRINGL STRINGXS STRINGL STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGXS, STRINGM, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGM STRINGL STRINGM STRINGL STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGM STRINGL STRINGM STRINGL STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_generic_all_offset_1 (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err =
    bl_dstr_set_lit (&s, STRINGXS STRINGL STRINGXS STRINGL STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGXS, STRINGM, 1, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGXS STRINGL STRINGM STRINGL STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGXS STRINGL STRINGM STRINGL STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_generic_all_tail_match (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err =
    bl_dstr_set_lit (&s, STRINGL STRINGXS STRINGL STRINGXS STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGXS, STRINGM, 0, 0);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGM STRINGL STRINGM STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGL STRINGM STRINGL STRINGM STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_replace_generic_one (void **state)
{
  bl_dstr s = bl_dstr_init_rv (&alloc);
  bl_err err =
    bl_dstr_set_lit (&s, STRINGL STRINGXS STRINGL STRINGXS STRING_240);
  assert_int_equal (err.own, bl_ok);
  err = bl_dstr_replace_lit (&s, STRINGXS, STRINGM, 0, 1);
  assert_int_equal (err.own, bl_ok);
  assert_int_equal(
    bl_dstr_len (&s), bl_lit_len (STRINGL STRINGM STRINGL STRINGXS STRING_240)
    );
  assert_string_equal(
    bl_dstr_get (&s), STRINGL STRINGM STRINGL STRINGXS STRING_240
    );
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static void bl_dstrt_append_file (void **state)
{
  static const char* fname = "testfile.txt";
  bl_dstr s;
  assert_int_equal (0, bl_dstr_init_str (&s, STRINGL, &alloc).own);
  FILE* f = fopen (fname, "w");
  assert_non_null (f);
  assert_int_equal(
    bl_lit_len (STRING_240),
    fwrite (STRING_240, 1, bl_lit_len (STRING_240), f)
    );
  fclose(f);
  f = fopen (fname, "r");
  assert_non_null (f);
  bl_err err = bl_dstr_append_file (&s, f, 0);
  fclose (f);
  remove(fname);
  assert_int_equal (err.own, bl_ok);
  assert_string_equal (bl_dstr_get (&s), STRINGL STRING_240);
  bl_dstr_destroy (&s);
}
/*----------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (bl_dstrt_init),
  cmocka_unit_test (bl_dstrt_set),
  cmocka_unit_test (bl_dstrt_set_null),
  cmocka_unit_test (bl_dstrt_append),
  cmocka_unit_test (bl_dstrt_append_null),
  cmocka_unit_test (bl_dstrt_append_to_empty),
  cmocka_unit_test (bl_dstrt_insert_head),
  cmocka_unit_test (bl_dstrt_insert_middle),
  cmocka_unit_test (bl_dstrt_insert_head_prealloc),
  cmocka_unit_test (bl_dstrt_insert_middle_prealloc),
  cmocka_unit_test (bl_dstrt_insert_head_null),
  cmocka_unit_test (bl_dstrt_insert_head_to_empty),
  cmocka_unit_test (bl_dstrt_insert_middle_to_empty),
  cmocka_unit_test (bl_dstrt_set_va),
  cmocka_unit_test (bl_dstrt_append_va),
  cmocka_unit_test (bl_dstrt_append_va_to_empty),
  cmocka_unit_test (bl_dstrt_insert_head_va),
  cmocka_unit_test (bl_dstrt_insert_middle_va),
  cmocka_unit_test (bl_dstrt_insert_head_va_prealloc),
  cmocka_unit_test (bl_dstrt_insert_middle_va_prealloc),
  cmocka_unit_test (bl_dstrt_insert_head_va_to_empty),
  cmocka_unit_test (bl_dstrt_insert_middle_va_to_empty),
  cmocka_unit_test (bl_dstrt_erase_head),
  cmocka_unit_test (bl_dstrt_erase_head_out_of_range),
  cmocka_unit_test (bl_dstrt_erase_tail),
  cmocka_unit_test (bl_dstrt_erase_tail_out_of_range),
  cmocka_unit_test (bl_dstrt_erase),
  cmocka_unit_test (bl_dstrt_erase_out_of_range),
  cmocka_unit_test (bl_dstrt_erase_head_while),
  cmocka_unit_test (bl_dstrt_erase_head_while_negated),
  cmocka_unit_test (bl_dstrt_erase_tail_while),
  cmocka_unit_test (bl_dstrt_erase_tail_while_negated),
  cmocka_unit_test (bl_dstrt_erase_head_tail_while),
  cmocka_unit_test (bl_dstrt_erase_head_tail_while_negated),
  cmocka_unit_test (bl_dstrt_apply_all),
  cmocka_unit_test (bl_dstrt_apply),
  cmocka_unit_test (bl_dstrt_apply_erange_end),
  cmocka_unit_test (bl_dstrt_apply_erange_start),
  cmocka_unit_test (bl_dstrt_apply_no_op),
  cmocka_unit_test (bl_dstrt_set_capacity_append_va),
  cmocka_unit_test (bl_dstrt_find),
  cmocka_unit_test (bl_dstrt_replace_replacelen_le_matchlen_no_match),
  cmocka_unit_test (bl_dstrt_replace_replacelen_le_matchlen_all),
  cmocka_unit_test (bl_dstrt_replace_replacelen_le_matchlen_all_offset_1),
  cmocka_unit_test (bl_dstrt_replace_replacelen_le_matchlen_all_tail_match),
  cmocka_unit_test (bl_dstrt_replace_replacelen_le_matchlen_one),
  cmocka_unit_test (bl_dstrt_replace_replacelen_gt_matchlen_no_match),
  cmocka_unit_test (bl_dstrt_replace_replacelen_gt_matchlen_all),
  cmocka_unit_test (bl_dstrt_replace_replacelen_gt_matchlen_all_offset_1),
  cmocka_unit_test (bl_dstrt_replace_replacelen_gt_matchlen_all_tail_match),
  cmocka_unit_test (bl_dstrt_replace_replacelen_gt_matchlen_one),
  cmocka_unit_test (bl_dstrt_replace_generic_no_match),
  cmocka_unit_test (bl_dstrt_replace_generic_all),
  cmocka_unit_test (bl_dstrt_replace_generic_all_offset_1),
  cmocka_unit_test (bl_dstrt_replace_generic_all_tail_match),
  cmocka_unit_test (bl_dstrt_replace_generic_one),
  cmocka_unit_test (bl_dstrt_append_file),
};
/*----------------------------------------------------------------------------*/
int bl_dstr_tests (void)
{
  alloc = bl_get_default_alloc();
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*----------------------------------------------------------------------------*/
