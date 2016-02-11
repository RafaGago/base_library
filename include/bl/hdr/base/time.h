#ifndef __BL_TIME_H__
#define __BL_TIME_H__

/*---------------------------------------------------------------------------*/
#define si_unit_factor 1000

#define nsec_in_sec (si_unit_factor * si_unit_factor * si_unit_factor)
#define usec_in_sec (si_unit_factor * si_unit_factor)
#define msec_in_sec (si_unit_factor)

#define nsec_in_msec (si_unit_factor * si_unit_factor)
#define usec_in_msec (si_unit_factor)

#define nsec_in_usec (si_unit_factor)

/*---------------------------------------------------------------------------*/
/*
 This header adds the next timestamp function facilities:
 
 --------------------------------------------------------------- 
 tstamp: 
  An unsigned type representing some units time and intended to measure short
  time periods (<60s). It can wrap and is compared using signed conversion.
 
 tstampdiff:
  The signed counterpart of "tstamp". Its absolute value can be safely added
  or substracted to a tstamp.
  
 toffset:
  An unsigned type representing time in time units (sec, usec ...).
 
  --------------------------------------------------------------- 
  tstampdiff tstamp_get_diff (tstamp a, tstamp b)
 
  Returns:
    negative: a is before b (a smaller than b)
    zero: equal
    positive: a is after b (a bigger than b)
          
  As said beffore this uses signed conversion for comparison, which implies
  that the values to compare can only go forward (or backwards) half the time-
  stamp range (1 bit is taken for the sign) minus one. This is to avoid
  storing the offset just to avoid losing 1 bit.
  
  Ilustrations of the algorithm for those that aren't well versed with integer
  arithmetic, using u8 (byte) types:

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
  of this comparision is absolutely time dependant and wraps. Nothing new,
  every timestamp wraps at some point (64 bit timestamps can take several
  hundred years to wrap with nanosecond resolution. This is designed mostly
  to be able to use tighter data types e.g. on microcontrollers).
 ---------------------------------------------------------------
 "static inline tstamp bl_get_tstamp (void)"

   Gets the current timestamp.
 ---------------------------------------------------------------
 "static inline u64 bl_get_tstamp_freq (void)"
 
  Gets the frequency (in Hz) at which the time base of the timestamp is opera-
  ting in. This function may be defined as a macro if the value is known
  at compile time (Don't try to take the address of the "function").
 
 ---------------------------------------------------------------
 "static inline toffset bl_tstamp_to_sec (tstamp ts)"
 "static inline toffset bl_tstamp_to_msec (tstamp ts)"
 "static inline toffset bl_tstamp_to_usec (tstamp ts)"
 "static inline toffset bl_tstamp_to_nsec (tstamp ts)"

   Convert a raw timestamp to a value representing time e.g:
    
    tstamp prev     = bl_get_tstamp();
    tstamp now      = bl_get_tstamp();
    word elapsed_ns = bl_tstamp_to_nsec (now - prev);
 ---------------------------------------------------------------
 "static inline toffset bl_tstamp_to_sec_ceil (tstamp ts)"
 "static inline toffset bl_tstamp_to_msec_ceil (tstamp ts)"
 "static inline toffset bl_tstamp_to_usec_ceil (tstamp ts)"
 "static inline toffset bl_tstamp_to_nsec_ceil (tstamp ts)"

    Same as above, but the values are rounded to the ceiling.
 ---------------------------------------------------------------
 "static inline tstamp bl_sec_to_tstamp (toffset sec_min)"
 "static inline tstamp bl_msec_to_tstamp (toffset msec_min)"
 "static inline tstamp bl_usec_to_tstamp (toffset usec_min)"
 "static inline tstamp bl_nsec_to_tstamp (toffset nsec_min)"

   Gets an offset that is AT LEAST the given amount which can be added or
   subtracted to a timestamp.

   Be aware that values are rounded up to the next clock resolution step
   (except 0).

   In other words, if on one hypothetical system the timestamps have a 1ms 
   resolution trying to get a 1ns offset will effectively yield a 1ms offset.

   Be aware that this can have very high cummulative errors, e.g on the pre-
   vious system getting the offset of (what it seems to be) "1ns" and adding 
   it up three times will increment the timestamp 3ms instead of 3ns.

   If one wants to get the timer resolution this call can give it:

   "bl_tstamp_to_nsec_ceil (bl_nsec_to_tstamp (1))"
   
 ---------------------------------------------------------------
 "static inline toffset bl_sec_to_tstamp_max (void)"
 "static inline toffset bl_msec_to_tstamp_max (void)"
 "static inline toffset bl_usec_to_tstamp_max (void)"
 "static inline toffset bl_nsec_to_tstamp_max (void)"

   Get the maximum input value that any of the functions above can take.
   This function group may be defined as macros if all values are known
   at compile time (Don't try to take the address of the "function").
   
   Be aware that this is the maximum time that can be safely added to a time-
   stamp in total. It isn't safe to add this amount twice.
   
 ---------------------------------------------------------------
*/

#include <bl/hdr/base/impl/timestamp.h>

/*----------------------------------------------------------------------------*/
#endif /* __BL_TIME_H__ */
