#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/nonblock/mpmc_bt.h>
#include <bl/nonblock/mpmc_bt.c>
#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef bl_u16 bl_mpmc_bt_type;
/*---------------------------------------------------------------------------*/
typedef struct bl_mpmc_bt_container {
  bl_mpmc_b_op    hdr;
  bl_mpmc_bt_type v;
  bl_mpmc_bt_type padding;
}
bl_mpmc_bt_container;
/*---------------------------------------------------------------------------*/
typedef struct bl_mpmc_bt_context {
  bl_mpmc_bt           q;
  bl_mpmc_bt_container buff[8];
  bl_alloc_tbl         alloc;
  bool              alloc_succeeds;
}
bl_mpmc_bt_context;
/*---------------------------------------------------------------------------*/
static void*
  bl_mpmc_bt_alloc_func (size_t bytes, bl_alloc_tbl const* invoker)
{
  bl_mpmc_bt_context* c =
    bl_to_type_containing (invoker, alloc, bl_mpmc_bt_context);
  assert_true (bytes == sizeof c->buff);
  return c->alloc_succeeds ? c->buff : nullptr;
}
/*---------------------------------------------------------------------------*/
static void
  bl_mpmc_bt_dealloc_func (void const* mem, bl_alloc_tbl const* invoker)
{
  bl_mpmc_bt_context* c =
    bl_to_type_containing (invoker, alloc, bl_mpmc_bt_context);
  assert_true (mem == (void*) c->buff);
}
/*---------------------------------------------------------------------------*/
static int bl_mpmc_bt_test_setup (void **state)
{
  static bl_mpmc_bt_context c;

  *state           = (void*) &c;
  c.alloc_succeeds = true;
  c.alloc.alloc    = bl_mpmc_bt_alloc_func;
  c.alloc.dealloc  = bl_mpmc_bt_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  bl_mpmc_bt_init(
    &c.q,
    &c.alloc,
    bl_arr_elems (c.buff),
    sizeof (bl_mpmc_bt_type),
    bl_alignof (bl_mpmc_bt_type)
    );
  return 0;
}
/*---------------------------------------------------------------------------*/
/* No teardown neccesary, a static array is used */
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_init_test (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_err err = bl_mpmc_bt_init(
    &c->q,
    &c->alloc,
    bl_arr_elems (c->buff),
    sizeof (bl_mpmc_bt_type),
    bl_alignof (bl_mpmc_bt_type)
    );
  assert_true (!err.bl);
  bl_mpmc_bt_destroy (&c->q, &c->alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_init_alloc_fail_test (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  c->alloc_succeeds = false;
  bl_err err = bl_mpmc_bt_init(
    &c->q,
    &c->alloc,
    bl_arr_elems (c->buff),
    sizeof (bl_mpmc_bt_type),
    bl_alignof (bl_mpmc_bt_type)
    );
  assert_true (err.bl == bl_alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_init_too_small_test (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_err err = bl_mpmc_bt_init(
    &c->q,
    &c->alloc,
    1,
    sizeof (bl_mpmc_bt_type),
    bl_alignof (bl_mpmc_bt_type)
    );
  assert_true (err.bl == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_init_too_big_test (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_err err = bl_mpmc_bt_init(
    &c->q,
    &c->alloc,
    bl_pow2_u (bl_mpmc_b_ticket_bits + 1),
    sizeof (bl_mpmc_bt_type),
    bl_alignof (bl_mpmc_bt_type)
    );
  assert_true (err.bl == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_mpw_mpr (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op op;
  bl_mpmc_bt_type v;
  for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
     assert_true (bl_mpmc_bt_produce (&c->q, &op, &i).bl == bl_ok);
     assert_true (bl_mpmc_b_ticket_decode (op) == i);
     assert_true (bl_mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_would_overflow
    );
  for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
    assert_true (bl_mpmc_bt_consume(&c->q, &op, &v).bl == bl_ok);
    assert_true (v == i);
    assert_true (bl_mpmc_b_ticket_decode (op) == i);
    assert_true (bl_mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    bl_mpmc_bt_consume (&c->q, &op, &v).bl == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void bl_mpmc_bt_mpw_mpr_wrap (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op op;
  bl_mpmc_bt_type  v;

  for (bl_uword j = 0;
       j < bl_pow2_u (bl_mpmc_b_ticket_bits) - 1;
       j += bl_arr_elems (c->buff)
     ) {
    for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
       assert_true (bl_mpmc_bt_produce (&c->q, &op, &i).bl == bl_ok);
       assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
       assert_true (bl_mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_would_overflow
      );
    for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
      assert_true (bl_mpmc_bt_consume (&c->q, &op, &v).bl == bl_ok);
      assert_true (v == i);
      assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
      assert_true (bl_mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      bl_mpmc_bt_consume (&c->q, &op, &v).bl == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_spw_spr (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op op;
  bl_mpmc_bt_type  v;
  for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
     assert_true (bl_mpmc_bt_produce_sp (&c->q, &op, &i).bl == bl_ok);
     assert_true (bl_mpmc_b_ticket_decode (op) == i);
     assert_true (bl_mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    bl_mpmc_bt_produce_sp (&c->q, &op, &v).bl == bl_would_overflow
    );
  for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
    assert_true (bl_mpmc_bt_consume_sc (&c->q, &op, &v).bl == bl_ok);
    assert_true (v == i);
    assert_true (bl_mpmc_b_ticket_decode (op) == i);
    assert_true (bl_mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    bl_mpmc_bt_consume_sc (&c->q, &op, &v).bl == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void bl_mpmc_bt_spw_spr_wrap (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op op;
  bl_mpmc_bt_type  v;
  for (bl_uword j = 0;
       j < bl_pow2_u (bl_mpmc_b_ticket_bits) - 1;
       j += bl_arr_elems (c->buff)
     ) {
    for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
       assert_true (bl_mpmc_bt_produce_sp (&c->q, &op, &i).bl == bl_ok);
       assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
       assert_true (bl_mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      bl_mpmc_bt_produce_sp (&c->q, &op, &v).bl == bl_would_overflow
      );
    for (bl_mpmc_bt_type i = 0; i < bl_arr_elems (c->buff); ++i) {
      assert_true (bl_mpmc_bt_consume_sc (&c->q, &op, &v).bl == bl_ok);
      assert_true (v == i);
      assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
      assert_true (bl_mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      bl_mpmc_bt_consume_sc (&c->q, &op, &v).bl == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_producer_signals_set (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op op;
  bl_mpmc_bt_type v = 0;
  bl_mpmc_b_sig   exp;
  bl_err       err;

  exp = 0;
  err = bl_mpmc_bt_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = bl_mpmc_bt_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.bl == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = bl_mpmc_bt_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.bl == bl_ok);
  assert_true (exp == 1);

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_consumer_signals_set (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op op;
  bl_mpmc_bt_type v = 0;
  bl_mpmc_b_sig   exp;
  bl_err       err;

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);

  exp = 0;
  err = bl_mpmc_bt_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = bl_mpmc_bt_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.bl == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = bl_mpmc_bt_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.bl == bl_ok);
  assert_true (exp == 1);

  assert_true (bl_mpmc_bt_consume (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_bt_consume (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_producer_signals_set_tmatch (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_ticket op, exp;
  bl_mpmc_bt_type  v = 0;
  bl_err        err;

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0);
  err = bl_mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 0);

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0); /*incorrect signal*/
  err = bl_mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.bl == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  exp = bl_mpmc_b_op_encode (1, 1); /*incorrect transaction*/
  err = bl_mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.bl == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  err = bl_mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_consumer_signals_set_tmatch (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op    op, exp;
  bl_mpmc_bt_type v = 0;
  bl_err       err;

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0);
  err = bl_mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 0);

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0); /*incorrect signal*/
  err = bl_mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.bl == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  exp = bl_mpmc_b_op_encode (1, 1); /*incorrect transaction*/
  err = bl_mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.bl == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  err = bl_mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  assert_true (bl_mpmc_bt_consume (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_bt_consume (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_producer_fallback (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_ticket op;
  bl_mpmc_bt_type  v = 0;
  bl_mpmc_b_sig    exp;
  bl_err        err;

  exp = 0;
  err = bl_mpmc_bt_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);

  err = bl_mpmc_bt_produce_fallback (&c->q, &op, &v, (bl_mpmc_b_sig) -1, 1);
  assert_true (err.bl == bl_preconditions);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_bt_produce_fallback (&c->q, &op, &v, (bl_mpmc_b_sig) -1, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  err = bl_mpmc_bt_produce_fallback (&c->q, &op, &v, (bl_mpmc_b_sig) -1, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_consumer_fallback (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op    op;
  bl_mpmc_bt_type v = 0;
  bl_mpmc_b_sig   exp;
  bl_err       err;

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);

  exp = 0;
  err = bl_mpmc_bt_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);

  err = bl_mpmc_bt_consume_fallback (&c->q, &op, &v, (bl_mpmc_b_sig) -1, 1);
  assert_true (err.bl == bl_preconditions);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_bt_consume_fallback (&c->q, &op, &v, (bl_mpmc_b_sig) -1, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  err = bl_mpmc_bt_consume_fallback (&c->q, &op, &v, (bl_mpmc_b_sig) -1, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_producer_signal_change (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op    op;
  bl_mpmc_bt_type v = 0;
  bl_mpmc_b_sig   exp;
  bl_err       err;

  exp = 0;
  err = bl_mpmc_bt_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);

  err = bl_mpmc_bt_produce_sig (&c->q, &op, &v, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_bt_produce_sig (&c->q, &op, &v, 3);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_bt_consumer_signal_change (void **state)
{
  bl_mpmc_bt_context* c = (bl_mpmc_bt_context*) *state;
  bl_mpmc_b_op    op;
  bl_mpmc_bt_type v = 0;
  bl_mpmc_b_sig   exp;
  bl_err       err;

  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);
  assert_true (bl_mpmc_bt_produce (&c->q, &op, &v).bl == bl_ok);

  exp = 0;
  err = bl_mpmc_bt_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.bl == bl_ok);

  err = bl_mpmc_bt_consume_sig (&c->q, &op, &v, 2);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_bt_consume_sig (&c->q, &op, &v, 3);
  assert_true (err.bl == bl_ok);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (bl_mpmc_bt_init_test, bl_mpmc_bt_test_setup),
  cmocka_unit_test_setup(
    bl_mpmc_bt_init_alloc_fail_test, bl_mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_bt_init_too_small_test, bl_mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup (bl_mpmc_bt_init_too_big_test, bl_mpmc_bt_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bt_mpw_mpr, bl_mpmc_bt_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bt_spw_spr, bl_mpmc_bt_test_setup),
#ifdef MPMC_B_TEST_WRAP /*take long to execute*/
  cmocka_unit_test_setup (bl_mpmc_bt_mpw_mpr_wrap, bl_mpmc_bt_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bt_spw_spr_wrap, bl_mpmc_bt_test_setup),
#endif
  cmocka_unit_test_setup(
    bl_mpmc_bt_producer_signals_set, bl_mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_bt_consumer_signals_set, bl_mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_bt_producer_signals_set_tmatch, bl_mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_bt_consumer_signals_set_tmatch, bl_mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup (bl_mpmc_bt_producer_fallback, bl_mpmc_bt_test_setup),
  cmocka_unit_test_setup (bl_mpmc_bt_consumer_fallback, bl_mpmc_bt_test_setup),
  cmocka_unit_test_setup(
    bl_mpmc_bt_producer_signal_change, bl_mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_bt_consumer_signal_change, bl_mpmc_bt_test_setup
    ),
};
/*---------------------------------------------------------------------------*/
int bl_mpmc_bt_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
