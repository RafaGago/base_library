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
  char b[1024];
  bl_strbuf buf;
  bl_u8 v;

  v = 1;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1");

  v = bl_utype_max (bl_u8);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "255");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_u16 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_u16 v;

  v = 1;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1");

  v = bl_utype_max (bl_u16);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "65535");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_u32 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_u32 v;

  v = 1;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1");

  v = bl_utype_max (bl_u32);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "4294967295");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_u64 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_u64 v;

  v = 1;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1");

  v = bl_utype_max (bl_u64);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "18446744073709551615");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i8 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i8 v;

  v = bl_itype_min (bl_i8);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-128");

  v = bl_itype_max (bl_i8);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "127");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i16 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i16 v;

  v = bl_itype_min (bl_i16);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-32768");

  v = bl_itype_max (bl_i16);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "32767");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i32 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i32 v;

  v = bl_itype_min (bl_i32);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-2147483648");

  v = bl_itype_max (bl_i32);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "2147483647");
}
/*---------------------------------------------------------------------------*/
static void itostr_dec_i64 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i64 v;

  v = bl_itype_min (bl_i64);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-9223372036854775808");

  v = bl_itype_max (bl_i64);
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "", v);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "9223372036854775807");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex8 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i8 s;
  bl_u8 u;

  s = 0xff;
  u = 0xff;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "ff");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "FF");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex16 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i16 s;
  bl_u16 u;

  s = 0xffff;
  u = 0xffff;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "ffff");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "FFFF");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex32 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i32 s;
  bl_u32 u;

  s = 0xffffffff;
  u = 0xffffffff;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "ffffffff");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "FFFFFFFF");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hex64 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i64 s;
  bl_u64 u;

  s = 0xffffffffffffffff;
  u = 0xffffffffffffffff;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "ffffffffffffffff");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "FFFFFFFFFFFFFFFF");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "x", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "X", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal8 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i8 s;
  bl_u8 u;

  s = 0xff;
  u = 0xff;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "377");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "377");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal16 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i16 s;
  bl_u16 u;

  s = 0xffff;
  u = 0xffff;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "177777");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "177777");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal32 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i32 s;
  bl_u32 u;

  s = 0xffffffff;
  u = 0xffffffff;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "37777777777");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "37777777777");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_octal64 (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_i64 s;
  bl_u64 u;

  s = 0xffffffffffffffff;
  u = 0xffffffffffffffff;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1777777777777777777777");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1777777777777777777777");

  s = 0x0;
  u = 0x0;
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "o", s);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_u (&buf, "o", u);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");
}
/*---------------------------------------------------------------------------*/
static void itostr_plus_flag (void **state)
{
  char b[1024];
  bl_strbuf buf;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "+", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+0");
}
/*---------------------------------------------------------------------------*/
static void itostr_space_flag (void **state)
{
  char b[1024];
  bl_strbuf buf;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, " ", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, " 1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, " ", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, " 0");
}
/*---------------------------------------------------------------------------*/
static void itostr_hash_flag (void **state)
{
  char b[1024];
  bl_strbuf buf;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_i (&buf, "#x", 0x123457890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0x123457890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "#o", 0xffffffff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "037777777777");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "#", 234);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "234");
}
/*---------------------------------------------------------------------------*/
static void itostr_padding (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "4", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "10", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "11", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, " 1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "23", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "             1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "23", -1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "            -1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+23", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "            +1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "#23x", 0x1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "           0x1234567890");
}
/*---------------------------------------------------------------------------*/
static void itostr_minus_flag (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "-4", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "-12", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890  ");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "-12", -1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-1234567890 ");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+-12", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+1234567890 ");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+-18", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+1234567890       ");
}
/*---------------------------------------------------------------------------*/
static void itostr_zero_flag (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "04", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "011", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "01234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "023", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "00000000000001234567890");

  /* ignored with left justification */
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "-012", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890  ");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+023", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+0000000000001234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "#010x", 0xff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0x000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_numeric_precision (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".4", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".10", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "1234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".12", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "001234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".12", -1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-001234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+.12", 1234567890);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+001234567890");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".0", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+.0", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".8x", 0xff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "000000ff");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "#.8x", 0xff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0x000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_automatic_precision (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".w", (bl_i8) 1);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "001");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".w", (bl_i16) 1);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "00001");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".w", (bl_i32) 1);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0000000001");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".w", (bl_i64) 1);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "00000000000000000001");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wx", (bl_i8) 15);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0f");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wx", (bl_i16) 15);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "000f");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wx", (bl_i32) 15);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0000000f");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wx", (bl_i64) 15);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "000000000000000f");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wo", (bl_i8) 7);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "007");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wo", (bl_i16) 7);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "000007");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wo", (bl_i32) 7);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "000000000007");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".wo", (bl_i64) 7);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "000000000000000000000007");
}
/*---------------------------------------------------------------------------*/
static void itostr_width_precision_combinations (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "20.10", 12345);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "          0000012345");

  /* zero flag ignored */
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "020.10", -12345);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "         -0000012345");

