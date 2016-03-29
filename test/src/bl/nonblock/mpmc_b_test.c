#include <bl/cmocka_pre.h>
#include <bl/nonblock/mpmc_b_test.h>

#include <bl/base/integer.h>
#include <bl/nonblock/mpmc_b.h>
#include <bl/nonblock/mpmc_b.c>
#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef u16 mpmc_b_type;
/*---------------------------------------------------------------------------*/
typedef struct mpmc_b_container {
  mpmc_b_info hdr;
  mpmc_b_type v;
}
mpmc_b_container;
/*---------------------------------------------------------------------------*/
typedef struct mpmc_b_context {
  mpmc_b           q;
  mpmc_b_container buff[8];
  alloc_tbl        alloc;
  bool             alloc_succeeds;
}
mpmc_b_context;
/*---------------------------------------------------------------------------*/
static void* mpmc_b_alloc_func (size_t bytes, alloc_tbl const* invoker)
{
  mpmc_b_context* c = to_type_containing (invoker, alloc, mpmc_b_context);
  assert_true (bytes == sizeof c->buff);
  return c->alloc_succeeds ? c->buff : nullptr;
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_dealloc_func (void const* mem, alloc_tbl const* invoker)
{
  mpmc_b_context* c = to_type_containing (invoker, alloc, mpmc_b_context);
  assert_true (mem == (void*) c->buff);
}
/*---------------------------------------------------------------------------*/
static int mpmc_b_test_setup (void **state)
{
  static mpmc_b_context c;

  *state           = (void*) &c;
  c.alloc_succeeds = true;
  c.alloc.alloc    = mpmc_b_alloc_func;
  c.alloc.dealloc  = mpmc_b_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  mpmc_b_init (&c.q, &c.alloc, arr_elems (c.buff), mpmc_b_type);
  return 0;
}
/*---------------------------------------------------------------------------*/
/* No teardown neccesary, a static array is used */
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  bl_err err = mpmc_b_init (&c->q, &c->alloc, arr_elems (c->buff), mpmc_b_type);
  assert_true (!err);
  mpmc_b_destroy (&c->q, &c->alloc);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_alloc_fail_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  c->alloc_succeeds = false;
  bl_err err = mpmc_b_init (&c->q, &c->alloc, arr_elems (c->buff), mpmc_b_type);
  assert_true (err == bl_alloc);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_too_small_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  bl_err err = mpmc_b_init (&c->q, &c->alloc, 1, mpmc_b_type);
  assert_true (err == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_too_big_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  bl_err err = mpmc_b_init(
    &c->q, &c->alloc, pow2_u (mpmc_b_info_transaction_bits + 1), mpmc_b_type
    );
  assert_true (err == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_mpw_mpr (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info inf;
  mpmc_b_type v;
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
     assert_true (mpmc_b_produce (&c->q, &inf, &i) == bl_ok);
     assert_true (inf.transaction == i);
     assert_true (inf.signal == 0);
  }
  assert_true(
    mpmc_b_produce (&c->q, &inf, &v) == bl_would_overflow
    );
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
    assert_true (mpmc_b_consume (&c->q, &inf, &v) == bl_ok);
    assert_true (v == i);
    assert_true (inf.transaction == i);
    assert_true (inf.signal == 0);
  }
  assert_true(
    mpmc_b_consume (&c->q, &inf, &v) == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void mpmc_b_mpw_mpr_wrap (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info inf;
  mpmc_b_type v;

  for (uword j = 0;
       j < pow2_u (mpmc_b_info_transaction_bits) - 1;
       j += arr_elems (c->buff)
     ) {
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
       assert_true (mpmc_b_produce (&c->q, &inf, &i) == bl_ok);
       assert_true (inf.transaction == i + j);
       assert_true (inf.signal == 0);
    }
    assert_true(
      mpmc_b_produce (&c->q, &inf, &v) == bl_would_overflow
      );
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
      assert_true (mpmc_b_consume (&c->q, &inf, &v) == bl_ok);
      assert_true (v == i);
      assert_true (inf.transaction == i + j);
      assert_true (inf.signal == 0);
    }
    assert_true(
      mpmc_b_consume (&c->q, &inf, &v) == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void mpmc_b_spw_spr (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info inf;
  mpmc_b_type v;
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
     assert_true (mpmc_b_produce_single_p (&c->q, &inf, &i) == bl_ok);
     assert_true (inf.transaction == i);
     assert_true (inf.signal == 0);
  }
  assert_true(
    mpmc_b_produce_single_p (&c->q, &inf, &v) == bl_would_overflow
    );
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
    assert_true (mpmc_b_consume_single_c (&c->q, &inf, &v) == bl_ok);
    assert_true (v == i);
    assert_true (inf.transaction == i);
    assert_true (inf.signal == 0);
  }
  assert_true(
    mpmc_b_consume_single_c (&c->q, &inf, &v) == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void mpmc_b_spw_spr_wrap (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info inf;
  mpmc_b_type v;
  for (uword j = 0;
       j < pow2_u (mpmc_b_info_transaction_bits) - 1;
       j += arr_elems (c->buff)
     ) {
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
       assert_true (mpmc_b_produce_single_p (&c->q, &inf, &i) == bl_ok);
       assert_true (inf.transaction == i + j);
       assert_true (inf.signal == 0);
    }
    assert_true(
      mpmc_b_produce_single_p (&c->q, &inf, &v) == bl_would_overflow
      );
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
      assert_true (mpmc_b_consume_single_c (&c->q, &inf, &v) == bl_ok);
      assert_true (v == i);
      assert_true (inf.transaction == i + j);
      assert_true (inf.signal == 0);
    }
    assert_true(
      mpmc_b_consume_single_c (&c->q, &inf, &v) == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_signals_set (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf;
  mpmc_b_type     v = 0;
  mpmc_b_sig      exp;
  bl_err          err;

  exp = 0;
  err = mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = mpmc_b_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = mpmc_b_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (exp == 1);

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 0);

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_signals_set (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf;
  mpmc_b_type     v = 0;
  mpmc_b_sig      exp;
  bl_err          err;

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);

  exp = 0;
  err = mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = mpmc_b_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = mpmc_b_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (exp == 1);

  assert_true (mpmc_b_consume (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 0);

  assert_true (mpmc_b_consume (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_signals_set_tmatch (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf, exp;
  mpmc_b_type     v = 0;
  bl_err          err;

  exp.transaction = mpmc_b_unset_transaction;
  exp.signal      = 0;
  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 0);

  exp.transaction = mpmc_b_unset_transaction; /*correct transaction*/
  exp.signal      = 0; /*incorrect signal*/
  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 1);

  exp.transaction = 1; /*incorrect transaction*/
  exp.signal      = 1; /*correct signal*/
  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 1);

  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 1);

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 0);

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_signals_set_tmatch (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf, exp;
  mpmc_b_type     v = 0;
  bl_err          err;

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);

  exp.transaction = mpmc_b_unset_transaction;
  exp.signal      = 0;
  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 0);

  exp.transaction = mpmc_b_unset_transaction; /*correct transaction*/
  exp.signal      = 0; /*incorrect signal*/
  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 1);

  exp.transaction = 1; /*incorrect transaction*/
  exp.signal      = 1; /*correct signal*/
  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 1);

  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (exp.transaction == mpmc_b_unset_transaction);
  assert_true (exp.signal == 1);

  assert_true (mpmc_b_consume (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 0);

  assert_true (mpmc_b_consume (&c->q, &inf, &v) == bl_ok);
  assert_true (inf.signal == 2);
  assert_true (inf.transaction == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_fallback (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf;
  mpmc_b_type     v = 0;
  mpmc_b_sig      exp;
  bl_err          err;

  exp = 0;
  err = mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_produce_fallback (&c->q, &inf, &v, (mpmc_b_sig) -1, 1);
  assert_true (err == bl_preconditions);
  assert_true (inf.signal == 1);

  err = mpmc_b_produce_fallback (&c->q, &inf, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 1);
  assert_true (inf.transaction == 0);

  err = mpmc_b_produce_fallback (&c->q, &inf, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 1);
  assert_true (inf.transaction == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_fallback (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf;
  mpmc_b_type     v = 0;
  mpmc_b_sig      exp;
  bl_err          err;

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);

  exp = 0;
  err = mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_consume_fallback (&c->q, &inf, &v, (mpmc_b_sig) -1, 1);
  assert_true (err == bl_preconditions);
  assert_true (inf.signal == 1);

  err = mpmc_b_consume_fallback (&c->q, &inf, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 1);
  assert_true (inf.transaction == 0);

  err = mpmc_b_consume_fallback (&c->q, &inf, &v, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 1);
  assert_true (inf.transaction == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_signal_change (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf;
  mpmc_b_type     v = 0;
  mpmc_b_sig      exp;
  bl_err          err;

  exp = 0;
  err = mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_produce_sig (&c->q, &inf, &v, 2);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 1);

  err = mpmc_b_produce_sig (&c->q, &inf, &v, 3);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 2);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_signal_change (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_info     inf;
  mpmc_b_type     v = 0;
  mpmc_b_sig      exp;
  bl_err          err;

  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);
  assert_true (mpmc_b_produce (&c->q, &inf, &v) == bl_ok);

  exp = 0;
  err = mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_consume_sig (&c->q, &inf, &v, 2);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 1);

  err = mpmc_b_consume_sig (&c->q, &inf, &v, 3);
  assert_true (err == bl_ok);
  assert_true (inf.signal == 2);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (mpmc_b_init_test, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_init_alloc_fail_test, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_init_too_small_test, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_init_too_big_test, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_mpw_mpr, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_spw_spr, mpmc_b_test_setup),
#ifdef MPMC_B_TEST_WRAP /*take long to execute*/
  cmocka_unit_test_setup (mpmc_b_mpw_mpr_wrap, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_spw_spr_wrap, mpmc_b_test_setup),
#endif
  cmocka_unit_test_setup (mpmc_b_producer_signals_set, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_consumer_signals_set, mpmc_b_test_setup),
  cmocka_unit_test_setup(
    mpmc_b_producer_signals_set_tmatch, mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    mpmc_b_consumer_signals_set_tmatch, mpmc_b_test_setup
    ),
  cmocka_unit_test_setup (mpmc_b_producer_fallback, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_consumer_fallback, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_producer_signal_change, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_consumer_signal_change, mpmc_b_test_setup),
};
/*---------------------------------------------------------------------------*/
int mpmc_b_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
