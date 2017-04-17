#ifndef __BL_ATOMIC_H__
#define __BL_ATOMIC_H__

#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
#if BL_HAS_C11_ATOMICS (BL_COMPILER)
  #include <bl/base/impl/atomic_c11.h>
#elif defined (__cplusplus)
  #include <bl/base/impl/atomic_cpp.hpp>
#elif defined (BL_GCC) && BL_GCC >= BL_GCC_VER (4, 7, 0)
  #include <bl/base/impl/atomic_gcc.h>
#else /*older "_sync" atomic builtins with full fences might be used on gcc*/
  #error "atomics unimplemented on this platform"
#endif
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns (sizeof (atomic_uword)  == sizeof (uword));
static_assert_outside_func_ns (sizeof (atomic_word)  == sizeof (word));
static_assert_outside_func_ns (sizeof (atomic_u32)  == sizeof (u32));
/*---------------------------------------------------------------------------*/
#define atomic_uword_load_rlx(a) atomic_uword_load ((a), mo_relaxed)

#define atomic_uword_store_rlx(a, v) atomic_uword_store ((a), (v), mo_relaxed)

#define atomic_uword_exchange_rlx(a, v) \
  atomic_uword_exchange ((a), (v), mo_relaxed)

#define atomic_uword_strong_cas_rlx(a, e, d)\
  atomic_uword_strong_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_uword_weak_cas_rlx(a, e, d)\
  atomic_uword_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_uword_weak_cas_rlx(a, e, d)\
  atomic_uword_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_uword_weak_cas_rlx(a, e, d)\
  atomic_uword_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_uword_fetch_add_rlx(a, v)\
  atomic_uword_fetch_add (a, v, mo_relaxed)

#define atomic_uword_fetch_sub_rlx(a, v)\
  atomic_uword_fetch_sub (a, v, mo_relaxed)

#define atomic_uword_fetch_or_rlx(a, v)\
  atomic_uword_fetch_or (a, v, mo_relaxed)

#define atomic_uword_fetch_xor_rlx(a, v)\
  atomic_uword_fetch_xor (a, v, mo_relaxed)

#define atomic_uword_fetch_and_rlx(a, v)\
  atomic_uword_fetch_and (a, v, mo_relaxed)
/*---------------------------------------------------------------------------*/
#define atomic_word_load_rlx(a) atomic_word_load ((a), mo_relaxed)

#define atomic_word_store_rlx(a, v) atomic_word_store ((a), (v), mo_relaxed)

#define atomic_word_exchange_rlx(a, v) \
  atomic_word_exchange ((a), (v), mo_relaxed)

#define atomic_word_strong_cas_rlx(a, e, d)\
  atomic_word_strong_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_word_weak_cas_rlx(a, e, d)\
  atomic_word_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_word_weak_cas_rlx(a, e, d)\
  atomic_word_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_word_weak_cas_rlx(a, e, d)\
  atomic_word_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_word_fetch_add_rlx(a, v)\
  atomic_word_fetch_add (a, v, mo_relaxed)

#define atomic_word_fetch_sub_rlx(a, v)\
  atomic_word_fetch_sub (a, v, mo_relaxed)

#define atomic_word_fetch_or_rlx(a, v)\
  atomic_word_fetch_or (a, v, mo_relaxed)

#define atomic_word_fetch_xor_rlx(a, v)\
  atomic_word_fetch_xor (a, v, mo_relaxed)

#define atomic_word_fetch_and_rlx(a, v)\
  atomic_word_fetch_and (a, v, mo_relaxed)
/*---------------------------------------------------------------------------*/
#define atomic_u32_load_rlx(a) atomic_u32_load ((a), mo_relaxed)

#define atomic_u32_store_rlx(a, v) atomic_u32_store ((a), (v), mo_relaxed)

#define atomic_u32_exchange_rlx(a, v) atomic_u32_exchange ((a), (v), mo_relaxed)

#define atomic_u32_strong_cas_rlx(a, e, d)\
  atomic_u32_strong_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_u32_weak_cas_rlx(a, e, d)\
  atomic_u32_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_u32_weak_cas_rlx(a, e, d)\
  atomic_u32_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_u32_weak_cas_rlx(a, e, d)\
  atomic_u32_weak_cas ((a), (e), (d), mo_relaxed, mo_relaxed)

#define atomic_u32_fetch_add_rlx(a, v)\
  atomic_u32_fetch_add (a, v, mo_relaxed)

#define atomic_u32_fetch_sub_rlx(a, v)\
  atomic_u32_fetch_sub (a, v, mo_relaxed)

#define atomic_u32_fetch_or_rlx(a, v)\
  atomic_u32_fetch_or (a, v, mo_relaxed)

#define atomic_u32_fetch_xor_rlx(a, v)\
  atomic_u32_fetch_xor (a, v, mo_relaxed)

#define atomic_u32_fetch_and_rlx(a, v)\
  atomic_u32_fetch_and (a, v, mo_relaxed)
/*---------------------------------------------------------------------------*/
#endif /* __BL_ATOMIC_H__ */

