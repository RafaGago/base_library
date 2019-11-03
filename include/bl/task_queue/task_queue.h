#ifndef TASK_QUEUE_H_
#define TASK_QUEUE_H_

#include <stdlib.h>

#include <bl/task_queue/libexport.h>

#include <bl/base/integer.h>
#include <bl/base/integer.h>
#include <bl/base/allocator.h>
#include <bl/base/error.h>
#include <bl/base/time.h>
#include <bl/base/assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/* Taskq: A Fixed-size (preallocated) task queue with support for delayed    */
/* tasks. Useful as building block for e.g. event loops or cooperative       */
/* task schedulers (FIFO). Can be used in places where one could use         */
/* libevent, libuv, etc without their IO (sockets, files) facilities.        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Types and helpers                                                         */
/*---------------------------------------------------------------------------*/
typedef bl_uword bl_taskq_id;
typedef bl_u32   bl_taskq_usec;
/*---------------------------------------------------------------------------*/
typedef void (*bl_taskq_task_func)(
  bl_err error, bl_taskq_id id, void* context
  );
/*---------------------------------------------------------------------------*/
typedef struct bl_taskq_task {
  bl_taskq_task_func func;
  void*              context;
}
bl_taskq_task;
/*---------------------------------------------------------------------------*/
/* rv suffix = r-value */
/*---------------------------------------------------------------------------*/
static inline bl_taskq_task
  bl_taskq_task_rv (bl_taskq_task_func func, void* context)
{
  bl_taskq_task tmp = { func, context };
  return tmp;
}
/*---------------------------------------------------------------------------*/
/* Interface                                                                 */
/*---------------------------------------------------------------------------*/
typedef struct bl_taskq bl_taskq;
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call.

  task_queue_capacity: maximum number of elements/comands that the task queue
  can hold.

  delayed_task_capacity: maximum number of delayed tasks acknowledged on the
  system.

  If receiving the "bl_taskq_invalid" error code and everything looks right
  consider that both maximum "regular_task_queue_capacity" and "
  delayed_task_queue_capacity" may not be allowed to be as big as its data
  type allows.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err bl_taskq_init(
    bl_taskq**          tq,
    bl_alloc_tbl const* alloc,
    bl_uword            task_queue_capacity,
    bl_uword            delayed_task_capacity
    );
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call (to be run from the worker thread). Tries to run an
  element of the work queue. If no elements were available returns
  "bl_taskq_nothing_to_do".
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT bl_err bl_taskq_try_run_one (bl_taskq* tq);
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call (to be run from the worker thread). Tries to run an
  element of the work queue. Blocks until an element is executed or until
  an error condition is found (e.g. timeout).
*/
/*---------------------------------------------------------------------------*/
#define bl_taskq_no_timeout ((bl_taskq_usec) 0)
extern BL_TASKQ_EXPORT
  bl_err bl_taskq_run_one (bl_taskq* tq, bl_taskq_usec timeout);
