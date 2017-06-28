#ifndef __TASKQ_DELAYED__H__
#define __TASKQ_DELAYED__H__

#include <bl/task_queue/task_queue.h>
#include <bl/base/flat_deadlines.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
typedef flat_deadlines taskq_delayed;
/*---------------------------------------------------------------------------*/
typedef struct taskq_delayed_entry {
  tstamp     time;
  taskq_task task;
  u32        id;
}
taskq_delayed_entry;
/*---------------------------------------------------------------------------*/
extern word taskq_delayed_entry_cmp_func (void const* av, void const* bv);
/*---------------------------------------------------------------------------*/
flat_deadlines_define_wrap_funcs(
    taskq_delayed, taskq_delayed_entry, taskq_delayed_entry_cmp_func
    )
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __TASKQ_DELAYED__H__ */

