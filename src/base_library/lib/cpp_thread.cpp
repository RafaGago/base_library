#include <base_library/hdr/platform.h>

#ifndef BL_HAS_C11_THREAD

#include <thread>
#include <type_traits>
#include <base_library/lib/cpp_thread.h>

extern "C" {
/*---------------------------------------------------------------------------*/
static_assert_outside_func_ns(
  std::alignment_of<std::thread>::value == std::alignment_of<bl_thread>::value
  );
static_assert_outside_func_ns (sizeof (std::thread) == sizeof (bl_thread));
/*---------------------------------------------------------------------------*/
int bl_thread_init (bl_thread* t, bl_thread_func f, void* context)
{
  try {
    new ((std::thread*) t) std::thread (f, context);
    return bl_thread_success;
  }
  catch (...) {    
    return bl_thread_error;
  }
}
/*---------------------------------------------------------------------------*/
void bl_thread_yield (void)
{
  std::this_thread::yield();
}
/*---------------------------------------------------------------------------*/
int bl_thread_join (bl_thread* t)
{
  try {
    ((std::thread*) t)->join();
    return bl_thread_success;
  }
  catch (...) {
    return bl_thread_error;   
  }
}
/*---------------------------------------------------------------------------*/
} //extern "C" {

#endif

