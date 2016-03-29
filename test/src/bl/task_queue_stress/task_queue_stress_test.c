#include <string.h>
#include <stdio.h>

#include <bl/task_queue/task_queue.h>

#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
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
#define tq_list_size       pow2_u (16)
#define tq_delayed_size    pow2_u (4)
#define iteration_elements 100000
/*----------------------------------------------------------------------------*/
enum thr_type_e{
  thr_regular,
  thr_delayed_spin,
  thr_delayed_sem,
  thr_delayed_cancel,
  thr_type_count,
};
typedef u8 thr_type;
/*----------------------------------------------------------------------------*/
typedef struct work_data {
  uword    received;
  uword    error_count;
  uword    expected_error_count;
  thr_type type;
}
work_data;
/*----------------------------------------------------------------------------*/
typedef struct producer_thread_data {
  taskq*     tq;
  bl_sem*    sem;
  work_data* wd;
  uword      remaining;
  uword      overflow;
  bl_err     last_err;
  thr_type   type;
}
producer_thread_data;
/*----------------------------------------------------------------------------*/
typedef struct consumer_thread_data {
  taskq*  tq;
  bl_sem* sem;
}
consumer_thread_data;
/*----------------------------------------------------------------------------*/
typedef struct padded_producer_thread_data {
  producer_thread_data dat;
  align_type_to_cache_line (producer_thread_data);
}
padded_producer_thread_data;
/*----------------------------------------------------------------------------*/
void task_callback (bl_err err, taskq_id id, void* context)
{
  work_data* wd= (work_data*) context;
  ++wd->received;
  wd->error_count += (err != 0);
}
/*----------------------------------------------------------------------------*/
void task_callback_delayed (bl_err err, taskq_id id, void* context)
{
  work_data* wd= (work_data*) context;
  ++wd->received;
  wd->error_count += (err != 0);
  /*"would oveflow" errors because of trying to use too many timers are
     expected,  as this test is "an attack" to the queue with an unrealistic
     load

    cancelled tasks are expected too, as timers can be programmed after
    shutdown or explicitly cancelled
  */
  wd->expected_error_count +=
    ((err == bl_would_overflow) || (err == bl_cancelled));
}
/*----------------------------------------------------------------------------*/
int producer_thread_regular (producer_thread_data* td)
{
  taskq_id id;
  bl_err   err;

  while (td->remaining) {
    err = taskq_post (td->tq, &id, taskq_task_rv (task_callback, td->wd));
    if (!err) {      
      --td->remaining;
      continue;
    }
    else if (err == bl_would_overflow) {
      for (uword i = 0; i < 5; ++i) {
        processor_pause();
      }
      ++td->overflow;
    }
    else {
      td->last_err = err;
      printf ("error on producer, code: %d\n", err);
      return 1;
    }    
  }
  return 0;
}
/*----------------------------------------------------------------------------*/
int producer_thread_delayed (producer_thread_data* td, u32 timeout_us)
{
  taskq_id id;
  bl_err   err;
  tstamp   tp_cancel;

  while (td->remaining) {
    err = taskq_post_delayed(
            td->tq,
            &id,
            &tp_cancel,
            taskq_task_rv (task_callback_delayed, td->wd),
            timeout_us
            );
    if (!err) {      
      --td->remaining;
      continue;
    }
    else if (err == bl_would_overflow) {
      for (uword i = 0; i < 5; ++i) {
        processor_pause();
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
  producer_thread_data* td, taskq_id id, tstamp tp_cancel
  )
{
  bl_err err;
  while (1) {
    err = taskq_post_try_cancel_delayed (td->tq, id, tp_cancel);
    if (!err) {      
      return true;
    }
    else if (err == bl_would_overflow) {
      for (uword i = 0; i < 5; ++i) {
        processor_pause();
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
  producer_thread_data* td, u32 timeout_us
  )
{
  taskq_id id;
  bl_err   err;
  tstamp   tp_cancel;

  while (td->remaining) {
    err = taskq_post_delayed(
            td->tq,
            &id,
            &tp_cancel,
            taskq_task_rv (task_callback_delayed, td->wd),
            timeout_us
            );
    if (!err) {      
      --td->remaining;
      if (td->remaining & 1) {
        if (!producer_thread_post_delayed_cancel (td, id, tp_cancel)) {
          return 1;
        }
      }
      continue;
    }
    else if (err == bl_would_overflow) {
      for (uword i = 0; i < 5; ++i) {
        processor_pause();
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

  for (uword i = 0; i < thread_count; ++i) {
    bl_sem_signal (cd->sem);
  }
  do {
    err = taskq_run_one_no_timeout (cd->tq);
  }
  while (!err);
  if (err != bl_nothing_to_do) {
    printf ("consumer exit with error:%d\n", err);
  }
  /*cancel all timed tasks planned for the future*/
  do {
    err = taskq_try_cancel_one (cd->tq);
  }
  while (!err);
  return 0;
}
/*----------------------------------------------------------------------------*/
void join_threads (bl_thread* thr, uword thr_count)
{
  printf (" thrjoin ");
  fflush (stdout);
  for (uword i = 0; i < thr_count; ++i) {
    bl_thread_join (thr + i);
    printf ("%u ", i);
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
  uword                       iterations;
  bool                        infinite_iterations;
  taskq*                      tq;
  alloc_tbl                   alloc;

  alloc = get_default_alloc();

  if (argc < 2) {
    printf ("usage: task_queue_stress_test [iterations]\n");
    return 1;
  }
  sscanf (argv[1], "%u", &iterations);
  if (iterations < 0) {
    printf ("wrong iteration count: %u\n", iterations);
    return 0;
  }
  infinite_iterations = (iterations == 0);
  if (infinite_iterations) {
    printf ("iteration count: infinite\n");
  }
  else {
    printf ("iteration count: %d\n", iterations);
  }
  bl_err err = bl_sem_init (&sem);
  if (err) {
    printf ("bl_sem_init err: %d\n", err);
    return (int) err;
  }

  for (uword itnum = 0; infinite_iterations || itnum < iterations; ++itnum) {
    printf ("iteration %d\n", itnum);

    err = taskq_init (&tq, &alloc, tq_list_size, tq_delayed_size);
    if (err) {
      printf ("taskq_init err: %d\n", err);
      goto do_sem_destroy;
    }
    memset (pd, 0, sizeof pd);
    memset (wd, 0, sizeof wd);

    for (uword i = 0; i < arr_elems (pd); ++i) {
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

    for (uword i = 0; i < arr_elems (pd); ++i) {
      int r = bl_thread_init (&producer_th[i], producer_thread, &pd[i].dat);
      if (r != bl_thread_success) {
        printf ("bl_thread_init err: %d\n", err);
        if (i != bl_thread_success) {
          taskq_block (tq);
          join_threads (producer_th, i - 1);
        }
        taskq_destroy (tq, &alloc);
        goto do_sem_destroy;
      }
    }
    int r = bl_thread_init (&consumer_th, consumer_thread, &cd);
    if (r != bl_thread_success) {
      printf ("bl_thread_init err: %d\n", err);
      taskq_block (tq);
      join_threads (producer_th, thread_count);
      taskq_destroy (tq, &alloc);
      goto do_sem_destroy;
    }
    join_threads (producer_th, thread_count);
    taskq_block (tq);
    join_threads (&consumer_th, 1);
    
    for (uword i = 0; i < 25000; ++i) {
      processor_pause();
    }

    uword overflow = 0;
    for (uword i = 0; i < arr_elems (pd); ++i) {
      if (pd[i].dat.last_err) {
        printf(
         "error: thread %d has exited because of error %d\n",
          i, pd[i].dat.last_err
          );
        err = pd[i].dat.last_err;
      }
      if (pd[i].dat.remaining) {
        printf(
          "error: thread %d has %d remaining operations\n", i, pd[i].dat.remaining
          );
        err = 2;
      }
      if (wd[i].error_count > wd[i].expected_error_count) {
        printf(
          "error: callback of thread %d invoked with error %d times\n",
           i, wd[i].error_count - wd[i].expected_error_count
          );
        err = 3;
      }
      if (wd[i].received != iteration_elements) {
        printf(
          "error: callback of thread %d missed %d invocations\n",
           i, iteration_elements - wd[i].received
          );
        bl_err ntd = taskq_try_run_one (tq);
        if (ntd != bl_nothing_to_do) {
          printf ("taskq_try_run_one unexpected error code: %d\n", ntd);
        }
        if (taskq_delayed_size (&tq->delayed) != 0) {
          printf(
            "remaining elements on delayed list detected: %d\n",
            taskq_delayed_size (&tq->delayed)
            );
          tstamp curr = bl_get_tstamp();
          printf ("current timestamp %d\n", curr);
          for (uword j = 0; j < taskq_delayed_size (&tq->delayed); ++j) {
            taskq_delayed_entry const* f = taskq_delayed_at (&tq->delayed, j);
            printf ("delayed list element %d timestamp: %d\n", j, f->key);
          }
        }
        err = 4;
      }
      overflow += pd[i].dat.overflow;
    }
    printf (" %d overflows\n", overflow);
    bl_err errd = taskq_destroy (tq, &alloc);
    if (errd) {
      printf ("taskq_destroy err: %d\n", errd);
    }
    if (err || errd) {
      goto do_sem_destroy;
    }
  }
  printf ("ending, err: %d\n", err);
do_sem_destroy:
  bl_sem_destroy (&sem);
  return (int) err;
}
/*----------------------------------------------------------------------------*/
