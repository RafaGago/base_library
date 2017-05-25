#include <bl/cmocka_pre.h>

#include <string.h>
#include <bl/base/integer.h>
#include <bl/nonblock/mpmc_bpm.h>
#include <bl/base/utility.h>
#include <bl/base/alignment.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

#define mpmc_bpm_slots 8
/*---------------------------------------------------------------------------*/
typedef struct mpmc_bpm_container {
  u32 seq;
  u32 v;
}
mpmc_bpm_container;
/*---------------------------------------------------------------------------*/
typedef struct mpmc_bpm_context {
  mpmc_bpm         q;
  alloc_tbl        alloc;
  u8               buff[
    ((mpmc_bpm_slots + mpmc_bpm_slots - 2) * sizeof (mpmc_bpm_container)) +
    sizeof (u32) + bl_alignof (mpmc_bpm_container)
    ];
  mpmc_bpm_container* debug;
}
mpmc_bpm_context;
/*---------------------------------------------------------------------------*/
static void* mpmc_bpm_alloc_func (size_t bytes, alloc_tbl const* invoker)
{
  mpmc_bpm_context* c = to_type_containing (invoker, alloc, mpmc_bpm_context);
  assert_true (bytes == sizeof c->buff);
  return c->buff;
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_dealloc_func (void const* mem, alloc_tbl const* invoker)
{
  mpmc_bpm_context* c = to_type_containing (invoker, alloc, mpmc_bpm_context);
  assert_true (mem == (void*) c->buff);
}
/*---------------------------------------------------------------------------*/
static int mpmc_bpm_test_setup (void **state)
{
  static mpmc_bpm_context c;

  *state          = (void*) &c;
  c.alloc.alloc   = mpmc_bpm_alloc_func;
  c.alloc.dealloc = mpmc_bpm_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  assert_true (mpmc_bpm_init(
    &c.q,
    &c.alloc,
    mpmc_bpm_slots,
    mpmc_bpm_slots - 1,
    sizeof (mpmc_bpm_container),
    bl_alignof (mpmc_bpm_container),
    false
    ) == bl_ok);
  c.debug = (mpmc_bpm_container*) c.q.mem;
  return 0;
}
/*---------------------------------------------------------------------------*/
static int mpmc_bpm_test_setup_fair (void **state)
{
  static mpmc_bpm_context c;

  *state          = (void*) &c;
  c.alloc.alloc   = mpmc_bpm_alloc_func;
  c.alloc.dealloc = mpmc_bpm_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  assert_true (mpmc_bpm_init(
    &c.q,
    &c.alloc,
    mpmc_bpm_slots,
    mpmc_bpm_slots - 1,
    sizeof (mpmc_bpm_container),
    bl_alignof (mpmc_bpm_container),
    true
    ) == bl_ok);
  c.debug = (mpmc_bpm_container*) c.q.mem;
  return 0;
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_test_produce(
  mpmc_bpm_context* c,
  mpmc_b_op*        op,
  u8**              mem,
  uword             slots,
  bl_err            err_expected
  )
{
  bl_err err = mpmc_bpm_produce_prepare (&c->q, op, mem, slots);
  assert_int_equal (err, err_expected);
  if (err_expected) {
    return;
  }
  uword u32s = mpmc_bpm_slot_payload (&c->q, slots) / sizeof (u32);
  for (uword i = 0; i < u32s; ++i) {
    ((u32*) *mem)[i] = (u32) ~i;
  }
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_test_verify_produced(
  mpmc_bpm_context* c, u8* mem, uword slots
  )
{
  uword u32s = mpmc_bpm_slot_payload (&c->q, slots) / sizeof (u32);
  for (uword i = 0; i < u32s; ++i) {
    assert_int_equal (((u32*) mem)[i], (u32) ~i);
  }
}
/*---------------------------------------------------------------------------*/
/* No teardown neccesary, a static array is used */
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_init_test (void **state)
{
  mpmc_bpm_context* c = (mpmc_bpm_context*) *state;
  assert_true (mpmc_bpm_slot_count (&c->q) == mpmc_bpm_slots);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_one (void **state)
{
  mpmc_bpm_context* c = (mpmc_bpm_context*) *state;
  u8*       mem;
  u32       slots;
  mpmc_b_op op;
  mpmc_b_op opc;

  mpmc_bpm_test_produce (c, &op, &mem, 1, bl_ok);
  bl_err err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_produce_commit (&c->q, op, mem, 1);
  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (opc, 0);
  assert_int_equal (slots, 1);
  mpmc_bpm_test_verify_produced (c, mem, 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_max (void **state)
{
  mpmc_bpm_context* c = (mpmc_bpm_context*) *state;
  u8*       mem;
  u32       slots;
  mpmc_b_op op;
  mpmc_b_op opc;

  mpmc_bpm_test_produce (c, &op, &mem, mpmc_bpm_slots - 1, bl_ok);
  bl_err err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_produce_commit (&c->q, op, mem, mpmc_bpm_slots - 1);
  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (opc, 0);
  assert_int_equal (slots, mpmc_bpm_slots - 1);
  mpmc_bpm_test_verify_produced (c, mem, mpmc_bpm_slots - 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_full (void **state)
{
  mpmc_bpm_context* c = (mpmc_bpm_context*) *state;
  u8*       mem1;
  u8*       mem2;
  u8*       mem;
  u32       slots;
  mpmc_b_op op1;
  mpmc_b_op op2;
  mpmc_b_op opc;

  mpmc_bpm_test_produce (c, &op1, &mem1, 1, bl_ok);
  mpmc_bpm_test_produce (c, &op2, &mem2, mpmc_bpm_slots - 1, bl_ok);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);
  bl_err err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  mpmc_bpm_produce_commit (&c->q, op1, mem1, 1);
  mpmc_bpm_produce_commit (&c->q, op2, mem2, mpmc_bpm_slots - 1);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, 1);
  mpmc_bpm_test_verify_produced (c, mem, 1);

  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, mpmc_bpm_slots - 1);
  mpmc_bpm_test_verify_produced (c, mem, mpmc_bpm_slots - 1);

  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_wrap (void **state)
{
  mpmc_bpm_context* c = (mpmc_bpm_context*) *state;
  u8*       mem1;
  u8*       mem2;
  u8*       mem;
  u32       slots;
  mpmc_b_op op1;
  mpmc_b_op op2;
  mpmc_b_op opc;

  mpmc_bpm_test_produce (c, &op1, &mem1, 1, bl_ok);
  mpmc_bpm_test_produce (c, &op2, &mem2, mpmc_bpm_slots - 1, bl_ok);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);
  bl_err err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  mpmc_bpm_produce_commit (&c->q, op1, mem1, 1);
  mpmc_bpm_produce_commit (&c->q, op2, mem2, mpmc_bpm_slots - 1);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  err = mpmc_bpm_consume_prepare (&c->q, &op1, &mem1, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, 1);
  mpmc_bpm_test_verify_produced (c, mem1, 1);

  err = mpmc_bpm_consume_prepare (&c->q, &op2, &mem2, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, mpmc_bpm_slots - 1);
  mpmc_bpm_test_verify_produced (c, mem2, mpmc_bpm_slots - 1);

  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);
  /*wrapping*/
  mpmc_bpm_consume_commit (&c->q, op2, mem2, mpmc_bpm_slots - 1);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  mpmc_bpm_consume_commit (&c->q, op1, mem1, 1);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_ok);
  mpmc_bpm_produce_commit (&c->q, opc, mem, 1);

  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, 1);
  mpmc_bpm_test_verify_produced (c, mem, 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_wrap_using_extra_tail_space (void **state)
{
  mpmc_bpm_context* c = (mpmc_bpm_context*) *state;
  u8*       mem;
  u8*       mem1;
  u8*       mem2;
  mpmc_b_op op1;
  mpmc_b_op op2;
  mpmc_b_op opc;
  u32       slots;

  mpmc_bpm_test_produce (c, &op1, &mem1, mpmc_bpm_slots - 1, bl_ok);
  mpmc_bpm_test_produce (c, &op2, &mem2, mpmc_bpm_slots - 1, bl_ok);
  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  bl_err err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  mpmc_bpm_produce_commit (&c->q, op1, mem1, mpmc_bpm_slots - 1);
  mpmc_bpm_produce_commit (&c->q, op2, mem2, mpmc_bpm_slots - 1);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  err = mpmc_bpm_consume_prepare (&c->q, &op1, &mem1, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, mpmc_bpm_slots - 1);
  mpmc_bpm_test_verify_produced (c, mem1, mpmc_bpm_slots - 1);

  err = mpmc_bpm_consume_prepare (&c->q, &op2, &mem2, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, mpmc_bpm_slots - 1);
  mpmc_bpm_test_verify_produced (c, mem2, mpmc_bpm_slots - 1);

  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_empty);
  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);
  /*wrapping*/
  mpmc_bpm_consume_commit (&c->q, op2, mem2, mpmc_bpm_slots - 1);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_would_overflow);

  mpmc_bpm_consume_commit (&c->q, op1, mem1, 1);

  mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_ok);
  mpmc_bpm_produce_commit (&c->q, opc, mem, 1);

  err = mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err, bl_ok);
  assert_int_equal (slots, 1);
  mpmc_bpm_test_verify_produced (c, mem, 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bpm_alloc_dealloc (void **state)
{
  mpmc_bpm_context* c = (mpmc_bpm_context*) *state;
  for (uword i = 0; i < 4; ++i) {
    u8* mem1 = mpmc_bpm_alloc (&c->q, mpmc_bpm_slots - 1);
    assert_non_null (mem1);
    u8* mem2 = mpmc_bpm_alloc (&c->q, mpmc_bpm_slots - 1);
    assert_non_null (mem2);
    assert_true(
      (mem2 - mem1) == ((mpmc_bpm_slots - 1) * mpmc_bpm_slot_size (&c->q))
      );
    assert_null (mpmc_bpm_alloc (&c->q, 1));
    mpmc_bpm_dealloc (&c->q, mem2, mpmc_bpm_slots - 1);
    assert_null (mpmc_bpm_alloc (&c->q, 1));
    mpmc_bpm_dealloc (&c->q, mem1, mpmc_bpm_slots - 1);
  }
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (mpmc_bpm_init_test, mpmc_bpm_test_setup),
  cmocka_unit_test_setup (mpmc_bpm_one, mpmc_bpm_test_setup),
  cmocka_unit_test_setup (mpmc_bpm_max, mpmc_bpm_test_setup),
  cmocka_unit_test_setup (mpmc_bpm_full, mpmc_bpm_test_setup),
  cmocka_unit_test_setup (mpmc_bpm_wrap, mpmc_bpm_test_setup),
  cmocka_unit_test_setup(
    mpmc_bpm_wrap_using_extra_tail_space, mpmc_bpm_test_setup
    ),
  cmocka_unit_test_setup (mpmc_bpm_alloc_dealloc, mpmc_bpm_test_setup),
  cmocka_unit_test_setup (mpmc_bpm_one, mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup (mpmc_bpm_max, mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup (mpmc_bpm_full, mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup (mpmc_bpm_wrap, mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup(
    mpmc_bpm_wrap_using_extra_tail_space, mpmc_bpm_test_setup_fair
    ),
  cmocka_unit_test_setup(
    mpmc_bpm_alloc_dealloc, mpmc_bpm_test_setup_fair
    ),
};
/*---------------------------------------------------------------------------*/
int mpmc_bpm_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
