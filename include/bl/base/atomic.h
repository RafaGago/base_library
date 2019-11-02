#ifndef __BL_ATOMIC_H__
#define __BL_ATOMIC_H__

#include <bl/base/platform.h>
/*---------------------------------------------------------------------------*/
#ifdef BL_ATOMIC_USE_RELACY
  #include <bl/base/impl/atomic_relacy.hpp>
#elif defined (__cplusplus)
  #include <bl/base/impl/atomic_cpp.hpp>
#elif BL_HAS_C11_ATOMICS (BL_COMPILER)
  #include <bl/base/impl/atomic_c11.h>
#elif defined (BL_GCC) && BL_GCC >= BL_GCC_VER (4, 7, 0)
  #include <bl/base/impl/atomic_gcc.h>
#else /*older "_sync" atomic builtins with full fences might be used on gcc*/
  #error "atomics unimplemented on this platform"
#endif
/*---------------------------------------------------------------------------*/
#ifndef BL_ATOMIC_USE_RELACY
  bl_static_assert_outside_func_ns(
    sizeof (bl_atomic_uword)  == sizeof (bl_uword)
    );
  bl_static_assert_outside_func_ns(
    sizeof (bl_atomic_word)  == sizeof (bl_word)
    );
  bl_static_assert_outside_func_ns(
    sizeof (bl_atomic_u32)  == sizeof (bl_u32)
    );
#endif
/*---------------------------------------------------------------------------*/
#define bl_atomic_uword_load_rlx(a) bl_atomic_uword_load ((a), bl_mo_relaxed)

#define bl_atomic_uword_store_rlx(a, v)\
  bl_atomic_uword_store ((a), (v), bl_mo_relaxed)

#define bl_atomic_uword_exchange_rlx(a, v) \
  bl_atomic_uword_exchange ((a), (v), bl_mo_relaxed)

#define bl_atomic_uword_strong_cas_rlx(a, e, d)\
  bl_atomic_uword_strong_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_uword_weak_cas_rlx(a, e, d)\
  bl_atomic_uword_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_uword_weak_cas_rlx(a, e, d)\
  bl_atomic_uword_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_uword_weak_cas_rlx(a, e, d)\
  bl_atomic_uword_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_uword_fetch_add_rlx(a, v)\
  bl_atomic_uword_fetch_add (a, v, bl_mo_relaxed)

#define bl_atomic_uword_fetch_sub_rlx(a, v)\
  bl_atomic_uword_fetch_sub (a, v, bl_mo_relaxed)

#define bl_atomic_uword_fetch_or_rlx(a, v)\
  bl_atomic_uword_fetch_or (a, v, bl_mo_relaxed)

#define bl_atomic_uword_fetch_xor_rlx(a, v)\
  bl_atomic_uword_fetch_xor (a, v, bl_mo_relaxed)

#define bl_atomic_uword_fetch_and_rlx(a, v)\
  bl_atomic_uword_fetch_and (a, v, bl_mo_relaxed)
/*---------------------------------------------------------------------------*/
#define bl_atomic_word_load_rlx(a) bl_atomic_word_load ((a), bl_mo_relaxed)

#define bl_atomic_word_store_rlx(a, v)\
  bl_atomic_word_store ((a), (v), bl_mo_relaxed)

#define bl_atomic_word_exchange_rlx(a, v) \
  bl_atomic_word_exchange ((a), (v), bl_mo_relaxed)

#define bl_atomic_word_strong_cas_rlx(a, e, d)\
  bl_atomic_word_strong_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_word_weak_cas_rlx(a, e, d)\
  bl_atomic_word_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_word_weak_cas_rlx(a, e, d)\
  bl_atomic_word_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_word_weak_cas_rlx(a, e, d)\
  bl_atomic_word_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_word_fetch_add_rlx(a, v)\
  bl_atomic_word_fetch_add (a, v, bl_mo_relaxed)

