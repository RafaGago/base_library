#include <bl/cmocka_pre.h>
#include <bl/nonblock/mpmc_bt_test.h>

#include <bl/base/integer.h>
#include <bl/nonblock/mpmc_bt.h>
#include <bl/nonblock/mpmc_bt.c>
#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef u16 mpmc_bt_type;
/*---------------------------------------------------------------------------*/
typedef struct mpmc_bt_container {
  mpmc_b_op    hdr;
  mpmc_bt_type v;
  mpmc_bt_type padding;
}
mpmc_bt_container;
/*---------------------------------------------------------------------------*/
typedef struct mpmc_bt_context {
  mpmc_bt           q;
  mpmc_bt_container buff[8];
  alloc_tbl         alloc;
  bool              alloc_succeeds;
}
mpmc_bt_context;
/*---------------------------------------------------------------------------*/
static void* mpmc_bt_alloc_func (size_t bytes, alloc_tbl const* invoker)
{
  mpmc_bt_context* c = to_type_containing (invoker, alloc, mpmc_bt_context);
  assert_true (bytes == sizeof c->buff);
  return c->alloc_succeeds ? c->buff : nullptr;
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_dealloc_func (void const* mem, alloc_tbl const* invoker)
{
  mpmc_bt_context* c = to_type_containing (invoker, alloc, mpmc_bt_context);
  assert_true (mem == (void*) c->buff);
}
/*---------------------------------------------------------------------------*/
static int mpmc_bt_test_setup (void **state)
{
  static mpmc_bt_context c;

  *state           = (void*) &c;
  c.alloc_succeeds = true;
  c.alloc.alloc    = mpmc_bt_alloc_func;
  c.alloc.dealloc  = mpmc_bt_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  mpmc_bt_init(
    &c.q,
    &c.alloc,
    arr_elems (c.buff),
    sizeof (mpmc_bt_type),
    bl_alignof (mpmc_bt_type)
    );
  return 0;
}
/*---------------------------------------------------------------------------*/
/* No teardown neccesary, a static array is used */
/*---------------------------------------------------------------------------*/
static void mpmc_bt_init_test (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  bl_err err = mpmc_bt_init(
    &c->q,
    &c->alloc,
    arr_elems (c->buff),
    sizeof (mpmc_bt_type),
    bl_alignof (mpmc_bt_type)
    );
  assert_true (!err);
  mpmc_bt_destroy (&c->q, &c->alloc);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_init_alloc_fail_test (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  c->alloc_succeeds = false;
  bl_err err = mpmc_bt_init(
    &c->q,
    &c->alloc,
    arr_elems (c->buff),
    sizeof (mpmc_bt_type),
    bl_alignof (mpmc_bt_type)
    );
  assert_true (err == bl_alloc);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_init_too_small_test (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  bl_err err = mpmc_bt_init(
    &c->q,
    &c->alloc,
    1,
    sizeof (mpmc_bt_type),
    bl_alignof (mpmc_bt_type)
    );
  assert_true (err == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_init_too_big_test (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  bl_err err = mpmc_bt_init(
    &c->q,
    &c->alloc,
    pow2_u (mpmc_b_ticket_bits + 1),
    sizeof (mpmc_bt_type),
    bl_alignof (mpmc_bt_type)
    );
  assert_true (err == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_mpw_mpr (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op op;
  mpmc_bt_type v;
  for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
     assert_true (mpmc_bt_produce (&c->q, &op, &i) == bl_ok);
     assert_true (mpmc_b_ticket_decode (op) == i);
     assert_true (mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    mpmc_bt_produce (&c->q, &op, &v) == bl_would_overflow
    );
  for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
    assert_true (mpmc_bt_consume(&c->q, &op, &v) == bl_ok);
    assert_true (v == i);
    assert_true (mpmc_b_ticket_decode (op) == i);
    assert_true (mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    mpmc_bt_consume (&c->q, &op, &v) == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void mpmc_bt_mpw_mpr_wrap (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op op;
  mpmc_bt_type  v;

  for (uword j = 0;
       j < pow2_u (mpmc_b_ticket_bits) - 1;
       j += arr_elems (c->buff)
     ) {
    for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
       assert_true (mpmc_bt_produce (&c->q, &op, &i) == bl_ok);
       assert_true (mpmc_b_ticket_decode (op) == i + j);
       assert_true (mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      mpmc_bt_produce (&c->q, &op, &v) == bl_would_overflow
      );
    for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
      assert_true (mpmc_bt_consume (&c->q, &op, &v) == bl_ok);
      assert_true (v == i);
      assert_true (mpmc_b_ticket_decode (op) == i + j);
      assert_true (mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      mpmc_bt_consume (&c->q, &op, &v) == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void mpmc_bt_spw_spr (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op op;
  mpmc_bt_type  v;
  for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
     assert_true (mpmc_bt_produce_sp (&c->q, &op, &i) == bl_ok);
     assert_true (mpmc_b_ticket_decode (op) == i);
     assert_true (mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    mpmc_bt_produce_sp (&c->q, &op, &v) == bl_would_overflow
    );
  for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
    assert_true (mpmc_bt_consume_sc (&c->q, &op, &v) == bl_ok);
    assert_true (v == i);
    assert_true (mpmc_b_ticket_decode (op) == i);
    assert_true (mpmc_b_sig_decode (op) == 0);
  }
  assert_true(
    mpmc_bt_consume_sc (&c->q, &op, &v) == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void mpmc_bt_spw_spr_wrap (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op op;
  mpmc_bt_type  v;
  for (uword j = 0;
       j < pow2_u (mpmc_b_ticket_bits) - 1;
       j += arr_elems (c->buff)
     ) {
    for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
       assert_true (mpmc_bt_produce_sp (&c->q, &op, &i) == bl_ok);
       assert_true (mpmc_b_ticket_decode (op) == i + j);
       assert_true (mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      mpmc_bt_produce_sp (&c->q, &op, &v) == bl_would_overflow
      );
    for (mpmc_bt_type i = 0; i < arr_elems (c->buff); ++i) {
      assert_true (mpmc_bt_consume_sc (&c->q, &op, &v) == bl_ok);
      assert_true (v == i);
      assert_true (mpmc_b_ticket_decode (op) == i + j);
      assert_true (mpmc_b_sig_decode (op) == 0);
    }
    assert_true(
      mpmc_bt_consume_sc (&c->q, &op, &v) == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void mpmc_bt_producer_signals_set (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op op;
  mpmc_bt_type v = 0;
  mpmc_b_sig   exp;
  bl_err       err;

  exp = 0;
  err = mpmc_bt_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = mpmc_bt_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = mpmc_bt_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (exp == 1);

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_consumer_signals_set (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op op;
  mpmc_bt_type v = 0;
  mpmc_b_sig   exp;
  bl_err       err;

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);

  exp = 0;
  err = mpmc_bt_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = mpmc_bt_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = mpmc_bt_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (exp == 1);

  assert_true (mpmc_bt_consume (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_bt_consume (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_producer_signals_set_tmatch (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_ticket op, exp;
  mpmc_bt_type  v = 0;
  bl_err        err;

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0);
  err = mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 0);

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0); /*incorrect signal*/
  err = mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  exp = mpmc_b_op_encode (1, 1); /*incorrect transaction*/
  err = mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  err = mpmc_bt_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_consumer_signals_set_tmatch (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op    op, exp;
  mpmc_bt_type v = 0;
  bl_err       err;

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0);
  err = mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 0);

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0); /*incorrect signal*/
  err = mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  exp = mpmc_b_op_encode (1, 1); /*incorrect transaction*/
  err = mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  err = mpmc_bt_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  assert_true (mpmc_bt_consume (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_bt_consume (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_producer_fallback (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_ticket op;
  mpmc_bt_type  v = 0;
  mpmc_b_sig    exp;
  bl_err        err;

  exp = 0;
  err = mpmc_bt_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_bt_produce_fallback (&c->q, &op, &v, (mpmc_b_sig) -1, 1);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_bt_produce_fallback (&c->q, &op, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  err = mpmc_bt_produce_fallback (&c->q, &op, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_consumer_fallback (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op    op;
  mpmc_bt_type v = 0;
  mpmc_b_sig   exp;
  bl_err       err;

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);

  exp = 0;
  err = mpmc_bt_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_bt_consume_fallback (&c->q, &op, &v, (mpmc_b_sig) -1, 1);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_bt_consume_fallback (&c->q, &op, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  err = mpmc_bt_consume_fallback (&c->q, &op, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_producer_signal_change (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op    op;
  mpmc_bt_type v = 0;
  mpmc_b_sig   exp;
  bl_err       err;

  exp = 0;
  err = mpmc_bt_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_bt_produce_sig (&c->q, &op, &v, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_bt_produce_sig (&c->q, &op, &v, 3);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static void mpmc_bt_consumer_signal_change (void **state)
{
  mpmc_bt_context* c = (mpmc_bt_context*) *state;
  mpmc_b_op    op;
  mpmc_bt_type v = 0;
  mpmc_b_sig   exp;
  bl_err       err;

  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);
  assert_true (mpmc_bt_produce (&c->q, &op, &v) == bl_ok);

  exp = 0;
  err = mpmc_bt_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_bt_consume_sig (&c->q, &op, &v, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_bt_consume_sig (&c->q, &op, &v, 3);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (mpmc_bt_init_test, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_init_alloc_fail_test, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_init_too_small_test, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_init_too_big_test, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_mpw_mpr, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_spw_spr, mpmc_bt_test_setup),
#ifdef MPMC_B_TEST_WRAP /*take long to execute*/
  cmocka_unit_test_setup (mpmc_bt_mpw_mpr_wrap, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_spw_spr_wrap, mpmc_bt_test_setup),
#endif
  cmocka_unit_test_setup (mpmc_bt_producer_signals_set, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_consumer_signals_set, mpmc_bt_test_setup),
  cmocka_unit_test_setup(
    mpmc_bt_producer_signals_set_tmatch, mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup(
    mpmc_bt_consumer_signals_set_tmatch, mpmc_bt_test_setup
    ),
  cmocka_unit_test_setup (mpmc_bt_producer_fallback, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_consumer_fallback, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_producer_signal_change, mpmc_bt_test_setup),
  cmocka_unit_test_setup (mpmc_bt_consumer_signal_change, mpmc_bt_test_setup),
};
/*---------------------------------------------------------------------------*/
int mpmc_bt_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
