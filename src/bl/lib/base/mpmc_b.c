/*
This file is a slight modification of the queue on the link below.
http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue

Covered by the license below.
*/
/*------------------------------------------------------------------------------
is licensed by Dmitry Vyukov under the terms below:
BSD 2-clause license
Copyright (c) 2010-2011 Dmitry Vyukov. All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY DMITRY VYUKOV "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL DMITRY VYUKOV OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Dmitry Vyukov.
/*---------------------------------------------------------------------------*/
#include <string.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer_math.h>
#include <bl/hdr/base/integer_manipulation.h>
#include <bl/lib/base/mpmc_b.h>
/*---------------------------------------------------------------------------*/
static_assert_outside_func(
  bl_has_two_comp_arithmetic,
  "this algorithm is just valid on systems with complement of two arithmetic"
  );
/*---------------------------------------------------------------------------*/
typedef union mpmc_b_i {
  mpmc_b_info inf;
  uword       raw;
}
mpmc_b_i;
/*---------------------------------------------------------------------------*/
static inline atomic_u32* mpmc_b_gate_ptr (mpmc_b* q, u8* join_data_ptr)
{
  return (atomic_u32*) (join_data_ptr + q->gate_offset);
}
/*---------------------------------------------------------------------------*/
static inline u8* mpmc_b_mem_content_ptr (mpmc_b* q, u8* join_data_ptr)
{
  return (u8*) join_data_ptr;
}
/*---------------------------------------------------------------------------*/
static inline u8* mpmc_b_join_data_ptr (mpmc_b* q, uword position)
{
  return q->buffer + ((position & q->buffer_mask) * q->join_size);
}
/*---------------------------------------------------------------------------*/
static inline void mpmc_b_write(
    mpmc_b* q, u32 gate, u8* join_data, const void* value
    )
{
  memcpy (mpmc_b_mem_content_ptr (q, join_data), value, q->contained_size);
  atomic_u32_store (mpmc_b_gate_ptr (q, join_data), gate, mo_release);
}
/*---------------------------------------------------------------------------*/
static inline void mpmc_b_read(
    mpmc_b* q, u32 gate, u8* join_data, void* value
    )
{
  memcpy (value, mpmc_b_mem_content_ptr (q, join_data), q->contained_size);
  atomic_u32_store (mpmc_b_gate_ptr (q, join_data), gate, mo_release);
}
/*---------------------------------------------------------------------------*/
/*returns:
  -negative if the insert position is occupied
  -positive if the producer gate looks outdated (someone inserting)
  -zero if everything looks right
*/
#if defined (BL32)
/*
  on 32 bit the transactions are 24 bit, so to calculate the status using the
  complement of two properties of the integer we must shift so the MSB of the
  transaction/gate maps to the MSB of a 32 bit integer (the sign).
*/
static_assert_outside_func_ns (mpmc_b_info_transaction_bits < type_bits (i32));
static inline word mpmc_b_transaction_status (u32 gate, mpmc_b_i now)
{
  u32 gate_u  = (gate << mpmc_b_info_signal_bits);
  u32 trans_u = ((u32) now.inf.transaction) << mpmc_b_info_signal_bits;
  /*assert ((gate_u - trans_u) <= compare_unsigned_as_signed_max_diff (u32));*/
  return (i32) (gate_u - trans_u);
}
/*---------------------------------------------------------------------------*/
#elif defined (BL64)
static_assert_outside_func_ns (mpmc_b_info_transaction_bits == type_bits (i32));
static inline word mpmc_b_transaction_status (u32 gate, mpmc_b_i now)
{
  /*assert ((gate_u - trans_u) <= compare_unsigned_as_signed_max_diff (u32));*/
  return (i32) (gate -((u32) now.inf.transaction));
}
#endif
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_produce_sig_fallback(
  mpmc_b*      q,
  mpmc_b_info* inf,
  const void*  value,
  bool         replace_sig,
  mpmc_b_sig   sig_replacement,
  mpmc_b_sig   sig_fallback_mask,
  mpmc_b_sig   sig_fallback_match
  )
{
  assert (q && inf && value && q->buffer);
  u8*      join_data;
  mpmc_b_i now;
  mpmc_b_i next;

  now.raw = atomic_uword_load_rlx (&q->i_producer);

  while (1) {
    word status;
    u32  gate;

    *inf = now.inf;
    if ((now.inf.signal & sig_fallback_mask) == sig_fallback_match) {
      return bl_preconditions;
    }
    join_data  = mpmc_b_join_data_ptr (q, now.inf.transaction);
    gate       = atomic_u32_load (mpmc_b_gate_ptr (q, join_data), mo_acquire);
    status     = mpmc_b_transaction_status (gate, now);

    if (status == 0) {
      next.inf.transaction = now.inf.transaction + 1;
      next.inf.signal      = replace_sig ? sig_replacement : now.inf.signal;
      if (atomic_uword_weak_cas_rlx (&q->i_producer, &now.raw, next.raw)) {
        break;
      }
    }
    else if (status < 0) {
      return bl_would_overflow;
    }
    else {
      now.raw = atomic_uword_load_rlx (&q->i_producer);
    }
  }
  mpmc_b_write (q, (u32) next.inf.transaction, join_data, value);
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_produce_single_p(
  mpmc_b* q, mpmc_b_info* inf, const void* value
  )
{
  assert (q->buffer && inf && value);
  u8*      join_data;
  mpmc_b_i now;
  u32      gate;

  now.raw     = atomic_uword_load_rlx (&q->i_producer);
  join_data   = mpmc_b_join_data_ptr (q, now.inf.transaction);
  gate        = atomic_u32_load (mpmc_b_gate_ptr (q, join_data), mo_acquire);
  word status = mpmc_b_transaction_status (gate, now);

  if (status == 0) {
    *inf = now.inf;
    ++now.inf.transaction;
    now.inf.signal = 0;
    mpmc_b_write (q, (u32) now.inf.transaction, join_data, value);
    atomic_uword_store_rlx (&q->i_producer, now.raw);
    return bl_ok;
  }
  assert (status < 0);
  return bl_would_overflow;
}
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_consume_sig_fallback(
  mpmc_b*      q,
  mpmc_b_info* inf,
  void*        value,
  bool         replace_sig,
  mpmc_b_sig   sig_replacement,
  mpmc_b_sig   sig_fallback_mask,
  mpmc_b_sig   sig_fallback_match
  )
{
  assert (q && inf && value && q->buffer);
  u8*      join_data;
  mpmc_b_i now;
  mpmc_b_i next;

  now.raw = atomic_uword_load_rlx (&q->i_consumer);

  while (1) {
    u32 gate;

    *inf = now.inf;
    if ((now.inf.signal & sig_fallback_mask) == sig_fallback_match) {
      return bl_preconditions;
    }
    join_data              = mpmc_b_join_data_ptr (q, now.inf.transaction);
    gate                   =
      atomic_u32_load (mpmc_b_gate_ptr (q, join_data), mo_acquire);
    next.inf.transaction   = now.inf.transaction + 1;
    next.inf.signal        = replace_sig ? sig_replacement : now.inf.signal;
    word status            = mpmc_b_transaction_status (gate, next);

    if (status == 0) {
      if (atomic_uword_weak_cas_rlx (&q->i_consumer, &now.raw, next.raw)) {
        break;
      }
    }
    else if (status < 0) {
      return bl_empty;
    }
    else {
      now.raw = atomic_uword_load_rlx (&q->i_consumer);
    }
  }
  next.inf.transaction += q->buffer_mask;
  mpmc_b_read (q, (u32) next.inf.transaction, join_data, value);
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_consume_single_c (mpmc_b* q, mpmc_b_info* inf, void* value)
{
  assert (q->buffer && inf && value);
  u8*      join_data;
  mpmc_b_i now;
  u32      gate;

  now.raw    = atomic_uword_load_rlx (&q->i_consumer);
  join_data  = mpmc_b_join_data_ptr (q, now.inf.transaction);
  gate       = atomic_u32_load (mpmc_b_gate_ptr (q, join_data), mo_acquire);
  *inf       = now.inf;
  ++now.inf.transaction;
  word status = mpmc_b_transaction_status (gate, now);

  if (status == 0) {
    now.inf.signal       = 0;
    atomic_uword_store_rlx (&q->i_consumer, now.raw);
    now.inf.transaction += q->buffer_mask;
    mpmc_b_read (q, now.inf.transaction, join_data, value);
    return bl_ok;
  }
  assert (status < 0);
  return bl_empty;
}
/*---------------------------------------------------------------------------*/
void BL_EXPORT mpmc_b_destroy (mpmc_b* q, const alloc_tbl* alloc)
{
  if (q->buffer) {
    bl_deallocate (alloc, q->buffer);
  }
}
/*---------------------------------------------------------------------------*/
static inline bl_err mpmc_b_signal(
  atomic_uword* dst, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  mpmc_b_i r;
  mpmc_b_i w;
  bl_err err = bl_ok;
  r.raw      = atomic_uword_load_rlx (dst);
  do {
    if (r.inf.signal != *expected) {
      err = bl_preconditions;
      break;
    }
    w            = r;
    w.inf.signal = desired;
  }
  while (!atomic_uword_weak_cas_rlx (dst, &r.raw, w.raw));
  *expected = r.inf.signal;
  return err;
}
/*---------------------------------------------------------------------------*/
static inline bl_err mpmc_b_signal_trans(
  atomic_uword* dst, mpmc_b_info* expected, mpmc_b_sig desired
  )
{
  mpmc_b_i r;
  mpmc_b_i w;
  mpmc_b_i exp;
  bl_err   err;

  err     = bl_preconditions;
  exp.inf = *expected;
  r.raw   = atomic_uword_load_rlx (dst);

  if (r.raw != exp.raw) {
    goto save_expected;
  }
  w            = r;
  w.inf.signal = desired;
  if (atomic_uword_weak_cas_rlx (dst, &r.raw, w.raw)) {
    err = bl_ok;
  }
save_expected:
  *expected = r.inf;
  return err;
}
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_producer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal (&q->i_producer, expected, desired);
}
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_consumer_signal_try_set(
  mpmc_b* q, mpmc_b_sig* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal (&q->i_consumer, expected, desired);

/*---------------------------------------------------------------------------*/}
bl_err BL_EXPORT mpmc_b_producer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_info* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_trans (&q->i_producer, expected, desired);
}
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_consumer_signal_try_set_tmatch(
  mpmc_b* q, mpmc_b_info* expected, mpmc_b_sig desired
  )
{
  return mpmc_b_signal_trans (&q->i_consumer, expected, desired);
}
/*---------------------------------------------------------------------------*/
bl_err BL_EXPORT mpmc_b_init_private(
  mpmc_b*          q,
  const alloc_tbl* alloc,
  uword            buffer_size,
  uword            join_size,
  uword            contained_size,
  uword            gate_offset
  )
{
  assert (join_size > contained_size);
  assert (gate_offset >= sizeof (u32));

  buffer_size = round_next_pow2_u (buffer_size);
  if (buffer_size < 2 ||
      buffer_size > pow2_u (mpmc_b_info_transaction_bits - 1)) {
    return bl_invalid;
  }

  uword bytes = (join_size * buffer_size);
  q->buffer   = bl_allocate (alloc, bytes);
  if (!q->buffer) {
    return bl_alloc;
  }
  q->buffer_mask    = buffer_size - 1;
  q->join_size      = join_size;
  q->contained_size = contained_size;
  q->gate_offset    = gate_offset;
  u8* ptr           = q->buffer;

  mpmc_b_i v;
  v.raw = 0;
  while (v.inf.transaction < buffer_size) {
    atomic_u32_store_rlx ( mpmc_b_gate_ptr (q, ptr), v.inf.transaction);
    ptr += q->join_size;
    ++v.inf.transaction;
  }
  atomic_uword_store_rlx (&q->i_producer, 0);
  atomic_uword_store_rlx (&q->i_consumer, 0);
  return bl_ok;
}
/*---------------------------------------------------------------------------*/