#define bl_atomic_word_fetch_sub_rlx(a, v)\
  bl_atomic_word_fetch_sub (a, v, bl_mo_relaxed)

#define bl_atomic_word_fetch_or_rlx(a, v)\
  bl_atomic_word_fetch_or (a, v, bl_mo_relaxed)

#define bl_atomic_word_fetch_xor_rlx(a, v)\
  bl_atomic_word_fetch_xor (a, v, bl_mo_relaxed)

#define bl_atomic_word_fetch_and_rlx(a, v)\
  bl_atomic_word_fetch_and (a, v, bl_mo_relaxed)
/*---------------------------------------------------------------------------*/
#define bl_atomic_u32_load_rlx(a) \
  bl_atomic_u32_load ((a), bl_mo_relaxed)

#define bl_atomic_u32_store_rlx(a, v)\
  bl_atomic_u32_store ((a), (v), bl_mo_relaxed)

#define bl_atomic_u32_exchange_rlx(a, v) \
  bl_atomic_u32_exchange ((a), (v), bl_mo_relaxed)

#define bl_atomic_u32_strong_cas_rlx(a, e, d)\
  bl_atomic_u32_strong_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u32_weak_cas_rlx(a, e, d)\
  bl_atomic_u32_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u32_weak_cas_rlx(a, e, d)\
  bl_atomic_u32_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u32_weak_cas_rlx(a, e, d)\
  bl_atomic_u32_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u32_fetch_add_rlx(a, v)\
  bl_atomic_u32_fetch_add (a, v, bl_mo_relaxed)

#define bl_atomic_u32_fetch_sub_rlx(a, v)\
  bl_atomic_u32_fetch_sub (a, v, bl_mo_relaxed)

#define bl_atomic_u32_fetch_or_rlx(a, v)\
  bl_atomic_u32_fetch_or (a, v, bl_mo_relaxed)

#define bl_atomic_u32_fetch_xor_rlx(a, v)\
  bl_atomic_u32_fetch_xor (a, v, bl_mo_relaxed)

#define bl_atomic_u32_fetch_and_rlx(a, v)\
  bl_atomic_u32_fetch_and (a, v, bl_mo_relaxed)
/*---------------------------------------------------------------------------*/
#define bl_atomic_u64_load_rlx(a)\
  bl_atomic_u64_load ((a), bl_mo_relaxed)

#define bl_atomic_u64_store_rlx(a, v)\
  bl_atomic_u64_store ((a), (v), bl_mo_relaxed)

#define bl_atomic_u64_exchange_rlx(a, v)\
  bl_atomic_u64_exchange ((a), (v), bl_mo_relaxed)

#define bl_atomic_u64_strong_cas_rlx(a, e, d)\
  bl_atomic_u64_strong_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u64_weak_cas_rlx(a, e, d)\
  bl_atomic_u64_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u64_weak_cas_rlx(a, e, d)\
  bl_atomic_u64_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u64_weak_cas_rlx(a, e, d)\
  bl_atomic_u64_weak_cas ((a), (e), (d), bl_mo_relaxed, bl_mo_relaxed)

#define bl_atomic_u64_fetch_add_rlx(a, v)\
  bl_atomic_u64_fetch_add (a, v, bl_mo_relaxed)

#define bl_atomic_u64_fetch_sub_rlx(a, v)\
  bl_atomic_u64_fetch_sub (a, v, bl_mo_relaxed)

#define bl_atomic_u64_fetch_or_rlx(a, v)\
  bl_atomic_u64_fetch_or (a, v, bl_mo_relaxed)

#define bl_atomic_u64_fetch_xor_rlx(a, v)\
  bl_atomic_u64_fetch_xor (a, v, bl_mo_relaxed)

#define bl_atomic_u64_fetch_and_rlx(a, v)\
  bl_atomic_u64_fetch_and (a, v, bl_mo_relaxed)
/*---------------------------------------------------------------------------*/
#endif /* __BL_ATOMIC_H__ */

