#ifndef __BL_ATOMIC_RELACY_H__
#define __BL_ATOMIC_RELACY_H__

#ifndef __BL_ATOMIC_H__
  #error "don't include this file directly, use <hdr/atomic.h> instead"
#endif

#ifndef __cplusplus
  #error "this file is being used by a non-C++ compiler"
#endif
/*---------------------------------------------------------------------------*/
#include <relacy/relacy.hpp>

#include <bl/base/platform.h>
#include <bl/base/integer.h>
/*---------------------------------------------------------------------------*/
extern "C" {
/*---------------------------------------------------------------------------*/
typedef rl::atomic<bl_word>  bl_atomic_word;
typedef rl::atomic<bl_uword> bl_atomic_uword;
typedef rl::atomic<bl_u32>   bl_atomic_u32;
typedef rl::atomic<bl_u64>   bl_atomic_u64;
/*---------------------------------------------------------------------------*/
#define bl_mo_relaxed rl::mo_relaxed
#define bl_mo_consume rl::mo_consume
#define bl_mo_acquire rl::mo_acquire
#define bl_mo_release rl::mo_release
#define bl_mo_acq_rel rl::mo_acq_rel
#define bl_mo_seq_cst rl::mo_seq_cst

typedef rl::memory_order bl_memory_order;
typedef rl::memory_order bl_mem_order;

/*---------------------------------------------------------------------------*/
/* bl_uword */
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_store(a, v, o) \
  bl_atomic_uword_store_impl ((a), (v), (o), $)

static inline void bl_atomic_uword_store_impl(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<bl_uword>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_load(a, o) bl_atomic_uword_load_impl ((a), (o), $)

static inline bl_uword bl_atomic_uword_load_impl(
  volatile bl_atomic_uword* a, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_uword>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_exchange(a, v, o)\
  bl_atomic_uword_exchange_impl ((a), (v), (o), $)

static inline bl_uword bl_atomic_uword_exchange_impl(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_uword>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_strong_cas(a, e, d, s, f)\
  bl_atomic_uword_strong_cas_impl ((a), (e), (d), (s), $, (f), $)

static inline bool bl_atomic_uword_strong_cas_impl(
  volatile bl_atomic_uword* a,
  bl_uword*                 expected,
  bl_uword                  desired,
  bl_mem_order              success,
  bl_mem_order              failure,
  rl::debug_info_param      di
  )
{
  return ((rl::atomic<bl_uword>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_weak_cas(a, e, d, s, f)\
  bl_atomic_uword_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool bl_atomic_uword_weak_cas_impl(
  volatile bl_atomic_uword* a,
  bl_uword*                 expected,
  bl_uword                  desired,
  bl_mem_order              success,
  bl_mem_order              failure,
  rl::debug_info_param      di
  )
{
  return ((rl::atomic<bl_uword>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_fetch_add(a, v, o)\
  bl_atomic_uword_fetch_add_impl ((a), (v), (o), $)

static inline bl_uword bl_atomic_uword_fetch_add_impl(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_uword>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_fetch_sub(a, v, o)\
  bl_atomic_uword_fetch_sub_impl ((a), (v), (o), $)


static inline bl_uword bl_atomic_uword_fetch_sub_impl(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_uword>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_fetch_or(a, v, o)\
  bl_atomic_uword_fetch_or_impl ((a), (v), (o), $)

static inline bl_uword bl_atomic_uword_fetch_or_impl(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_uword>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_fetch_xor(a, v, o)\
  bl_atomic_uword_fetch_xor_impl ((a), (v), (o), $)

static inline bl_uword bl_atomic_uword_fetch_xor_impl(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_uword>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_fetch_and(a, v, o)\
  bl_atomic_uword_fetch_and_impl ((a), (v), (o), $)

static inline bl_uword bl_atomic_uword_fetch_and_impl(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_uword>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* bl_word */
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_store(a, v, o)\
  bl_atomic_word_store_impl ((a), (v), (o), $)

static inline void bl_atomic_word_store_impl(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<bl_word>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_load(a, o) bl_atomic_word_load_impl ((a), (o), $)

static inline bl_word bl_atomic_word_load_impl(
  volatile bl_atomic_word* a, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_word>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_exchange(a, v, o)\
  bl_atomic_word_exchange_impl ((a), (v), (o), $)

static inline bl_word bl_atomic_word_exchange_impl(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_word>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_strong_cas(a, e, d, s, f)\
  bl_atomic_word_strong_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool bl_atomic_word_strong_cas_impl(
  volatile bl_atomic_word* a,
  bl_word*                 expected,
  bl_word                  desired,
  bl_mem_order             success,
  bl_mem_order             failure,
  rl::debug_info_param     di
  )
{
  return ((rl::atomic<bl_word>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_weak_cas(a, e, d, s, f)\
  bl_atomic_word_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool bl_atomic_word_weak_cas_impl(
  volatile bl_atomic_word* a,
  bl_word*                 expected,
  bl_word                  desired,
  bl_mem_order             success,
  bl_mem_order             failure,
  rl::debug_info_param     di
  )
{
  return ((rl::atomic<bl_word>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_fetch_add(a, v, o)\
  bl_atomic_word_fetch_add_impl ((a), (v), (o), $)

static inline bl_word bl_atomic_word_fetch_add_impl(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_word>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_fetch_sub(a, v, o)\
  bl_atomic_word_fetch_sub_impl ((a), (v), (o), $)


static inline bl_word bl_atomic_word_fetch_sub_impl(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_word>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_fetch_or(a, v, o)\
  bl_atomic_word_fetch_or_impl ((a), (v), (o), $)

static inline bl_word bl_atomic_word_fetch_or_impl(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_word>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_fetch_xor(a, v, o)\
  bl_atomic_word_fetch_xor_impl ((a), (v), (o), $)

static inline bl_word bl_atomic_word_fetch_xor_impl(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_word>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_fetch_and(a, v, o)\
  bl_atomic_word_fetch_and_impl ((a), (v), (o), $)

static inline bl_word bl_atomic_word_fetch_and_impl(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_word>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* bl_u32 */
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_store(a, v, o)\
  bl_atomic_u32_store_impl ((a), (v), (o), $)

static inline void bl_atomic_u32_store_impl(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<bl_u32>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_load(a, o) bl_atomic_u32_load_impl ((a), (o), $)

static inline bl_u32 bl_atomic_u32_load_impl(
  volatile bl_atomic_u32* a, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u32>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_exchange(a, v, o)\
  bl_atomic_u32_exchange_impl ((a), (v), (o), $)

static inline bl_u32 bl_atomic_u32_exchange_impl(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u32>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_strong_cas(a, e, d, s, f)\
  bl_atomic_u32_strong_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool bl_atomic_u32_strong_cas_impl(
  volatile bl_atomic_u32* a,
  bl_u32*                 expected,
  bl_u32                  desired,
  bl_mem_order            success,
  bl_mem_order            failure,
  rl::debug_info_param    di
  )
{
  return ((rl::atomic<bl_u32>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_weak_cas(a, e, d, s, f)\
  bl_atomic_u32_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool bl_atomic_u32_weak_cas_impl(
  volatile bl_atomic_u32* a,
  bl_u32*                 expected,
  bl_u32                  desired,
  bl_mem_order            success,
  bl_mem_order            failure,
  rl::debug_info_param    di
  )
{
  return ((rl::atomic<bl_u32>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_fetch_add(a, v, o)\
  bl_atomic_u32_fetch_add_impl ((a), (v), (o), $)

static inline bl_u32 bl_atomic_u32_fetch_add_impl(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u32>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_fetch_sub(a, v, o)\
  bl_atomic_u32_fetch_sub_impl ((a), (v), (o), $)


static inline bl_u32 bl_atomic_u32_fetch_sub_impl(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u32>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_fetch_or(a, v, o)\
  bl_atomic_u32_fetch_or_impl ((a), (v), (o), $)

static inline bl_u32 bl_atomic_u32_fetch_or_impl(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u32>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_fetch_xor(a, v, o)\
  bl_atomic_u32_fetch_xor_impl ((a), (v), (o), $)

static inline bl_u32 bl_atomic_u32_fetch_xor_impl(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u32>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_fetch_and(a, v, o)\
  bl_atomic_u32_fetch_and_impl ((a), (v), (o), $)

static inline bl_u32 bl_atomic_u32_fetch_and_impl(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u32>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* bl_u64 */
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_store(a, v, o) bl_atomic_u64_store_impl ((a), (v), (o), $)

static inline void bl_atomic_u64_store_impl(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<bl_u64>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_load(a, o) bl_atomic_u64_load_impl ((a), (o), $)

static inline bl_u64 bl_atomic_u64_load_impl(
  volatile bl_atomic_u64* a, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u64>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_exchange(a, v, o)\
  bl_atomic_u64_exchange_impl ((a), (v), (o), $)

static inline bl_u64 bl_atomic_u64_exchange_impl(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u64>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_strong_cas(a, e, d, s, f)\
  bl_atomic_u64_strong_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool bl_atomic_u64_strong_cas_impl(
  volatile bl_atomic_u64* a,
  bl_u64*                 expected,
  bl_u64                  desired,
  bl_mem_order            success,
  bl_mem_order            failure,
  rl::debug_info_param    di
  )
{
  return ((rl::atomic<bl_u64>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_weak_cas(a, e, d, s, f)\
  bl_atomic_u64_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool bl_atomic_u64_weak_cas_impl(
  volatile bl_atomic_u64* a,
  bl_u64*                 expected,
  bl_u64                  desired,
  bl_mem_order            success,
  bl_mem_order            failure,
  rl::debug_info_param    di
  )
{
  return ((rl::atomic<bl_u64>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_fetch_add(a, v, o)\
  bl_atomic_u64_fetch_add_impl ((a), (v), (o), $)

static inline bl_u64 bl_atomic_u64_fetch_add_impl(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u64>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_fetch_sub(a, v, o)\
  bl_atomic_u64_fetch_sub_impl ((a), (v), (o), $)


static inline bl_u64 bl_atomic_u64_fetch_sub_impl(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u64>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_fetch_or(a, v, o)\
  bl_atomic_u64_fetch_or_impl ((a), (v), (o), $)

static inline bl_u64 bl_atomic_u64_fetch_or_impl(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u64>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_fetch_xor(a, v, o)\
  bl_atomic_u64_fetch_xor_impl ((a), (v), (o), $)

static inline bl_u64 bl_atomic_u64_fetch_xor_impl(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u64>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_fetch_and(a, v, o)\
  bl_atomic_u64_fetch_and_impl ((a), (v), (o), $)

static inline bl_u64 bl_atomic_u64_fetch_and_impl(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<bl_u64>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* Fences */
/*---------------------------------------------------------------------------*/
#define atomic_fence(mo) atomic_fence_impl (mo, $)
static inline void atomic_fence_impl (bl_mem_order o, rl::debug_info_param di)
{
  rl::atomic_thread_fence (o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_sigfence(mo) atomic_singfence_impl (mo, $)
static inline void atomic_sigfence_impl(
  bl_mem_order o, rl::debug_info_param di
  )
{
  rl::atomic_signal_fence (o, di);
}
/*---------------------------------------------------------------------------*/
} //extern "C" {

#endif
