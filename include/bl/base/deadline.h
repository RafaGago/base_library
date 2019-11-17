#ifndef __BL_BASE_DEADLINE_H__
#define __BL_BASE_DEADLINE_H__

#include <bl/base/static_assert.h>
#include <bl/base/time.h>

#include <bl/base/impl/generated/time/timept64_deadline.h>
#include <bl/base/impl/generated/time/timept32_deadline.h>
#if BL_TIMEPT_BITS == 64
  #include <bl/base/impl/generated/time/timept_deadline_from_timept64.h>
#else
  bl_static_assert_ns (BL_TIMEPT_BITS == 32);
  #include <bl/base/impl/generated/time/timept_deadline_from_timept32.h>
#endif

#include <bl/base/impl/generated/time/sysclock64_deadline.h>
#include <bl/base/impl/generated/time/sysclock32_deadline.h>
#if BL_SYSCLOCK_BITS == 64
  #include <bl/base/impl/generated/time/sysclock_deadline_from_sysclock64.h>
#else
  bl_static_assert_ns (BL_SYSCLOCK_BITS == 32);
  #include <bl/base/impl/generated/time/sysclock_deadline_from_sysclock32.h>
#endif

#endif /* __BL_BASE_DEADLINE_H__ */
