#ifndef __BL_TIMESTAMP_H__
#define __BL_TIMESTAMP_H__

/*----------------------------------------------------------------------------*/
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/integer.h>
/*---------------------------------------------------------------------------*/
#if !defined BL_TIMESTAMP_64BIT
typedef u32 tstamp;
typedef i32 tstampdiff;
typedef u32 toffset;
#else
typedef u64 tstamp;
typedef i64 tstampdiff;  
typedef u64 toffset;
#endif
/*---------------------------------------------------------------------------*/
#define tstamp_max_safe_add_sub ((utype_max (tstamp) >> 1) - 1)
/*---------------------------------------------------------------------------*/
static inline tstampdiff tstamp_get_diff (tstamp a, tstamp b)
{
  static_assert(
    bl_has_two_comp_arithmetic,
    "this is just valid on systems with complement of two arithmetic"
    );
  return (tstampdiff) (a - b);
}
/*---------------------------------------------------------------------------*/
#if defined (BL_TSTAMP_CUSTOM_INCLUDE)
  #include BL_TSTAMP_CUSTOM_INCLUDE
#elif defined (BL_TSTAMP_MOCK_FOR_TESTS)
  #include <bl/hdr/base/impl/timestamp_to_mock.h>
#elif defined (BL_POSIX)
  #include <bl/hdr/base/impl/timestamp_posix.h>
#elif defined (BL_WINDOWS)
  #include <bl/hdr/base/impl/timestamp_windows.h>  
#else 
  #error "no tstamp implementation available"
#endif
/*---------------------------------------------------------------------------*/
#endif /*__BL_TIMESTAMP_H__*/