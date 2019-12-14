#ifndef __BL_TIMEPOINT_H__
#define __BL_TIMEPOINT_H__

/*----------------------------------------------------------------------------*/
#include <bl/base/platform.h>
#include <bl/base/utility.h>
#include <bl/base/integer.h>

/*---------------------------------------------------------------------------*/
/* 32 bit */
/*---------------------------------------------------------------------------*/
typedef bl_u32 bl_timept32;
typedef bl_i32 bl_timept32diff;
typedef bl_u32 bl_timeoft32;
/*---------------------------------------------------------------------------*/
static inline bl_timept32diff
  bl_timept32_get_diff (bl_timept32 a, bl_timept32 b)
{
  bl_static_assert_funcscope(
    bl_has_two_comp_arithmetic,
    "this is just valid on systems with complement of two arithmetic"
    );
  return (bl_timept32diff) (a - b);
}
/*---------------------------------------------------------------------------*/
#define bl_timept32_max_safe_add_sub ((bl_utype_max (bl_timept32) >> 1) - 1)
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_max (bl_timept32 a, bl_timept32 b)
{
  return bl_timept32_get_diff (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept32 bl_timept32_min (bl_timept32 a, bl_timept32 b)
{
  return bl_timept32_get_diff (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
/*the BL_FMT_TPOINT printf format macro requires including
 "integer_printf_format.h" for it to be correctly expanded*/
#define BL_FMT_TPOINT32       BL_FMT_U32
#define BL_FMT_TPOINT32DIFF   BL_FMT_I32
#define BL_FMT_TPOINT32OFF    BL_FMT_U32
#define BL_FMT_TPOINT32OFFSET BL_FMT_TPOINT32OFF
#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TPOINT32       BL_FMT_TPOINT32
#define FMT_TPOINT32DIFF   BL_FMT_TPOINT32DIFF
#define FMT_TPOINT32OFF    BL_FMT_TPOINT32OFF
#define FMT_TPOINT32OFFSET BL_FMT_TPOINT32OFFSET
#endif
/*---------------------------------------------------------------------------*/
/* 64 bit */
/*---------------------------------------------------------------------------*/
typedef bl_u64 bl_timept64;
typedef bl_i64 bl_timept64diff;
typedef bl_u64 bl_timeoft64;
/*---------------------------------------------------------------------------*/
static inline bl_timept64diff
  bl_timept64_get_diff (bl_timept64 a, bl_timept64 b)
{
  bl_static_assert_funcscope(
    bl_has_two_comp_arithmetic,
    "this is just valid on systems with complement of two arithmetic"
    );
  return (bl_timept64diff) (a - b);
}
/*---------------------------------------------------------------------------*/
#define bl_timept64_max_safe_add_sub ((bl_utype_max (bl_timept64) >> 1) - 1)
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_max (bl_timept64 a, bl_timept64 b)
{
  return bl_timept64_get_diff (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_timept64 bl_timept64_min (bl_timept64 a, bl_timept64 b)
{
  return bl_timept64_get_diff (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
/*the BL_FMT_TPOINT printf format macro requires including
 "integer_printf_format.h" for it to be correctly expanded*/
#define BL_FMT_TPOINT64       BL_FMT_U64
#define BL_FMT_TPOINT64DIFF   BL_FMT_I64
#define BL_FMT_TPOINT64OFF    BL_FMT_U64
#define BL_FMT_TPOINT64OFFSET BL_FMT_TPOINT64OFF
#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TPOINT64       BL_FMT_TPOINT64
#define FMT_TPOINT64DIFF   BL_FMT_TPOINT64DIFF
#define FMT_TPOINT64OFF    BL_FMT_TPOINT64OFF
#define FMT_TPOINT64OFFSET BL_FMT_TPOINT64OFFSET
#endif
/*---------------------------------------------------------------------------*/
/* common */
/*---------------------------------------------------------------------------*/
#if BL_TIMEPOINT_BITS == 32

typedef bl_timept32     bl_timept;
typedef bl_timept32diff bl_timeptdiff;
typedef bl_timeoft32    bl_timeoft;

#define bl_timept_get_diff(x, y)   bl_timept32_get_diff (x, y)
#define bl_timept_max_safe_add_sub bl_timept32_max_safe_add_sub
#define bl_timept_max(x, y)        bl_timept32_max (x, y)
#define bl_timept_min(x, y)        bl_timept32_min (x, y)

#define BL_FMT_TPOINT       BL_FMT_TPOINT32
#define BL_FMT_TPOINTDIFF   BL_FMT_TPOINT32DIFF
#define BL_FMT_TPOINTOFF    BL_FMT_TPOINT32OFF
#define BL_FMT_TPOINTOFFSET BL_FMT_TPOINT32OFFSET

#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TPOINT       FMT_TPOINT32
#define FMT_TPOINTDIFF   FMT_TPOINT32DIFF
#define FMT_TPOINTOFF    FMT_TPOINT32OFF
#define FMT_TPOINTOFFSET FMT_TPOINT32OFFSET
#endif

#else /* 64 bit timepoints */

typedef bl_timept64     bl_timept;
typedef bl_timept64diff bl_timeptdiff;
typedef bl_timeoft64    bl_timeoft;

#define bl_timept_get_diff(x, y)   bl_timept64_get_diff (x, y)
#define bl_timept_max_safe_add_sub bl_timept64_max_safe_add_sub
#define bl_timept_max(x, y)        bl_timept64_max (x, y)
#define bl_timept_min(x, y)        bl_timept64_min (x, y)

#define BL_FMT_TPOINT       BL_FMT_TPOINT64
#define BL_FMT_TPOINTDIFF   BL_FMT_TPOINT64DIFF
#define BL_FMT_TPOINTOFF    BL_FMT_TPOINT64OFF
#define BL_FMT_TPOINTOFFSET BL_FMT_TPOINT64OFFSET

#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TPOINT       FMT_TPOINT64
#define FMT_TPOINTDIFF   FMT_TPOINT64DIFF
#define FMT_TPOINTOFF    FMT_TPOINT64OFF
#define FMT_TPOINTOFFSET FMT_TPOINT64OFFSET
#endif

#endif /* 64 bit timepoints */

/*---------------------------------------------------------------------------*/
#define BL_ARBITRARY_BASE -1
#define BL_NANOSECOND_BASE 120
#define BL_MICROSECOND_BASE 123
#define BL_MILLISECOND_BASE 126
#define BL_SECOND_BASE 129
/*---------------------------------------------------------------------------*/
#if defined (BL_TPOINT_CUSTOM_INCLUDE)
  #include BL_TPOINT_CUSTOM_INCLUDE
#elif defined (BL_TPOINT_MOCK_FOR_TESTS)
  #include <bl/base/impl/timepoint_to_mock.h>
#elif BL_OS_IS_MOSTLY_POSIX
  #include <bl/base/impl/timepoint_posix.h>
#elif BL_OS_IS (WINDOWS)
  #include <bl/base/impl/timepoint_windows.h>
#else
  #error "no bl_timept implementation available"
#endif
/*---------------------------------------------------------------------------*/
/* Default timepoint */
/*---------------------------------------------------------------------------*/
#if BL_TIMEPOINT_BITS == 32
  #include <bl/base/impl/generated/time/timept_funcs_from_timept32.h>
  #include <bl/base/impl/generated/time/sysclock_funcs_from_sysclock32.h>
#else
  #include <bl/base/impl/generated/time/timept_funcs_from_timept64.h>
  #include <bl/base/impl/generated/time/sysclock_funcs_from_sysclock64.h>
#endif

#if !defined BL_TPOINT_MOCK_FOR_TESTS
#if BL_TIMEPOINT_BITS == 32

#define bl_timept_get_freq()    bl_timept32_get_freq()
#define bl_timept_get()         bl_timept32_get()
#define bl_sysclock_get_freq()  bl_sysclock32_get_freq()
#define bl_sysclock_get()       bl_sysclock32_get()
#define bl_sysclock_to_epoch(t) bl_sysclock32_to_epoch (t)

#else

#define bl_timept_get_freq()    bl_timept64_get_freq()
#define bl_timept_get()         bl_timept64_get()
#define bl_sysclock_get_freq()  bl_sysclock64_get_freq()
#define bl_sysclock_get()       bl_sysclock64_get()
#define bl_sysclock_to_epoch(t) bl_sysclock64_to_epoch (t)
#endif /* #if BL_TIMEPOINT_BITS == 32 */

#endif /* #if !defined BL_TPOINT_MOCK_FOR_TESTS */
#endif /*__BL_TIMEPOINT_H__*/
