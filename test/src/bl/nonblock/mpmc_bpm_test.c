#include <bl/cmocka_pre.h>

#include <string.h>
#include <bl/base/integer.h>
#include <bl/nonblock/mpmc_bpm.h>
#include <bl/base/utility.h>
#include <bl/base/alignment.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

#define bl_mpmc_bpm_slots 8
/*---------------------------------------------------------------------------*/
typedef struct bl_mpmc_bpm_container {
  bl_u32 seq;
  bl_u32 v;
}
bl_mpmc_bpm_container;
/*---------------------------------------------------------------------------*/
typedef struct bl_mpmc_bpm_context {
  bl_mpmc_bpm         q;
  bl_alloc_tbl        alloc;
  bl_u8               buff[
    ((bl_mpmc_bpm_slots + bl_mpmc_bpm_slots - 2)
      * sizeof (bl_mpmc_bpm_container))
      + sizeof (bl_u32) + bl_alignof (bl_mpmc_bpm_container)
    ];
  bl_mpmc_bpm_container* debug;
}
bl_mpmc_bpm_context;
/*---------------------------------------------------------------------------*/
static void* bl_mpmc_bpm_alloc_func (size_t bytes, bl_alloc_tbl const* invoker)
{
  bl_mpmc_bpm_context* c =
    bl_to_type_containing (invoker, alloc, bl_mpmc_bpm_context);
  assert_true (bytes == sizeof c->buff);
  return c->buff;
}
/*---------------------------------------------------------------------------*/
static void
  bl_mpmc_bpm_dealloc_func (void const* mem, bl_alloc_tbl const* invoker)
{
  bl_mpmc_bpm_context* c =
    bl_to_type_containing (invoker, alloc, bl_mpmc_bpm_context);
  assert_true (mem == (void*) c->buff);
}
/*---------------------------------------------------------------------------*/
static int bl_mpmc_bpm_test_setup (void **state)
{
  static bl_mpmc_bpm_context c;

  *state          = (void*) &c;
  c.alloc.alloc   = bl_mpmc_bpm_alloc_func;
  c.alloc.dealloc = bl_mpmc_bpm_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  assert_true (bl_mpmc_bpm_init(
    &c.q,
    &c.alloc,
    bl_mpmc_bpm_slots,
    bl_mpmc_bpm_slots - 1,
    sizeof (bl_mpmc_bpm_container),
    bl_alignof (bl_mpmc_bpm_container),
    false
    ).bl == bl_ok);
  c.debug = (bl_mpmc_bpm_container*) c.q.mem;
  return 0;
}
/*---------------------------------------------------------------------------*/
static int bl_mpmc_bpm_test_setup_fair (void **state)
{
  static bl_mpmc_bpm_context c;

  *state          = (void*) &c;
  c.alloc.alloc   = bl_mpmc_bpm_alloc_func;
  c.alloc.dealloc = bl_mpmc_bpm_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  assert_true (bl_mpmc_bpm_init(
    &c.q,
    &c.alloc,
    bl_mpmc_bpm_slots,
    bl_mpmc_bpm_slots - 1,
    sizeof (bl_mpmc_bpm_container),
    bl_alignof (bl_mpmc_bpm_container),
    true
    ).bl == bl_ok);
  c.debug = (bl_mpmc_bpm_container*) c.q.mem;
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_test_produce(
  bl_mpmc_bpm_context* c,
  bl_mpmc_b_op*        op,
  bl_u8**              mem,
  bl_uword             slots,
  bl_err            err_expected
  )
{
  bl_err err = bl_mpmc_bpm_produce_prepare (&c->q, op, mem, slots);
  assert_int_equal (err.bl, err_expected.bl);
  if (err_expected.bl) {
    return;
  }
  bl_uword bl_u32s = bl_mpmc_bpm_slot_payload (&c->q, slots) / sizeof (bl_u32);
  for (bl_uword i = 0; i < bl_u32s; ++i) {
    ((bl_u32*) *mem)[i] = (bl_u32) ~i;
  }
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_test_verify_produced(
  bl_mpmc_bpm_context* c, bl_u8* mem, bl_uword slots
  )
{
  bl_uword bl_u32s = bl_mpmc_bpm_slot_payload (&c->q, slots) / sizeof (bl_u32);
  for (bl_uword i = 0; i < bl_u32s; ++i) {
    assert_int_equal (((bl_u32*) mem)[i], (bl_u32) ~i);
  }
}
/*---------------------------------------------------------------------------*/
/* No teardown neccesary, a static array is used */
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_init_test (void **state)
{
  bl_mpmc_bpm_context* c = (bl_mpmc_bpm_context*) *state;
  assert_true (bl_mpmc_bpm_slot_count (&c->q) == bl_mpmc_bpm_slots);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_one (void **state)
{
  bl_mpmc_bpm_context* c = (bl_mpmc_bpm_context*) *state;
  bl_u8*       mem;
  bl_u32       slots;
  bl_mpmc_b_op op;
  bl_mpmc_b_op opc;

  bl_mpmc_bpm_test_produce (c, &op, &mem, 1, bl_mkok());
  bl_err err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_produce_commit (&c->q, op, mem, 1);
  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (opc, 0);
  assert_int_equal (slots, 1);
  bl_mpmc_bpm_test_verify_produced (c, mem, 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_max (void **state)
{
  bl_mpmc_bpm_context* c = (bl_mpmc_bpm_context*) *state;
  bl_u8*       mem;
  bl_u32       slots;
  bl_mpmc_b_op op;
  bl_mpmc_b_op opc;

  bl_mpmc_bpm_test_produce (c, &op, &mem, bl_mpmc_bpm_slots - 1, bl_mkok());
  bl_err err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_produce_commit (&c->q, op, mem, bl_mpmc_bpm_slots - 1);
  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (opc, 0);
  assert_int_equal (slots, bl_mpmc_bpm_slots - 1);
  bl_mpmc_bpm_test_verify_produced (c, mem, bl_mpmc_bpm_slots - 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_full (void **state)
{
  bl_mpmc_bpm_context* c = (bl_mpmc_bpm_context*) *state;
  bl_u8*       mem1;
  bl_u8*       mem2;
  bl_u8*       mem;
  bl_u32       slots;
  bl_mpmc_b_op op1;
  bl_mpmc_b_op op2;
  bl_mpmc_b_op opc;

  bl_mpmc_bpm_test_produce (c, &op1, &mem1, 1, bl_mkok());
  bl_mpmc_bpm_test_produce (c, &op2, &mem2, bl_mpmc_bpm_slots - 1, bl_mkok());

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));
  bl_err err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  bl_mpmc_bpm_produce_commit (&c->q, op1, mem1, 1);
  bl_mpmc_bpm_produce_commit (&c->q, op2, mem2, bl_mpmc_bpm_slots - 1);

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, 1);
  bl_mpmc_bpm_test_verify_produced (c, mem, 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, bl_mpmc_bpm_slots - 1);
  bl_mpmc_bpm_test_verify_produced (c, mem, bl_mpmc_bpm_slots - 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_wrap (void **state)
{
  bl_mpmc_bpm_context* c = (bl_mpmc_bpm_context*) *state;
  bl_u8*       mem1;
  bl_u8*       mem2;
  bl_u8*       mem;
  bl_u32       slots;
  bl_mpmc_b_op op1;
  bl_mpmc_b_op op2;
  bl_mpmc_b_op opc;

  bl_mpmc_bpm_test_produce (c, &op1, &mem1, 1, bl_mkok());
  bl_mpmc_bpm_test_produce (c, &op2, &mem2, bl_mpmc_bpm_slots - 1, bl_mkok());

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));
  bl_err err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  bl_mpmc_bpm_produce_commit (&c->q, op1, mem1, 1);
  bl_mpmc_bpm_produce_commit (&c->q, op2, mem2, bl_mpmc_bpm_slots - 1);

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  err = bl_mpmc_bpm_consume_prepare (&c->q, &op1, &mem1, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, 1);
  bl_mpmc_bpm_test_verify_produced (c, mem1, 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &op2, &mem2, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, bl_mpmc_bpm_slots - 1);
  bl_mpmc_bpm_test_verify_produced (c, mem2, bl_mpmc_bpm_slots - 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));
  /*wrapping*/
  bl_mpmc_bpm_consume_commit (&c->q, op2, mem2, bl_mpmc_bpm_slots - 1);

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  bl_mpmc_bpm_consume_commit (&c->q, op1, mem1, 1);

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkok());
  bl_mpmc_bpm_produce_commit (&c->q, opc, mem, 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, 1);
  bl_mpmc_bpm_test_verify_produced (c, mem, 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_wrap_using_extra_tail_space (void **state)
{
  bl_mpmc_bpm_context* c = (bl_mpmc_bpm_context*) *state;
  bl_u8*       mem;
  bl_u8*       mem1;
  bl_u8*       mem2;
  bl_mpmc_b_op op1;
  bl_mpmc_b_op op2;
  bl_mpmc_b_op opc;
  bl_u32       slots;

  bl_mpmc_bpm_test_produce (c, &op1, &mem1, bl_mpmc_bpm_slots - 1, bl_mkok());
  bl_mpmc_bpm_test_produce (c, &op2, &mem2, bl_mpmc_bpm_slots - 1, bl_mkok());
  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  bl_err err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  bl_mpmc_bpm_produce_commit (&c->q, op1, mem1, bl_mpmc_bpm_slots - 1);
  bl_mpmc_bpm_produce_commit (&c->q, op2, mem2, bl_mpmc_bpm_slots - 1);

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  err = bl_mpmc_bpm_consume_prepare (&c->q, &op1, &mem1, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, bl_mpmc_bpm_slots - 1);
  bl_mpmc_bpm_test_verify_produced (c, mem1, bl_mpmc_bpm_slots - 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &op2, &mem2, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, bl_mpmc_bpm_slots - 1);
  bl_mpmc_bpm_test_verify_produced (c, mem2, bl_mpmc_bpm_slots - 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_empty);
  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));
  /*wrapping*/
  bl_mpmc_bpm_consume_commit (&c->q, op2, mem2, bl_mpmc_bpm_slots - 1);

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkerr (bl_would_overflow));

  bl_mpmc_bpm_consume_commit (&c->q, op1, mem1, 1);

  bl_mpmc_bpm_test_produce (c, &opc, &mem, 1, bl_mkok());
  bl_mpmc_bpm_produce_commit (&c->q, opc, mem, 1);

  err = bl_mpmc_bpm_consume_prepare (&c->q, &opc, &mem, &slots);
  assert_int_equal (err.bl, bl_ok);
  assert_int_equal (slots, 1);
  bl_mpmc_bpm_test_verify_produced (c, mem, 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bpm_alloc_dealloc (void **state)
{
  bl_mpmc_bpm_context* c = (bl_mpmc_bpm_context*) *state;
  for (bl_uword i = 0; i < 4; ++i) {
    bl_u8* mem1 = bl_mpmc_bpm_alloc (&c->q, bl_mpmc_bpm_slots - 1);
    assert_non_null (mem1);
    bl_u8* mem2 = bl_mpmc_bpm_alloc (&c->q, bl_mpmc_bpm_slots - 1);
    assert_non_null (mem2);
    assert_true(
      (mem2 - mem1) == ((bl_mpmc_bpm_slots - 1) * bl_mpmc_bpm_slot_size (&c->q))
      );
    assert_null (bl_mpmc_bpm_alloc (&c->q, 1));
    bl_mpmc_bpm_dealloc (&c->q, mem2, bl_mpmc_bpm_slots - 1);
    assert_null (bl_mpmc_bpm_alloc (&c->q, 1));
    bl_mpmc_bpm_dealloc (&c->q, mem1, bl_mpmc_bpm_slots - 1);
  }
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (bl_mpmc_bpm_init_test, bl_mpmc_bpm_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bpm_one, bl_mpmc_bpm_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bpm_max, bl_mpmc_bpm_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bpm_full, bl_mpmc_bpm_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bpm_wrap, bl_mpmc_bpm_test_setup),
  cmocka_unit_test_setup(
    bl_mpmc_bpm_wrap_using_extra_tail_space, bl_mpmc_bpm_test_setup
    ),
  cmocka_unit_test_setup (bl_mpmc_bpm_alloc_dealloc, bl_mpmc_bpm_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bpm_one, bl_mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup (bl_mpmc_bpm_max, bl_mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup (bl_mpmc_bpm_full, bl_mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup (bl_mpmc_bpm_wrap, bl_mpmc_bpm_test_setup_fair),
  cmocka_unit_test_setup(
    bl_mpmc_bpm_wrap_using_extra_tail_space, bl_mpmc_bpm_test_setup_fair
    ),
  cmocka_unit_test_setup(
    bl_mpmc_bpm_alloc_dealloc, bl_mpmc_bpm_test_setup_fair
    ),
};
/*---------------------------------------------------------------------------*/
int bl_mpmc_bpm_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
