#ifndef __BL_C11_ATOMIC_H__
#define __BL_C11_ATOMIC_H__

#ifndef __BL_ATOMIC_H__
  #error "don't include this file directly, use <hdr/atomic.h> instead"
#endif

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/alignment.h>
#include <stdatomic.h>
/*---------------------------------------------------------------------------*/
typedef atomic_uintptr_t bl_atomic_uword;
bl_static_assert_ns(
  sizeof (bl_atomic_uword) == sizeof (bl_uword)
  );
bl_static_assert_ns(
  bl_alignof (bl_atomic_uword) == bl_alignof (bl_uword)
  );
bl_static_assert_ns (ATOMIC_POINTER_LOCK_FREE == 2);
/*---------------------------------------------------------------------------*/
typedef atomic_intptr_t bl_atomic_word;
bl_static_assert_ns(
  sizeof (bl_atomic_word) == sizeof (bl_word)
  );
bl_static_assert_ns(
  bl_alignof (bl_atomic_word) == bl_alignof (bl_word)
  );
bl_static_assert_ns (ATOMIC_POINTER_LOCK_FREE == 2);
/*---------------------------------------------------------------------------*/
typedef atomic_uint bl_atomic_u32;
bl_static_assert_ns(
  sizeof (bl_atomic_u32) == sizeof (bl_u32)
  );
bl_static_assert_ns(
  bl_alignof (bl_atomic_u32) == bl_alignof (bl_u32)
  );
bl_static_assert_ns (ATOMIC_INT_LOCK_FREE == 2);
/*---------------------------------------------------------------------------*/
typedef atomic_ulong bl_atomic_u64;
bl_static_assert_ns(
  sizeof (bl_atomic_u64) == sizeof (bl_u64)
  );
bl_static_assert_ns(
  bl_alignof (bl_atomic_u64) == bl_alignof (bl_u64)
  );
bl_static_assert_ns (ATOMIC_LONG_LOCK_FREE == 2);
/*---------------------------------------------------------------------------*/
typedef atomic_uint bl_atomic_i32;
bl_static_assert_ns(
  sizeof (bl_atomic_i32) == sizeof (bl_i32)
  );
bl_static_assert_ns(
  bl_alignof (bl_atomic_i32) == bl_alignof (bl_i32)
  );
bl_static_assert_ns (ATOMIC_INT_LOCK_FREE == 2);
/*---------------------------------------------------------------------------*/
typedef atomic_ulong bl_atomic_i64;
bl_static_assert_ns(
  sizeof (bl_atomic_i64) == sizeof (bl_i64)
  );
bl_static_assert_ns(
  bl_alignof (bl_atomic_i64) == bl_alignof (bl_i64)
  );
bl_static_assert_ns (ATOMIC_LONG_LOCK_FREE == 2);
/*---------------------------------------------------------------------------*/
typedef bl_atomic_u32 bl_atomic_float;
typedef bl_atomic_u64 bl_atomic_double;
/*---------------------------------------------------------------------------*/
typedef union {
  bl_u32 i;
  float  f;
}
bl_u32_float_union;
/*---------------------------------------------------------------------------*/
typedef union {
  bl_u64 i;
  double f;
}
bl_u64_double_union;
/*---------------------------------------------------------------------------*/
#define bl_mo_relaxed  memory_order_relaxed
#define bl_mo_consume  memory_order_consume
#define bl_mo_acquire  memory_order_acquire
#define bl_mo_release  memory_order_release
#define bl_mo_acq_rel  memory_order_acq_rel
#define bl_mo_seq_cst  memory_order_seq_cst

