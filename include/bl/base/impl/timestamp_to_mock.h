#ifndef __BL_TIMESTAMP_TO_MOCK_H__
#define __BL_TIMESTAMP_TO_MOCK_H__

/*---------------------------------------------------------------------------*/
#define bl_tstamp32_get_freq() bl_usec_in_sec

#include <bl/base/impl/generated/timestamp/timestamp32_funcs_microsecond_base.h>
#include <bl/base/impl/generated/timestamp/timestamp32_sysclock_funcs_second_base.h>
/*---------------------------------------------------------------------------*/
#define bl_tstamp64_get_freq() bl_nsec_in_sec

#include <bl/base/impl/generated/timestamp/timestamp64_funcs_nanosecond_base.h>
#include <bl/base/impl/generated/timestamp/timestamp64_sysclock_funcs_nanosecond_base.h>
/*---------------------------------------------------------------------------*/
#if !defined BL_TIMESTAMP_64BIT
  #include <bl/base/impl/generated/timestamp/timestamp_funcs_microsecond_base.h>
  #include <bl/base/impl/generated/timestamp/timestamp_sysclock_funcs_second_base.h>
#else
  #include <bl/base/impl/generated/timestamp/timestamp_funcs_nanosecond_base.h>
  #include <bl/base/impl/generated/timestamp/timestamp_sysclock_funcs_nanosecond_base.h>
#endif /* #if !defined (BL_TIMESTAMP_64BIT) */
/*---------------------------------------------------------------------------*/
#endif