/* zero flag ignored again */
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "#020.10x", 0xff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "        0x00000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_combinations (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+- #015.10x", 0xff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0x00000000ff   ");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+ #015.10x", 0xff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "   0x00000000ff");
}
/*---------------------------------------------------------------------------*/
static void itostr_repeated_flags (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "     ", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, " 0");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+++++", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "+0");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "-----2", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0 ");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "#####x", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0x0");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "000002", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "00");

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "-#-#-4x", 0);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0x0 ");
}
/*---------------------------------------------------------------------------*/
static void itostr_fmt_string_length_limiting (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "++++++", 0);;
  assert_int_equal (err.own, bl_invalid);

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "123", 0);;
  assert_int_equal (err.own, bl_invalid);

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".123", 0);;
  assert_int_equal (err.own, bl_invalid);

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "xX", 0);;
  assert_int_equal (err.own, bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void itostr_invalid_chars (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "+_", 0);;
  assert_int_equal (err.own, bl_invalid);

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "12j", 0);;
  assert_int_equal (err.own, bl_invalid);

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".12j", 0);;
  assert_int_equal (err.own, bl_invalid);

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, "n", 0);;
  assert_int_equal (err.own, bl_invalid);

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, ".0w", 0);;
  assert_int_equal (err.own, bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void itostr_no_fmt (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_i (&buf, nullptr, 1);
  assert_int_equal (err.own, bl_ok);
  assert_string_equal (b, "1");
}
/*---------------------------------------------------------------------------*/
static void itostr_not_enough_space (void **state)
{
  char b[1024];
  bl_strbuf buf;
  bl_err err;

  buf = bl_strbuf_init (b, 0, 1);
  err = bl_itostr_i (&buf, "", 11);
  assert_int_equal (err.own, bl_would_overflow);

  buf = bl_strbuf_init (b, 0, 3);
  err = bl_itostr_i (&buf, "x", 0xffff);;
  assert_int_equal (err.own, bl_would_overflow);

  buf = bl_strbuf_init (b, 0, 4);
  err = bl_itostr_i (&buf, "x", 0xfffff);;
  assert_int_equal (err.own, bl_would_overflow);

  buf = bl_strbuf_init (b, 0, 5);
  err = bl_itostr_i (&buf, "x", 0xffff);;
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "ffff");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr8 (void **state)
{
  /* not testing formattings, just data types */
  char b[1024];
  bl_strbuf buf;

  bl_u8 v[] = { 0, 1, 255 };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_arr_u (&buf, "", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1, 255");

  bl_i8 w[] = { -128, 0, 127 };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_arr_i (&buf, "", ", ", w, bl_arr_elems (w));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-128, 0, 127");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr16 (void **state)
{
  /* not testing formattings, just data types */
  char b[1024];
  bl_strbuf buf;

  bl_u16 v[] = { 0, 1, 65535 };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_arr_u (&buf, "", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1, 65535");

  bl_i16 w[] = { -32768, 0, 32767 };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_arr_i (&buf, "", ", ", w, bl_arr_elems (w));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-32768, 0, 32767");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr32 (void **state)
{
  /* not testing formattings, just data types */
  char b[1024];
  bl_strbuf buf;

  bl_u32 v[] = { 0, 1, 4294967295 };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_arr_u (&buf, "", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1, 4294967295");

  bl_i32 w[] = { -2147483648, 0, 2147483647 };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_arr_i (&buf, "", ", ", w, bl_arr_elems (w));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "-2147483648, 0, 2147483647");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr64 (void **state)
{
  /* not testing formattings, just data types */
  char b[1024];
  bl_strbuf buf;

  bl_u64 v[] = { 0, 1, 18446744073709551615ull };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_arr_u (&buf, "", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1, 18446744073709551615");

  bl_i64 w[] = { -9223372036854775808ull, 0, 9223372036854775807ll };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  err = bl_itostr_arr_i (&buf, "", ", ", w, bl_arr_elems (w));
  BL_ITOSTR_CHECK_RESULT(
    err, buf.len, buf.str, "-9223372036854775808, 0, 9223372036854775807"
    );
}
/*---------------------------------------------------------------------------*/
static void itostr_arr_no_fmt_no_sep (void **state)
{
  char b[1024];
  bl_strbuf buf;

  bl_u8 v[] = { 0, 1, 2 };
  buf = bl_strbuf_init (b, 0, bl_arr_elems (b));
  bl_err err = bl_itostr_arr_u (&buf, nullptr, nullptr, v, bl_arr_elems (v));
  assert_int_equal (err.own, bl_ok);
  assert_string_equal (b, "0, 1, 2");
}
/*---------------------------------------------------------------------------*/
static void itostr_arr_not_enough_space (void **state)
{
  bl_err err;
  char b[1024];
  bl_strbuf buf;

  bl_u8 v[] = { 0, 1 };
  buf = bl_strbuf_init (b, 0, 2);
  err = bl_itostr_arr_i (&buf, "", ", ", v, 1 );
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0");

  buf = bl_strbuf_init (b, 0, 1);
  err = bl_itostr_arr_i (&buf, "", ", ", v, bl_arr_elems (v));
  assert_int_equal (err.own, bl_would_overflow);

  buf = bl_strbuf_init (b, 0, 5);
  err = bl_itostr_arr_i (&buf, "", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1");

  buf = bl_strbuf_init (b, 0, 3);
  err = bl_itostr_arr_i (&buf, "", ", ", v, bl_arr_elems (v));
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
  bl_dstrbuf buf;

  itostr_test_alloc_init (&a);
  buf = bl_dstrbuf_init (nullptr, 0, 0, &a.tbl);
  err = bl_itostr_dyn_i (&buf, "x", 0xfff);
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "fff");
  assert_int_equal (buf.maxlen + 1, a.last_alloc_size);

  memcpy (buf.str, "is ", sizeof "is ");
  buf.maxlen = buf.len = (sizeof "is " - 1);
  err = bl_itostr_dyn_i (&buf, "x", 0xbad);
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "is bad");
  assert_int_equal (buf.maxlen + 1, a.last_alloc_size);

  buf.maxlen = bl_arr_elems (a.buffer) - 1;
  buf.len    = 0;
  a.last_alloc_size = 0;
  err = bl_itostr_dyn_i (&buf, "x", 0xffff);
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "ffff");
  /* no new allocations */
  assert_int_equal (a.last_alloc_size, 0);
  assert_int_equal (buf.maxlen, bl_arr_elems (a.buffer) - 1);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_no_fmt (void **state)
{
  /* not testing without allocator to avoid allocating */
  bl_err err;
  itostr_test_alloc a;
  bl_dstrbuf buf;

  itostr_test_alloc_init (&a);
  buf = bl_dstrbuf_init (nullptr, 0, 0, &a.tbl);
  err = bl_itostr_dyn_i (&buf, nullptr, 1);
  BL_ITOSTR_CHECK_RESULT (err, 1, buf.str, "1");
  assert_int_equal (buf.maxlen + 1, a.last_alloc_size);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_alloc_failures (void **state)
{
  bl_err err;
  itostr_test_alloc a;
  bl_dstrbuf buf;

  itostr_test_alloc_init (&a);
  a.do_fail = true;

  buf = bl_dstrbuf_init (nullptr, 0, 0, &a.tbl);
  err = bl_itostr_dyn_i (&buf, "", 1);
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (buf.str, nullptr);
  assert_int_equal (buf.len, 0);
  assert_int_equal (buf.maxlen, 0);

  buf = bl_dstrbuf_init (a.buffer, 2, 5, &a.tbl);
  err = bl_itostr_dyn_i (&buf, "x", 0xfffff);
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (buf.str, a.buffer);
  assert_int_equal (buf.len, 2);
  assert_int_equal (buf.maxlen, 5);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_arr (void **state)
{
  /* not testing formattings */
  bl_err err;
  itostr_test_alloc a;
  bl_dstrbuf buf;

  itostr_test_alloc_init (&a);
  
  bl_u8 v[] = { 0, 1, 255 };
  buf = bl_dstrbuf_init (nullptr, 0, 0, &a.tbl);
  err = bl_itostr_dyn_arr_u (&buf, "x", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1, ff");
  assert_int_equal (buf.maxlen + 1, a.last_alloc_size);

  memcpy (buf.str, "values: ", sizeof "values: ");
  buf.maxlen = buf.len = (sizeof "values: " - 1);
  err = bl_itostr_dyn_arr_u (&buf, "x", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "values: 0, 1, ff");
  assert_int_equal (buf.maxlen + 1, a.last_alloc_size);

  buf.maxlen = bl_arr_elems (a.buffer) - 1;
  buf.len    = 0;
  a.last_alloc_size = 0;
  err = bl_itostr_dyn_arr_u (&buf, "x", ", ", v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1, ff");
  /* no new allocations */
  assert_int_equal (a.last_alloc_size, 0);
  assert_int_equal (buf.maxlen, bl_arr_elems (a.buffer) - 1);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_arr_no_fmt_no_sep (void **state)
{
  bl_err err;
  itostr_test_alloc a;
  bl_dstrbuf buf;

  itostr_test_alloc_init (&a);

  bl_u8 v[] = { 0, 1, 255 };
  buf = bl_dstrbuf_init (nullptr, 0, 0, &a.tbl);
  err = bl_itostr_dyn_arr_u (&buf, nullptr, nullptr, v, bl_arr_elems (v));
  BL_ITOSTR_CHECK_RESULT (err, buf.len, buf.str, "0, 1, 255");
  assert_int_equal (buf.maxlen + 1, a.last_alloc_size);
}
/*---------------------------------------------------------------------------*/
static void itostr_dyn_arr_alloc_failures (void **state)
{
  bl_err err;
  itostr_test_alloc a;
  bl_dstrbuf buf;

  itostr_test_alloc_init (&a);
  a.do_fail = true;

  bl_u8 v[] = { 0, 1, 255 };

  buf = bl_dstrbuf_init (nullptr, 0, 0, &a.tbl);
  err = bl_itostr_dyn_arr_u (&buf, "x", ", ", v, bl_arr_elems (v));
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (buf.str, nullptr);
  assert_int_equal (buf.len, 0);
  assert_int_equal (buf.maxlen, 0);

  buf = bl_dstrbuf_init (a.buffer, 2, 5, &a.tbl);
  err = bl_itostr_dyn_arr_u (&buf, "x", ", ", v, bl_arr_elems (v));
  assert_int_equal (err.own, bl_alloc);
  assert_ptr_equal (buf.str, a.buffer);
  assert_int_equal (buf.len, 2);
  assert_int_equal (buf.maxlen, 5);
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
  cmocka_unit_test (itostr_no_fmt),
  cmocka_unit_test (itostr_not_enough_space),
  cmocka_unit_test (itostr_arr8),
  cmocka_unit_test (itostr_arr16),
  cmocka_unit_test (itostr_arr32),
  cmocka_unit_test (itostr_arr64),
  cmocka_unit_test (itostr_arr_no_fmt_no_sep),
  cmocka_unit_test (itostr_arr_not_enough_space),
  cmocka_unit_test (itostr_dyn),
  cmocka_unit_test (itostr_dyn_no_fmt),
  cmocka_unit_test (itostr_dyn_alloc_failures),
  cmocka_unit_test (itostr_dyn_arr),
  cmocka_unit_test (itostr_dyn_arr_no_fmt_no_sep),
  cmocka_unit_test (itostr_dyn_arr_alloc_failures),
};
/*---------------------------------------------------------------------------*/
int bl_itostr_test (void)
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