typedef memory_order bl_mem_order;
/*---------------------------------------------------------------------------*/
/* bl_uword */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_uword_store(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_load(
  volatile bl_atomic_uword* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_exchange(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order mo
  )
{
  return atomic_exchange_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_uword_strong_cas(
  volatile bl_atomic_uword* a,
  bl_uword*                 expected,
  bl_uword                  desired,
  bl_mem_order              success,
  bl_mem_order              failure
  )
{
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_uword_weak_cas(
  volatile bl_atomic_uword* a,
  bl_uword*                 expected,
  bl_uword                  desired,
  bl_mem_order              success,
  bl_mem_order              failure
  )
{
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_add(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_sub(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_or(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_xor(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_and(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_word */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_word_store(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_load(
  volatile bl_atomic_word* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_exchange(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order mo
  )
{
  return atomic_exchange_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_word_strong_cas(
  volatile bl_atomic_word* a,
  bl_word*                 expected,
  bl_word                  desired,
  bl_mem_order             success,
  bl_mem_order             failure
  )
{
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_word_weak_cas(
  volatile bl_atomic_word* a,
  bl_word*                 expected,
  bl_word                  desired,
  bl_mem_order             success,
  bl_mem_order             failure
  )
{
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_add(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_sub(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_or(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_word_fetch_xor(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_word_fetch_and(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_u32 */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_u32_store(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_load(
  volatile bl_atomic_u32* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_exchange(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order mo
  )
{
  return atomic_exchange_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_u32_strong_cas(
  volatile bl_atomic_u32* a,
  bl_u32*                 expected,
  bl_u32                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_u32_weak_cas(
  volatile bl_atomic_u32* a,
  bl_u32*                 expected,
  bl_u32                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_add(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_sub(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_or(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_xor(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_and(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_u64 */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_u64_store(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_load(
  volatile bl_atomic_u64* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_exchange(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order mo
  )
{
  return atomic_exchange_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_u64_strong_cas(
  volatile bl_atomic_u64* a,
  bl_u64*                 expected,
  bl_u64                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_u64_weak_cas(
  volatile bl_atomic_u64* a,
  bl_u64*                 expected,
  bl_u64                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_add(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_sub(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_or(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_xor(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_and(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_i32 */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_i32_store(
  volatile bl_atomic_i32* a, bl_i32 v, bl_mem_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_i32 bl_atomic_i32_load(
  volatile bl_atomic_i32* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_i32 bl_atomic_i32_exchange(
  volatile bl_atomic_i32* a, bl_i32 v, bl_mem_order mo
  )
{
  return atomic_exchange_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_i32_strong_cas(
  volatile bl_atomic_i32* a,
  bl_i32*                 expected,
  bl_i32                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_i32_weak_cas(
  volatile bl_atomic_i32* a,
  bl_i32*                 expected,
  bl_i32                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_i32 bl_atomic_i32_fetch_add(
  volatile bl_atomic_i32* a, bl_i32 v, bl_mem_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i32 bl_atomic_i32_fetch_sub(
  volatile bl_atomic_i32* a, bl_i32 v, bl_mem_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i32 bl_atomic_i32_fetch_or(
  volatile bl_atomic_i32* a, bl_i32 v, bl_mem_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i32 bl_atomic_i32_fetch_xor(
  volatile bl_atomic_i32* a, bl_i32 v, bl_mem_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i32 bl_atomic_i32_fetch_and(
  volatile bl_atomic_i32* a, bl_i32 v, bl_mem_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_i64 */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_i64_store(
  volatile bl_atomic_i64* a, bl_i64 v, bl_mem_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_i64 bl_atomic_i64_load(
  volatile bl_atomic_i64* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_i64 bl_atomic_i64_exchange(
  volatile bl_atomic_i64* a, bl_i64 v, bl_mem_order mo
  )
{
  return atomic_exchange_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_i64_strong_cas(
  volatile bl_atomic_i64* a,
  bl_i64*                 expected,
  bl_i64                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_i64_weak_cas(
  volatile bl_atomic_i64* a,
  bl_i64*                 expected,
  bl_i64                  desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_i64 bl_atomic_i64_fetch_add(
  volatile bl_atomic_i64* a, bl_i64 v, bl_mem_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i64 bl_atomic_i64_fetch_sub(
  volatile bl_atomic_i64* a, bl_i64 v, bl_mem_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i64 bl_atomic_i64_fetch_or(
  volatile bl_atomic_i64* a, bl_i64 v, bl_mem_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i64 bl_atomic_i64_fetch_xor(
  volatile bl_atomic_i64* a, bl_i64 v, bl_mem_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_i64 bl_atomic_i64_fetch_and(
  volatile bl_atomic_i64* a, bl_i64 v, bl_mem_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* float */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_float_store(
  volatile bl_atomic_float* a, float v, bl_mem_order mo
  )
{
  bl_u32_float_union c;
  c.f = v;
  atomic_store_explicit (a, c.i, mo);
}
/*---------------------------------------------------------------------------*/
static inline float bl_atomic_float_load(
  volatile bl_atomic_float* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline float bl_atomic_float_exchange(
  volatile bl_atomic_float* a, float v, bl_mem_order mo
  )
{
  bl_u32_float_union c;
  c.f = v;
  return atomic_exchange_explicit (a, c.i, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_float_strong_cas(
  volatile bl_atomic_float* a,
  float*                    expected,
  float                     desired,
  bl_mem_order              success,
  bl_mem_order              failure
  )
{
  bl_u32_float_union d;
  d.f = desired;
  return atomic_compare_exchange_strong_explicit(
    a, (bl_u32*) expected, d.i, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_float_weak_cas(
  volatile bl_atomic_float* a,
  float*                    expected,
  float                     desired,
  bl_mem_order              success,
  bl_mem_order              failure
  )
{
  bl_u32_float_union d;
  d.f = desired;
  return atomic_compare_exchange_weak_explicit(
    a, (bl_u32*) expected, d.i, success, failure
    );
}
/*---------------------------------------------------------------------------*/
/* double */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_double_store(
  volatile bl_atomic_double* a, double v, bl_mem_order mo
  )
{
  bl_u64_double_union c;
  c.f = v;
  atomic_store_explicit (a, c.i, mo);
}
/*---------------------------------------------------------------------------*/
static inline double bl_atomic_double_load(
  volatile bl_atomic_double* a, bl_mem_order mo
  )
{
  return atomic_load_explicit (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline double bl_atomic_double_exchange(
  volatile bl_atomic_double* a, double v, bl_mem_order mo
  )
{
  bl_u64_double_union c;
  c.f = v;
  return atomic_exchange_explicit (a, c.i, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_double_strong_cas(
  volatile bl_atomic_double* a,
  double*                    expected,
  double                     desired,
  bl_mem_order               success,
  bl_mem_order               failure
  )
{
  bl_u64_double_union d;
  d.f = desired;
  return atomic_compare_exchange_strong_explicit(
    a, (bl_u64*) expected, d.i, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_double_weak_cas(
  volatile bl_atomic_double* a,
  double*                    expected,
  double                     desired,
  bl_mem_order               success,
  bl_mem_order               failure
  )
{
  bl_u64_double_union d;
  d.f = desired;
  return atomic_compare_exchange_weak_explicit(
    a, (bl_u64*) expected, d.i, success, failure
    );
}
/*---------------------------------------------------------------------------*/
/* Fences */
/*---------------------------------------------------------------------------*/
static inline void atomic_fence (bl_mem_order o)
{
  __atomic_thread_fence (o);
}
/*---------------------------------------------------------------------------*/
static inline void atomic_sigfence (bl_mem_order o)
{
  __atomic_signal_fence (o);
}
/*---------------------------------------------------------------------------*/

#endif /*__BL_C11_ATOMIC_H__*/
