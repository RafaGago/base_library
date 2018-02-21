#include <bl/base/platform.h>
#include <stdio.h>
/*----------------------------------------------------------------------------*/
#ifdef BL_LINUX

#include <string.h>
#include <pthread.h>

#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/integer_printf_format.h>
#include <bl/base/utility.h>
#include <bl/base/time.h>
#include <bl/base/deadline.h>
#include <bl/base/cache.h>
#include <bl/base/atomic.h>

#include <bl/base/thread.h>
#include <bl/base/semaphore_linux.c>

#define thread_count 16
/*----------------------------------------------------------------------------*/
typedef struct signal_thread_context {
  uword overflow;
}
signal_thread_context;
/*----------------------------------------------------------------------------*/
typedef struct wait_thread_context {
  uword timed_out;
}
wait_thread_context;
/*----------------------------------------------------------------------------*/
typedef struct thread_context {
  bl_tm_sem*            sem;
  pthread_barrier_t*    barrier;
  atomic_uword*         force_abort;
  uword                 id;
  uword                 remaining;
  u32                   wait_us;
  bl_err                last_error;
  tstamp                elapsed;
  bool                  is_signaler;
  signal_thread_context s;
  wait_thread_context   w;
}
thread_context;
/*----------------------------------------------------------------------------*/
typedef struct thread_context_padded {
  thread_context c;
  align_type_to_cache_line (thread_context);
}
thread_context_padded;
/*----------------------------------------------------------------------------*/
#define thcon_hdr\
  "thread id=%"FMT_UWORD" ,is_signaler=%d, remaining_ops=%"FMT_UWORD\
  ", wait_us=%"FMT_U32

#define thcon_hdr_v(thcon)\
  thcon->id, thcon->is_signaler, thcon->remaining, thcon->wait_us
