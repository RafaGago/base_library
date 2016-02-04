#ifndef __BL_TIME_H__
#define __BL_TIME_H__

/*----------------------------------------------------------------------------*/
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/integer.h>
/*---------------------------------------------------------------------------*/
#define si_unit_factor 1000

#define nsec_in_sec (si_unit_factor * si_unit_factor * si_unit_factor)
#define usec_in_sec (si_unit_factor * si_unit_factor)
#define msec_in_sec (si_unit_factor)

#define nsec_in_msec (si_unit_factor * si_unit_factor)
#define usec_in_msec (si_unit_factor)

#define nsec_in_usec (si_unit_factor)
/*---------------------------------------------------------------------------*/
#ifndef BL64BIT_TIMESTAMP
  typedef u32 tstamp;
  typedef i32 ststamp;
#else
  typedef u64 tstamp;
  typedef i64 ststamp;
#endif
/*---------------------------------------------------------------------------*/
#define tstamp_safe_add_sub_max ((utype_max (tstamp) >> 1) - 1)
/*---------------------------------------------------------------------------*/
/*
  ststamp tstamp_compare (tstamp a, tstamp b)
 
  Returns:
  negative: a is before b (a smaller than b)
  zero: equal
  positive: a is after b (a bigger than b)

  examples with the algorithm below but using u8 resolution:

  a     b
  ---   ---
  255 - 0   = ((i8) 255) = -1
  1   - 0   = ((i8)   1) =  1

  129 - 0   = ((i8) 129) = -127
  127 - 0   = ((i8) 127) = 127

  128 - 0   = ((i8) 128) = 128
  128 - 0   = ((i8) 128) = 128

  127 - 0   = ((i8) 127) = 127
  129 - 0   = ((i8) 129) = -127

  As can be observed, the expiration wraps around for each given value at
  half the data type resolution (7 bits in this case) minus one, so the result
  of this comparision is absolutely time dependant and wraps. If this function
  is used as-is you have to be aware of the range and frequency of such
  comparisons.
*/
/*---------------------------------------------------------------------------*/
static inline ststamp tstamp_compare (tstamp a, tstamp b)
{
  static_assert(
    bl_has_two_comp_arithmetic,
    "this is just valid on systems with complement of two arithmetic"
    );
  return (ststamp) (a - b);
}
/*---------------------------------------------------------------------------*/
/*

 This header adds the next timestamp function wrappers:

 ---------------------------------------------------------------
 "static inline tstamp bl_get_tstamp (void)"

   Gets an unsigned integer representing time on unspecified units but 
   guaranteed to wrap around on its integer type maximum (no discontinuities
   on wrapping) and hence can be added and subtracted to measure time
   differences.
 ---------------------------------------------------------------
 "static inline uword bl_tstamp_sec (tstamp ts)"
 "static inline uword bl_tstamp_msec (tstamp ts)"
 "static inline uword bl_tstamp_usec (tstamp ts)"
 "static inline uword bl_tstamp_nsec (tstamp ts)"

   Convert a raw timestamp to a value representing time e.g:
    
    tstamp prev     = bl_get_tstamp();
    tstamp now      = bl_get_tstamp();
    word elapsed_ns = bl_tstamp_nsec (now - prev);
 ---------------------------------------------------------------
 "static inline uword bl_tstamp_sec_ceil (tstamp ts)"
 "static inline uword bl_tstamp_msec_ceil (tstamp ts)"
 "static inline uword bl_tstamp_usec_ceil (tstamp ts)"
 "static inline uword bl_tstamp_nsec_ceil (tstamp ts)"

    Same as above, but the values are rounded to the ceiling.
 ---------------------------------------------------------------
 "static inline tstamp bl_tstamp_offset_sec (i32 sec_min)"
 "static inline tstamp bl_tstamp_offset_msec (i32 msec_min)"
 "static inline tstamp bl_tstamp_offset_usec (i32 usec_min)"
 "static inline tstamp bl_tstamp_offset_nsec (i32 nsec_min)"

   Gets an offset that is AT LEAST the given amount which can be added or
   subtracted to a timestamp.

   Be aware that values are rounded up to the next clock resolution step
   (except 0).

   In other words, if on one hypothetical system the timestamps have a 1ms 
   resolution trying to get a 1ns offset will effectively yield a 1ms offset.

   Be aware that this can have very high cummulative errors, e.g on the previous
   system getting the offset of (what it seems to be) "1ns" and adding it up
   three times will increment the timestamp 3ms instead of 3ns.
 ---------------------------------------------------------------
*/
#if defined (BL_TIME_NO_TIMESTAMP)
/*nothing...*/
/*---------------------------------------------------------------------------*/
#elif defined (BL_CUSTOM_TIMESTAMP_HEADER)
  #include BL_CUSTOM_TIMESTAMP_HEADER
#elif defined (BL_POSIX)
#include <time.h>
#include <bl/hdr/base/impl/timestamp_posix.h>
/*---------------------------------------------------------------------------*/
#elif defined (BL_WINDOWS)
/*---------------------------------------------------------------------------*/
#include <bl/hdr/base/impl/timestamp_windows.h>
/*---------------------------------------------------------------------------*/
#else
 #error "tstamp unimplemented on this platform"
#endif
/*----------------------------------------------------------------------------*/
#ifdef BL_POSIX
#include <time.h>
/*----------------------------------------------------------------------------*/
static void timespec_normalize (struct timespec* t)
{
  if (unlikely (t->tv_nsec >= nsec_in_sec)) {   
    uword div   = (t->tv_nsec / nsec_in_sec);
    t->tv_sec  += div;
    t->tv_nsec -= (div * nsec_in_sec);
    static_assert_ns (sizeof div >= sizeof t->tv_nsec);
  }
}
/*----------------------------------------------------------------------------*/
static struct timespec timespec_us_from_now (u32 usec, int clock)
{
  struct timespec t;
  u32 sec = usec / usec_in_sec;
  clock_gettime (clock, &t);
  t.tv_sec  += sec;
  t.tv_nsec += (usec - (sec * usec_in_sec)) * nsec_in_usec;
  timespec_normalize (&t);
  return t;
}
#endif /*BL_POSIX*/
/*----------------------------------------------------------------------------*/
#endif /* __BL_TIME_H__ */

