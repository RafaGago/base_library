#include <bl/base/preprocessor_basic.h>
#include <bl/task_queue/delayed.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
word taskq_delayed_entry_cmp_func (void const* av, void const* bv)
{
  taskq_delayed_entry const* a = (taskq_delayed_entry const*) av;
  taskq_delayed_entry const* b = (taskq_delayed_entry const*) bv;
  return (word) ((i32) a->id) - ((i32) b->id);
}
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif
