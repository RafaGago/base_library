#ifndef __BL_CPP_ATOMIC_H__
#define __BL_CPP_ATOMIC_H__

#ifndef __BL_ATOMIC_H__
  #error "don't include this file directly, use <hdr/atomic.h> instead"
#endif

#ifndef __cplusplus
  #error "this file is being used by a non-C++ compiler"
#endif
/*---------------------------------------------------------------------------*/
/* As a matter of fact I now when adding the "atomic_thread_fence" I realized
  that the C++ version defines the C11  atomics with the same names and with
  C linkage. TODO check and delete this if required.

  If it works it's just to take the atomic_c11.h header and add:

  #ifndef __cplusplus
    #include <stdatomic.h>
  #else
    #include <atomic>
  #endif
*/
/*---------------------------------------------------------------------------*/
#include <atomic>
#include <bl/base/alignment.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
/*---------------------------------------------------------------------------*/
template <class T>
struct bl_cbl_pp_atomic_size_align_checker {
  static const bool ok =
    sizeof (std::atomic<T>) == sizeof (T) &&
    bl_alignof (std::atomic<T>) == bl_alignof (T);
};
/*---------------------------------------------------------------------------*/
extern "C" {
/*---------------------------------------------------------------------------*/
typedef bl_word  bl_atomic_word;
typedef bl_uword bl_atomic_uword;
typedef bl_u32   bl_atomic_u32;
typedef bl_u64   bl_atomic_u64;
/*---------------------------------------------------------------------------*/
typedef enum bl_mem_order_e {
  bl_mo_relaxed = (bl_uword) std::memory_order_relaxed,
  bl_mo_consume = (bl_uword) std::memory_order_consume,
  bl_mo_acquire = (bl_uword) std::memory_order_acquire,
  bl_mo_release = (bl_uword) std::memory_order_release,
  bl_mo_acq_rel = (bl_uword) std::memory_order_acq_rel,
  bl_mo_seq_cst = (bl_uword) std::memory_order_seq_cst
}
bl_mem_order;
/*---------------------------------------------------------------------------*/
/* bl_uword */
/*---------------------------------------------------------------------------*/
bl_static_assert_outside_func_ns(
  bl_cbl_pp_atomic_size_align_checker<bl_uword>::ok
  );
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_uword_store(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  ((std::atomic<bl_uword>*) a)->store (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_load(
  volatile bl_atomic_uword* a, bl_mem_order o
  )
{
  return ((std::atomic<bl_uword>*) a)->load ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_exchange(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return ((std::atomic<bl_uword>*) a)->exchange (v, (std::memory_order) o);
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
  return ((std::atomic<bl_uword>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
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
  return ((std::atomic<bl_uword>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_add(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return ((std::atomic<bl_uword>*) a)->fetch_add (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_sub(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return ((std::atomic<bl_uword>*) a)->fetch_sub (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_or(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return ((std::atomic<bl_uword>*) a)->fetch_or (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_xor(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return ((std::atomic<bl_uword>*) a)->fetch_xor (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_and(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return ((std::atomic<bl_uword>*) a)->fetch_and (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* bl_word */
/*---------------------------------------------------------------------------*/
bl_static_assert_outside_func_ns(
  bl_cbl_pp_atomic_size_align_checker<bl_word>::ok
  );
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_word_store(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  ((std::atomic<bl_word>*) a)->store (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_load(
  volatile bl_atomic_word* a, bl_mem_order o
  )
{
  return ((std::atomic<bl_word>*) a)->load ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_exchange(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return ((std::atomic<bl_word>*) a)->exchange (v, (std::memory_order) o);
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
  return ((std::atomic<bl_word>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
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
  return ((std::atomic<bl_word>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_add(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return ((std::atomic<bl_word>*) a)->fetch_add (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_sub(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return ((std::atomic<bl_word>*) a)->fetch_sub (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_or(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return ((std::atomic<bl_word>*) a)->fetch_or (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_xor(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return ((std::atomic<bl_word>*) a)->fetch_xor (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_and(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return ((std::atomic<bl_word>*) a)->fetch_and (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* bl_u32 */
/*---------------------------------------------------------------------------*/
bl_static_assert_outside_func_ns (bl_cbl_pp_atomic_size_align_checker<bl_u32>::ok);
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_u32_store(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  ((std::atomic<bl_u32>*) a)->store(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_load(
  volatile bl_atomic_u32* a, bl_mem_order o
  )
{
  return ((std::atomic<bl_u32>*) a)->load((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_exchange(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u32>*) a)->exchange (v, (std::memory_order) o);
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
  return ((std::atomic<bl_u32>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_u32_weak_cas(
  volatile bl_atomic_u32* a,
  bl_u32*                    expected,
  bl_u32                     desired,
  bl_mem_order            success,
  bl_mem_order            failure
  )
{
  return ((std::atomic<bl_u32>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_add(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u32>*) a)->fetch_add(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_sub(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u32>*) a)->fetch_sub(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_or(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u32>*) a)->fetch_or(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_xor(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u32>*) a)->fetch_xor(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_and(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u32>*) a)->fetch_and(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* bl_u64 */
/*---------------------------------------------------------------------------*/
bl_static_assert_outside_func_ns(bl_cbl_pp_atomic_size_align_checker<bl_u64>::ok);
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_u64_store(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  ((std::atomic<bl_u64>*) a)->store(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_load (volatile bl_atomic_u64* a, bl_mem_order o)
{
  return ((std::atomic<bl_u64>*) a)->load((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_exchange(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u64>*) a)->exchange (v, (std::memory_order) o);
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
  return ((std::atomic<bl_u64>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
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
  return ((std::atomic<bl_u64>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_add(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u64>*) a)->fetch_add(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_sub(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u64>*) a)->fetch_sub(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_or(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u64>*) a)->fetch_or(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_xor(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u64>*) a)->fetch_xor(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_and(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return ((std::atomic<bl_u64>*) a)->fetch_and(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* Fences */
/*---------------------------------------------------------------------------*/
static inline void atomic_fence (bl_mem_order o)
{
  std::atomic_thread_fence ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline void atomic_sigfence (bl_mem_order o)
{
  std::atomic_signal_fence ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
} //extern "C" {

#endif