/*---------------------------------------------------------------------------*/
static inline bl_err bl_taskq_run_one_no_timeout (bl_taskq* tq)
{
  return bl_taskq_run_one (tq, bl_taskq_no_timeout);
}
/*---------------------------------------------------------------------------*/
/*
  Multi-threaded call. Posts an event to the worker queue.

  If this call returns "bl_taskq_would_overflow" you need to increment the
  "task_queue_capacity" parameter.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err bl_taskq_post (bl_taskq* tq, bl_taskq_id* id, bl_taskq_task task);
/*---------------------------------------------------------------------------*/
/*
  Multi-threaded call. Posts an delayed/timed event to the worker queue.

  The user is resposible of not overflowing the timer queue
  (delayed_task_capacity parameter on init). If the timer queue is overflowed it
  will only be known at dispatching time and it will be notified through the
  callback by an "bl_taskq_would_overflow" error code.

  If this call returns "bl_taskq_would_overflow" you need to increment the
  "task_queue_capacity" parameter.

  Note that to cancel a task through the "bl_taskq_post_try_cancel_delayed" you
  need the value of "abs_time_point".
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT bl_err bl_taskq_post_delayed_abs(
  bl_taskq*     tq,
  bl_taskq_id*  id,
  bl_timept32   abs_time_point,
  bl_taskq_task task
  );
/*---------------------------------------------------------------------------*/
static inline bl_err bl_taskq_post_delayed(
  bl_taskq*     tq,
  bl_taskq_id*  id,
  bl_timept32*  abs_time_point_out,
  bl_taskq_task task,
  bl_u32        delay_us
  )
{
  bl_assert (abs_time_point_out);
  *abs_time_point_out = bl_timept32_get() + bl_usec_to_timept32 (delay_us);
  return bl_taskq_post_delayed_abs (tq, id, *abs_time_point_out, task);
}
/*---------------------------------------------------------------------------*/
/*
  Multi-threaded call. Tries to cancel the given delayed event id. If succeeding
  the event callback will get invoked with the "bl_taskq_cancelled" error code.

  If this call returns "bl_taskq_would_overflow" you need to increment the
  "task_queue_capacity" parameter.

  Note that when this function returns "bl_ok" it doesn't mean that the
  timed cancelation has succeeded, it just means that posting the "try to
  cancel" request has succeeded. The request would still have "to make its way"
  to the worker thread and operate on the timer wait list. This scenario is
  impossible to acknowledge at the producer call point without heavy-duty
  locking, which defeats the purpose of this queue.

  With another bl_wording, the delayed task (to be cancelled) resources should be
  released on or after the callback has run, not just after succeeding on this
  function call.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
   bl_err bl_taskq_post_try_cancel_delayed(
    bl_taskq* tq, bl_taskq_id id, bl_timept32 abs_time_point
    );
/*---------------------------------------------------------------------------*/
/*
  Multi-threaded call. This function is to be used on shutdown.

  This irreversible call makes all subsequent "bl_taskq_post_*" and the
  bl_taskq_try_cancel_delayed to fail with the error code "bl_taskq_blocked".
  In other bl_words, no more "commands" can be added to the internal queue.

  After this call the "bl_taskq_run_one" call (which has timeout) will become
  equivalent to the non-blocking "bl_taskq_try_run_one" function and hence can
  return the "bl_taskq_nothing_to_do" error code when empty.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err bl_taskq_block (bl_taskq* tq);
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call (to be runned from the worker thread). This function is
  to be used on shutdown.

  Equivalent to "bl_taskq_try_run_one" but cancels one task (not ordered by time
  of entrance to the system) and the callbacks get the "bl_taskq_cancelled" error
  code on its first parameter.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err bl_taskq_try_cancel_one (bl_taskq* tq);
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call. Destructs (deallocates) a correctly shat down
  object (see below for a shut down procedure example).
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err bl_taskq_destroy (bl_taskq* bl_taskq, bl_alloc_tbl const* alloc);
/*---------------------------------------------------------------------------*/
/*
shut down procedure example1. Will run every planned task (and wait for all
delayed tasks to be scheduled):

  void worker_thread_func (void* context)
  {
    ...
    bl_taskq* tq = ...;
    bl_err err = bl_mkok();
    while (!err.bl || err.bl == bl_taskq_timeout) {
      err = bl_taskq_run_one (tq, 1000000);
    }
    ->bl_taskq ready to destroy
  }

  void some_other_thread_func (void* context)
  {
    ...
    bl_taskq* tq = ...;
    bl_taskq_block (tq);
  }
--------------------------------------------------------------------------------
shut down procedure example2. Will cancel tasks when noticing the blocking:

  void worker_thread_func (void* context)
  {
    ...
    bl_taskq* tq  = ...;
    int* run = ...;
    err = bl_mkok();
    while ((!err.bl || err.bl == bl_taskq_timeout) && *run) {
      err = bl_taskq_run_one (tq, 1000000);
    }
    while (!err.bl) {
      err = bl_taskq_try_cancel (tq);
    }
    ->bl_taskq ready to destroy
  }

  void some_other_thread_func (void* context)
  {
    ...
    bl_taskq* tq = ...;
    int*          run = ...;
    bl_taskq_block (tq);
    *run = false; -> should be atomic
  }
--------------------------------------------------------------------------------
*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /*TASK_QUEUE_H*/

