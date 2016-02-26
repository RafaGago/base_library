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
struct cpp_atomic_size_align_checker {
  static const bool ok = 
    sizeof (std::atomic<T>) == sizeof (T) &&
    bl_alignof (std::atomic<T>) == bl_alignof (T);
};
/*---------------------------------------------------------------------------*/
extern "C" {
/*---------------------------------------------------------------------------*/
typedef word  atomic_word;
typedef uword atomic_uword;
typedef u32   atomic_u32;
/*---------------------------------------------------------------------------*/
typedef enum mem_order_e {
  mo_relaxed = (uword) std::memory_order_relaxed,
  mo_consume = (uword) std::memory_order_consume,
  mo_acquire = (uword) std::memory_order_acquire,
  mo_release = (uword) std::memory_order_release,
  mo_acq_rel = (uword) std::memory_order_acq_rel,
  mo_seq_cst = (uword) std::memory_order_seq_cst
}
mem_order;
/*---------------------------------------------------------------------------*/
/* uword */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns (cpp_atomic_size_align_checker<uword>::ok);
/*---------------------------------------------------------------------------*/
static inline void atomic_uword_store(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  ((std::atomic<uword>*) a)->store (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_load (volatile atomic_uword* a, mem_order o)
{
  return ((std::atomic<uword>*) a)->load ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_uword_strong_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              failure
  )
{
  return ((std::atomic<uword>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_uword_weak_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              failure
  )
{
  return ((std::atomic<uword>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_add(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_add (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_sub(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_sub (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_or(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_or (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_xor(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_xor (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_and(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_and (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* word */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns (cpp_atomic_size_align_checker<word>::ok);
/*---------------------------------------------------------------------------*/
static inline void atomic_word_store (volatile atomic_word* a, word v, mem_order o)
{
  ((std::atomic<word>*) a)->store (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_load (volatile atomic_word* a, mem_order o)
{
  return ((std::atomic<word>*) a)->load ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_word_strong_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order             success,
  mem_order             failure
  )
{
  return ((std::atomic<word>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_word_weak_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order             success,
  mem_order             failure
  )
{
  return ((std::atomic<word>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_add(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_add (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_sub(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_sub (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_or(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_or (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_xor(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_xor (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_and(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_and (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* u32 */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(cpp_atomic_size_align_checker<u32>::ok);
/*---------------------------------------------------------------------------*/
static inline void atomic_u32_store(volatile atomic_u32* a, u32 v, mem_order o)
{
  ((std::atomic<u32>*) a)->store(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_load(volatile atomic_u32* a, mem_order o)
{
  return ((std::atomic<u32>*) a)->load((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_u32_strong_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            failure
  )
{
  return ((std::atomic<u32>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_u32_weak_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            failure
  )
{
  return ((std::atomic<u32>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) failure
    );
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_add(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_add(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_sub(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_sub(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_or(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_or(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_xor(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_xor(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_and(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_and(v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
} //extern "C" {

#endif
