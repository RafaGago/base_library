#include <base_library/hdr/platform.h>

#if defined (BL_ATOMIC_USING_CPP_AS_INCLUDE) ||\
    (!defined (BL_HAS_C11_ATOMICS) &&\
      !defined (BL_CPP_COMPILER_FOR_WHOLE_PROJECT))

#ifdef ATOM_INLINE
  #error "some other header using this macro name"
#endif

#ifdef BL_ATOMIC_USING_CPP_AS_INCLUDE
  #define ATOM_INLINE inline
#else 
  #define ATOM_INLINE
#endif

#include <atomic>
#include <type_traits>
#include <base_library/lib/cpp_atomic.h>

extern "C" {
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
  (std::memory_order) mo_relaxed == std::memory_order_relaxed
  );
static_assert_outside_func_ns(
  (std::memory_order) mo_consume == std::memory_order_consume
  );
static_assert_outside_func_ns(
  (std::memory_order) mo_acquire == std::memory_order_acquire
  );
static_assert_outside_func_ns(
  (std::memory_order) mo_release == std::memory_order_release
  );
static_assert_outside_func_ns(
  (std::memory_order) mo_acq_rel == std::memory_order_acq_rel
  );
static_assert_outside_func_ns(
  (std::memory_order) mo_seq_cst == std::memory_order_seq_cst
  );
/*---------------------------------------------------------------------------*/
/* uword */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
    std::alignment_of<std::atomic<uword>>::value ==
        std::alignment_of<uword>::value
    );
static_assert_outside_func_ns (sizeof (std::atomic<uword>) == sizeof (uword));
/*---------------------------------------------------------------------------*/
ATOM_INLINE void atomic_uword_store (volatile atomic_uword* a, uword v, mem_order o)
{
  ((std::atomic<uword>*) a)->store (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE uword atomic_uword_load (volatile atomic_uword* a, mem_order o)
{
  return ((std::atomic<uword>*) a)->load ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE bool atomic_uword_strong_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              fail
  )
{
  return ((std::atomic<uword>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) fail
    );
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE bool atomic_uword_weak_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              fail
  )
{
  return ((std::atomic<uword>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) fail
    );
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE uword atomic_uword_fetch_add(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_add (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE uword atomic_uword_fetch_sub(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_sub (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE uword atomic_uword_fetch_or(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_or (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE uword atomic_uword_fetch_xor(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_xor (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE uword atomic_uword_fetch_and(
  volatile atomic_uword* a, uword v, mem_order o
  )
{
  return ((std::atomic<uword>*) a)->fetch_and (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* word */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
    std::alignment_of<std::atomic<word>>::value ==
        std::alignment_of<word>::value
    );
static_assert_outside_func_ns (sizeof (std::atomic<word>) == sizeof (word));
/*---------------------------------------------------------------------------*/
ATOM_INLINE void atomic_word_store (volatile atomic_word* a, word v, mem_order o)
{
  ((std::atomic<word>*) a)->store (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE word atomic_word_load (volatile atomic_word* a, mem_order o)
{
  return ((std::atomic<word>*) a)->load ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE bool atomic_word_strong_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order             success,
  mem_order             fail
  )
{
  return ((std::atomic<word>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) fail
    );
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE bool atomic_word_weak_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order             success,
  mem_order             fail
  )
{
  return ((std::atomic<word>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) fail
    );
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE word atomic_word_fetch_add(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_add (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE word atomic_word_fetch_sub(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_sub (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE word atomic_word_fetch_or(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_or (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE word atomic_word_fetch_xor(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_xor (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE word atomic_word_fetch_and(
  volatile atomic_word* a, word v, mem_order o
  )
{
  return ((std::atomic<word>*) a)->fetch_and (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
/* u32 */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
    std::alignment_of<std::atomic<u32>>::value ==
        std::alignment_of<u32>::value
    );
static_assert_outside_func_ns (sizeof (std::atomic<u32>) == sizeof (u32));
/*---------------------------------------------------------------------------*/
ATOM_INLINE void atomic_u32_store (volatile atomic_u32* a, u32 v, mem_order o)
{
  ((std::atomic<u32>*) a)->store (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE u32 atomic_u32_load (volatile atomic_u32* a, mem_order o)
{
  return ((std::atomic<u32>*) a)->load ((std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE bool atomic_u32_strong_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            fail
  )
{
  return ((std::atomic<u32>*) a)->compare_exchange_strong(
    *expected, desired, (std::memory_order) success, (std::memory_order) fail
    );
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE bool atomic_u32_weak_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            fail
  )
{
  return ((std::atomic<u32>*) a)->compare_exchange_weak(
    *expected, desired, (std::memory_order) success, (std::memory_order) fail
    );
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE u32 atomic_u32_fetch_add(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_add (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE u32 atomic_u32_fetch_sub(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_sub (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE u32 atomic_u32_fetch_or(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_or (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE u32 atomic_u32_fetch_xor(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_xor (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
ATOM_INLINE u32 atomic_u32_fetch_and(
  volatile atomic_u32* a, u32 v, mem_order o
  )
{
  return ((std::atomic<u32>*) a)->fetch_and (v, (std::memory_order) o);
}
/*---------------------------------------------------------------------------*/
} // extern "C" {

#undef ATOM_INLINE

#endif
