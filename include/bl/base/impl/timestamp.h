#ifndef __BL_TIMESTAMP_H__
#define __BL_TIMESTAMP_H__

/*----------------------------------------------------------------------------*/
#include <bl/base/platform.h>
#include <bl/base/utility.h>
#include <bl/base/integer.h>
/*---------------------------------------------------------------------------*/
#if !defined BL_TIMESTAMP_64BIT
typedef bl_u32 bl_tstamp;
typedef bl_i32 bl_tstampdiff;
typedef bl_u32 bl_toffset;
/*the BL_FMT_TSTAMP printf format macro requires including
 "integer_printf_format.h" for it to be correctly expanded*/
#define BL_FMT_TSTAMP     BL_FMT_U32
#define BL_FMT_TSTAMPDIFF BL_FMT_I32
#define BL_FMT_TSTAMPOFF  BL_FMT_U32
#else
typedef bl_u64 bl_tstamp;
typedef bl_i64 bl_tstampdiff;
typedef bl_u64 bl_toffset;
/*the BL_FMT_TSTAMP printf format macro requires including
 "integer_printf_format.h" for it to be correctly expanded*/
#define BL_FMT_TSTAMP     BL_FMT_U64
#define BL_FMT_TSTAMPDIFF BL_FMT_I64
#define BL_FMT_TSTAMPOFF  BL_FMT_I64
#endif
#define BL_FMT_TSTAMPOFFSET BL_FMT_TSTAMPOFF

#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TSTAMP       BL_FMT_TSTAMP
#define FMT_TSTAMPDIFF   BL_FMT_TSTAMPDIFF
#define FMT_TSTAMPOFF    BL_FMT_TSTAMPOFF
#define FMT_TSTAMPOFFSET BL_FMT_TSTAMPOFFSET
#endif

/*---------------------------------------------------------------------------*/
#define bl_tstamp_max_safe_add_sub ((bl_utype_max (bl_tstamp) >> 1) - 1)
/*---------------------------------------------------------------------------*/
static inline bl_tstampdiff bl_tstamp_get_diff (bl_tstamp a, bl_tstamp b)
{
  bl_static_assert(
    bl_has_two_comp_arithmetic,
    "this is just valid on systems with complement of two arithmetic"
    );
  return (bl_tstampdiff) (a - b);
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_tstamp_max (bl_tstamp a, bl_tstamp b)
{
  return bl_tstamp_get_diff (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp bl_tstamp_min (bl_tstamp a, bl_tstamp b)
{
  return bl_tstamp_get_diff (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
#if defined (BL_TSTAMP_CUSTOM_INCLUDE)
  #include BL_TSTAMP_CUSTOM_INCLUDE
#elif defined (BL_TSTAMP_MOCK_FOR_TESTS)
  #include <bl/base/impl/timestamp_to_mock.h>
#elif defined (BL_POSIX)
  #include <bl/base/impl/timestamp_posix.h>
#elif defined (BL_WINDOWS)
  #include <bl/base/impl/timestamp_windows.h>
#else
  #error "no bl_tstamp implementation available"
#endif
/*---------------------------------------------------------------------------*/
#endif /*__BL_TIMESTAMP_H__*/