/*----------------------------------------------------------------------------*/
static inline void signaler_wait (thread_context* c)
{
  if (c->wait_us == 0) {
    return;
  }
  tstamp d;
  deadline_init (&d, c->wait_us);
  while (!deadline_expired (d)) {
    for (uword i = 0; i < 600; ++i) {
      if (deadline_expired (d)) {
        return;
      }
    }
    bl_thread_yield();
  }
}
/*----------------------------------------------------------------------------*/
static inline void run_signaler (thread_context* c)
{
  if (c->remaining == 0) {
    return;
  }
  bl_err err;
  bool force_abort;
  do {
retry:
    signaler_wait (c);
    force_abort = atomic_uword_load_rlx (c->force_abort);
    err = bl_tm_sem_signal (c->sem);
    if (err.bl) {
      if (err.bl == bl_would_overflow) {
        ++c->s.overflow;
        if (!force_abort) {
          goto retry;
        }
      }
      else {
        printf(
          "on " thcon_hdr ": semaphore error %" FMT_ERR, thcon_hdr_v (c), err.bl
           );
        break;
      }
    }
  }
  while (!force_abort && --c->remaining);
  c->last_error = err;
}
/*----------------------------------------------------------------------------*/
static inline void run_waiter (thread_context* c)
{
  bl_assert (c->remaining > 0);
  bl_err err;
  do {
    err = bl_tm_sem_wait (c->sem, c->wait_us);
    if (err.bl && err.bl != bl_timeout) {
      printf(
        "on " thcon_hdr ": semaphore error %" FMT_ERR, thcon_hdr_v (c), err.bl
        );
      break;
    }
    c->w.timed_out += (uword)(err.bl == bl_timeout);
  }
  while (--c->remaining);
  c->last_error = err;
}
/*----------------------------------------------------------------------------*/
int thread (void* context)
{
  thread_context* c = (thread_context*) context;
  int barrier_err   = pthread_barrier_wait (c->barrier);

  if (barrier_err && barrier_err != PTHREAD_BARRIER_SERIAL_THREAD) {
    c->last_error.bl = barrier_err;
    printf(
      "on " thcon_hdr ": barrier error %d", thcon_hdr_v (c), barrier_err
      );
  }
  tstamp start = bl_get_tstamp();
  if (c->is_signaler) {
    run_signaler (c);
  }
  else {
    run_waiter (c);
  }
  c->elapsed = bl_get_tstamp() - start;
  return 0;
}
/*----------------------------------------------------------------------------*/
bool launch_threads(
  thread_context_padded (*thr)[thread_count],
  bl_thread           (*thr_id)[thread_count]
  )
{
  uword i;
  for (i = 0; i < arr_elems (*thr); ++i) {
    bl_err err = bl_thread_init (&(*thr_id)[i], thread, &(*thr)[i]);
    (*thr)[i].c.id = (*thr_id)[i];
    if (err.bl) {
      printf(
        "error %"FMT_ERR" on thread initialization, shutting down process",
         err.bl
         );
      return false;
    }
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool join_thread_and_print (thread_context* c, bl_thread* t)
{
  bl_err err = bl_thread_join (t);
  if (err.bl) {
    printf ("error %"FMT_ERR" on thread join, shutting down process", err.bl);
    return false;
  }
  if (c->is_signaler) {
    printf(
      " sig thr finished: us=%"FMT_TSTAMP", overflows=%"FMT_UWORD" " thcon_hdr
      "\n",
      bl_tstamp_to_usec (c->elapsed), c->s.overflow, thcon_hdr_v (c)
      );
  }
  else {
    printf(
      " wait thr finished: us=%"FMT_TSTAMP", timeouts=%"FMT_UWORD" " thcon_hdr
      "\n",
      bl_tstamp_to_usec (c->elapsed), c->w.timed_out, thcon_hdr_v (c)
      );
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool join_threads_and_show_results(
  thread_context_padded (*thr)[thread_count],
  bl_thread           (*thr_id)[thread_count],
  bool                   abort_signallers_when_waiters_finished,
  atomic_uword*          force_abort_flag
  )
{
  uword i;
  for (i = 0; i < (arr_elems (*thr) / 2); ++i) {
    join_thread_and_print (&((*thr)[i].c), &(*thr_id)[i]);
  }
  atomic_uword_store_rlx(
    force_abort_flag, abort_signallers_when_waiters_finished
    );
  for (; i < arr_elems (*thr); ++i) {
    join_thread_and_print (&((*thr)[i].c), &(*thr_id)[i]);
  }
  return true;
}
/*----------------------------------------------------------------------------*/
bool check_print_sem_status (bl_tm_sem* sem, bool sem_val_zero)
{
  printf(
    "sem status: sig_count:%"FMT_U32", no_waiters=%"FMT_U32,
  	tm_sem_futex_get_sig (sem->sem),
	  tm_sem_futex_get_wait (sem->sem) == 0
    );
  if ((sem_val_zero && tm_sem_futex_get_sig (sem->sem) != 0) ||
	   tm_sem_futex_get_wait (sem->sem) != 0
      ) {
    printf ("semaphore state was corrupt. failed test. aborting\n");
    return false;
  }
  return true;
}
/*----------------------------------------------------------------------------*/
void setup_signal_overflow_test(
  thread_context_padded (*thr)[thread_count], uword remaining
  )
{
  printf (
    "\nmostly non contended test (many signals) with %u threads acting %"
    FMT_UWORD" times each\n",
    thread_count,
    remaining
    );
  uword i;
  for (i = 0; i < (arr_elems (*thr) / 2); ++i) {
    thread_context* c = &(*thr)[i].c;
    c->wait_us        = 0;
    c->is_signaler    = false;
    c->remaining      = remaining;
  }
  for (; i < arr_elems (*thr); ++i) {
    thread_context* c = &(*thr)[i].c;
    c->wait_us        = 0;
    c->is_signaler    = true;
    c->remaining      = remaining;
  }
}
/*----------------------------------------------------------------------------*/
void setup_waiter_timeout_test(
  thread_context_padded (*thr)[thread_count], uword remaining
  )
{
  printf (
    "\nmixed test with little successes and many timeouts with %u threads "
    "acting %"FMT_UWORD" times each\n",
    thread_count,
    remaining
    );
  uword i;
  for (i = 0; i < (arr_elems (*thr) / 2); ++i) {
    thread_context* c = &(*thr)[i].c;
    c->wait_us        = 300;
    c->is_signaler    = false;
    c->remaining      = remaining;
  }
  for (; i < arr_elems (*thr); ++i) {
    thread_context* c = &(*thr)[i].c;
    c->wait_us        = 1500;
    c->is_signaler    = true;
    c->remaining      = remaining;
  }
}
/*----------------------------------------------------------------------------*/
void setup_timeout_only_test(
  thread_context_padded (*thr)[thread_count], uword remaining
  )
{
  printf (
    "\ntimeout only test with %u threads acting %"FMT_UWORD" times each\n",
    thread_count / 2,
    remaining
    );
  uword i;
  for (i = 0; i < (arr_elems (*thr) / 2); ++i) {
    thread_context* c = &(*thr)[i].c;
    c->wait_us        = 10;
    c->is_signaler    = false;
    c->remaining      = remaining;
  }
  for (; i < arr_elems (*thr); ++i) {
    thread_context* c = &(*thr)[i].c;
    c->wait_us        = 0;
    c->is_signaler    = true;
    c->remaining      = 0;
  }
}
/*----------------------------------------------------------------------------*/
void thread_context_set_common(
  thread_context_padded (*thr)[thread_count],
  pthread_barrier_t*      barrier,
  bl_tm_sem*   sem,
  atomic_uword*           force_abort
  )
{
  memset (*thr, 0, sizeof *thr);
  for (uword i = 0; i < arr_elems (*thr); ++i) {
    (*thr)[i].c.id          = i + 1;
    (*thr)[i].c.barrier     = barrier;
    (*thr)[i].c.sem         = sem;
    (*thr)[i].c.force_abort = force_abort;
  }
}
/*----------------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
  int                     iterations;
  bool                    infinite_iterations;
  pthread_barrier_t       barrier;
  bl_tm_sem               sem;
  thread_context_padded   thr   [thread_count];
  bl_thread               thr_id[thread_count];
  bl_err                  err          = bl_mkok();
  uword                   subtest_size = 1000000;
  atomic_uword            force_abort;

  if (argc < 2) {
    printf ("usage: semaphore_test [iterations]\n");
    return 1;
  }
  sscanf (argv[1], "%d", &iterations);

  if (iterations != 0) {
    infinite_iterations = false;
    printf ("iterations: %d\n", iterations);
  }
  else {
    infinite_iterations = true;
    printf ("iterations: infinite\n");
  }
  err = bl_tm_sem_init (&sem);
  if (err.bl) { return err.bl; }

  err.bl = pthread_barrier_init (&barrier, nullptr, arr_elems (thr));
  if (err.bl) {
    goto sem_destroy;
  }
  while (infinite_iterations || iterations--) {

    thread_context_set_common (&thr, &barrier, &sem, &force_abort);
    setup_signal_overflow_test (&thr, subtest_size);
    atomic_uword_store_rlx (&force_abort, false);
    if (!launch_threads (&thr, &thr_id)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }
    if (!join_threads_and_show_results (&thr, &thr_id, false, &force_abort)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }
    if (!check_print_sem_status (&sem, true)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }

    thread_context_set_common (&thr, &barrier, &sem, &force_abort);
    setup_timeout_only_test (&thr, subtest_size / 6);
    atomic_uword_store_rlx (&force_abort, false);
    if (!launch_threads (&thr, &thr_id)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }
    if (!join_threads_and_show_results (&thr, &thr_id, true, &force_abort)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }
    if (!check_print_sem_status (&sem, true)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }

    thread_context_set_common (&thr, &barrier, &sem, &force_abort);
    setup_waiter_timeout_test (&thr, subtest_size / 6);
    atomic_uword_store_rlx (&force_abort, false);
    if (!launch_threads (&thr, &thr_id)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }
    if (!join_threads_and_show_results (&thr, &thr_id, true, &force_abort)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }
    if (!check_print_sem_status (&sem, false)) {
      err.bl = __LINE__;
      goto barrier_destroy;
    }
    bl_tm_sem_init (&sem);
  }

barrier_destroy:
  pthread_barrier_destroy (&barrier);

sem_destroy:
  bl_tm_sem_destroy (&sem);

  return err.bl;
}
/*----------------------------------------------------------------------------*/
#else
int main (int argc, char* argv[])
{
  printf ("unnecessary for this platform\n");
  return 0;
}
#endif /*BL_LINUX*/
