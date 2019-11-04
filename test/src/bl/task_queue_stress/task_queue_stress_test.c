#include <string.h>
#include <stdio.h>

#include <bl/task_queue/task_queue.h>

#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/integer_printf_format.h>
#include <bl/base/processor_pause.h>
#include <bl/base/cache.h>
#include <bl/base/default_allocator.h>
#include <bl/base/thread.h>
#include <bl/base/semaphore.h>

/*the c files are included here to be able to printf data that would otherwise
  be opaque or to avoid triggering assertions on release*/
#include <bl/task_queue/task_queue.c>
#include <bl/task_queue/delayed.c>
#include <bl/base/semaphore_win.c>
#include <bl/base/semaphore_linux.c>
/*----------------------------------------------------------------------------*/
#define thread_count       8
#define tq_list_size       bl_pow2_u (19)
#define tq_delayed_size    bl_pow2_u (8)
#define iteration_elements 100000
/*----------------------------------------------------------------------------*/
enum thr_type_e{
  thr_regular,
  thr_delayed_spin,
  thr_delayed_sem,
  thr_delayed_cancel,
  thr_type_count,
};
typedef bl_u8 thr_type;
/*----------------------------------------------------------------------------*/
typedef struct work_data {
  bl_uword received;
  bl_uword error_count;
  bl_uword expected_error_count;
  thr_type type;
}
work_data;
/*----------------------------------------------------------------------------*/
typedef struct producer_thread_data {
  bl_taskq*  tq;
  bl_sem*    sem;
  work_data* wd;
  bl_uword   remaining;
  bl_uword   overflow;
  bl_err     last_err;
  thr_type   type;
}
producer_thread_data;
/*----------------------------------------------------------------------------*/
typedef struct consumer_thread_data {
  bl_taskq*  tq;
  bl_sem* sem;
}
consumer_thread_data;
/*----------------------------------------------------------------------------*/
typedef struct padded_producer_thread_data {
  producer_thread_data dat;
  bl_align_type_to_cache_line (producer_thread_data);
}
padded_producer_thread_data;
/*----------------------------------------------------------------------------*/
void task_callback (bl_err err, bl_taskq_id id, void* context)
{
  work_data* wd= (work_data*) context;
  ++wd->received;
  wd->error_count += (err.bl != bl_ok);
}
/*----------------------------------------------------------------------------*/
void task_callback_delayed (bl_err err, bl_taskq_id id, void* context)
{
  work_data* wd= (work_data*) context;
  ++wd->received;
  wd->error_count += (err.bl != bl_ok);
  /*"would oveflow" errors because of trying to use too many timers are
     expected,  as this test is "an attack" to the queue with an unrealistic
     load

    cancelled tasks are expected too, as timers can be programmed after
    shutdown or explicitly cancelled
  */
  wd->expected_error_count +=
    ((err.bl == bl_would_overflow) || (err.bl == bl_cancelled));
}
/*----------------------------------------------------------------------------*/
int producer_thread_regular (producer_thread_data* td)
{
  bl_taskq_id id;
  bl_err      err;

  while (td->remaining) {
    err = bl_taskq_post (td->tq, &id, bl_taskq_task_rv (task_callback, td->wd));
    if (!err.bl) {
      --td->remaining;
      continue;
    }
    else if (err.bl == bl_would_overflow) {
      for (bl_uword i = 0; i < 5; ++i) {
        bl_processor_pause();
      }
      ++td->overflow;
    }
    else {
      td->last_err = err;
      printf ("error on producer, code: %"BL_FMT_ERR"\n", err.bl);
      return 1;
    }
  }
  return 0;
}
/*----------------------------------------------------------------------------*/
int producer_thread_delayed (producer_thread_data* td, bl_u32 timeout_us)
{
  bl_taskq_id id;
  bl_err      err;
  bl_timept32 tp_cancel;

  while (td->remaining) {
    err = bl_taskq_post_delayed(
            td->tq,
            &id,
            &tp_cancel,
            bl_taskq_task_rv (task_callback_delayed, td->wd),
            timeout_us
            );
    if (!err.bl) {
      --td->remaining;
      continue;
    }
    else if (err.bl == bl_would_overflow) {
      for (bl_uword i = 0; i < 5; ++i) {
        bl_processor_pause();
      }
      ++td->overflow;
    }
    else {
      td->last_err = err;
      return 1;
    }
  }
  return 0;
}
/*----------------------------------------------------------------------------*/
static inline bool producer_thread_post_delayed_cancel(
  producer_thread_data* td, bl_taskq_id id, bl_timept tp_cancel
  )
{
  bl_err err;
  while (1) {
    err = bl_taskq_post_try_cancel_delayed (td->tq, id, tp_cancel);
    if (!err.bl) {
      return true;
    }
    else if (err.bl == bl_would_overflow) {
      for (bl_uword i = 0; i < 5; ++i) {
        bl_processor_pause();
      }
      ++td->overflow;
    }
    else {
      td->last_err = err;
      return false;
    }
  }
  return false;
}
/*----------------------------------------------------------------------------*/
int producer_thread_delayed_cancel(
  producer_thread_data* td, bl_u32 timeout_us
  )
{
  bl_taskq_id id;
  bl_err      err;
  bl_timept32 tp_cancel;

  while (td->remaining) {
    err = bl_taskq_post_delayed(
            td->tq,
            &id,
            &tp_cancel,
            bl_taskq_task_rv (task_callback_delayed, td->wd),
            timeout_us
            );
    if (!err.bl) {
      --td->remaining;
      if (td->remaining & 1) {
        if (!producer_thread_post_delayed_cancel (td, id, tp_cancel)) {
          return 1;
        }
      }
      continue;
    }
    else if (err.bl == bl_would_overflow) {
      for (bl_uword i = 0; i < 5; ++i) {
        bl_processor_pause();
      }
      ++td->overflow;
    }
    else {
      td->last_err = err;
      return 1;
    }
  }
  return 0;
}
/*----------------------------------------------------------------------------*/
int producer_thread (void* context)
{
  producer_thread_data* td = (producer_thread_data*) context;
  bl_sem_wait (td->sem);

  switch (td->type) {
    case thr_regular:
      return producer_thread_regular (td);
    case thr_delayed_spin:
      return producer_thread_delayed (td, BL_SCHED_TMIN_US);
    case thr_delayed_sem:
      return producer_thread_delayed (td, BL_SCHED_TMIN_US * 2);
    case thr_delayed_cancel:
      return producer_thread_delayed_cancel (td, BL_SCHED_TMIN_US * 5);
    default:
      printf ("bug\n");
      return 1;
  }
}
/*----------------------------------------------------------------------------*/
int consumer_thread (void* context)
{
  consumer_thread_data* cd = (consumer_thread_data*) context;
  bl_err err;

  for (bl_uword i = 0; i < thread_count; ++i) {
    bl_sem_signal (cd->sem);
  }
  do {
    err = bl_taskq_run_one_no_timeout (cd->tq);
  }
  while (!err.bl);
  if (err.bl != bl_nothing_to_do) {
    printf ("consumer exit with error:%"BL_FMT_ERR"\n", err.bl);
  }
  /*cancel all timed tasks planned for the future*/
  do {
    err = bl_taskq_try_cancel_one (cd->tq);
  }
  while (!err.bl);
  return 0;
}
/*----------------------------------------------------------------------------*/
void join_threads (bl_thread* thr, bl_uword thr_count)
{
  printf (" thrjoin ");
  fflush (stdout);
  for (bl_uword i = 0; i < thr_count; ++i) {
    bl_thread_join (thr + i);
    printf ("%"BL_FMT_UWORD" ", i);
  }
  printf ("|");
}
/*----------------------------------------------------------------------------*/
int main (int argc, char* argv[])
{
  padded_producer_thread_data pd[thread_count];
  consumer_thread_data        cd;
  work_data                   wd[thread_count];
  bl_thread                   producer_th[thread_count];
  bl_thread                   consumer_th;
  bl_sem                      sem;
  bl_uword                    iterations;
  bool                        infinite_iterations;
  bl_taskq*                   tq;
  bl_alloc_tbl                alloc;

  alloc = bl_get_default_alloc();

  if (argc < 2) {
    printf ("usage: task_queue_stress_test [iterations]\n");
    return 1;
  }
  sscanf (argv[1], "%"BL_FMT_UWORD, &iterations);
  if (iterations < 0) {
    printf ("wrong iteration count: %"BL_FMT_UWORD"\n", iterations);
    return 0;
  }
  infinite_iterations = (iterations == 0);
  if (infinite_iterations) {
    printf ("iteration count: infinite\n");
  }
  else {
    printf ("iteration count: %"BL_FMT_UWORD"\n", iterations);
  }
  bl_err err = bl_sem_init (&sem);
  if (err.bl) {
    printf ("bl_sem_init err: %"BL_FMT_ERR"\n", err.bl);
    return (int) err.bl;
  }

  for (bl_uword itnum = 0; infinite_iterations || itnum < iterations; ++itnum) {
    printf ("iteration %"BL_FMT_UWORD"\n", itnum);

    err = bl_taskq_init (&tq, &alloc, tq_list_size, tq_delayed_size);
    if (err.bl) {
      printf ("bl_taskq_init err: %"BL_FMT_ERR"\n", err.bl);
      goto do_sem_destroy;
    }
    memset (pd, 0, sizeof pd);
    memset (wd, 0, sizeof wd);

    for (bl_uword i = 0; i < bl_arr_elems (pd); ++i) {
      pd[i].dat.tq        = tq;
      pd[i].dat.sem       = &sem;
      pd[i].dat.remaining = iteration_elements;
      pd[i].dat.wd        = &wd[i];
      wd[i].type          = i % thr_type_count;
      pd[i].dat.type      = wd[i].type;
    }
    cd.tq  = tq;
    cd.sem = &sem;

    printf (" init ok |");

    for (bl_uword i = 0; i < bl_arr_elems (pd); ++i) {
      int r = bl_thread_init (&producer_th[i], producer_thread, &pd[i].dat).bl;
      if (r != bl_ok) {
        printf ("bl_thread_init err: %d\n", r);
        if (i != bl_ok) {
          bl_taskq_block (tq);
          join_threads (producer_th, i - 1);
        }
        bl_taskq_destroy (tq, &alloc);
        goto do_sem_destroy;
      }
    }
    int r = bl_thread_init (&consumer_th, consumer_thread, &cd).bl;
    if (r != bl_ok) {
      printf ("bl_thread_init err: %d\n", r);
      bl_taskq_block (tq);
      join_threads (producer_th, thread_count);
      bl_taskq_destroy (tq, &alloc);
      goto do_sem_destroy;
    }
    join_threads (producer_th, thread_count);
    bl_taskq_block (tq);
    join_threads (&consumer_th, 1);

    for (bl_uword i = 0; i < 25000; ++i) {
      bl_processor_pause();
    }

    bl_uword overflow = 0;
    for (bl_uword i = 0; i < bl_arr_elems (pd); ++i) {
      if (pd[i].dat.last_err.bl) {
        printf(
         "error: thread %"BL_FMT_UWORD" has exited because of error %"BL_FMT_ERR"\n",
          i, pd[i].dat.last_err.bl
          );
        err = pd[i].dat.last_err;
      }
      if (pd[i].dat.remaining) {
        printf(
          "error: thread %"BL_FMT_UWORD" has %"BL_FMT_UWORD" remaining operations\n",
           i,
           pd[i].dat.remaining
          );
        err.bl = 2;
      }
      if (wd[i].error_count > wd[i].expected_error_count) {
        printf(
          "error: callback of thread %"BL_FMT_UWORD" invoked with error %"
          BL_FMT_UWORD" times\n",
           i, wd[i].error_count - wd[i].expected_error_count
          );
        err.bl = 3;
      }
      if (wd[i].received != iteration_elements) {
        printf(
          "error: callback of thread %"BL_FMT_UWORD" missed %"BL_FMT_UWORD
          " invocations\n",
           i, iteration_elements - wd[i].received
          );
        bl_err ntd = bl_taskq_try_run_one (tq);
        if (ntd.bl != bl_nothing_to_do) {
          printf(
            "bl_taskq_try_run_one unexpected error code: %"BL_FMT_ERR"\n",
            ntd.bl
            );
        }
        if (bl_taskq_delayed_size (&tq->delayed) != 0) {
          printf(
            "remaining elements on delayed list detected: %"BL_FMT_UWORD"\n",
            bl_taskq_delayed_size (&tq->delayed)
            );
          bl_timept curr = bl_timept_get();
          printf ("current timepoint %"BL_FMT_TPOINT"\n", curr);
          for (bl_uword j = 0; j < bl_taskq_delayed_size (&tq->delayed); ++j) {
            bl_taskq_delayed_entry const* f =
              bl_taskq_delayed_at (&tq->delayed, j);
            printf(
              "delayed list element %"BL_FMT_UWORD" timepoint: %"BL_FMT_TPOINT32"\n",
               j,
               f->time
              );
          }
        }
        err.bl = 4;
      }
      overflow += pd[i].dat.overflow;
    }
    printf (" %"BL_FMT_UWORD" overflows\n", overflow);
    bl_err errd = bl_taskq_destroy (tq, &alloc);
    if (errd.bl) {
      printf ("bl_taskq_destroy err: %"BL_FMT_ERR"\n", errd.bl);
    }
    if (err.bl || errd.bl) {
      goto do_sem_destroy;
    }
  }
  printf ("ending, err: %"BL_FMT_ERR"\n", err.bl);
do_sem_destroy:
  bl_sem_destroy (&sem);
  return (int) err.bl;
}
/*----------------------------------------------------------------------------*/
