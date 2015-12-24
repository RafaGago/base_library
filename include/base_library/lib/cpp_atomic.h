#ifndef __EVK_CPP_ATOMIC_H__
#define __EVK_CPP_ATOMIC_H__

#include <base_library/hdr/platform.h>
#include <base_library/hdr/integer.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
typedef word  atomic_word;
typedef uword atomic_uword;
typedef u32   atomic_u32;
/*---------------------------------------------------------------------------*/
#ifdef BL_CPP_COMPILER_FOR_WHOLE_PROJECT
/*---------------------------------------------------------------------------*/
#include <atomic>
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
#define BL_ATOMIC_USING_CPP_AS_INCLUDE
#include <base_library/lib/cpp_atomic_impl.hpp>
#undef BL_ATOMIC_USING_CPP_AS_INCLUDE
/*---------------------------------------------------------------------------*/
#else /*!BL_WHOLE_PROJECT_USES_CPP_COMPILER*/
/*---------------------------------------------------------------------------*/
/* When using old gcc versions I could just use some C11 wrappers. As these
   functions might not be inlined */
/*---------------------------------------------------------------------------*/
typedef enum mem_order_e {
  mo_relaxed,
  mo_consume,
  mo_acquire,
  mo_release,
  mo_acq_rel,
  mo_seq_cst
}
mem_order;
/*---------------------------------------------------------------------------*/
extern void atomic_uword_store (volatile atomic_uword* a, uword v, mem_order o);
extern uword atomic_uword_load (volatile atomic_uword* a, mem_order o);

extern bool atomic_uword_strong_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              fail
  );
extern bool atomic_uword_weak_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  mem_order              success,
  mem_order              fail
  );

extern uword atomic_uword_fetch_add(
  volatile atomic_uword* a, uword v, mem_order o
  );
extern uword atomic_uword_fetch_sub(
  volatile atomic_uword* a, uword v, mem_order o
  );
extern uword atomic_uword_fetch_or(
  volatile atomic_uword* a, uword v, mem_order o
  );
extern uword atomic_uword_fetch_xor(
  volatile atomic_uword* a, uword v, mem_order o
  );
extern uword atomic_uword_fetch_and(
  volatile atomic_uword* a, uword v, mem_order o
  );
/*---------------------------------------------------------------------------*/
extern void atomic_word_store (volatile atomic_word* a, word v, mem_order o);
extern word atomic_word_load (volatile atomic_word* a, mem_order o);

extern bool atomic_word_strong_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order              success,
  mem_order              fail
  );
extern bool atomic_word_weak_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  mem_order              success,
  mem_order              fail
  );

extern word atomic_word_fetch_add(
  volatile atomic_word* a, word v, mem_order o
  );
extern word atomic_word_fetch_sub(
  volatile atomic_word* a, word v, mem_order o
  );
extern word atomic_word_fetch_or(
  volatile atomic_word* a, word v, mem_order o
  );
extern word atomic_word_fetch_xor(
  volatile atomic_word* a, word v, mem_order o
  );
extern word atomic_word_fetch_and(
  volatile atomic_word* a, word v, mem_order o
  );
/*---------------------------------------------------------------------------*/
extern void atomic_u32_store (volatile atomic_u32* a, u32 v, mem_order o);
extern u32 atomic_u32_load (volatile atomic_u32* a, mem_order o);

extern bool atomic_u32_strong_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            fail
  );
extern bool atomic_u32_weak_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  mem_order            success,
  mem_order            fail
  );

extern u32 atomic_u32_fetch_add (volatile atomic_u32* a, u32 v, mem_order o);
extern u32 atomic_u32_fetch_sub (volatile atomic_u32* a, u32 v, mem_order o);
extern u32 atomic_u32_fetch_or (volatile atomic_u32* a, u32 v, mem_order o);
extern u32 atomic_u32_fetch_xor (volatile atomic_u32* a, u32 v, mem_order o);
extern u32 atomic_u32_fetch_and (volatile atomic_u32* a, u32 v, mem_order o);
/*---------------------------------------------------------------------------*/
#endif /*BL_WHOLE_PROJECT_USES_CPP_COMPILER*/
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
