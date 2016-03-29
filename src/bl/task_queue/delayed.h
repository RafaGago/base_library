#ifndef __TASKQ_DELAYED__H__
#define __TASKQ_DELAYED__H__

#include <bl/task_queue/task_queue.h>
#include <bl/base/flat_deadlines.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
typedef struct taskq_delayed_data {
  u32        id;
  taskq_task task;
}
taskq_delayed_data;
/*---------------------------------------------------------------------------*/
define_flat_deadlines_types (taskq_delayed, taskq_delayed_data)
declare_flat_deadlines_funcs (taskq_delayed, taskq_delayed_data, extern)
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __TASKQ_DELAYED__H__ */

