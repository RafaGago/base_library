#ifndef __TASKQ_DELAYED__H__
#define __TASKQ_DELAYED__H__

#include <bl/task_queue/task_queue.h>
#include <bl/base/flat_deadlines.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
typedef bl_flat_deadlines bl_taskq_delayed;
/*---------------------------------------------------------------------------*/
typedef struct bl_taskq_delayed_entry {
  bl_tstamp     time;
  bl_taskq_task task;
  bl_u32        id;
}
bl_taskq_delayed_entry;
/*---------------------------------------------------------------------------*/
extern bl_word bl_taskq_delayed_entry_cmp_func (void const* av, void const* bv);
/*---------------------------------------------------------------------------*/
define_bl_flat_deadlines_funcs(
    bl_taskq_delayed, bl_taskq_delayed_entry, bl_taskq_delayed_entry_cmp_func
    )
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __TASKQ_DELAYED__H__ */

