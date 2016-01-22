#ifndef __BL_C11_ATOMIC_H__
#define __BL_C11_ATOMIC_H__

#ifndef __BL_ATOMIC_H__
  #error "don't include this file directly, use <hdr/atomic.h> instead"
#endif

#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/alignment.h>
#include <stdatomic.h>
/*---------------------------------------------------------------------------*/
typedef atomic_uintptr_t atomic_uword;
typedef atomic_intptr_t  atomic_word;
typedef atomic_ulong     atomic_u32;
/*---------------------------------------------------------------------------*/
#define mo_relaxed  memory_order_relaxed
#define mo_consume  memory_order_consume
#define mo_acquire  memory_order_acquire
#define mo_release  memory_order_release
#define mo_acq_rel  memory_order_acq_rel
#define mo_seq_cst  memory_order_seq_cst
/*---------------------------------------------------------------------------*/
/* uword */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
  sizeof (atomic_uword) == sizeof (uword) && 
  bl_alignof (atomic_uword) == bl_alignof (uword)
  );
/*---------------------------------------------------------------------------*/
static inline void atomic_uword_store(
  volatile atomic_uword* a, uword v, memory_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_load(
  volatile atomic_uword* a, memory_order mo
  )
{
  return atomic_load_explicit (a, mo);
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
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
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
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_add(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_sub(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_or(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_xor(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_uword_fetch_and(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* word */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
  sizeof (atomic_word) == sizeof (word) && 
  bl_alignof (atomic_word) == bl_alignof (word)
  );
/*---------------------------------------------------------------------------*/
static inline void atomic_word_store(
  volatile atomic_word* a, word v, memory_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_load (volatile atomic_word* a, memory_order mo)
{
  return atomic_load_explicit (a, mo);
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
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
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
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_add(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_sub(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline word atomic_word_fetch_or(
  volatile atomic_uword* a, uword v, memory_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_word_fetch_xor(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline uword atomic_word_fetch_and(
  volatile atomic_word* a, word v, memory_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
/* u32 */
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
  sizeof (atomic_u32) == sizeof (u32) && 
  bl_alignof (atomic_u32) == bl_alignof (u32)
  );
/*---------------------------------------------------------------------------*/
static inline void atomic_u32_store(
  volatile atomic_u32* a, u32 v, memory_order mo
  )
{
  atomic_store_explicit (a, v, mo);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_load (volatile atomic_u32* a, memory_order mo)
{
  return atomic_load_explicit (a, mo);
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
  return atomic_compare_exchange_strong_explicit(
    a, expected, desired, success, failure
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
  return atomic_compare_exchange_weak_explicit(
    a, expected, desired, success, failure
    );
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_add(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return atomic_fetch_add_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_sub(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return atomic_fetch_sub_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_or(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return atomic_fetch_or_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_xor(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return atomic_fetch_xor_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline u32 atomic_u32_fetch_and(
  volatile atomic_u32* a, u32 v, memory_order o
  )
{
  return atomic_fetch_and_explicit (a, v, o);
}
/*---------------------------------------------------------------------------*/
static inline void atomic_fence (memory_order o)
{
  __atomic_thread_fence (o);
}
/*---------------------------------------------------------------------------*/
static inline void atomic_sigfence (memory_order o)
{
  __atomic_signal_fence (o);
}
/*---------------------------------------------------------------------------*/

#endif /*__BL_C11_ATOMIC_H__*/
