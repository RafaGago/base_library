/*---------------------------------------------------------------------------*/
#include <bl/lib/task_queue/task_queue.h>
#include <bl/lib/task_queue/delayed.h>

#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/integer_math.h>
#include <bl/hdr/base/deadline.h>
#include <bl/hdr/base/processor_pause.h>
#include <bl/hdr/base/atomic.h>
#include <bl/lib/base/semaphore.h>

#include <bl/lib/nonblock/mpmc_b.h>
/*---------------------------------------------------------------------------*/
enum cmd_type_e {
  cmd_task,
  cmd_delayed,
  cmd_delayed_cancel,
  cmd_wakeup,
};
/*---------------------------------------------------------------------------*/
typedef struct task_cmd {
  taskq_task task;
}
task_cmd;
/*---------------------------------------------------------------------------*/
typedef struct delayed_cmd {  
  taskq_task task;
  tstamp     tp;
}
delayed_cmd;
/*---------------------------------------------------------------------------*/
typedef struct delayed_cancel_cmd {  
  taskq_id id;
  tstamp   tp;
}
delayed_cancel_cmd;
/*---------------------------------------------------------------------------*/
typedef union all_cmds {
  task_cmd           e;
  delayed_cmd        d;
  delayed_cancel_cmd dcancel;
}
all_cmds;
/*---------------------------------------------------------------------------*/
typedef struct cmd_elem {  
  all_cmds data;
  u8       type;
}
cmd_elem;
/*---------------------------------------------------------------------------*/
enum taskq_signals_e {
  taskq_q_ok          = 0,
  taskq_q_waiting_sem = 1,
  taskq_q_blocked     = 2,
};
/*---------------------------------------------------------------------------*/
typedef struct taskq {
  mpmc_b       queue;  
  mpmc_b_info  last_consumed;
  bl_tm_sem    sem;
  delayed      delayed_ls;
}
taskq;
/*---------------------------------------------------------------------------*/
static inline void taskq_task_run (taskq_task tq, bl_err err, taskq_id tid) 
{
  tq.func (err, tid, tq.context);
}
/*---------------------------------------------------------------------------*/
static inline bool taskq_handle_cmd (taskq* tq, cmd_elem* cmd, taskq_id tid)
{
  switch (cmd->type) {
  case cmd_task: {
    taskq_task_run (cmd->data.e.task, bl_ok, tid);
    return true;
  }
  case cmd_delayed: {
    /*no check for expiration here, as we might have expired events on the queue
      and we want to guarantee that they are called on expiration order*/
    delayed_entry d;
    d.task     = cmd->data.d.task;
    d.deadl    = cmd->data.d.tp;
    d.id       = tid;
    bl_err err = delayed_insert (&tq->delayed_ls, &d);
    if (unlikely (err)) {
      taskq_task_run (cmd->data.d.task, err, tid);
      return true;
    }
    return false;
  }
  case cmd_delayed_cancel: {
    taskq_task task;
    if (delayed_try_get_task_and_drop(
        &tq->delayed_ls,
        &task,
        cmd->data.dcancel.tp,
        cmd->data.dcancel.id
        )) {
      taskq_task_run (task, bl_cancelled, cmd->data.dcancel.id);
      return true;
    }
    return false;
  } 
  case cmd_wakeup:
  default:
    return false;
  } /*switch*/
}
/*---------------------------------------------------------------------------*/
static inline bl_err taskq_post_impl(
  taskq* tq, cmd_elem* cmd, taskq_id* id
  )
{
  mpmc_b_info inf;
  bl_err err = mpmc_b_produce_sig_fallback(
    &tq->queue, &inf, cmd, true, taskq_q_ok, taskq_q_blocked, taskq_q_blocked
    );
  if (likely (err == bl_ok)) {
    if (unlikely (inf.signal == taskq_q_waiting_sem)) {
      bl_tm_sem_signal (&tq->sem);
    }
    *id = inf.transaction;
  }
  else {
    err = (err != bl_preconditions) ? err : bl_locked;
  }
  return err;
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_try_run_one (taskq* tq)
{
  delayed_entry const* expired;
  cmd_elem             cmd;
  bl_err               err;
  bool                 retry;
  do {
    retry   = false;
    expired = delayed_get_head_if_expired (&tq->delayed_ls);
    if (expired) {
      taskq_task_run (expired->task, bl_ok, expired->id);
      delayed_drop_head (&tq->delayed_ls);
      return bl_ok;
    }
    err = mpmc_b_consume_single_c (&tq->queue, &tq->last_consumed, &cmd);
    if (likely (!err)) {
      retry = !taskq_handle_cmd (tq, &cmd, tq->last_consumed.transaction);
    }
    else if (unlikely (err = bl_empty)) {
      err = bl_nothing_to_do;
    }
  } 
  while (retry);
  return err;
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_run_one (taskq* tq, u32 timeout_us)
{
  bl_err err = taskq_try_run_one (tq);
  while (err == bl_nothing_to_do) {
    mpmc_b_info  expected = tq->last_consumed;
    bl_err       ierr     = bl_ok;
   	u32          sem_time = 0;

    delayed_entry const* dhead;
    dhead = delayed_get_head (&tq->delayed_ls);

    if (!dhead) {
      static_assert_ns (taskq_no_timeout == bl_tm_sem_infinity);
      sem_time = timeout_us;
    }
    else {
      tstamp now = bl_get_tstamp();
      if (deadline_expired_explicit (dhead->deadl, now)) {
        goto try_again;
      }
      sem_time = bl_tstamp_usec_ceil (dhead->deadl - now);
      if (timeout_us != taskq_no_timeout) {
        sem_time = bl_min (timeout_us, sem_time);
      }
    }  
    if (sem_time > BL_SCHED_TMIN_US || sem_time == bl_tm_sem_infinity) {
      ierr = mpmc_b_producer_signal_try_set_tmatch(
        &tq->queue, &expected, taskq_q_waiting_sem
        );
      if (ierr == bl_ok || expected.signal == taskq_q_waiting_sem) {
        ierr = bl_tm_sem_wait (&tq->sem, sem_time);
      }
    }
    else {
      /* giving up a time slice can introduce latency -> busy waiting */
      for (uword i = 0; i < 10; ++i) {
        processor_pause();
      }
      expected.signal = taskq_q_ok;
      ierr            = bl_ok;
    }
try_again:
    err = taskq_try_run_one (tq);

    if (err == bl_nothing_to_do) {
      if (unlikely (expected.signal == taskq_q_blocked)) {
        /*no long blocking behavior on termination contexts*/
        return bl_nothing_to_do;
      }
      else if (unlikely (ierr == bl_timeout)) {
        return bl_timeout;
      }
      /* else: keep going...*/
    }
  }
  return err;
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_block (taskq* tq)
{
  mpmc_b_info expected;
  expected        = tq->last_consumed;
  expected.signal = taskq_q_ok;
  while(
    mpmc_b_producer_signal_try_set_tmatch(
	    &tq->queue, &expected, taskq_q_blocked
	    ) == bl_preconditions
    );
  bl_tm_sem_signal (&tq->sem);
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_try_cancel_one (taskq* tq)
{
  cmd_elem cmd;
retry:
  if (mpmc_b_consume_single_c(
       &tq->queue, &tq->last_consumed, &cmd
       ) == bl_ok) {
    switch (cmd.type) {
    case cmd_task: {
      taskq_task_run(
        cmd.data.e.task, bl_cancelled, tq->last_consumed.transaction
        );
      return bl_ok;
    }
    case cmd_delayed: {
      taskq_task_run(
        cmd.data.d.task, bl_cancelled, tq->last_consumed.transaction
        );
      return bl_ok;
    }
    default: {
      goto retry;
    }
    } /*switch*/        
  }
  delayed_entry const* dhead;
  dhead = delayed_get_head (&tq->delayed_ls);
  if (!dhead) {
    return bl_nothing_to_do;
  }
  taskq_task_run (dhead->task, bl_cancelled, dhead->id);
  delayed_drop_head (&tq->delayed_ls);
  return bl_ok;
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_post (taskq* tq, taskq_id* id, taskq_task task)
{
  bl_assert (tq && id);
  cmd_elem cmd;
  cmd.type        = cmd_task;
  cmd.data.e.task = task;
  return taskq_post_impl (tq, &cmd, id);
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_post_delayed(
  taskq*     tq, 
  taskq_id*  id, 
  tstamp*    scheduled_time_point, 
  taskq_task task, 
  u32        delay_us
  )
{
  bl_assert (tq && id && h);
  cmd_elem cmd;
  cmd.type        = cmd_delayed;
  cmd.data.d.task = task;
  bl_err err      = deadline_init (&cmd.data.d.tp, delay_us);
  if (unlikely (err)) {
    return err;
  }
  *scheduled_time_point = cmd.data.d.tp;
  return taskq_post_impl (tq, &cmd, id);
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_post_try_cancel_delayed(
  taskq* tq, taskq_id id, tstamp scheduled_time_point
  )
{
  bl_assert (tq);
  cmd_elem cmd;
  cmd.type            = cmd_delayed_cancel;
  cmd.data.dcancel.id = id;
  cmd.data.dcancel.tp = scheduled_time_point;
  taskq_id dummy;
  return taskq_post_impl (tq, &cmd, &dummy);
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_destroy (taskq* tq, alloc_tbl const* alloc)
{
  bl_assert (tq && alloc);
  mpmc_b_destroy (&tq->queue, alloc);
  delayed_destroy (&tq->delayed_ls, alloc);
  bl_err err = bl_tm_sem_destroy (&tq->sem);
  bl_assert (!err);
  bl_dealloc (alloc, tq);
  return err;
}
/*---------------------------------------------------------------------------*/
bl_err TASKQ_EXPORT taskq_init(
  taskq**          tqueue,
  alloc_tbl const* alloc,
  uword            regular_capacity,
  uword            delayed_capacity
  )
{
  bl_assert (tqueue && alloc);
  regular_capacity = round_next_pow2_u (regular_capacity);
  delayed_capacity = round_next_pow2_u (delayed_capacity);

  if (regular_capacity < 2 || delayed_capacity < 2) {
    return bl_invalid;
  }
  taskq* tq = bl_alloc (alloc, sizeof *tq);
  if (!tq) {
    return bl_alloc;
  }
  bl_err err;
  err = mpmc_b_init (&tq->queue, alloc, regular_capacity, cmd_elem);
  if (err) {
    goto taskq_free;
  }
  /*NOTE the delay list could be easily placed adjacently with the
    taskq struct using just one allocator call, oringb has the "init_extern"
    call in which the user externally provides the buffer */
  err = delayed_init (&tq->delayed_ls, alloc, delayed_capacity);
  if (err) {
    goto taskq_queue_destroy;
  }
  err = bl_tm_sem_init (&tq->sem);
  if (err) {
    goto taskq_delayed_destroy;
  }
  *tqueue = tq;
  tq->last_consumed.transaction = 0;
  tq->last_consumed.signal      = 0;
  return bl_ok;

taskq_delayed_destroy:
  delayed_destroy (&tq->delayed_ls, alloc);

taskq_queue_destroy:
  mpmc_b_destroy (&tq->queue, alloc);

taskq_free:
  bl_dealloc (alloc, tq);
  *tqueue = nullptr;
  return err;
}
/*---------------------------------------------------------------------------*/

