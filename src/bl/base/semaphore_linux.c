#include <bl/base/platform.h>

#ifdef BL_LINUX
/*----------------------------------------------------------------------------*/
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <errno.h>
/*----------------------------------------------------------------------------*/
#include <bl/base/assert.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/utility.h>
#include <bl/base/atomic.h>
#include <bl/base/semaphore.h>
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
static inline int futex_wait_masked_absolute_monotonic(
  bl_atomic_u32*            f,
  bl_u32                    expected_fval,
  bl_u32                    expected_fval_mask,
  const struct timespec* tp
  )
{
  return syscall(
    SYS_futex,
    f,
    FUTEX_WAIT_BITSET_PRIVATE,
    expected_fval,
    tp,
    nullptr,
    expected_fval_mask
    );
}
/*----------------------------------------------------------------------------*/
static inline int futex_wake (bl_atomic_u32* f, bl_i32 thread_count)
{
  return syscall(
    SYS_futex,
    f,
    FUTEX_WAKE_BITSET_PRIVATE,
    thread_count,
    nullptr,
    nullptr,
    FUTEX_BITSET_MATCH_ANY
    );
}
/*----------------------------------------------------------------------------*/
#define tm_sem_futex_sig_bits  24
#define tm_sem_futex_wait_bits 8
/*----------------------------------------------------------------------------*/
#define tm_sem_futex_sig_mask    bl_u32_lsb_set (tm_sem_futex_sig_bits)
#define tm_sem_futex_get_sig(v)  (v & tm_sem_futex_sig_mask)
#define tm_sem_futex_get_wait(v) (v >> tm_sem_futex_sig_bits)
#define tm_sem_futex_set(s, w)\
  (((s) & tm_sem_futex_sig_mask) | ((w) << tm_sem_futex_sig_bits))
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_tm_sem_signal (bl_tm_sem* s)
{
  bl_u32 sig, wait, curr;
  bl_u32 prev = bl_atomic_u32_load_rlx (&s->sem);
  do {
    sig  = tm_sem_futex_get_sig (prev);
    wait = tm_sem_futex_get_wait (prev);
    curr = tm_sem_futex_set (sig + 1, wait);

    if (bl_unlikely (tm_sem_futex_get_sig (curr) < sig)) {
      return bl_mkerr (bl_would_overflow);
    }
  }
  while (!bl_atomic_u32_weak_cas(
    &s->sem, &prev, curr, bl_mo_release, bl_mo_relaxed)
    );

  if (bl_unlikely (wait != 0)) {
    (void) futex_wake (&s->sem, 1);
  }
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_tm_sem_wait (bl_tm_sem* s, bl_u32 usec)
{
  bl_u32    sig;
  bl_u32    wait;
  bl_u32    curr;
  bl_u32    prev;
  bl_err err;

  err  = bl_mkok();
  prev = bl_atomic_u32_load_rlx (&s->sem);
  do {
    sig  = tm_sem_futex_get_sig (prev);
    wait = tm_sem_futex_get_wait (prev);

    if (bl_likely (sig > 0)) {
      curr = tm_sem_futex_set (sig - 1, wait);
    }
    else {
      curr = tm_sem_futex_set (sig, wait + 1);
      if (bl_unlikely (tm_sem_futex_get_wait (curr) < wait)) {
        bl_assert (false && "too many waiter threads");
        return bl_mkerr (bl_would_overflow);
      }
    }
  }
  while (!bl_atomic_u32_weak_cas(
    &s->sem, &prev, curr, bl_mo_acquire, bl_mo_relaxed)
    );

  if (sig == 0) {
    struct timespec t;
    if (usec != bl_tm_sem_infinity) {
      t = bl_timespec_us_from_now (usec, CLOCK_MONOTONIC);
    }
    int ferr;
  do_wait:
    ferr = futex_wait_masked_absolute_monotonic(
      &s->sem,
      curr,
      tm_sem_futex_sig_mask,
      (usec != bl_tm_sem_infinity) ? &t : nullptr
      );
    if (ferr == 0 || errno == EWOULDBLOCK) {
      /*new signals, try to acquire one*/
      prev = bl_atomic_u32_load_rlx (&s->sem);
      do {
        sig  = tm_sem_futex_get_sig (prev);
        wait = tm_sem_futex_get_wait (prev);

        if (sig > 0) {
          curr = tm_sem_futex_set (sig - 1, wait - 1);
        }
        else {
          /*contention lost*/
          curr = prev;
          goto do_wait;
        }
      }
      while(
        !bl_atomic_u32_weak_cas(
          &s->sem, &prev, curr, bl_mo_acquire, bl_mo_relaxed
        ));
    }
    else {
      switch (errno) {
      case ETIMEDOUT:
        err = bl_mkerr_sys (bl_timeout, ETIMEDOUT);
        break;
#if !defined (BL_TM_SEM_LINUX_WAKE_ON_EINTR)
      case EINTR:
        goto do_wait;
#else
      case EINTR:
        err = bl_mkerr_sys (bl_interrupted, EINTR);
        break;
#endif
      default:
        err = bl_mkerr_sys (bl_error, errno);
        break;
      }
      bl_atomic_u32_fetch_sub_rlx (&s->sem, (1 << tm_sem_futex_sig_bits));
    }
  }
  return err;
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_tm_sem_init (bl_tm_sem* s)
{
  bl_atomic_u32_store_rlx (&s->sem, 0);
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_EXPORT bl_err bl_tm_sem_destroy (bl_tm_sem* s)
{
  bl_word woken = futex_wake (&s->sem, bl_itype_max (bl_word));
  bl_assert (woken == 0);
  /*if this returns an error there still are waiters -> wrong user shutdown*/
  return bl_mkerr ((woken == 0) ? bl_ok : bl_error);
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
#endif /* BL_LINUX */
