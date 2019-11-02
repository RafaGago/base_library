#include <bl/base/preprocessor_basic.h>
#include <bl/task_queue/delayed.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
bl_word bl_taskq_delayed_entry_cmp_func (void const* av, void const* bv)
{
  bl_taskq_delayed_entry const* a = (bl_taskq_delayed_entry const*) av;
  bl_taskq_delayed_entry const* b = (bl_taskq_delayed_entry const*) bv;
  return (bl_word) ((bl_i32) a->id) - ((bl_i32) b->id);
}
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif
