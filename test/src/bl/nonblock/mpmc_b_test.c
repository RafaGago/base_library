#include <bl/cmocka_pre.h>
#include <bl/nonblock/mpmc_b_test.h>

#include <string.h>
#include <bl/base/integer.h>
#include <bl/nonblock/mpmc_b.h>
#include <bl/nonblock/mpmc_b.c>
#include <bl/nonblock/mpmc_b.c>
#include <bl/nonblock/mpmc_b_common.c>
#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef u16 mpmc_b_type;
/*---------------------------------------------------------------------------*/
typedef struct mpmc_b_container {
  mpmc_b_op   hdr;
  mpmc_b_type v;
  mpmc_b_type padding;
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
static void mpmc_b_test_queue_fill (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
     u8* mem;
     assert_true (mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_ok);
     *((mpmc_b_type*) mem) = i;
     assert_true (mpmc_b_ticket_decode (op) == i);
     assert_true (mpmc_b_sig_decode (op) == 0);
     mpmc_b_produce_commit (&c->q, op);
  }
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
  mpmc_b_init(
    &c.q,
    &c.alloc,
    arr_elems (c.buff),
    sizeof (mpmc_b_type),
    bl_alignof (mpmc_b_type)
    );
  return 0;
}
/*---------------------------------------------------------------------------*/
/* No teardown neccesary, a static array is used */
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  bl_err err = mpmc_b_init(
    &c->q,
    &c->alloc,
    arr_elems (c->buff),
    sizeof (mpmc_b_type),
    bl_alignof (mpmc_b_type)
    );
  assert_true (!err);
  mpmc_b_destroy (&c->q, &c->alloc);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_alloc_fail_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  c->alloc_succeeds = false;
  bl_err err = mpmc_b_init(
    &c->q,
    &c->alloc,
    arr_elems (c->buff),
    sizeof (mpmc_b_type),
    bl_alignof (mpmc_b_type)
    );
  assert_true (err == bl_alloc);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_too_small_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  bl_err err = mpmc_b_init(
    &c->q,
    &c->alloc,
    1,
    sizeof (mpmc_b_type),
    bl_alignof (mpmc_b_type)
    );
  assert_true (err == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_init_too_big_test (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  bl_err err = mpmc_b_init(
    &c->q,
    &c->alloc,
    pow2_u (mpmc_b_ticket_bits + 1),
    sizeof (mpmc_b_type),
    bl_alignof (mpmc_b_type)
    );
  assert_true (err == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_mpw_mpr (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8* mem;
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
     assert_true (mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_ok);
     *((mpmc_b_type*) mem) = i;
     assert_true (mpmc_b_ticket_decode (op) == i);
     assert_true (mpmc_b_sig_decode (op) == 0);
     mpmc_b_produce_commit (&c->q, op);
  }
  assert_true(
    mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_would_overflow
    );
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
    assert_true (mpmc_b_consume_prepare(&c->q, &op, &mem) == bl_ok);
    assert_true (*((mpmc_b_type*) mem) == i);
    assert_true (mpmc_b_ticket_decode (op) == i);
    assert_true (mpmc_b_sig_decode (op) == 0);
    mpmc_b_consume_commit (&c->q, op);
  }
  assert_true(
    mpmc_b_consume_prepare (&c->q, &op, &mem) == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void mpmc_b_mpw_mpr_wrap (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8* mem;
  for (uword j = 0;
       j < pow2_u (mpmc_b_ticket_bits) - 1;
       j += arr_elems (c->buff)
     ) {
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
       u8* mem;
       assert_true (mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_ok);
       *((mpmc_b_type*) mem) = i;
       assert_true (mpmc_b_ticket_decode (op) == i + j);
       assert_true (mpmc_b_sig_decode (op) == 0);
       mpmc_b_produce_commit (&c->q, op);
    }
    assert_true(
      mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_would_overflow
      );
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
      assert_true (mpmc_b_consume_prepare (&c->q, &op, &mem) == bl_ok);
      assert_true (*((mpmc_b_type*) mem) == i);
      assert_true (mpmc_b_ticket_decode (op) == i + j);
      assert_true (mpmc_b_sig_decode (op) == 0);
      mpmc_b_consume_commit (&c->q, op);
    }
    assert_true(
      mpmc_b_consume_prepare (&c->q, &op, &mem) == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void mpmc_b_spw_spr (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8* mem;
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
     assert_true (mpmc_b_produce_prepare_sp (&c->q, &op, &mem) == bl_ok);
     *((mpmc_b_type*) mem) = i;
     assert_true (mpmc_b_ticket_decode (op) == i);
     assert_true (mpmc_b_sig_decode (op) == 0);
     mpmc_b_produce_commit (&c->q, op);
  }
  assert_true(
    mpmc_b_produce_prepare_sp (&c->q, &op, &mem) == bl_would_overflow
    );
  for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
    assert_true (mpmc_b_consume_prepare_sc (&c->q, &op, &mem) == bl_ok);
    assert_true (*((mpmc_b_type*) mem) == i);
    assert_true (mpmc_b_ticket_decode (op) == i);
    assert_true (mpmc_b_sig_decode (op) == 0);
    mpmc_b_consume_commit (&c->q, op);
  }
  assert_true(
    mpmc_b_consume_prepare_sc (&c->q, &op, &mem) == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void mpmc_b_spw_spr_wrap (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8* mem;
  for (uword j = 0;
       j < pow2_u (mpmc_b_ticket_bits) - 1;
       j += arr_elems (c->buff)
     ) {
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
       assert_true (mpmc_b_produce_prepare_sp (&c->q, &op, &mem) == bl_ok);
       *((mpmc_b_type*) mem) = i;
       assert_true (mpmc_b_ticket_decode (op) == i + j);
       assert_true (mpmc_b_sig_decode (op) == 0);
       mpmc_b_produce_commit (&c->q, op);
    }
    assert_true(
      mpmc_b_produce_prepare_sp (&c->q, &op, &mem) == bl_would_overflow
      );
    for (mpmc_b_type i = 0; i < arr_elems (c->buff); ++i) {
      assert_true (mpmc_b_consume_prepare_sc (&c->q, &op, &mem) == bl_ok);
      assert_true (*((mpmc_b_type*) mem) == i);
      assert_true (mpmc_b_ticket_decode (op) == i + j);
      assert_true (mpmc_b_sig_decode (op) == 0);
      mpmc_b_consume_commit (&c->q, op);
    }
    assert_true(
      mpmc_b_consume_prepare_sc (&c->q, &op, &mem) == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_signals_set (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op  op;
  u8*        mem;
  mpmc_b_sig exp;
  bl_err     err;

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

  assert_true (mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_signals_set (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op  op;
  u8*        mem;
  mpmc_b_sig exp;
  bl_err     err;

  mpmc_b_test_queue_fill (state);

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

  assert_true (mpmc_b_consume_prepare (&c->q, &op, &mem) == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_b_consume_prepare (&c->q, &op, &mem) == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_signals_set_tmatch (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op, exp;
  u8*       mem;
  bl_err    err;

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0);
  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 0);

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0); /*incorrect signal*/
  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  exp = mpmc_b_op_encode (2, 1); /*incorrect transaction*/
  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  err = mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  assert_true (mpmc_b_produce_prepare(&c->q, &op, &mem) == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_b_produce_prepare(&c->q, &op, &mem) == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_signals_set_tmatch (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op, exp;
  u8*       mem;
  bl_err    err;

  mpmc_b_test_queue_fill (state);

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0);
  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 0);

  exp = mpmc_b_op_encode (mpmc_b_first_op, 0); /*incorrect signal*/
  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  exp = mpmc_b_op_encode (1, 1); /*incorrect transaction*/
  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  err = mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err == bl_ok);
  assert_true (mpmc_b_ticket_decode (exp) == mpmc_b_first_op);
  assert_true (mpmc_b_sig_decode (exp) == 1);

  assert_true (mpmc_b_consume_prepare(&c->q, &op, &mem) == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  assert_true (mpmc_b_consume_prepare(&c->q, &op, &mem) == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_fallback (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op  op;
  u8*        mem;
  mpmc_b_sig exp;
  bl_err     err;

  exp = 0;
  err = mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_produce_prepare_fallback (&c->q, &op, &mem, (mpmc_b_sig) -1, 1);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_b_produce_prepare_fallback (&c->q, &op, &mem, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  err = mpmc_b_produce_prepare_fallback (&c->q, &op, &mem, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_fallback (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op  op;
  u8*        mem;
  mpmc_b_sig exp;
  bl_err     err;

  mpmc_b_test_queue_fill (state);

  exp = 0;
  err = mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_consume_prepare_fallback (&c->q, &op, &mem, (mpmc_b_sig) -1, 1);
  assert_true (err == bl_preconditions);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_b_consume_prepare_fallback (&c->q, &op, &mem, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 0);

  err = mpmc_b_consume_prepare_fallback (&c->q, &op, &mem, (mpmc_b_sig) -1, 2);
  assert_true (err == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 1);
  assert_true (mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_signal_change (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op  op;
  u8*        mem;
  mpmc_b_sig exp;
  bl_err     err;

  exp = 0;
  err = mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_produce_prepare_sig (&c->q, &op, &mem, 2);
  assert_true (err == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_b_produce_prepare_sig (&c->q, &op, &mem, 3);
  assert_true (err == bl_ok);
  mpmc_b_produce_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_consumer_signal_change (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op  op;
  u8*        mem;
  mpmc_b_sig exp;
  bl_err     err;

  mpmc_b_test_queue_fill (state);

  exp = 0;
  err = mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err == bl_ok);

  err = mpmc_b_consume_prepare_sig (&c->q, &op, &mem, 2);
  assert_true (err == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 1);

  err = mpmc_b_consume_prepare_sig (&c->q, &op, &mem, 3);
  assert_true (err == bl_ok);
  mpmc_b_consume_commit (&c->q, op);
  assert_true (mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_fifo_produce_empty_queue (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8*       mem;
  bl_err    err;

  mpmc_b_fifo_produce_prepare (&c->q, &op);
  err = mpmc_b_fifo_produce_prepare_is_ready (&c->q, op, &mem);
  assert_true (err == bl_ok);
  *((mpmc_b_type*) mem) = 0;
  assert_true (mpmc_b_ticket_decode (op) == 0);
  mpmc_b_produce_commit (&c->q, op);

  assert_true (mpmc_b_consume_prepare (&c->q, &op, &mem) == bl_ok);
  assert_true (*((mpmc_b_type*) mem) == 0);
  assert_true (mpmc_b_ticket_decode (op) == 0);
  assert_true (mpmc_b_sig_decode (op) == 0);
  mpmc_b_consume_commit (&c->q, op);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_fifo_produce_full_queue (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op, opfifo;
  u8*       mem;
  bl_err    err;

  mpmc_b_test_queue_fill (state);

  mpmc_b_fifo_produce_prepare (&c->q, &opfifo);
  err = mpmc_b_fifo_produce_prepare_is_ready (&c->q, opfifo, &mem);
  assert_true (err == bl_would_overflow);
  assert_true (mpmc_b_ticket_decode (opfifo) == arr_elems (c->buff));

  assert_true (mpmc_b_consume_prepare (&c->q, &op, &mem) == bl_ok);
  assert_true (*((mpmc_b_type*) mem) == 0);
  assert_true (mpmc_b_ticket_decode (op) == 0);
  assert_true (mpmc_b_sig_decode (op) == 0);
  mpmc_b_consume_commit (&c->q, op);

  err = mpmc_b_fifo_produce_prepare_is_ready (&c->q, opfifo, &mem);
  assert_true (err == bl_ok);
  *((mpmc_b_type*) mem) = arr_elems (c->buff);
  mpmc_b_produce_commit (&c->q, opfifo);

  for (mpmc_b_type i = 1; i < arr_elems (c->buff) + 1; ++i) {
    assert_true (mpmc_b_consume_prepare(&c->q, &op, &mem) == bl_ok);
    assert_true (*((mpmc_b_type*) mem) == i);
    assert_true (mpmc_b_ticket_decode (op) == i);
    assert_true (mpmc_b_sig_decode (op) == 0);
    mpmc_b_consume_commit (&c->q, op);
  }
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_block (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8*       mem;
  bl_err    err;

  assert_true (mpmc_b_produce_prepare (&c->q, &op, &mem) == bl_ok);
  mpmc_b_produce_commit (&c->q, op);

  mpmc_b_block_producers (&c->q);
  err = mpmc_b_produce_prepare (&c->q, &op, &mem);
  assert_true (err == bl_locked);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_block_full_queue (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8*       mem;
  bl_err    err;

  mpmc_b_test_queue_fill (state);

  mpmc_b_block_producers (&c->q);
  err = mpmc_b_produce_prepare (&c->q, &op, &mem);
  assert_true (err == bl_locked);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_block_fifo_produce (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op1, op2;
  u8*       mem;
  bl_err    err;

  mpmc_b_fifo_produce_prepare (&c->q, &op1);

  mpmc_b_block_producers (&c->q);

  mpmc_b_fifo_produce_prepare (&c->q, &op2);

  err = mpmc_b_fifo_produce_prepare_is_ready (&c->q, op1, &mem);
  assert_true (err == bl_ok);

  err = mpmc_b_fifo_produce_prepare_is_ready (&c->q, op2, &mem);
  assert_true (err == bl_locked);
}
/*---------------------------------------------------------------------------*/
static void mpmc_b_producer_block_fifo_produce_full_queue (void **state)
{
  mpmc_b_context* c = (mpmc_b_context*) *state;
  mpmc_b_op op;
  u8*       mem;
  bl_err    err;

  mpmc_b_test_queue_fill (state);

  mpmc_b_fifo_produce_prepare (&c->q, &op);

  mpmc_b_block_producers (&c->q);

  err = mpmc_b_fifo_produce_prepare_is_ready (&c->q, op, &mem);
  assert_true (err == bl_locked);
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
  cmocka_unit_test_setup (mpmc_b_fifo_produce_full_queue, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_fifo_produce_empty_queue, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_producer_block, mpmc_b_test_setup),
  cmocka_unit_test_setup (mpmc_b_producer_block_full_queue, mpmc_b_test_setup),
  cmocka_unit_test_setup(
    mpmc_b_producer_block_fifo_produce, mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    mpmc_b_producer_block_fifo_produce_full_queue, mpmc_b_test_setup
    ),
};
/*---------------------------------------------------------------------------*/
int mpmc_b_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
