#include <bl/hdr/base//platform.h>

#ifdef BL_LINUX
/*----------------------------------------------------------------------------*/
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <errno.h>
/*----------------------------------------------------------------------------*/
#include <bl/hdr/base//integer_manipulation.h>
#include <bl/hdr/base//utility.h>
#include <bl/hdr/base//atomic.h>
#include <bl/lib/base//semaphore.h>
/*----------------------------------------------------------------------------*/
static inline int futex_wait_masked_absolute_monotonic(
  atomic_u32*            f,
  u32                    expected_fval,
  u32                    expected_fval_mask,
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
static inline int futex_wake (atomic_u32* f, i32 thread_count)
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
#define tm_sem_futex_sig_mask    u32_lsb_set (tm_sem_futex_sig_bits)
#define tm_sem_futex_get_sig(v)  (v & tm_sem_futex_sig_mask)
#define tm_sem_futex_get_wait(v) (v >> tm_sem_futex_sig_bits)
#define tm_sem_futex_set(s, w)\
  (((s) & tm_sem_futex_sig_mask) | ((w) << tm_sem_futex_sig_bits))
/*----------------------------------------------------------------------------*/
bl_err BL_EXPORT bl_tm_sem_signal (bl_tm_sem* s)
{
  u32 sig, wait, curr;
  u32 prev = atomic_u32_load_rlx (&s->sem);
  do {
    sig  = tm_sem_futex_get_sig (prev);
    wait = tm_sem_futex_get_wait (prev);
    curr = tm_sem_futex_set (sig + 1, wait);

    if (unlikely (tm_sem_futex_get_sig (curr) < sig)) {
      return bl_would_overflow;      
    }
  }
  while (!atomic_u32_weak_cas (&s->sem, &prev, curr, mo_release, mo_relaxed));

  if (unlikely (wait != 0)) {
    (void) futex_wake (&s->sem, 1);
  }
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
bl_err BL_EXPORT bl_tm_sem_wait (bl_tm_sem* s, u32 usec)
{
  u32    sig;
  u32    wait;
  u32    curr;
  u32    prev;
  bl_err err;

  err  = bl_ok;
  prev = atomic_u32_load_rlx (&s->sem);
  do {
    sig  = tm_sem_futex_get_sig (prev);
    wait = tm_sem_futex_get_wait (prev);

    if (likely (sig > 0)) { 
      curr = tm_sem_futex_set (sig - 1, wait);
    }
    else {
      curr = tm_sem_futex_set (sig, wait + 1); 
      if (unlikely (tm_sem_futex_get_wait (curr) < wait)) {
        assert (false && "too many waiter threads");
        return bl_would_overflow;      
      }
    }
  }
  while (!atomic_u32_weak_cas (&s->sem, &prev, curr, mo_acquire, mo_relaxed));

  if (sig == 0) {
    struct timespec t;
    if (usec != bl_tm_sem_infinity) {
      t = timespec_us_from_now (usec, CLOCK_MONOTONIC);
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
      prev = atomic_u32_load_rlx (&s->sem);
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
        !atomic_u32_weak_cas (&s->sem, &prev, curr, mo_acquire, mo_relaxed)
        );
    }
    else {
      switch (errno) {
      case ETIMEDOUT: err = bl_timeout;     break;
#if !defined (BL_TM_SEM_LINUX_WAKE_ON_EINTR)
      case EINTR:     goto do_wait;
#else
      case EINTR:     err = bl_interrupted; break;
#endif
      default:        err = bl_error;       break;
      }
      atomic_u32_fetch_sub_rlx (&s->sem, (1 << tm_sem_futex_sig_bits));
    }
  }
  return err;
}
/*----------------------------------------------------------------------------*/
bl_err BL_EXPORT bl_tm_sem_init (bl_tm_sem* s)
{
  atomic_u32_store_rlx (&s->sem, 0);
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
bl_err BL_EXPORT bl_tm_sem_destroy (bl_tm_sem* s)
{
  word woken = futex_wake (&s->sem, itype_max (word));
  assert (woken == 0);
  /*if this returns an error there still are waiters -> wrong user shutdown*/
  return (woken == 0) ? bl_ok : bl_error;                                   
}
/*----------------------------------------------------------------------------*/
#endif /* BL_LINUX */
