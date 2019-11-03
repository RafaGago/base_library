/*---------------------------------------------------------------------------*/
#include <bl/task_queue/task_queue.h>
#include <bl/task_queue/delayed.h>

#include <bl/base/alignment.h>
#include <bl/base/assert.h>
#include <bl/base/integer_math.h>
#include <bl/base/deadline.h>
#include <bl/base/utility.h>
#include <bl/base/processor_pause.h>
#include <bl/base/atomic.h>
#include <bl/base/semaphore.h>

#include <bl/nonblock/mpmc_bt.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
enum cmd_type_e {
  cmd_task,
  cmd_delayed,
  cmd_delayed_cancel,
  cmd_wakeup,
};
/*---------------------------------------------------------------------------*/
typedef struct task_cmd {
  bl_taskq_task task;
}
task_cmd;
/*---------------------------------------------------------------------------*/
typedef struct delayed_cmd {
  bl_taskq_task task;
  bl_tstamp     tp;
}
delayed_cmd;
/*---------------------------------------------------------------------------*/
typedef struct delayed_cancel_cmd {
  bl_taskq_id id;
  bl_tstamp   tp;
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
  bl_u8       type;
}
cmd_elem;
/*---------------------------------------------------------------------------*/
enum bl_taskq_signals_e {
  bl_taskq_q_ok          = 0,
  bl_taskq_q_waiting_sem = 1,
  bl_taskq_q_blocked     = 2,
};
/*---------------------------------------------------------------------------*/
typedef struct bl_taskq {
  bl_mpmc_bt       queue;
  bl_mpmc_b_op     last_consumed;
  bl_tm_sem     sem;
  bl_taskq_delayed delayed;
}
bl_taskq;
/*---------------------------------------------------------------------------*/
static inline void
  bl_taskq_task_run (bl_taskq_task tq, bl_err err, bl_taskq_id tid)
{
  tq.func (err, tid, tq.context);
}
/*---------------------------------------------------------------------------*/
static inline bool
  bl_taskq_handle_cmd (bl_taskq* tq, cmd_elem* cmd, bl_taskq_id tid)
{
  switch (cmd->type) {
  case cmd_task: {
    bl_taskq_task_run (cmd->data.e.task, bl_mkok(), tid);
    return true;
  }
  case cmd_delayed: {
    /*no check for expiration here, as we might have expired events on the queue
      and we want to guarantee that they are called on expiration order*/
    bl_taskq_delayed_entry d;
    d.time = cmd->data.d.tp;
    d.task = cmd->data.d.task;
    d.id   = tid;
    bl_err err = bl_taskq_delayed_insert (&tq->delayed, &d);
    if (bl_unlikely (err.bl)) {
      bl_taskq_task_run (cmd->data.d.task, err, tid);
      return true;
    }
    return false;
  }
  case cmd_delayed_cancel: {
    bl_taskq_delayed_entry task;
    bl_taskq_delayed_entry match;
    match.time = cmd->data.dcancel.tp;
    match.id   = cmd->data.dcancel.id;
    if (bl_taskq_delayed_try_get_and_drop (&tq->delayed, &task, &match)) {
      bl_taskq_task_run(
        task.task, bl_mkerr (bl_cancelled), cmd->data.dcancel.id
        );
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
static inline bl_err bl_taskq_post_impl(
  bl_taskq* tq, cmd_elem* cmd, bl_taskq_id* id
  )
{
  bl_mpmc_b_op op;
  bl_err err = bl_mpmc_bt_produce_sig_fallback(
    &tq->queue,
    &op,
    cmd,
    true,
    bl_taskq_q_ok,
    bl_taskq_q_blocked,
    bl_taskq_q_blocked
    );
  if (bl_likely (err.bl == bl_ok)) {
    if (bl_unlikely (bl_mpmc_b_sig_decode (op) == bl_taskq_q_waiting_sem)) {
      bl_tm_sem_signal (&tq->sem);
    }
    *id = bl_mpmc_b_ticket_decode (op);
  }
  else {
    err = (err.bl != bl_preconditions) ? err : bl_mkerr (bl_locked);
  }
  return err;
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err bl_taskq_try_run_one (bl_taskq* tq)
{
  bl_taskq_delayed_entry const* expired;
  cmd_elem cmd;
  bl_err   err;
  bool     retry;
  do {
    retry   = false;
    expired = bl_taskq_delayed_get_head_if_expired (&tq->delayed, false, 0);
    if (expired) {
      bl_taskq_task_run (expired->task, bl_mkok(), expired->id);
      bl_taskq_delayed_drop_head (&tq->delayed);
      return bl_mkok();
    }
    err = bl_mpmc_bt_consume_sc (&tq->queue, &tq->last_consumed, &cmd);
    if (bl_likely (!err.bl)) {
      retry = !bl_taskq_handle_cmd(
        tq, &cmd, bl_mpmc_b_ticket_decode (tq->last_consumed)
        );
    }
    else if (bl_unlikely (err.bl == bl_empty)) {
      err = bl_mkerr (bl_nothing_to_do);
    }
  }
  while (retry);
  return err;
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err bl_taskq_run_one (bl_taskq* tq, bl_u32 timeout_us)
{
  bl_err err = bl_taskq_try_run_one (tq);
  if (!err.bl || err.bl != bl_nothing_to_do) {
    return err;
  }
  /*slow path*/
  bool   has_deadline = timeout_us != bl_taskq_no_timeout;
  bl_tstamp deadline     = 0;
  if (has_deadline) {
    err = bl_deadline_init (&deadline, timeout_us);
    if (bl_unlikely (err.bl)) {
      return err;
    }
  }
  while (true) {
    bl_mpmc_b_op expect = tq->last_consumed;
    bl_err    ierr   = bl_mkok();

    bl_taskq_delayed_entry const* dhead;
    dhead = bl_taskq_delayed_get_head (&tq->delayed);

    if (dhead) {
      if (has_deadline) {
        deadline = bl_deadline_min (deadline, dhead->time);
      }
      else {
        has_deadline = true;
        deadline     = dhead->time;
      }
    }
    bl_u32 sem_us;
    if (has_deadline) {
      bl_tstamp now = bl_tstamp_get();
      if (!bl_deadline_expired_explicit (deadline, now)) {
        sem_us = bl_tstamp_to_usec_ceil (deadline - now);
      }
      else {
        ierr = bl_mkerr (bl_timeout);
        goto try_again;
      }
    }
    else {
      sem_us = bl_tm_sem_infinity;
    }

    if (sem_us > BL_SCHED_TMIN_US || sem_us == bl_tm_sem_infinity) {
      ierr = bl_mpmc_bt_producer_signal_try_set_tmatch(
        &tq->queue, &expect, bl_taskq_q_waiting_sem
        );
      if (ierr.bl == bl_ok
        || bl_mpmc_b_sig_decode (expect) == bl_taskq_q_waiting_sem
        ) {
        ierr = bl_tm_sem_wait (&tq->sem, sem_us);
      }
    }
    else {
      /* greedy usage of the timeslice */
      for (bl_uword i = 0; i < 8; ++i) {
        bl_processor_pause();
        bl_processor_pause();
        bl_processor_pause();
        bl_processor_pause();
      }
      expect = bl_mpmc_b_op_encode (expect, bl_taskq_q_ok);
      ierr = bl_mkok();
    }

try_again:
    err = bl_taskq_try_run_one (tq);

    if (err.bl != bl_nothing_to_do) {
      break;
    }
    else {
      if (bl_unlikely (bl_mpmc_b_sig_decode (expect) == bl_taskq_q_blocked)) {
        /*no long blocking behavior on termination contexts*/
        return bl_mkerr (bl_nothing_to_do);
      }
      else if (bl_unlikely (ierr.bl == bl_timeout)) {
        bl_assert (timeout_us != bl_taskq_no_timeout);
        return bl_mkerr (bl_timeout);
      }
      /* else: keep going...*/
    }
  } /*while (true)*/
  return err;
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err bl_taskq_block (bl_taskq* tq)
{
  bl_mpmc_b_op expected =
    bl_mpmc_b_op_encode (tq->last_consumed, bl_taskq_q_ok);
  while(
    bl_mpmc_bt_producer_signal_try_set_tmatch(
      &tq->queue, &expected, bl_taskq_q_blocked
      ).bl == bl_preconditions
    );
  bl_tm_sem_signal (&tq->sem);
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err bl_taskq_try_cancel_one (bl_taskq* tq)
{
  cmd_elem cmd;
retry:
  if (bl_mpmc_bt_consume_sc(
       &tq->queue, &tq->last_consumed, &cmd
       ).bl == bl_ok) {
    switch (cmd.type) {
    case cmd_task: {
      bl_taskq_task_run(
        cmd.data.e.task,
        bl_mkerr (bl_cancelled),
        bl_mpmc_b_ticket_decode (tq->last_consumed)
        );
      return bl_mkok();
    }
    case cmd_delayed: {
      bl_taskq_task_run(
        cmd.data.d.task,
        bl_mkerr (bl_cancelled),
        bl_mpmc_b_ticket_decode (tq->last_consumed)
        );
      return bl_mkok();
    }
    default: {
      goto retry;
    }
    } /*switch*/
  }
  bl_taskq_delayed_entry const* dhead;
  dhead = bl_taskq_delayed_get_head (&tq->delayed);
  if (!dhead) {
    return bl_mkerr (bl_nothing_to_do);
  }
  bl_taskq_task_run (dhead->task, bl_mkerr (bl_cancelled), dhead->id);
  bl_taskq_delayed_drop_head (&tq->delayed);
  return bl_mkok();
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err
  bl_taskq_post (bl_taskq* tq, bl_taskq_id* id, bl_taskq_task task)
{
  bl_assert (tq && id);
  cmd_elem cmd;
  cmd.type        = cmd_task;
  cmd.data.e.task = task;
  return bl_taskq_post_impl (tq, &cmd, id);
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err bl_taskq_post_delayed_abs(
  bl_taskq*     tq,
  bl_taskq_id*  id,
  bl_tstamp     abs_time_point,
  bl_taskq_task task
  )
{
  bl_assert (tq && id);
  cmd_elem cmd;
  cmd.type        = cmd_delayed;
  cmd.data.d.task = task;
  cmd.data.d.tp   = abs_time_point;
  return bl_taskq_post_impl (tq, &cmd, id);
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err bl_taskq_post_try_cancel_delayed(
  bl_taskq* tq, bl_taskq_id id, bl_tstamp abs_time_point
  )
{
  bl_assert (tq);
  cmd_elem cmd;
  cmd.type            = cmd_delayed_cancel;
  cmd.data.dcancel.id = id;
  cmd.data.dcancel.tp = abs_time_point;
  bl_taskq_id dummy;
  return bl_taskq_post_impl (tq, &cmd, &dummy);
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err
  bl_taskq_destroy (bl_taskq* tq, bl_alloc_tbl const* alloc)
{
  bl_assert (tq && alloc);
  bl_mpmc_bt_destroy (&tq->queue, alloc);
  bl_taskq_delayed_destroy (&tq->delayed, alloc);
  bl_err err = bl_tm_sem_destroy (&tq->sem);
  bl_assert (!err.bl);
  bl_dealloc (alloc, tq);
  return err;
}
/*---------------------------------------------------------------------------*/
BL_TASKQ_EXPORT bl_err bl_taskq_init(
  bl_taskq**          tqueue,
  bl_alloc_tbl const* alloc,
  bl_uword            regular_capacity,
  bl_uword            delayed_capacity
  )
{
  bl_assert (tqueue && alloc);
  regular_capacity = bl_round_next_pow2_u (regular_capacity);
  delayed_capacity = bl_round_next_pow2_u (delayed_capacity);

  regular_capacity = bl_max (regular_capacity, 2);
  delayed_capacity = bl_max (delayed_capacity, 2);

  bl_taskq* tq = (bl_taskq*) bl_alloc (alloc, sizeof *tq);
  if (!tq) {
    return bl_mkerr (bl_alloc);
  }
  bl_err err;
  err = bl_mpmc_bt_init(
    &tq->queue,
    alloc,
    regular_capacity,
    sizeof (cmd_elem),
    bl_alignof (cmd_elem)
    );
  if (err.bl) {
    goto bl_taskq_free;
  }
  /*NOTE the delay list could be easily placed adjacently with the
    bl_taskq struct using just one allocator call, bl_oringb has the
    "init_extern" call in which the user externally provides the buffer */
  err = bl_taskq_delayed_init(
    &tq->delayed, bl_tstamp_get(), delayed_capacity, alloc
    );
  if (err.bl) {
    goto bl_taskq_queue_destroy;
  }
  err = bl_tm_sem_init (&tq->sem);
  if (err.bl) {
    goto do_bl_taskq_delayed_destroy;
  }
  *tqueue = tq;
  tq->last_consumed = bl_mpmc_b_first_op;
  return bl_mkok();

do_bl_taskq_delayed_destroy:
  bl_taskq_delayed_destroy (&tq->delayed, alloc);

bl_taskq_queue_destroy:
  bl_mpmc_bt_destroy (&tq->queue, alloc);

bl_taskq_free:
  bl_dealloc (alloc, tq);
  *tqueue = nullptr;
  return err;
}
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
