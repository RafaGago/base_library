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
typedef rl::atomic<word>  atomic_word;
typedef rl::atomic<uword> atomic_uword;
typedef rl::atomic<u32>   atomic_u32;
typedef rl::atomic<u64>   atomic_u64;
/*---------------------------------------------------------------------------*/
#define mo_relaxed rl::mo_relaxed
#define mo_consume rl::mo_consume
#define mo_acquire rl::mo_acquire
#define mo_release rl::mo_release
#define mo_acq_rel rl::mo_acq_rel
#define mo_seq_cst rl::mo_seq_cst

typedef rl::memory_order memory_order;
typedef rl::memory_order mem_order;

/*---------------------------------------------------------------------------*/
/* uword */
/*---------------------------------------------------------------------------*/
#define atomic_uword_store(a, v, o) atomic_uword_store_impl ((a), (v), (o), $)

static inline void atomic_uword_store_impl(
  volatile atomic_uword* a, uword v, mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<uword>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_load(a, o) atomic_uword_load_impl ((a), (o), $)

static inline uword atomic_uword_load_impl(
  volatile atomic_uword* a, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<uword>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_exchange(a, v, o)\
  atomic_uword_exchange_impl ((a), (v), (o), $)

static inline uword atomic_uword_exchange_impl(
  volatile atomic_uword* a, uword v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<uword>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_strong_cas(a, e, d, s, f)\
  atomic_uword_strong_cas_impl ((a), (e), (d), (s), $, (f), $)

static inline bool atomic_uword_strong_cas_impl(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              failure,
  rl::debug_info_param   di
  )
{
  return ((rl::atomic<uword>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_weak_cas(a, e, d, s, f)\
  atomic_uword_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool atomic_uword_weak_cas_impl(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              failure,
  rl::debug_info_param   di
  )
{
  return ((rl::atomic<uword>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_fetch_add(a, v, o)\
  atomic_uword_fetch_add_impl ((a), (v), (o), $)

static inline uword atomic_uword_fetch_add_impl(
  volatile atomic_uword* a, uword v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<uword>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_fetch_sub(a, v, o)\
  atomic_uword_fetch_sub_impl ((a), (v), (o), $)


static inline uword atomic_uword_fetch_sub_impl(
  volatile atomic_uword* a, uword v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<uword>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_fetch_or(a, v, o)\
  atomic_uword_fetch_or_impl ((a), (v), (o), $)

static inline uword atomic_uword_fetch_or_impl(
  volatile atomic_uword* a, uword v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<uword>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_fetch_xor(a, v, o)\
  atomic_uword_fetch_xor_impl ((a), (v), (o), $)

static inline uword atomic_uword_fetch_xor_impl(
  volatile atomic_uword* a, uword v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<uword>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_uword_fetch_and(a, v, o)\
  atomic_uword_fetch_and_impl ((a), (v), (o), $)

static inline uword atomic_uword_fetch_and_impl(
  volatile atomic_uword* a, uword v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<uword>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* word */
/*---------------------------------------------------------------------------*/
#define atomic_word_store(a, v, o) atomic_word_store_impl ((a), (v), (o), $)

static inline void atomic_word_store_impl(
  volatile atomic_word* a, word v, mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<word>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_word_load(a, o) atomic_word_load_impl ((a), (o), $)

static inline word atomic_word_load_impl(
  volatile atomic_word* a, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<word>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_word_exchange(a, v, o)\
  atomic_word_exchange_impl ((a), (v), (o), $)

static inline word atomic_word_exchange_impl(
  volatile atomic_word* a, word v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<word>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_word_strong_cas(a, e, d, s, f)\
  atomic_word_strong_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool atomic_word_strong_cas_impl(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order             success,
  mem_order             failure,
  rl::debug_info_param  di
  )
{
  return ((rl::atomic<word>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_word_weak_cas(a, e, d, s, f)\
  atomic_word_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool atomic_word_weak_cas_impl(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order             success,
  mem_order             failure,
  rl::debug_info_param  di
  )
{
  return ((rl::atomic<word>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_word_fetch_add(a, v, o)\
  atomic_word_fetch_add_impl ((a), (v), (o), $)

static inline word atomic_word_fetch_add_impl(
  volatile atomic_word* a, word v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<word>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_word_fetch_sub(a, v, o)\
  atomic_word_fetch_sub_impl ((a), (v), (o), $)


static inline word atomic_word_fetch_sub_impl(
  volatile atomic_word* a, word v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<word>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_word_fetch_or(a, v, o)\
  atomic_word_fetch_or_impl ((a), (v), (o), $)

static inline word atomic_word_fetch_or_impl(
  volatile atomic_word* a, word v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<word>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_word_fetch_xor(a, v, o)\
  atomic_word_fetch_xor_impl ((a), (v), (o), $)

static inline word atomic_word_fetch_xor_impl(
  volatile atomic_word* a, word v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<word>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_word_fetch_and(a, v, o)\
  atomic_word_fetch_and_impl ((a), (v), (o), $)

static inline word atomic_word_fetch_and_impl(
  volatile atomic_word* a, word v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<word>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* u32 */
/*---------------------------------------------------------------------------*/
#define atomic_u32_store(a, v, o) atomic_u32_store_impl ((a), (v), (o), $)

static inline void atomic_u32_store_impl(
  volatile atomic_u32* a, u32 v, mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<u32>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_load(a, o) atomic_u32_load_impl ((a), (o), $)

static inline u32 atomic_u32_load_impl(
  volatile atomic_u32* a, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_exchange(a, v, o)\
  atomic_u32_exchange_impl ((a), (v), (o), $)

static inline u32 atomic_u32_exchange_impl(
  volatile atomic_u32* a, u32 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_strong_cas(a, e, d, s, f)\
  atomic_u32_strong_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool atomic_u32_strong_cas_impl(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            failure,
  rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_weak_cas(a, e, d, s, f)\
  atomic_u32_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool atomic_u32_weak_cas_impl(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            failure,
  rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_fetch_add(a, v, o)\
  atomic_u32_fetch_add_impl ((a), (v), (o), $)

static inline u32 atomic_u32_fetch_add_impl(
  volatile atomic_u32* a, u32 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_fetch_sub(a, v, o)\
  atomic_u32_fetch_sub_impl ((a), (v), (o), $)


static inline u32 atomic_u32_fetch_sub_impl(
  volatile atomic_u32* a, u32 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_fetch_or(a, v, o)\
  atomic_u32_fetch_or_impl ((a), (v), (o), $)

static inline u32 atomic_u32_fetch_or_impl(
  volatile atomic_u32* a, u32 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_fetch_xor(a, v, o)\
  atomic_u32_fetch_xor_impl ((a), (v), (o), $)

static inline u32 atomic_u32_fetch_xor_impl(
  volatile atomic_u32* a, u32 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u32_fetch_and(a, v, o)\
  atomic_u32_fetch_and_impl ((a), (v), (o), $)

static inline u32 atomic_u32_fetch_and_impl(
  volatile atomic_u32* a, u32 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u32>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* u64 */
/*---------------------------------------------------------------------------*/
#define atomic_u64_store(a, v, o) atomic_u64_store_impl ((a), (v), (o), $)

static inline void atomic_u64_store_impl(
  volatile atomic_u64* a, u64 v, mem_order o, rl::debug_info_param di
  )
{
  ((rl::atomic<u64>*) a)->store (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_load(a, o) atomic_u64_load_impl ((a), (o), $)

static inline u64 atomic_u64_load_impl(
  volatile atomic_u64* a, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->load (o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_exchange(a, v, o)\
  atomic_u64_exchange_impl ((a), (v), (o), $)

static inline u64 atomic_u64_exchange_impl(
  volatile atomic_u64* a, u64 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->exchange (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_strong_cas(a, e, d, s, f)\
  atomic_u64_strong_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool atomic_u64_strong_cas_impl(
  volatile atomic_u64* a,
  u64*                 expected,
  u64                  desired,
  mem_order            success,
  mem_order            failure,
  rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->compare_exchange_strong(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_weak_cas(a, e, d, s, f)\
  atomic_u64_weak_cas_impl ((a), (e), (d), (s), (f), $)

static inline bool atomic_u64_weak_cas_impl(
  volatile atomic_u64* a,
  u64*                 expected,
  u64                  desired,
  mem_order            success,
  mem_order            failure,
  rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->compare_exchange_weak(
    *expected, desired, success, di, failure, di
    );
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_fetch_add(a, v, o)\
  atomic_u64_fetch_add_impl ((a), (v), (o), $)

static inline u64 atomic_u64_fetch_add_impl(
  volatile atomic_u64* a, u64 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->fetch_add (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_fetch_sub(a, v, o)\
  atomic_u64_fetch_sub_impl ((a), (v), (o), $)


static inline u64 atomic_u64_fetch_sub_impl(
  volatile atomic_u64* a, u64 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->fetch_sub (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_fetch_or(a, v, o)\
  atomic_u64_fetch_or_impl ((a), (v), (o), $)

static inline u64 atomic_u64_fetch_or_impl(
  volatile atomic_u64* a, u64 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->fetch_or (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_fetch_xor(a, v, o)\
  atomic_u64_fetch_xor_impl ((a), (v), (o), $)

static inline u64 atomic_u64_fetch_xor_impl(
  volatile atomic_u64* a, u64 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->fetch_xor (v, o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_u64_fetch_and(a, v, o)\
  atomic_u64_fetch_and_impl ((a), (v), (o), $)

static inline u64 atomic_u64_fetch_and_impl(
  volatile atomic_u64* a, u64 v, mem_order o, rl::debug_info_param di
  )
{
  return ((rl::atomic<u64>*) a)->fetch_and (v, o, di);
}
/*---------------------------------------------------------------------------*/
/* Fences */
/*---------------------------------------------------------------------------*/
#define atomic_fence(mo) atomic_fence_impl (mo, $)
static inline void atomic_fence_impl (mem_order o, rl::debug_info_param di)
{
  rl::atomic_thread_fence (o, di);
}
/*---------------------------------------------------------------------------*/
#define atomic_sigfence(mo) atomic_singfence_impl (mo, $)
static inline void atomic_sigfence_impl (mem_order o, rl::debug_info_param di)
{
  rl::atomic_signal_fence (o, di);
}
/*---------------------------------------------------------------------------*/
} //extern "C" {

#endif
