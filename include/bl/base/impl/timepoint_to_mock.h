#ifndef __BL_TIMEPOINT_TO_MOCK_H__
#define __BL_TIMEPOINT_TO_MOCK_H__

/*---------------------------------------------------------------------------*/
#define bl_timept32_get_freq() bl_usec_in_sec

#include <bl/base/impl/generated/time/timept32_funcs_microsecond_base.h>
#include <bl/base/impl/generated/time/sysclock32_funcs_second_base.h>
/*---------------------------------------------------------------------------*/
#define bl_timept64_get_freq() bl_nsec_in_sec

#include <bl/base/impl/generated/time/timept64_funcs_nanosecond_base.h>
#include <bl/base/impl/generated/time/sysclock64_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
#endif
