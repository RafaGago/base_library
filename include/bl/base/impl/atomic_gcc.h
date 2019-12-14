#ifndef __BL_GCC_ATOMIC_H__
#define __BL_GCC_ATOMIC_H__

#ifndef __BL_ATOMIC_H__
  #error "don't include this file directly, use <hdr/atomic.h> instead"
#endif

#include <bl/base/platform.h>
#include <bl/base/integer.h>
/*---------------------------------------------------------------------------*/
typedef bl_uword bl_atomic_uword;
typedef bl_word  bl_atomic_word;
typedef bl_u32   bl_atomic_u32;
typedef bl_u64   bl_atomic_u64;
typedef int   bl_memory_order;
typedef bl_memory_order bl_mem_order;
/*---------------------------------------------------------------------------*/
#define bl_mo_relaxed  __ATOMIC_RELAXED
#define bl_mo_consume  __ATOMIC_CONSUME
#define bl_mo_acquire  __ATOMIC_ACQUIRE
#define bl_mo_release  __ATOMIC_RELEASE
#define bl_mo_acq_rel  __ATOMIC_ACQ_REL
#define bl_mo_seq_cst  __ATOMIC_SEQ_CST
/*---------------------------------------------------------------------------*/
/* bl_uword */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_uword_store(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order mo
  )
{
  __atomic_store_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_load(
  volatile bl_atomic_uword* a, bl_mem_order mo
  )
{
  return __atomic_load_n (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_exchange(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order mo
  )
{
  return __atomic_exchange_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_uword_strong_cas(
  volatile bl_atomic_uword* a,
  bl_uword*              expected,
  bl_uword               desired,
  bl_mem_order           success,
  bl_mem_order           failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, false, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool bl_atomic_uword_weak_cas(
  volatile bl_atomic_uword* a,
  bl_uword*              expected,
  bl_uword               desired,
  bl_mem_order           success,
  bl_mem_order           failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, true, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_add(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return __atomic_fetch_add (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_sub(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return __atomic_fetch_sub (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_or(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return __atomic_fetch_or (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_xor(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return __atomic_fetch_xor (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_uword_fetch_and(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return __atomic_fetch_and (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_word */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_word_store(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order mo
  )
{
  __atomic_store_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_load (volatile bl_atomic_word* a, bl_mem_order mo)
{
  return __atomic_load_n (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_exchange(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order mo
  )
{
  return __atomic_exchange_n (a, v, mo);
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
  return __atomic_compare_exchange_n(
    a, expected, desired, false, success, failure
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
  return __atomic_compare_exchange_n(
    a, expected, desired, true, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_add(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return __atomic_fetch_add (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_sub(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return __atomic_fetch_sub (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_word bl_atomic_word_fetch_or(
  volatile bl_atomic_uword* a, bl_uword v, bl_mem_order o
  )
{
  return __atomic_fetch_or (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_word_fetch_xor(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return __atomic_fetch_xor (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_uword bl_atomic_word_fetch_and(
  volatile bl_atomic_word* a, bl_word v, bl_mem_order o
  )
{
  return __atomic_fetch_and (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_u32 */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_u32_store(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order mo
  )
{
  __atomic_store_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_load(
  volatile bl_atomic_u32* a, bl_mem_order mo
  )
{
  return __atomic_load_n (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_exchange(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order mo
  )
{
  return __atomic_exchange_n (a, v, mo);
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
  return __atomic_compare_exchange_n(
    a, expected, desired, false, success, failure
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
  return __atomic_compare_exchange_n(
    a, expected, desired, true, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_add(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return __atomic_fetch_add (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_sub(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return __atomic_fetch_sub (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_or(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return __atomic_fetch_or (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u32 bl_atomic_u32_fetch_xor(
  volatile bl_atomic_u32* a, bl_u32 v, bl_mem_order o
  )
{
  return __atomic_fetch_xor (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* bl_u64 */
/*---------------------------------------------------------------------------*/
static inline void bl_atomic_u64_store(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order mo
  )
{
  __atomic_store_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_load(
  volatile bl_atomic_u64* a, bl_mem_order mo
  )
{
  return __atomic_load_n (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_exchange(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order mo
  )
{
  return __atomic_exchange_n (a, v, mo);
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
  return __atomic_compare_exchange_n(
    a, expected, desired, false, success, failure
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
  return __atomic_compare_exchange_n(
    a, expected, desired, true, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_add(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return __atomic_fetch_add (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_sub(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return __atomic_fetch_sub (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_or(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return __atomic_fetch_or (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline bl_u64 bl_atomic_u64_fetch_xor(
  volatile bl_atomic_u64* a, bl_u64 v, bl_mem_order o
  )
{
  return __atomic_fetch_xor (a, v, o);
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

#endif /*__BL_GCC_ATOMIC_H__*/
