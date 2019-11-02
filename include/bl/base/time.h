#ifndef __BL_TIME_H__
#define __BL_TIME_H__

#include <bl/config.h>

/*---------------------------------------------------------------------------*/
#define bl_si_unit_factor 1000

#define bl_nsec_in_sec\
  (bl_si_unit_factor * bl_si_unit_factor * bl_si_unit_factor)

#define bl_usec_in_sec (bl_si_unit_factor * bl_si_unit_factor)
#define bl_msec_in_sec (bl_si_unit_factor)

#define bl_nsec_in_msec (bl_si_unit_factor * bl_si_unit_factor)
#define bl_usec_in_msec (bl_si_unit_factor)

#define bl_nsec_in_usec (bl_si_unit_factor)

/*---------------------------------------------------------------------------*/
/*
 This header adds the next time functions:

 ---------------------------------------------------------------
 bl_tstamp:
  An unsigned type representing some time units and intended to measure
  time periods. It can wrap and is compared using signed conversion.

  Define "BL_TIMESTAMP_64BIT" most of the time if you want the timestamps to be
  64 bit (recommended). Having 32-bit timestamps affects the resolution of all
  the timing functions. The 32-bit timestamp is microcontroller legacy.

 bl_tstampdiff:
  The signed counterpart of "bl_tstamp". Its absolute value can be safely added
  or substracted to a bl_tstamp.

 bl_toffset:
  An unsigned type representing time in time units (sec, usec ...).

  ---------------------------------------------------------------
 BL_FMT_TSTAMP, BL_FMT_TSTAMPDIFF and BL_FMT_TSTAMPOFF define the printf
  integer format string for the bl_tstamp, bl_tstampdiff and bl_tstampoffset
  types.

  ---------------------------------------------------------------
  bl_tstampdiff bl_tstamp_get_diff (bl_tstamp a, bl_tstamp b)

  Returns:
    negative: a is before b (a smaller than b)
    zero: equal
    positive: a is after b (a bigger than b)

  As said beffore this uses signed conversion for comparison, which implies
  that the values to compare can only go forward (or backwards) half the time-
  stamp range (1 bit is taken for the sign) minus one. This is to avoid
  storing the offset just to avoid losing 1 bit.

  Ilustrations of the algorithm for those that aren't well versed on integer
  arithmetic, using bl_u8 (byte) types:

  a     b
  ---   ---
  255 - 0   = ((bl_i8) 255) = -1
  1   - 0   = ((bl_i8)   1) =  1

  129 - 0   = ((bl_i8) 129) = -127
  127 - 0   = ((bl_i8) 127) = 127

  128 - 0   = ((bl_i8) 128) = 128
  128 - 0   = ((bl_i8) 128) = 128

  127 - 0   = ((bl_i8) 127) = 127
  129 - 0   = ((bl_i8) 129) = -127

  As can be observed, the expiration wraps around for each given value at
  half the data type resolution (7 bits in this case) minus one, so the result
  of this comparison is absolutely time dependant and wraps. Nothing new;
  every timestamp wraps at some point (64 bit timestamps can take several
  hundred years to wrap with nanosecond resolution. This is designed mostly
  to be able to use small data types for short periods of time e.g.
  on microcontrollers).

 ---------------------------------------------------------------
  static inline bl_tstamp bl_tstamp_max (bl_tstamp a, bl_tstamp b);
  static inline bl_tstamp bl_tstamp_min (bl_tstamp a, bl_tstamp b);

  Select one of the input values.

 ---------------------------------------------------------------
 "static inline bl_tstamp bl_get_tstamp (void)"

  Gets the current timestamp of a monotonic clock. The reference point is
  arbitrary (maybe system boot time) but the clock is always guarenteed to
  advance.
 ---------------------------------------------------------------
 "static inline bl_u64 bl_get_tstamp_freq (void)"

  Gets the frequency (in Hz) at which the time base of the timestamp is opera-
  ting in. This function may be defined as a macro if the value is known
  at compile time (Don't try to take the address of the "function").

  On most cases you may want to use the "bl_tstamp_to_sec/msec/usec/nsec"
  function series instead of doing conversions manually.

 ---------------------------------------------------------------
 "static inline bl_tstamp bl_tstamp_sysclock_get (void)"

  Gets the current timestamp of a system clock. The reference point is
  arbitrary (the one returned by the OS).

  On Unix derivatives this is based on CLOCK_REALTIME, on Windows this is
  implemented through a call to "GetSystemTimeAsFileTime".

  Be aware that sysclock timestamps don't need to be on the same units of its
  underlying call.
 ---------------------------------------------------------------
 "static inline bl_u64 bl_get_sysclock_freq (void)"

  Gets the frequency (in Hz) at which the time base of the timestamp is opera-
  ting in. This function may be defined as a macro if the value is known
  at compile time (Don't try to take the address of the "function").

  ---------------------------------------------------------------
 "static inline bl_tstamp bl_tstamp_sysclock_to_epoch (bl_tstamp)"

  Makes a sysclock call to be referenced to UNIX epoch. This is a no-op on UNIX
  OSes.
 ---------------------------------------------------------------
 "static inline bl_toffset bl_tstamp_to_sec (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_to_msec (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_to_usec (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_to_nsec (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_sec (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_msec (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_usec (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_nsec (bl_tstamp ts)"

   Convert a raw timestamp to a value representing time e.g:

    bl_tstamp prev     = bl_get_tstamp();
    bl_tstamp now      = bl_get_tstamp();
    bl_word elapsed_ns = bl_tstamp_to_nsec (now - prev);
 ---------------------------------------------------------------
 "static inline bl_toffset bl_tstamp_to_sec_ceil (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_to_msec_ceil (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_to_usec_ceil (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_to_nsec_ceil (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_sec_ceil (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_msec_ceil (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_usec_ceil (bl_tstamp ts)"
 "static inline bl_toffset bl_tstamp_sysclock_to_nsec_ceil (bl_tstamp ts)"

    Same as above, but the values are rounded to the ceiling.
 ---------------------------------------------------------------
 "static inline bl_tstamp bl_sec_to_bl_tstamp (bl_toffset sec_min)"
 "static inline bl_tstamp bl_msec_to_bl_tstamp (bl_toffset msec_min)"
 "static inline bl_tstamp bl_usec_to_bl_tstamp (bl_toffset usec_min)"
 "static inline bl_tstamp bl_nsec_to_bl_tstamp (bl_toffset nsec_min)"
 "static inline bl_tstamp bl_sec_to_bl_tstamp_sysclock (bl_toffset sec_min)"
 "static inline bl_tstamp bl_msec_to_bl_tstamp_sysclock (bl_toffset msec_min)"
 "static inline bl_tstamp bl_usec_to_bl_tstamp_sysclock (bl_toffset usec_min)"
 "static inline bl_tstamp bl_nsec_to_bl_tstamp_sysclock (bl_toffset nsec_min)"

   Gets an offset that is AT LEAST the given amount which can be added or
   subtracted to a timestamp.

   Be aware that values are rounded up to the next clock resolution step
   (except 0).

   In other bl_words, if on one hypothetical system the timestamps have a 1ms
   resolution trying to get a 1ns offset will effectively yield a 1ms offset.

   Be aware that this can have very high cummulative errors, e.g on the pre-
   vious system getting the offset of (what it seems to be) "1ns" and adding
   it up three times will increment the timestamp 3ms instead of 3ns.

   If one wants to get the timer resolution this call can give it:

   "bl_tstamp_to_nsec_ceil (bl_nsec_to_bl_tstamp (1))"

 ---------------------------------------------------------------
 "static inline bl_toffset bl_sec_to_bl_tstamp_max (void)"
 "static inline bl_toffset bl_msec_to_bl_tstamp_max (void)"
 "static inline bl_toffset bl_usec_to_bl_tstamp_max (void)"
 "static inline bl_toffset bl_nsec_to_bl_tstamp_max (void)"
 "static inline bl_toffset bl_sec_to_bl_tstamp_sysclock_max (void)"
 "static inline bl_toffset bl_msec_to_bl_tstamp_sysclock_max (void)"
 "static inline bl_toffset bl_usec_to_bl_tstamp_sysclock_max (void)"
 "static inline bl_toffset bl_nsec_to_bl_tstamp_sysclock_max (void)"

   Get the maximum input value that any of the functions above can take.
   This function group may be defined as macros if all values are known
   at compile time (Don't try to take the address of the "function").

   Be aware that this is the maximum time that can be safely added to a time-
   stamp in total. It isn't safe to add this amount twice.

 ---------------------------------------------------------------
*/
#include <bl/base/impl/timestamp.h>
/*----------------------------------------------------------------------------*/
#endif /* __BL_TIME_H__ */
