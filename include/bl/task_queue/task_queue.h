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
typedef uword taskq_id;
typedef u32   taskq_usec;
/*---------------------------------------------------------------------------*/
typedef void (*taskq_task_func) (bl_err error, taskq_id id, void* context);
/*---------------------------------------------------------------------------*/
typedef struct taskq_task {
  taskq_task_func func;
  void*           context;
}
taskq_task;
/*---------------------------------------------------------------------------*/
/* rv suffix = r-value */
/*---------------------------------------------------------------------------*/
static inline taskq_task taskq_task_rv (taskq_task_func func, void* context)
{
  taskq_task tmp = { func, context };
  return tmp;
}
/*---------------------------------------------------------------------------*/
/* Interface                                                                 */
/*---------------------------------------------------------------------------*/
typedef struct taskq taskq;  
/*---------------------------------------------------------------------------*/
/* 
  Single-threaded call. 

  task_queue_capacity: maximum number of elements/comands that the task queue
  can hold.

  delayed_task_capacity: maximum number of delayed tasks acknowledged on the 
  system.

  If receiving the "taskq_invalid" error code and everything looks right 
  consider that both maximum "regular_task_queue_capacity" and "
  delayed_task_queue_capacity" may not be allowed to be as big as its data 
  type allows.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT 
  bl_err taskq_init(
    taskq**          tq,
    alloc_tbl const* alloc,
    uword            task_queue_capacity,
    uword            delayed_task_capacity
    );
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call (to be run from the worker thread). Tries to run an 
  element of the work queue. If no elements were available returns 
  "taskq_nothing_to_do".
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT bl_err taskq_try_run_one (taskq* tq);
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call (to be run from the worker thread). Tries to run an
  element of the work queue. Blocks until an element is executed or until 
  an error condition is found (e.g. timeout).
*/
/*---------------------------------------------------------------------------*/
#define taskq_no_timeout ((taskq_usec) 0)
extern BL_TASKQ_EXPORT 
  bl_err taskq_run_one (taskq* tq, taskq_usec timeout);
/*---------------------------------------------------------------------------*/
static inline bl_err taskq_run_one_no_timeout (taskq* tq)
{
  return taskq_run_one (tq, taskq_no_timeout);
}  
/*---------------------------------------------------------------------------*/
/* 
  Multi-threaded call. Posts an event to the worker queue.

  If this call returns "taskq_would_overflow" you need to increment the
  "task_queue_capacity" parameter.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT 
  bl_err taskq_post (taskq* tq, taskq_id* id, taskq_task task);
/*---------------------------------------------------------------------------*/
/* 
  Multi-threaded call. Posts an delayed/timed event to the worker queue. 

  The user is resposible of not overflowing the timer queue 
  (delayed_task_capacity parameter on init). If the timer queue is overflowed it 
  will only be known at dispatching time and it will be notified through the
  callback by an "taskq_would_overflow" error code.

  If this call returns "taskq_would_overflow" you need to increment the
  "task_queue_capacity" parameter.

  Note that to cancel a task through the "taskq_post_try_cancel_delayed" you
  need the value of "abs_time_point".
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT bl_err taskq_post_delayed_abs(
  taskq*     tq, 
  taskq_id*  id, 
  tstamp     abs_time_point, 
  taskq_task task
  );
/*---------------------------------------------------------------------------*/
static inline bl_err taskq_post_delayed(
  taskq*     tq, 
  taskq_id*  id, 
  tstamp*    abs_time_point_out, 
  taskq_task task, 
  u32        delay_us
  )
{
  bl_assert (abs_time_point_out);
  *abs_time_point_out = bl_get_tstamp() + bl_usec_to_tstamp (delay_us);
  return taskq_post_delayed_abs (tq, id, *abs_time_point_out, task);
}
/*---------------------------------------------------------------------------*/
/*
  Multi-threaded call. Tries to cancel the given delayed event id. If succeeding
  the event callback will get invoked with the "taskq_cancelled" error code.

  If this call returns "taskq_would_overflow" you need to increment the
  "task_queue_capacity" parameter.

  Note that when this function returns "bl_ok" it doesn't mean that the 
  timed cancelation has succeeded, it just means that posting the "try to 
  cancel" request has succeeded. The request would still have "to make its way"
  to the worker thread and operate on the timer wait list. This scenario is 
  impossible to acknowledge at the producer call point without heavy-duty
  locking, which defeats the purpose of this queue.

  With another wording, the delayed task (to be cancelled) resources should be
  released on or after the callback has run, not just after succeeding on this
  function call.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
   bl_err taskq_post_try_cancel_delayed(
    taskq* tq, taskq_id id, tstamp abs_time_point
    );
/*---------------------------------------------------------------------------*/
/*
  Multi-threaded call. This function is to be used on shutdown.

  This irreversible call makes all subsequent "taskq_post_*" and the
  taskq_try_cancel_delayed to fail with the error code "taskq_blocked". 
  In other words, no more "commands" can be added to the internal queue.

  After this call the "taskq_run_one" call (which has timeout) will become
  equivalent to the non-blocking "taskq_try_run_one" function and hence can 
  return the "taskq_nothing_to_do" error code when empty.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err taskq_block (taskq* tq);
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call (to be runned from the worker thread). This function is
  to be used on shutdown.

  Equivalent to "taskq_try_run_one" but cancels one task (not ordered by time
  of entrance to the system) and the callbacks get the "taskq_cancelled" error
  code on its first parameter.
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err taskq_try_cancel_one (taskq* tq);
/*---------------------------------------------------------------------------*/
/*
  Single-threaded call. Destructs (deallocates) a correctly shat down 
  object (see below for a shut down procedure example).
*/
/*---------------------------------------------------------------------------*/
extern BL_TASKQ_EXPORT
  bl_err taskq_destroy (taskq* taskq, alloc_tbl const* alloc);
/*---------------------------------------------------------------------------*/
/*
shut down procedure example1. Will run every planned task (and wait for all 
delayed tasks to be scheduled):

  void worker_thread_func (void* context)
  {
    ...
    taskq* tq = ...;
    bl_err err = bl_ok;
    while (!err || err == taskq_timeout) {
      err = taskq_run_one (tq, 1000000);
    }
    ->taskq ready to destroy 
  }

  void some_other_thread_func (void* context)
  {
    ...
    taskq* tq = ...;
    taskq_block (tq);
  }
--------------------------------------------------------------------------------
shut down procedure example2. Will cancel tasks when noticing the blocking:

  void worker_thread_func (void* context)
  {
    ...
    taskq* tq  = ...;
    int* run = ...;   
    err = bl_ok;
    while ((!err || err == taskq_timeout) && *run) {
      err = taskq_run_one (tq, 1000000);
    }
    while (!err) {
      err = taskq_try_cancel (tq);
    }
    ->taskq ready to destroy 
  }

  void some_other_thread_func (void* context)
  {
    ...
    taskq* tq = ...;
    int*          run = ...;
    taskq_block (tq);
    *run = false; -> should be atomic
  }
--------------------------------------------------------------------------------
*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /*TASK_QUEUE_H*/

