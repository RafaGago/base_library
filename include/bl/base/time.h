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
 This header adds the next time functions in two variants, 32 and 64 bit,
 suffixed with the 32/64 suffixes. There is an unsuffixed variant that is
 selected by defining BL_TIMEPOINT_BITS to 32 or 64

 All the functions, types or definitions described here are available with
 and without suffix, e.g. there is bl_timept32, bl_timept64 and bl_timept.
 ---------------------------------------------------------------
 bl_timept:
  An unsigned type representing some time units and intended to measure
  time periods. It can wrap and is compared using signed conversion.

 bl_timeptdiff:
  The signed counterpart of "bl_timept". Its absolute value can be safely added
  or substracted to a bl_timept.

 bl_timeoft:
  An unsigned type representing time in time units (sec, usec ...).

  ---------------------------------------------------------------
 BL_FMT_TSTAMP, BL_FMT_TSTAMPDIFF and BL_FMT_TSTAMPOFF define the printf
  integer format string for the bl_timept, bl_timeptdiff and bl_timeptimeoft
  types.

  ---------------------------------------------------------------
  bl_timeptdiff bl_timept_get_diff (bl_timept a, bl_timept b)

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
  every timepoint wraps at some point (64 bit timepoints can take several
  hundred years to wrap with nanosecond resolution. This is designed mostly
  to be able to use small data types for short periods of time e.g.
  on microcontrollers).

 ---------------------------------------------------------------
  static inline bl_timept bl_timept_max (bl_timept a, bl_timept b);
  static inline bl_timept bl_timept_min (bl_timept a, bl_timept b);

  Select one of the input values.

 ---------------------------------------------------------------
 "static inline bl_timept bl_timept_get (void)"

  Gets the current timepoint of a monotonic clock. The reference point is
  arbitrary (maybe system boot time) but the clock is always guarenteed to
  advance.
 ---------------------------------------------------------------
 "static inline bl_u64 bl_timept_get_freq (void)"

  Gets the frequency (in Hz) at which the time base of the timepoint is opera-
  ting in. This function may be defined as a macro if the value is known
  at compile time (Don't try to take the address of the "function").

  On most cases you may want to use the "bl_timept_to_sec/msec/usec/nsec"
  function series instead of doing conversions manually.

 ---------------------------------------------------------------
 "static inline bl_timept bl_timept_sysclock_get (void)"

  Gets the current timepoint of a system clock. The reference point is
  arbitrary (the one returned by the OS).

  On Unix derivatives this is based on CLOCK_REALTIME, on Windows this is
  implemented through a call to "GetSystemTimeAsFileTime".

  Be aware that sysclock timepoints don't need to be on the same units of its
  underlying call.
 ---------------------------------------------------------------
 "static inline bl_u64 bl_get_sysclock_freq (void)"

  Gets the frequency (in Hz) at which the time base of the timepoint is opera-
  ting in. This function may be defined as a macro if the value is known
  at compile time (Don't try to take the address of the "function").

  ---------------------------------------------------------------
 "static inline bl_timept bl_timept_sysclock_to_epoch (bl_timept)"

  Makes a sysclock call to be referenced to UNIX epoch. This is a no-op on UNIX
  OSes.
 ---------------------------------------------------------------
 "static inline bl_timeoft bl_timept_to_sec (bl_timept ts)"
 "static inline bl_timeoft bl_timept_to_msec (bl_timept ts)"
 "static inline bl_timeoft bl_timept_to_usec (bl_timept ts)"
 "static inline bl_timeoft bl_timept_to_nsec (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_sec (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_msec (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_usec (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_nsec (bl_timept ts)"

   Convert a raw timepoint to a value representing time e.g:

    bl_timept prev     = bl_timept_get();
    bl_timept now      = bl_timept_get();
    bl_word elapsed_ns = bl_timept_to_nsec (now - prev);
 ---------------------------------------------------------------
 "static inline bl_timeoft bl_timept_to_sec_ceil (bl_timept ts)"
 "static inline bl_timeoft bl_timept_to_msec_ceil (bl_timept ts)"
 "static inline bl_timeoft bl_timept_to_usec_ceil (bl_timept ts)"
 "static inline bl_timeoft bl_timept_to_nsec_ceil (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_sec_ceil (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_msec_ceil (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_usec_ceil (bl_timept ts)"
 "static inline bl_timeoft bl_timept_sysclock_to_nsec_ceil (bl_timept ts)"

    Same as above, but the values are rounded to the ceiling.
 ---------------------------------------------------------------
 "static inline bl_timept bl_sec_to_timept (bl_timeoft sec_min)"
 "static inline bl_timept bl_msec_to_timept (bl_timeoft msec_min)"
 "static inline bl_timept bl_usec_to_timept (bl_timeoft usec_min)"
 "static inline bl_timept bl_nsec_to_timept (bl_timeoft nsec_min)"
 "static inline bl_timept bl_sec_to_timept_sysclock (bl_timeoft sec_min)"
 "static inline bl_timept bl_msec_to_timept_sysclock (bl_timeoft msec_min)"
 "static inline bl_timept bl_usec_to_timept_sysclock (bl_timeoft usec_min)"
 "static inline bl_timept bl_nsec_to_timept_sysclock (bl_timeoft nsec_min)"

   Gets an offset that is AT LEAST the given amount which can be added or
   subtracted to a timepoint.

   Be aware that values are rounded up to the next clock resolution step
   (except 0).

   In other bl_words, if on one hypothetical system the timepoints have a 1ms
   resolution trying to get a 1ns offset will effectively yield a 1ms offset.

   Be aware that this can have very high cummulative errors, e.g on the pre-
   vious system getting the offset of (what it seems to be) "1ns" and adding
   it up three times will increment the timepoint 3ms instead of 3ns.

   If one wants to get the timer resolution this call can give it:

   "bl_timept_to_nsec_ceil (bl_nsec_to_timept (1))"

 ---------------------------------------------------------------
 "static inline bl_timeoft bl_sec_to_timept_max (void)"
 "static inline bl_timeoft bl_msec_to_timept_max (void)"
 "static inline bl_timeoft bl_usec_to_timept_max (void)"
 "static inline bl_timeoft bl_nsec_to_timept_max (void)"
 "static inline bl_timeoft bl_sec_to_timept_sysclock_max (void)"
 "static inline bl_timeoft bl_msec_to_timept_sysclock_max (void)"
 "static inline bl_timeoft bl_usec_to_timept_sysclock_max (void)"
 "static inline bl_timeoft bl_nsec_to_timept_sysclock_max (void)"

   Get the maximum input value that any of the functions above can take.
   This function group may be defined as macros if all values are known
   at compile time (Don't try to take the address of the "function").

   Be aware that this is the maximum time that can be safely added to a time-
   stamp in total. It isn't safe to add this amount twice.

 ---------------------------------------------------------------
*/
#include <bl/base/impl/timepoint.h>
/*----------------------------------------------------------------------------*/
#endif /* __BL_TIME_H__ */
