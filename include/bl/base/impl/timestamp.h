#ifndef __BL_TIMESTAMP_H__
#define __BL_TIMESTAMP_H__

/*----------------------------------------------------------------------------*/
#include <bl/base/platform.h>
#include <bl/base/utility.h>
#include <bl/base/integer.h>

/*---------------------------------------------------------------------------*/
/* 32 bit */
/*---------------------------------------------------------------------------*/
typedef bl_u32 bl_tstamp32;
typedef bl_i32 bl_tstamp32diff;
typedef bl_u32 bl_toffset32;
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32diff
  bl_tstamp32_get_diff (bl_tstamp32 a, bl_tstamp32 b)
{
  bl_static_assert(
    bl_has_two_comp_arithmetic,
    "this is just valid on systems with complement of two arithmetic"
    );
  return (bl_tstamp32diff) (a - b);
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp32_max_safe_add_sub ((bl_utype_max (bl_tstamp32) >> 1) - 1)
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_max (bl_tstamp32 a, bl_tstamp32 b)
{
  return bl_tstamp32_get_diff (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp32 bl_tstamp32_min (bl_tstamp32 a, bl_tstamp32 b)
{
  return bl_tstamp32_get_diff (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
/*the BL_FMT_TSTAMP printf format macro requires including
 "integer_printf_format.h" for it to be correctly expanded*/
#define BL_FMT_TSTAMP32       BL_FMT_U32
#define BL_FMT_TSTAMP32DIFF   BL_FMT_I32
#define BL_FMT_TSTAMP32OFF    BL_FMT_U32
#define BL_FMT_TSTAMP32OFFSET BL_FMT_TSTAMP32OFF
#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TSTAMP32       BL_FMT_TSTAMP32
#define FMT_TSTAMP32DIFF   BL_FMT_TSTAMP32DIFF
#define FMT_TSTAMP32OFF    BL_FMT_TSTAMP32OFF
#define FMT_TSTAMP32OFFSET BL_FMT_TSTAMP32OFFSET
#endif
/*---------------------------------------------------------------------------*/
/* 64 bit */
/*---------------------------------------------------------------------------*/
typedef bl_u64 bl_tstamp64;
typedef bl_i64 bl_tstamp64diff;
typedef bl_u64 bl_toffset64;
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64diff
  bl_tstamp64_get_diff (bl_tstamp64 a, bl_tstamp64 b)
{
  bl_static_assert(
    bl_has_two_comp_arithmetic,
    "this is just valid on systems with complement of two arithmetic"
    );
  return (bl_tstamp64diff) (a - b);
}
/*---------------------------------------------------------------------------*/
#define bl_tstamp64_max_safe_add_sub ((bl_utype_max (bl_tstamp64) >> 1) - 1)
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_max (bl_tstamp64 a, bl_tstamp64 b)
{
  return bl_tstamp64_get_diff (a, b) >= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
static inline bl_tstamp64 bl_tstamp64_min (bl_tstamp64 a, bl_tstamp64 b)
{
  return bl_tstamp64_get_diff (a, b) <= 0 ? a : b;
}
/*---------------------------------------------------------------------------*/
/*the BL_FMT_TSTAMP printf format macro requires including
 "integer_printf_format.h" for it to be correctly expanded*/
#define BL_FMT_TSTAMP64       BL_FMT_U64
#define BL_FMT_TSTAMP64DIFF   BL_FMT_I64
#define BL_FMT_TSTAMP64OFF    BL_FMT_U64
#define BL_FMT_TSTAMP64OFFSET BL_FMT_TSTAMP64OFF
#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TSTAMP64       BL_FMT_TSTAMP64
#define FMT_TSTAMP64DIFF   BL_FMT_TSTAMP64DIFF
#define FMT_TSTAMP64OFF    BL_FMT_TSTAMP64OFF
#define FMT_TSTAMP64OFFSET BL_FMT_TSTAMP64OFFSET
#endif
/*---------------------------------------------------------------------------*/
/* common */
/*---------------------------------------------------------------------------*/
#if !defined BL_TIMESTAMP_64BIT

typedef bl_tstamp32     bl_tstamp;
typedef bl_tstamp32diff bl_tstampdiff;
typedef bl_toffset32    bl_toffset;

#define bl_tstamp_get_diff(x, y)   bl_tstamp32_get_diff (x, y)
#define bl_tstamp_max_safe_add_sub bl_tstamp32_max_safe_add_sub
#define bl_tstamp_max(x, y)        bl_tstamp32_max (x, y)
#define bl_tstamp_min(x, y)        bl_tstamp32_min (x, y)

#define BL_FMT_TSTAMP       BL_FMT_TSTAMP32
#define BL_FMT_TSTAMPDIFF   BL_FMT_TSTAMP32DIFF
#define BL_FMT_TSTAMPOFF    BL_FMT_TSTAMP32OFF
#define BL_FMT_TSTAMPOFFSET BL_FMT_TSTAMP32OFFSET

#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TSTAMP       FMT_TSTAMP32
#define FMT_TSTAMPDIFF   FMT_TSTAMP32DIFF
#define FMT_TSTAMPOFF    FMT_TSTAMP32OFF
#define FMT_TSTAMPOFFSET FMT_TSTAMP32OFFSET
#endif

#else /* 64 bit timestamps */

typedef bl_tstamp64     bl_tstamp;
typedef bl_tstamp64diff bl_tstampdiff;
typedef bl_toffset64    bl_toffset;

#define bl_tstamp_get_diff(x, y)   bl_tstamp64_get_diff (x, y)
#define bl_tstamp_max_safe_add_sub bl_tstamp64_max_safe_add_sub
#define bl_tstamp_max(x, y)        bl_tstamp64_max (x, y)
#define bl_tstamp_min(x, y)        bl_tstamp64_min (x, y)

#define BL_FMT_TSTAMP       BL_FMT_TSTAMP64
#define BL_FMT_TSTAMPDIFF   BL_FMT_TSTAMP64DIFF
#define BL_FMT_TSTAMPOFF    BL_FMT_TSTAMP64OFF
#define BL_FMT_TSTAMPOFFSET BL_FMT_TSTAMP64OFFSET

#ifdef BL_UNPREFIXED_PRINTF_FORMATS
#define FMT_TSTAMP       FMT_TSTAMP64
#define FMT_TSTAMPDIFF   FMT_TSTAMP64DIFF
#define FMT_TSTAMPOFF    FMT_TSTAMP64OFF
#define FMT_TSTAMPOFFSET FMT_TSTAMP64OFFSET
#endif

#endif /* 64 bit timestamps */

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
/* Default timestamp */
/*---------------------------------------------------------------------------*/
#if !defined BL_TSTAMP_MOCK_FOR_TESTS
#if !defined BL_TIMESTAMP_64BIT

#define bl_tstamp_get_freq()           bl_tstamp32_get_freq()
#define bl_tstamp_get()                bl_tstamp32_get()
#define bl_tstamp_sysclock_get_freq()  bl_tstamp32_sysclock_get_freq()
#define bl_tstamp_sysclock_get()       bl_tstamp32_sysclock_get()
#define bl_tstamp_sysclock_to_epoch(t) bl_tstamp32_sysclock_to_epoch (t)

#else

#define bl_tstamp_get_freq()           bl_tstamp64_get_freq()
#define bl_tstamp_get()                bl_tstamp64_get()
#define bl_tstamp_sysclock_get_freq()  bl_tstamp64_sysclock_get_freq()
#define bl_tstamp_sysclock_get()       bl_tstamp64_sysclock_get()
#define bl_tstamp_sysclock_to_epoch(t) bl_tstamp64_sysclock_to_epoch (t)

#endif /* #if !defined (BL_TIMESTAMP_64BIT) */
#endif /* #if !defined BL_TSTAMP_MOCK_FOR_TESTS */
#endif /*__BL_TIMESTAMP_H__*/
