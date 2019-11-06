#ifndef __BL_TIME_EXTRAS_DEADLINE_H__
#define __BL_TIME_EXTRAS_DEADLINE_H__

#include <bl/base/static_assert.h>
#include <bl/time_extras/time_extras.h>

#ifdef BL_HAS_CPU_TIMEPT
  #include <bl/time_extras/generated/cpu_timept_deadline.h>
  #include <bl/time_extras/generated/fast_timept_deadline_from_cpu_timept.h>
#else
  #include <bl/time_extras/generated/fast_timept_deadline_from_timept64.h>
#endif

#endif /* __BL_DEADLINE_H__ */
