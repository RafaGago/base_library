#ifndef __BL_TIMEPOINT_TO_MOCK_H__
#define __BL_TIMEPOINT_TO_MOCK_H__

/*---------------------------------------------------------------------------*/
#define bl_timept32_get_freq() bl_usec_in_sec

#include <bl/base/impl/generated/timepoint/timepoint32_funcs_microsecond_base.h>
#include <bl/base/impl/generated/timepoint/timepoint32_sysclock_funcs_second_base.h>
/*---------------------------------------------------------------------------*/
#define bl_timept64_get_freq() bl_nsec_in_sec

#include <bl/base/impl/generated/timepoint/timepoint64_funcs_nanosecond_base.h>
#include <bl/base/impl/generated/timepoint/timepoint64_sysclock_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
#if BL_TIMEPOINT_BITS == 32
  #include <bl/base/impl/generated/timepoint/timepoint_funcs_microsecond_base.h>
  #include <bl/base/impl/generated/timepoint/timepoint_sysclock_funcs_second_base.h>
#else
  #include <bl/base/impl/generated/timepoint/timepoint_funcs_nanosecond_base.h>
  #include <bl/base/impl/generated/timepoint/timepoint_sysclock_funcs_nanosecond_base.h>
#endif
/*---------------------------------------------------------------------------*/
#endif
