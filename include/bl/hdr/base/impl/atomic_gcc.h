#ifndef __BL_GCC_ATOMIC_H__
#define __BL_GCC_ATOMIC_H__

#ifndef __BL_ATOMIC_H__
  #error "don't include this file directly, use <hdr/atomic.h> instead"
#endif

#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
/*---------------------------------------------------------------------------*/
typedef uword atomic_uword;
typedef word  atomic_word;
typedef u32   atomic_u32;
typedef int   memory_order;
/*---------------------------------------------------------------------------*/
#define mo_relaxed  __ATOMIC_RELAXED
#define mo_consume  __ATOMIC_CONSUME
#define mo_acquire  __ATOMIC_ACQUIRE
#define mo_release  __ATOMIC_RELEASE
#define mo_acq_rel  __ATOMIC_ACQ_REL
#define mo_seq_cst  __ATOMIC_SEQ_CST
/*---------------------------------------------------------------------------*/
/* uword */
/*---------------------------------------------------------------------------*/
static inline void atomic_uword_store(
  volatile atomic_uword* a, uword v, memory_order mo
  )
{
  __atomic_store_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_load(
  volatile atomic_uword* a, memory_order mo
  )
{
  return __atomic_load_n (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_uword_strong_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  memory_order           success,
  memory_order           failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, false, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_uword_weak_cas(
  volatile atomic_uword* a,
  uword*                 expected,
  uword                  desired,
  memory_order           success,
  memory_order           failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, true, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_add(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return __atomic_fetch_add (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_sub(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return __atomic_fetch_sub (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_or(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return __atomic_fetch_or (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_xor(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return __atomic_fetch_xor (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_and(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return __atomic_fetch_and (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* word */
/*---------------------------------------------------------------------------*/
static inline void atomic_word_store(
  volatile atomic_word* a, word v, memory_order mo
  )
{
  __atomic_store_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_load (volatile atomic_word* a, memory_order mo)
{
  return __atomic_load_n (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_word_strong_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  memory_order          success,
  memory_order          failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, false, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_word_weak_cas(
  volatile atomic_word* a,
  word*                 expected,
  word                  desired,
  memory_order          success,
  memory_order          failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, true, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_add(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return __atomic_fetch_add (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_sub(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return __atomic_fetch_sub (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_or(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return __atomic_fetch_or (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_word_fetch_xor(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return __atomic_fetch_xor (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_word_fetch_and(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return __atomic_fetch_and (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* u32 */
/*---------------------------------------------------------------------------*/
static inline void atomic_u32_store(
  volatile atomic_u32* a, u32 v, memory_order mo
  )
{
  __atomic_store_n (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_load(
  volatile atomic_u32* a, memory_order mo
  )
{
  return __atomic_load_n (a, mo);
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_u32_strong_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  memory_order         success,
  memory_order         failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, false, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline bool atomic_u32_weak_cas(
  volatile atomic_u32* a,
  u32*                 expected,
  u32                  desired,
  memory_order         success,
  memory_order         failure
  )
{
  return __atomic_compare_exchange_n(
    a, expected, desired, true, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_add(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return __atomic_fetch_add (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_sub(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return __atomic_fetch_sub (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_or(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return __atomic_fetch_or (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_xor(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return __atomic_fetch_xor (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline void atomic_thread_fence (memory_order o)
{
  __atomic_thread_fence (o);
}
/*---------------------------------------------------------------------------*/

#endif /*__BL_GCC_ATOMIC_H__*/
