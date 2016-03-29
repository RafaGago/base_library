#include <bl/base/preprocessor_basic.h>
#include <bl/task_queue/delayed.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
static inline word taskq_delayed_data_cmp_func(
  taskq_delayed_data const* a, taskq_delayed_data const* b
  )
{
  return (word) ((i32) a->id) - ((i32) b->id);
}
/*---------------------------------------------------------------------------*/
define_flat_deadlines_funcs(
  taskq_delayed, taskq_delayed_data, taskq_delayed_data_cmp_func, pp_empty()
  )

#ifdef __cplusplus
} /*extern "C" {*/
#endif
