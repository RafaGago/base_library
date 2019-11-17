#include <bl/cmocka_pre.h>

#include <string.h>
#include <bl/base/integer.h>
#include <bl/nonblock/mpmc_b.h>
#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef bl_u16 bl_mpmc_b_type;
/*---------------------------------------------------------------------------*/
typedef struct bl_mpmc_b_container {
  bl_mpmc_b_op   hdr;
  bl_mpmc_b_type v;
  bl_mpmc_b_type padding;
}
bl_mpmc_b_container;
/*---------------------------------------------------------------------------*/
typedef struct bl_mpmc_b_context {
  bl_mpmc_b           q;
  bl_mpmc_b_container buff[8];
  bl_alloc_tbl        alloc;
  bool             alloc_succeeds;
}
bl_mpmc_b_context;
/*---------------------------------------------------------------------------*/
static void* bl_mpmc_b_alloc_func (size_t bytes, bl_alloc_tbl const* invoker)
{
  bl_mpmc_b_context* c =
    bl_to_type_containing (invoker, alloc, bl_mpmc_b_context);
  assert_true (bytes == sizeof c->buff);
  return c->alloc_succeeds ? c->buff : nullptr;
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_dealloc_func(
  void const* mem, bl_alloc_tbl const* invoker
  )
{
  bl_mpmc_b_context* c =
    bl_to_type_containing (invoker, alloc, bl_mpmc_b_context);
  assert_true (mem == (void*) c->buff);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_test_queue_fill (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
     bl_u8* mem;
     assert_true (bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_ok);
     *((bl_mpmc_b_type*) mem) = i;
     assert_true (bl_mpmc_b_ticket_decode (op) == i);
     assert_true (bl_mpmc_b_sig_decode (op) == 0);
     bl_mpmc_b_produce_commit (&c->q, op);
  }
}
/*---------------------------------------------------------------------------*/
static int bl_mpmc_b_test_setup (void **state)
{
  static bl_mpmc_b_context c;

  *state           = (void*) &c;
  c.alloc_succeeds = true;
  c.alloc.alloc    = bl_mpmc_b_alloc_func;
  c.alloc.dealloc  = bl_mpmc_b_dealloc_func;
  memset (c.buff, 0, sizeof c.buff);
  bl_mpmc_b_init(
    &c.q,
    &c.alloc,
    bl_arr_elems (c.buff),
    sizeof (bl_mpmc_b_type),
    bl_alignof (bl_mpmc_b_type)
    );
  return 0;
}
/*---------------------------------------------------------------------------*/
/* No teardown neccesary, a static array is used */
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_init_test (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_err err = bl_mpmc_b_init(
    &c->q,
    &c->alloc,
    bl_arr_elems (c->buff),
    sizeof (bl_mpmc_b_type),
    bl_alignof (bl_mpmc_b_type)
    );
  assert_true (!err.own);
  bl_mpmc_b_destroy (&c->q, &c->alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_init_alloc_fail_test (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  c->alloc_succeeds = false;
  bl_err err = bl_mpmc_b_init(
    &c->q,
    &c->alloc,
    bl_arr_elems (c->buff),
    sizeof (bl_mpmc_b_type),
    bl_alignof (bl_mpmc_b_type)
    );
  assert_true (err.own == bl_alloc);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_init_too_small_test (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_err err = bl_mpmc_b_init(
    &c->q,
    &c->alloc,
    1,
    sizeof (bl_mpmc_b_type),
    bl_alignof (bl_mpmc_b_type)
    );
  assert_true (err.own == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_init_too_big_test (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_err err = bl_mpmc_b_init(
    &c->q,
    &c->alloc,
    bl_pow2_u (bl_mpmc_b_ticket_bits + 1),
    sizeof (bl_mpmc_b_type),
    bl_alignof (bl_mpmc_b_type)
    );
  assert_true (err.own == bl_invalid);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_mpw_mpr (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8* mem;
  for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
     assert_true (bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_ok);
     *((bl_mpmc_b_type*) mem) = i;
     assert_true (bl_mpmc_b_ticket_decode (op) == i);
     assert_true (bl_mpmc_b_sig_decode (op) == 0);
     bl_mpmc_b_produce_commit (&c->q, op);
  }
  assert_true(
    bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_would_overflow
    );
  for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
    assert_true (bl_mpmc_b_consume_prepare(&c->q, &op, &mem).own == bl_ok);
    assert_true (*((bl_mpmc_b_type*) mem) == i);
    assert_true (bl_mpmc_b_ticket_decode (op) == i);
    assert_true (bl_mpmc_b_sig_decode (op) == 0);
    bl_mpmc_b_consume_commit (&c->q, op);
  }
  assert_true(
    bl_mpmc_b_consume_prepare (&c->q, &op, &mem).own == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void bl_mpmc_b_mpw_mpr_wrap (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8* mem;
  for (bl_uword j = 0;
       j < bl_pow2_u (bl_mpmc_b_ticket_bits) - 1;
       j += bl_arr_elems (c->buff)
     ) {
    for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
       bl_u8* mem;
       assert_true (bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_ok);
       *((bl_mpmc_b_type*) mem) = i;
       assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
       assert_true (bl_mpmc_b_sig_decode (op) == 0);
       bl_mpmc_b_produce_commit (&c->q, op);
    }
    assert_true(
      bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_would_overflow
      );
    for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
      assert_true (bl_mpmc_b_consume_prepare (&c->q, &op, &mem).own == bl_ok);
      assert_true (*((bl_mpmc_b_type*) mem) == i);
      assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
      assert_true (bl_mpmc_b_sig_decode (op) == 0);
      bl_mpmc_b_consume_commit (&c->q, op);
    }
    assert_true(
      bl_mpmc_b_consume_prepare (&c->q, &op, &mem).own == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_spw_spr (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8* mem;
  for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
     assert_true (bl_mpmc_b_produce_prepare_sp (&c->q, &op, &mem).own == bl_ok);
     *((bl_mpmc_b_type*) mem) = i;
     assert_true (bl_mpmc_b_ticket_decode (op) == i);
     assert_true (bl_mpmc_b_sig_decode (op) == 0);
     bl_mpmc_b_produce_commit (&c->q, op);
  }
  assert_true(
    bl_mpmc_b_produce_prepare_sp (&c->q, &op, &mem).own == bl_would_overflow
    );
  for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
    assert_true (bl_mpmc_b_consume_prepare_sc (&c->q, &op, &mem).own == bl_ok);
    assert_true (*((bl_mpmc_b_type*) mem) == i);
    assert_true (bl_mpmc_b_ticket_decode (op) == i);
    assert_true (bl_mpmc_b_sig_decode (op) == 0);
    bl_mpmc_b_consume_commit (&c->q, op);
  }
  assert_true(
    bl_mpmc_b_consume_prepare_sc (&c->q, &op, &mem).own == bl_empty
  );
}
/*---------------------------------------------------------------------------*/
#ifdef MPMC_B_TEST_WRAP /*slow execution*/
static void bl_mpmc_b_spw_spr_wrap (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8* mem;
  for (bl_uword j = 0;
       j < bl_pow2_u (bl_mpmc_b_ticket_bits) - 1;
       j += bl_arr_elems (c->buff)
     ) {
    for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
       assert_true(
        bl_mpmc_b_produce_prepare_sp (&c->q, &op, &mem).own == bl_ok
        );
       *((bl_mpmc_b_type*) mem) = i;
       assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
       assert_true (bl_mpmc_b_sig_decode (op) == 0);
       bl_mpmc_b_produce_commit (&c->q, op);
    }
    assert_true(
      bl_mpmc_b_produce_prepare_sp (&c->q, &op, &mem).own == bl_would_overflow
      );
    for (bl_mpmc_b_type i = 0; i < bl_arr_elems (c->buff); ++i) {
      assert_true (bl_mpmc_b_consume_prepare_sc (&c->q, &op, &mem).own == bl_ok);
      assert_true (*((bl_mpmc_b_type*) mem) == i);
      assert_true (bl_mpmc_b_ticket_decode (op) == i + j);
      assert_true (bl_mpmc_b_sig_decode (op) == 0);
      bl_mpmc_b_consume_commit (&c->q, op);
    }
    assert_true(
      bl_mpmc_b_consume_prepare_sc (&c->q, &op, &mem).own == bl_empty
    );
  }
}
#endif
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_signals_set (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op  op;
  bl_u8*        mem;
  bl_mpmc_b_sig exp;
  bl_err     err;

  exp = 0;
  err = bl_mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = bl_mpmc_b_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.own == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = bl_mpmc_b_producer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.own == bl_ok);
  assert_true (exp == 1);

  assert_true (bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_consumer_signals_set (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op  op;
  bl_u8*        mem;
  bl_mpmc_b_sig exp;
  bl_err     err;

  bl_mpmc_b_test_queue_fill (state);

  exp = 0;
  err = bl_mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);
  assert_true (exp == 0);

  exp = 0;
  err = bl_mpmc_b_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.own == bl_preconditions);
  assert_true (exp == 1);

  exp = 1;
  err = bl_mpmc_b_consumer_signal_try_set (&c->q, &exp, 2);
  assert_true (err.own == bl_ok);
  assert_true (exp == 1);

  assert_true (bl_mpmc_b_consume_prepare (&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_b_consume_prepare (&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_signals_set_tmatch (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op, exp;
  bl_u8*       mem;
  bl_err    err;

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0);
  err = bl_mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 0);

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0); /*incorrect signal*/
  err = bl_mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.own == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  exp = bl_mpmc_b_op_encode (2, 1); /*incorrect transaction*/
  err = bl_mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.own == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  err = bl_mpmc_b_producer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.own == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  assert_true (bl_mpmc_b_produce_prepare(&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_b_produce_prepare(&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_consumer_signals_set_tmatch (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op, exp;
  bl_u8*       mem;
  bl_err    err;

  bl_mpmc_b_test_queue_fill (state);

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0);
  err = bl_mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 0);

  exp = bl_mpmc_b_op_encode (bl_mpmc_b_first_op, 0); /*incorrect signal*/
  err = bl_mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.own == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  exp = bl_mpmc_b_op_encode (1, 1); /*incorrect transaction*/
  err = bl_mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.own == bl_preconditions);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  err = bl_mpmc_b_consumer_signal_try_set_tmatch (&c->q, &exp, 2);
  assert_true (err.own == bl_ok);
  assert_true (bl_mpmc_b_ticket_decode (exp) == bl_mpmc_b_first_op);
  assert_true (bl_mpmc_b_sig_decode (exp) == 1);

  assert_true (bl_mpmc_b_consume_prepare(&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  assert_true (bl_mpmc_b_consume_prepare(&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_fallback (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op  op;
  bl_u8*        mem;
  bl_mpmc_b_sig exp;
  bl_err     err;

  exp = 0;
  err = bl_mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);

  err = bl_mpmc_b_produce_prepare_fallback (&c->q, &op, &mem, (bl_mpmc_b_sig) -1, 1);
  assert_true (err.own == bl_preconditions);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_b_produce_prepare_fallback (&c->q, &op, &mem, (bl_mpmc_b_sig) -1, 2);
  assert_true (err.own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  err = bl_mpmc_b_produce_prepare_fallback (&c->q, &op, &mem, (bl_mpmc_b_sig) -1, 2);
  assert_true (err.own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_consumer_fallback (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op  op;
  bl_u8*        mem;
  bl_mpmc_b_sig exp;
  bl_err     err;

  bl_mpmc_b_test_queue_fill (state);

  exp = 0;
  err = bl_mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);

  err = bl_mpmc_b_consume_prepare_fallback(
    &c->q, &op, &mem, (bl_mpmc_b_sig) -1, 1
    );
  assert_true (err.own == bl_preconditions);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_b_consume_prepare_fallback(
    &c->q, &op, &mem, (bl_mpmc_b_sig) -1, 2
    );
  assert_true (err.own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);

  err = bl_mpmc_b_consume_prepare_fallback(
    &c->q, &op, &mem, (bl_mpmc_b_sig) -1, 2
    );
  assert_true (err.own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);
  assert_true (bl_mpmc_b_ticket_decode (op) == 1);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_signal_change (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op  op;
  bl_u8*        mem;
  bl_mpmc_b_sig exp;
  bl_err     err;

  exp = 0;
  err = bl_mpmc_b_producer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);

  err = bl_mpmc_b_produce_prepare_sig (&c->q, &op, &mem, 2);
  assert_true (err.own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_b_produce_prepare_sig (&c->q, &op, &mem, 3);
  assert_true (err.own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_consumer_signal_change (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op  op;
  bl_u8*        mem;
  bl_mpmc_b_sig exp;
  bl_err     err;

  bl_mpmc_b_test_queue_fill (state);

  exp = 0;
  err = bl_mpmc_b_consumer_signal_try_set (&c->q, &exp, 1);
  assert_true (err.own == bl_ok);

  err = bl_mpmc_b_consume_prepare_sig (&c->q, &op, &mem, 2);
  assert_true (err.own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 1);

  err = bl_mpmc_b_consume_prepare_sig (&c->q, &op, &mem, 3);
  assert_true (err.own == bl_ok);
  bl_mpmc_b_consume_commit (&c->q, op);
  assert_true (bl_mpmc_b_sig_decode (op) == 2);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_fifo_produce_empty_queue (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8*       mem;
  bl_err    err;

  bl_mpmc_b_fifo_produce_prepare (&c->q, &op);
  err = bl_mpmc_b_fifo_produce_prepare_is_ready (&c->q, op, &mem);
  assert_true (err.own == bl_ok);
  *((bl_mpmc_b_type*) mem) = 0;
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);
  bl_mpmc_b_produce_commit (&c->q, op);

  assert_true (bl_mpmc_b_consume_prepare (&c->q, &op, &mem).own == bl_ok);
  assert_true (*((bl_mpmc_b_type*) mem) == 0);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);
  assert_true (bl_mpmc_b_sig_decode (op) == 0);
  bl_mpmc_b_consume_commit (&c->q, op);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_fifo_produce_full_queue (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op, opfifo;
  bl_u8*       mem;
  bl_err    err;

  bl_mpmc_b_test_queue_fill (state);

  bl_mpmc_b_fifo_produce_prepare (&c->q, &opfifo);
  err = bl_mpmc_b_fifo_produce_prepare_is_ready (&c->q, opfifo, &mem);
  assert_true (err.own == bl_would_overflow);
  assert_true (bl_mpmc_b_ticket_decode (opfifo) == bl_arr_elems (c->buff));

  assert_true (bl_mpmc_b_consume_prepare (&c->q, &op, &mem).own == bl_ok);
  assert_true (*((bl_mpmc_b_type*) mem) == 0);
  assert_true (bl_mpmc_b_ticket_decode (op) == 0);
  assert_true (bl_mpmc_b_sig_decode (op) == 0);
  bl_mpmc_b_consume_commit (&c->q, op);

  err = bl_mpmc_b_fifo_produce_prepare_is_ready (&c->q, opfifo, &mem);
  assert_true (err.own == bl_ok);
  *((bl_mpmc_b_type*) mem) = bl_arr_elems (c->buff);
  bl_mpmc_b_produce_commit (&c->q, opfifo);

  for (bl_mpmc_b_type i = 1; i < bl_arr_elems (c->buff) + 1; ++i) {
    assert_true (bl_mpmc_b_consume_prepare(&c->q, &op, &mem).own == bl_ok);
    assert_true (*((bl_mpmc_b_type*) mem) == i);
    assert_true (bl_mpmc_b_ticket_decode (op) == i);
    assert_true (bl_mpmc_b_sig_decode (op) == 0);
    bl_mpmc_b_consume_commit (&c->q, op);
  }
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_block (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8*       mem;
  bl_err    err;

  assert_true (bl_mpmc_b_produce_prepare (&c->q, &op, &mem).own == bl_ok);
  bl_mpmc_b_produce_commit (&c->q, op);

  bl_mpmc_b_block_producers (&c->q);
  err = bl_mpmc_b_produce_prepare (&c->q, &op, &mem);
  assert_true (err.own == bl_locked);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_block_full_queue (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8*       mem;
  bl_err    err;

  bl_mpmc_b_test_queue_fill (state);

  bl_mpmc_b_block_producers (&c->q);
  err = bl_mpmc_b_produce_prepare (&c->q, &op, &mem);
  assert_true (err.own == bl_locked);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_block_fifo_produce (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op1, op2;
  bl_u8*       mem;
  bl_err    err;

  bl_mpmc_b_fifo_produce_prepare (&c->q, &op1);

  bl_mpmc_b_block_producers (&c->q);

  bl_mpmc_b_fifo_produce_prepare (&c->q, &op2);

  err = bl_mpmc_b_fifo_produce_prepare_is_ready (&c->q, op1, &mem);
  assert_true (err.own == bl_ok);

  err = bl_mpmc_b_fifo_produce_prepare_is_ready (&c->q, op2, &mem);
  assert_true (err.own == bl_locked);
}
/*---------------------------------------------------------------------------*/
static void bl_mpmc_b_producer_block_fifo_produce_full_queue (void **state)
{
  bl_mpmc_b_context* c = (bl_mpmc_b_context*) *state;
  bl_mpmc_b_op op;
  bl_u8*       mem;
  bl_err    err;

  bl_mpmc_b_test_queue_fill (state);

  bl_mpmc_b_fifo_produce_prepare (&c->q, &op);

  bl_mpmc_b_block_producers (&c->q);

  err = bl_mpmc_b_fifo_produce_prepare_is_ready (&c->q, op, &mem);
  assert_true (err.own == bl_locked);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (bl_mpmc_b_init_test, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_init_alloc_fail_test, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_init_too_small_test, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_init_too_big_test, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_mpw_mpr, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_spw_spr, bl_mpmc_b_test_setup),
#ifdef MPMC_B_TEST_WRAP /*take long to execute*/
  cmocka_unit_test_setup (bl_mpmc_b_mpw_mpr_wrap, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_spw_spr_wrap, bl_mpmc_b_test_setup),
#endif
  cmocka_unit_test_setup (bl_mpmc_b_producer_signals_set, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_consumer_signals_set, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup(
    bl_mpmc_b_producer_signals_set_tmatch, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_b_consumer_signals_set_tmatch, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup (bl_mpmc_b_producer_fallback, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup (bl_mpmc_b_consumer_fallback, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup(
    bl_mpmc_b_producer_signal_change, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_b_consumer_signal_change, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_b_fifo_produce_full_queue, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_b_fifo_produce_empty_queue, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup (bl_mpmc_b_producer_block, bl_mpmc_b_test_setup),
  cmocka_unit_test_setup(
    bl_mpmc_b_producer_block_full_queue, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_b_producer_block_fifo_produce, bl_mpmc_b_test_setup
    ),
  cmocka_unit_test_setup(
    bl_mpmc_b_producer_block_fifo_produce_full_queue, bl_mpmc_b_test_setup
    ),
};
/*---------------------------------------------------------------------------*/
int bl_mpmc_b_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
