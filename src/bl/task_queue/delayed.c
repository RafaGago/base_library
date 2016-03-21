#include <bl/base/preprocessor_basic.h>
#include <bl/task_queue/delayed.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
static inline word delayed_data_cmp_func(
  delayed_data const* a, delayed_data const* b
  )
{
  return (word) ((i32) a->id) - ((i32) b->id);
}
/*---------------------------------------------------------------------------*/
define_flat_deadlines_funcs(
  delayed, delayed_data, delayed_data_cmp_func, pp_empty()
  )

#ifdef __cplusplus
} /*extern "C" {*/
#endif
