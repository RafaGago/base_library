#include <string.h>

#include <bl/cmocka_pre.h>

#include <bl/base/utility.h>
#include <bl/base/integer.h>

#include <bl/tostr/itostr.h>

/*---------------------------------------------------------------------------*/
#define BL_ITOSTR_CHECK_RESULT(err, len, b, lit) \
do { \
  assert_int_equal (err.own, bl_ok); \
  assert_int_equal (len, sizeof lit - 1); \
  assert_string_equal (b, lit); \
} while (0)
/*---------------------------------------------------------------------------*/
static void itostr_dec_u8 (void **state)
{
  size_t len;
  char b[1024];
  bl_u8 v;

  v = 1;
  bl_err err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1");

  v = bl_utype_max (bl_u8);
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "255");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_u16 (void **state)
{
  size_t len;
  char b[1024];
  bl_u16 v;

  v = 1;
  bl_err err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1");

  v = bl_utype_max (bl_u16);
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "65535");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_u32 (void **state)
{
  size_t len;
  char b[1024];
  bl_u32 v;

  v = 1;
  bl_err err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1");

  v = bl_utype_max (bl_u32);
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "4294967295");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_u64 (void **state)
{
  size_t len;
  char b[1024];
  bl_u64 v;

  v = 1;
  bl_err err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1");

  v = bl_utype_max (bl_u64);
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "18446744073709551615");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i8 (void **state)
{
  size_t len;
  char b[1024];
  bl_i8 v;

  v = bl_itype_min (bl_i8);
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-128");

  v = bl_itype_max (bl_i8);
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "127");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i16 (void **state)
{
  size_t len;
  char b[1024];
  bl_i16 v;

  v = bl_itype_min (bl_i16);
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-32768");

  v = bl_itype_max (bl_i16);
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "32767");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i32 (void **state)
{
  size_t len;
  char b[1024];
  bl_i32 v;

  v = bl_itype_min (bl_i32);
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-2147483648");

  v = bl_itype_max (bl_i32);
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "2147483647");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i64 (void **state)
{
  size_t len;
  char b[1024];
  bl_i64 v;

  v = bl_itype_min (bl_i64);
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-9223372036854775808");

  v = bl_itype_max (bl_i64);
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "", v);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "9223372036854775807");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex8 (void **state)
{
  size_t len;
  char b[1024];
  bl_i8 s;
  bl_u8 u;

  s = 0xff;
  u = 0xff;
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "ff");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "FF");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex16 (void **state)
{
  size_t len;
  char b[1024];
  bl_i16 s;
  bl_u16 u;

  s = 0xffff;
  u = 0xffff;
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "ffff");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "FFFF");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex32 (void **state)
{
  size_t len;
  char b[1024];
  bl_i32 s;
  bl_u32 u;

  s = 0xffffffff;
  u = 0xffffffff;
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "ffffffff");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "FFFFFFFF");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex64 (void **state)
{
  size_t len;
  char b[1024];
  bl_i64 s;
  bl_u64 u;

  s = 0xffffffffffffffff;
  u = 0xffffffffffffffff;

  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "ffffffffffffffff");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "FFFFFFFFFFFFFFFF");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "x", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "X", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal8 (void **state)
{
  size_t len;
  char b[1024];
  bl_i8 s;
  bl_u8 u;

  s = 0xff;
  u = 0xff;
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "377");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "377");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal16 (void **state)
{
  size_t len;
  char b[1024];
  bl_i16 s;
  bl_u16 u;

  s = 0xffff;
  u = 0xffff;
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "177777");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "177777");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal32 (void **state)
{
  size_t len;
  char b[1024];
  bl_i32 s;
  bl_u32 u;

  s = 0xffffffff;
  u = 0xffffffff;
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "37777777777");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "37777777777");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal64 (void **state)
{
  size_t len;
  char b[1024];
  bl_i64 s;
  bl_u64 u;

  s = 0xffffffffffffffff;
  u = 0xffffffffffffffff;
  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1777777777777777777777");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1777777777777777777777");

  s = 0x0;
  u = 0x0;
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "o", s);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
  err = bl_itostr_u (b, bl_arr_elems (b), &len, "o", u);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_plus_flag (void **state)
{
  size_t len;
  char b[1024];

  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "+", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+0");
}
/*---------------------------------------------------------------------------*/
static void itostr_space_flag (void **state)
{
  size_t len;
  char b[1024];

  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, " ", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, " 1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, " ", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, " 0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hash_flag (void **state)
{
  size_t len;
  char b[1024];

  bl_err err = bl_itostr_i (b, bl_arr_elems (b), &len, "#x", 0x123457890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0x123457890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "#o", 0xffffffff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "037777777777");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "#", 234);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "234");
}
/*---------------------------------------------------------------------------*/
static void itostr_padding (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "4", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "10", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "11", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, " 1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "23", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "             1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "23", -1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "            -1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+23", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "            +1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "#23x", 0x1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "           0x1234567890");
}
/*---------------------------------------------------------------------------*/
static void itostr_minus_flag (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "-4", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "-12", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890  ");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "-12", -1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-1234567890 ");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+-12", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+1234567890 ");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+-18", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+1234567890       ");
}
/*---------------------------------------------------------------------------*/
static void itostr_zero_flag (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "04", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "011", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "01234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "023", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "00000000000001234567890");

  /* ignored with left justification */
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "-012", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890  ");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+023", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+0000000000001234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "#010x", 0xff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0x000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_numeric_precision (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".4", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".10", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "1234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".12", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "001234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".12", -1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-001234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+.12", 1234567890);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+001234567890");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".0", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+.0", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".8x", 0xff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "000000ff");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "#.8x", 0xff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0x000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_automatic_precision (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".w", (bl_i8) 1);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "001");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".w", (bl_i16) 1);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "00001");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".w", (bl_i32) 1);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0000000001");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".w", (bl_i64) 1);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "00000000000000000001");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wx", (bl_i8) 15);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0f");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wx", (bl_i16) 15);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "000f");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wx", (bl_i32) 15);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0000000f");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wx", (bl_i64) 15);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "000000000000000f");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wo", (bl_i8) 7);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "007");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wo", (bl_i16) 7);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "000007");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wo", (bl_i32) 7);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "000000000007");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".wo", (bl_i64) 7);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "000000000000000000000007");
}
/*---------------------------------------------------------------------------*/
static void itostr_width_precision_combinations (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "20.10", 12345);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "          0000012345");

  /* zero flag ignored */
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "020.10", -12345);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "         -0000012345");

/* zero flag ignored again */
  err = bl_itostr_i (b, bl_arr_elems (b), &len, "#020.10x", 0xff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "        0x00000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_combinations (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+- #015.10x", 0xff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0x00000000ff   ");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+ #015.10x", 0xff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "   0x00000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_repeated_flags (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "     ", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, " 0");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+++++", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "+0");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "-----2", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0 ");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "#####x", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0x0");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "000002", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "00");

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "-#-#-4x", 0);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0x0 ");
}
/*---------------------------------------------------------------------------*/
static void itostr_fmt_string_length_limiting (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "++++++", 0);
  assert_int_equal (err.own, bl_invalid);

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "123", 0);
  assert_int_equal (err.own, bl_invalid);

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".123", 0);
  assert_int_equal (err.own, bl_invalid);

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "xX", 0);
  assert_int_equal (err.own, bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void itostr_invalid_chars (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "+_", 0);
  assert_int_equal (err.own, bl_invalid);

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "12j", 0);
  assert_int_equal (err.own, bl_invalid);

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".12j", 0);
  assert_int_equal (err.own, bl_invalid);

  err = bl_itostr_i (b, bl_arr_elems (b), &len, "n", 0);
  assert_int_equal (err.own, bl_invalid);

  err = bl_itostr_i (b, bl_arr_elems (b), &len, ".0w", 0);
  assert_int_equal (err.own, bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void itostr_no_len_no_fmt (void **state)
{
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, 2, nullptr, nullptr, 1);
  assert_int_equal (err.own, bl_ok);
  assert_string_equal (b, "1");
}
/*---------------------------------------------------------------------------*/
static void itostr_not_enough_space (void **state)
{
  size_t len;
  char b[1024];
  bl_err err;

  err = bl_itostr_i (b, 1, &len, "", 1);
  assert_int_equal (err.own, bl_would_overflow);

  err = bl_itostr_i (b, 3, &len, "x", 0xffff);
  assert_int_equal (err.own, bl_would_overflow);

  err = bl_itostr_i (b, 4, &len, "x", 0xffff);
  assert_int_equal (err.own, bl_would_overflow);

  err = bl_itostr_i (b, 5, &len, "x", 0xffff);
  BL_ITOSTR_CHECK_RESULT (err, len, b, "ffff");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr8 (void **state)
{
  /* not testing formattings, just data types */
  size_t len;
  char b[1024];

  bl_u8 v[] = { 0, 1, 255 };
  bl_err err = bl_itostr_arr_u(
    b, bl_arr_elems (b), &len, "", ", ", v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0, 1, 255");

  bl_i8 w[] = { -128, 0, 127 };
  err = bl_itostr_arr_i(
    b, bl_arr_elems (b), &len, "", ", ", w, bl_arr_elems (w)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-128, 0, 127");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr16 (void **state)
{
  /* not testing formattings, just data types */
  size_t len;
  char b[1024];

  bl_u16 v[] = { 0, 1, 65535 };
  bl_err err = bl_itostr_arr_u(
    b, bl_arr_elems (b), &len, "", ", ", v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0, 1, 65535");

  bl_i16 w[] = { -32768, 0, 32767 };
  err = bl_itostr_arr_i(
    b, bl_arr_elems (b), &len, "", ", ", w, bl_arr_elems (w)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-32768, 0, 32767");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr32 (void **state)
{
  /* not testing formattings, just data types */
  size_t len;
  char b[1024];

  bl_u32 v[] = { 0, 1, 4294967295 };
  bl_err err = bl_itostr_arr_u(
    b, bl_arr_elems (b), &len, "", ", ", v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0, 1, 4294967295");

  bl_i32 w[] = { -2147483648, 0, 2147483647 };
  err = bl_itostr_arr_i(
    b, bl_arr_elems (b), &len, "", ", ", w, bl_arr_elems (w)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "-2147483648, 0, 2147483647");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr64 (void **state)
{
  /* not testing formattings, just data types */
  size_t len;
  char b[1024];

  bl_u64 v[] = { 0, 1, 18446744073709551615ull };
  bl_err err = bl_itostr_arr_u(
    b, bl_arr_elems (b), &len, "", ", ", v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0, 1, 18446744073709551615");

  bl_i64 w[] = { -9223372036854775808ull, 0, 9223372036854775807ll };
  err = bl_itostr_arr_i(
    b, bl_arr_elems (b), &len, "", ", ", w, bl_arr_elems (w)
    );
  BL_ITOSTR_CHECK_RESULT(
    err, len, b, "-9223372036854775808, 0, 9223372036854775807"
    );
}
/*---------------------------------------------------------------------------*/
static void itostr_arr_no_len_no_fmt_no_sep (void **state)
{
  char b[1024];

  bl_u8 v[] = { 0, 1, 2 };
  bl_err err = bl_itostr_arr_u(
    b, bl_arr_elems (b), nullptr, nullptr, nullptr, v, bl_arr_elems (v)
    );
  assert_int_equal (err.own, bl_ok);
  assert_string_equal (b, "0, 1, 2");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr_not_enough_space (void **state)
{
  size_t len;
  bl_err err;
  char b[1024];

  bl_u8 v[] = { 0, 1 };
  err = bl_itostr_arr_i (b, 2, &len, "", ", ", v, 1 );
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0");

  err = bl_itostr_arr_i (b, 1, &len, "", ", ", v, bl_arr_elems (v));
  assert_int_equal (err.own, bl_would_overflow);

  err = bl_itostr_arr_i (b, 5, &len, "", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, len, b, "0, 1");

  err = bl_itostr_arr_i (b, 4, &len, "", ", ", v, bl_arr_elems (v));
  assert_int_equal (err.own, bl_would_overflow);
}
/*---------------------------------------------------------------------------*/
typedef struct itostr_test_alloc {
  bl_alloc_tbl tbl;
  bl_uword     last_alloc_size;
  bl_uword     realloc_count;
  bool         do_fail;
  char         buffer[1024];
}
itostr_test_alloc;
/*---------------------------------------------------------------------------*/
static void* itostr_test_realloc(
  void* mem, size_t size, bl_alloc_tbl const* invoker
  )
{
  itostr_test_alloc* a = (itostr_test_alloc*) invoker;
  assert_true (size <= bl_arr_elems (a->buffer));
  a->last_alloc_size = size;
  ++a->realloc_count;
  return a->do_fail ? nullptr : a->buffer;
}
/*---------------------------------------------------------------------------*/
static void itostr_test_alloc_init (struct itostr_test_alloc* a)
{
  memset (a, 0, sizeof *a);
  a->tbl.realloc = itostr_test_realloc;
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn (void **state)
{
  /* not testing formattings */
  bl_err err;
  itostr_test_alloc a;
  itostr_test_alloc_init (&a);
  char* dst     = nullptr;
  size_t maxlen = 0;
  size_t len    = 0;

  err = bl_itostr_dyn_i (&dst, &maxlen, &len, &a.tbl, "x", 0xfff);
  BL_ITOSTR_CHECK_RESULT (err, len, dst, "fff");
  assert_int_equal (maxlen + 1, a.last_alloc_size);

  memcpy (dst, "is ", sizeof "is ");
  maxlen = len = (sizeof "is " - 1);
  err = bl_itostr_dyn_i (&dst, &maxlen, &len, &a.tbl, "x", 0xbad);
  BL_ITOSTR_CHECK_RESULT (err, len, dst, "is bad");
  assert_int_equal (maxlen + 1, a.last_alloc_size);

  maxlen = bl_arr_elems (a.buffer) - 1;
  len    = 0;
  a.last_alloc_size = 0;
  err = bl_itostr_dyn_i (&dst, &maxlen, &len, &a.tbl, "x", 0xffff);
  BL_ITOSTR_CHECK_RESULT (err, len, dst, "ffff");
  /* no new allocations */
  assert_int_equal (a.last_alloc_size, 0);
  assert_int_equal (maxlen, bl_arr_elems (a.buffer) - 1);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_no_len_no_fmt (void **state)
{
  /* not testing without allocator to avoid allocating */
  bl_err err;
  itostr_test_alloc a;
  itostr_test_alloc_init (&a);
  char* dst     = nullptr;
  size_t maxlen = 0;

  err = bl_itostr_dyn_i (&dst, &maxlen, nullptr, &a.tbl, nullptr, 1);
  BL_ITOSTR_CHECK_RESULT (err, 1, dst, "1");
  assert_int_equal (maxlen + 1, a.last_alloc_size);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_alloc_failures (void **state)
{
  bl_err err;
  itostr_test_alloc a;
  itostr_test_alloc_init (&a);
  char* dst     = nullptr;
  size_t maxlen = 0;
  size_t len    = 0;

  a.do_fail = true;
  err = bl_itostr_dyn_i (&dst, &maxlen, &len, &a.tbl, "", 1);
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (dst, nullptr);
  assert_int_equal (len, 0);
  assert_int_equal (maxlen, 0);

  dst    = a.buffer;
  maxlen = 5;
  len    = 2;
  err = bl_itostr_dyn_i (&dst, &maxlen, &len, &a.tbl, "x", 0xfffff);
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (dst, a.buffer);
  assert_int_equal (len, 2);
  assert_int_equal (maxlen, 5);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_arr (void **state)
{
  /* not testing formattings */
  bl_err err;
  itostr_test_alloc a;
  itostr_test_alloc_init (&a);
  char* dst     = nullptr;
  size_t maxlen = 0;
  size_t len    = 0;

  bl_u8 v[] = { 0, 1, 255 };
  err = bl_itostr_dyn_arr_u(
    &dst, &maxlen, &len, &a.tbl, "x", ", ", v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, dst, "0, 1, ff");
  assert_int_equal (maxlen + 1, a.last_alloc_size);

  memcpy (dst, "values: ", sizeof "values: ");
  maxlen = len = (sizeof "values: " - 1);
  err = bl_itostr_dyn_arr_u(
    &dst, &maxlen, &len, &a.tbl, "x", ", ", v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, dst, "values: 0, 1, ff");
  assert_int_equal (maxlen + 1, a.last_alloc_size);

  maxlen = bl_arr_elems (a.buffer) - 1;
  len    = 0;
  a.last_alloc_size = 0;
  err = bl_itostr_dyn_arr_u(
    &dst, &maxlen, &len, &a.tbl, "x", ", ", v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, len, dst, "0, 1, ff");
  /* no new allocations */
  assert_int_equal (a.last_alloc_size, 0);
  assert_int_equal (maxlen, bl_arr_elems (a.buffer) - 1);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_arr_no_len_no_fmt (void **state)
{
  /* not testing without allocator to avoid allocating */
  bl_err err;
  itostr_test_alloc a;
  itostr_test_alloc_init (&a);
  char* dst     = nullptr;
  size_t maxlen = 0;

  bl_u8 v[] = { 0, 1, 255 };
  err = bl_itostr_dyn_arr_u(
    &dst, &maxlen, nullptr, &a.tbl, nullptr, nullptr, v, bl_arr_elems (v)
    );
  BL_ITOSTR_CHECK_RESULT (err, sizeof "0, 1, 255" - 1, dst, "0, 1, 255");
  assert_int_equal (maxlen + 1, a.last_alloc_size);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_arr_alloc_failures (void **state)
{
  bl_err err;
  itostr_test_alloc a;
  itostr_test_alloc_init (&a);
  char* dst     = nullptr;
  size_t maxlen = 0;
  size_t len    = 0;


  bl_u8 v[] = { 0, 1, 255 };
  a.do_fail = true;
  err = bl_itostr_dyn_arr_u(
    &dst, &maxlen, &len, &a.tbl, "x", ", ", v, bl_arr_elems (v)
    );
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (dst, nullptr);
  assert_int_equal (len, 0);
  assert_int_equal (maxlen, 0);

  dst    = a.buffer;
  maxlen = 5;
  len    = 2;
  err = bl_itostr_dyn_arr_u(
    &dst, &maxlen, &len, &a.tbl, "x", ", ", v, bl_arr_elems (v)
    );
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (dst, a.buffer);
  assert_int_equal (len, 2);
  assert_int_equal (maxlen, 5);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test (itostr_dec_u8),
  cmocka_unit_test (itostr_dec_u16),
  cmocka_unit_test (itostr_dec_u32),
  cmocka_unit_test (itostr_dec_u64),
  cmocka_unit_test (itostr_dec_i8),
  cmocka_unit_test (itostr_dec_i16),
  cmocka_unit_test (itostr_dec_i32),
  cmocka_unit_test (itostr_dec_i64),
  cmocka_unit_test (itostr_hex8),
  cmocka_unit_test (itostr_hex16),
  cmocka_unit_test (itostr_hex32),
  cmocka_unit_test (itostr_hex64),
  cmocka_unit_test (itostr_octal8),
  cmocka_unit_test (itostr_octal16),
  cmocka_unit_test (itostr_octal32),
  cmocka_unit_test (itostr_octal64),
  cmocka_unit_test (itostr_plus_flag),
  cmocka_unit_test (itostr_space_flag),
  cmocka_unit_test (itostr_hash_flag),
  cmocka_unit_test (itostr_padding),
  cmocka_unit_test (itostr_minus_flag),
  cmocka_unit_test (itostr_zero_flag),
  cmocka_unit_test (itostr_numeric_precision),
  cmocka_unit_test (itostr_automatic_precision),
  cmocka_unit_test (itostr_width_precision_combinations),
  cmocka_unit_test (itostr_combinations),
  cmocka_unit_test (itostr_repeated_flags),
  cmocka_unit_test (itostr_fmt_string_length_limiting),
  cmocka_unit_test (itostr_invalid_chars),
  cmocka_unit_test (itostr_no_len_no_fmt),
  cmocka_unit_test (itostr_not_enough_space),
  cmocka_unit_test (itostr_arr8),
  cmocka_unit_test (itostr_arr16),
  cmocka_unit_test (itostr_arr32),
  cmocka_unit_test (itostr_arr64),
  cmocka_unit_test (itostr_arr_no_len_no_fmt_no_sep),
  cmocka_unit_test (itostr_arr_not_enough_space),
  cmocka_unit_test (itostr_dyn),
  cmocka_unit_test (itostr_dyn_no_len_no_fmt),
  cmocka_unit_test (itostr_dyn_alloc_failures),
  cmocka_unit_test (itostr_dyn_arr),
  cmocka_unit_test (itostr_dyn_arr_no_len_no_fmt),
  cmocka_unit_test (itostr_dyn_arr_alloc_failures),
};
/*---------------------------------------------------------------------------*/
int bl_itostr_test (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
