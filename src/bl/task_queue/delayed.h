#ifndef __TASKQ_DELAYED__H__
#define __TASKQ_DELAYED__H__

#include <bl/task_queue/task_queue.h>

#include <bl/base/deadline.h>
#include <bl/base/ordered_ringbuffer.h>
#include <bl/base/error.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
typedef struct delayed_entry {
  tstamp     deadl;
  u32        id;
  taskq_task task;
}
delayed_entry;
/*---------------------------------------------------------------------------*/
#if 0
static inline delayed_entry 
  delayed_entry_rv (deadline dead d, u32 id)
{
  delayed_entry de;
  de.dead = d;
  de.id   = id;
  return de;
}
#endif
/*---------------------------------------------------------------------------*/
define_oringb_types (delayed_ringb, delayed_entry)
/*---------------------------------------------------------------------------*/
typedef struct delayed {
  delayed_ringb list;
  tstamp        time_offset;
}
delayed;
/*---------------------------------------------------------------------------*/
extern bl_err delayed_init(
  delayed* dl, alloc_tbl const* alloc, uword capacity
  );
/*---------------------------------------------------------------------------*/
extern void delayed_destroy (delayed* dl, alloc_tbl const* alloc);
/*---------------------------------------------------------------------------*/
extern bl_err delayed_insert (delayed* dl, delayed_entry* e);
/*---------------------------------------------------------------------------*/
extern bool delayed_try_get_task_and_drop(
  delayed* dl, taskq_task* dst, tstamp d, u32 id
  );
/*---------------------------------------------------------------------------*/
extern delayed_entry const* delayed_get_head (delayed* dl);
/*---------------------------------------------------------------------------*/
extern delayed_entry const* delayed_get_head_if_expired (delayed* dl);
/*---------------------------------------------------------------------------*/
extern void delayed_drop_head (delayed* dl);
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __TASKQ_DELAYED__H__ */
