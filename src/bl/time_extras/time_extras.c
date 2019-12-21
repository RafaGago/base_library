#include <stdlib.h>
#include <string.h>

#include <bl/base/thread.h>
#include <bl/base/atomic.h>
#include <bl/base/utility.h>
#include <bl/base/processor_pause.h>
#include <bl/base/deadline.h>

#include <bl/time_extras/time_extras.h>
/*----------------------------------------------------------------------------*/
#ifndef BL_TIME_QSAMPLES
  #define BL_TIME_QSAMPLES 50
#endif
/*----------------------------------------------------------------------------*/
#ifndef BL_TIME_MIN_QSAMPLES
  #define BL_TIME_MIN_QSAMPLES 30
#endif
/*----------------------------------------------------------------------------*/
#ifndef BL_TIME_CONTEXT_SWITCH_FACTOR
  #define BL_TIME_CONTEXT_SWITCH_FACTOR 5
#endif
/*----------------------------------------------------------------------------*/
#if BL_HAS_CPU_TIMEPT
  #ifndef BL_TIME_QPAUSE_US
    #define BL_TIME_QPAUSE_US 50
  #endif
#else
  #ifdef BL_TIME_QPAUSE_US
    #undef BL_TIME_QPAUSE_US
  #endif
  #define BL_TIME_QPAUSE_US 0
#endif
/*----------------------------------------------------------------------------*/
static inline void bl_time_extras_spinwait (bl_timeoft64 us)
{
  if (us == 0) {
    return;
  }
  bl_timept64 deadline;
  bl_timept64_deadline_init_usec (&deadline, us);
  while (!bl_timept64_deadline_expired (deadline)) {
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
    bl_processor_pause();
  }
}
/*----------------------------------------------------------------------------*/
typedef struct bl_time_extras_syncdata {
  /* all latencies in clock cycles from "bl_fast_timept" */
  bl_timept64  timept64_latency;
  bl_timept64  sysclock64_latency;
  bl_timept64  tcpu_freq;
  bl_timeoft64 tcpu_to_timept64_ns;
}
bl_time_extras_syncdata;
/*----------------------------------------------------------------------------*/
typedef struct bl_time_extras {
  bl_atomic_i64           timept64_to_sysclock64_ns;
  bl_atomic_u32           init_flag;
  bl_time_extras_syncdata syncdata;
}
bl_time_extras;
/*----------------------------------------------------------------------------*/
enum {
  bl_textras_unitialized  = 0,
  bl_textras_initializing = 1,
  bl_textras_initialized  = 2,
};
/*----------------------------------------------------------------------------*/
bl_time_extras state = { 0, bl_textras_unitialized };
/*----------------------------------------------------------------------------*/
typedef struct measurements {
  bl_timept64 t[6];
  bl_timept64 l[2];
}
measurements;
/*----------------------------------------------------------------------------*/
static
  bl_timeoft64 bl_time_extras_interpolate (bl_timept64 small, bl_timept64 big)
{
  return (bl_timeoft64) (small + ((big - small) / 2));
}
/*----------------------------------------------------------------------------*/
static bl_err bl_time_extras_get_t64_to_sys64(
  bl_timeoft64* res, bl_time_extras_syncdata const* s
  )
{
  measurements meas;
  bl_timept64 maxl = (s->timept64_latency + s->sysclock64_latency) * 2;
  maxl *= BL_TIME_CONTEXT_SWITCH_FACTOR;

  for (int i = 0; i < BL_TIME_QSAMPLES; ++i) {
    meas.t[0] = bl_fast_timept_get();
    meas.t[1] = bl_timept64_get();
    meas.t[2] = bl_sysclock64_get();
    meas.t[3] = bl_sysclock64_get();
    meas.t[4] = bl_timept64_get();
    meas.t[5] = bl_fast_timept_get();
    meas.l[0] = meas.t[5] - meas.t[0];

    if (meas.l[0] < maxl) {
      bl_timept64 t64 = bl_time_extras_interpolate (meas.t[1], meas.t[4]);
      bl_assert (t64 >= meas.t[1]); /* overflow */
      bl_timept64 sys = bl_time_extras_interpolate (meas.t[2], meas.t[3]);
      bl_assert (sys >= meas.t[2]); /* overflow */
      bl_timeoft64 t64o = bl_timept64_to_nsec (t64);
      bl_timeoft64 syso = bl_sysclock64_to_nsec (sys);
      *res = syso - t64o;
      return bl_mkok();
    }
    bl_processor_pause();
  }
  return bl_mkerr (bl_timeout);
}
/*----------------------------------------------------------------------------*/
static bl_err bl_time_extras_get_ro_data (bl_time_extras_syncdata* s) {

  measurements meas[BL_TIME_QSAMPLES];

  memset (s, 0, sizeof *s);

  bl_timept64 timept64_latency = 0; --timept64_latency;
  /* calculate timept64_latency. saving the measurements too in case that
  BL_HAS_CPU_TIMEPT is enabled. */
  for (int i = 0; i < bl_arr_elems (meas); ++i) {
    meas[i].t[0] = bl_fast_timept_get();
    meas[i].t[1] = bl_timept64_get();
    meas[i].t[2] = bl_fast_timept_get();
    bl_time_extras_spinwait (BL_TIME_QPAUSE_US);
    meas[i].t[3] = bl_fast_timept_get();
    meas[i].t[4] = bl_timept64_get();
    meas[i].t[5] = bl_fast_timept_get();
    meas[i].l[0] = meas[i].t[2] - meas[i].t[0];
    meas[i].l[1] = meas[i].t[5] - meas[i].t[3];
    timept64_latency = bl_min (timept64_latency, meas[i].l[0]);
    timept64_latency = bl_min (timept64_latency, meas[i].l[1]);
  }
  s->timept64_latency = timept64_latency;
  bl_timept64 timept64_latency_max =
    timept64_latency * BL_TIME_CONTEXT_SWITCH_FACTOR;
  bl_assert (timept64_latency_max > timept64_latency); /* overflow */

#if BL_HAS_CPU_TIMEPT
  int first = -1;
  int last  = -1;

  /* obtaining first and last good measurements */
  for (int i = 0; i < bl_arr_elems (meas); ++i) {
    if(
      meas[i].l[0] < timept64_latency_max && meas[i].l[1] < timept64_latency_max
      ) {
      first = i;
      break;
    }
  }
  for (int i = bl_arr_elems (meas) - 1; i >= 0; --i) {
    if(
      meas[i].l[0] < timept64_latency_max && meas[i].l[1] < timept64_latency_max
      ) {
      last = i;
      break;
    }
  }
  if (first == -1 || last == -1 || last - first < BL_TIME_MIN_QSAMPLES) {
    return bl_mkerr (bl_timeout);
  }
  /* calculate cputimept freq */
  bl_u64 t64_cycles;
  bl_u64 cpu_cycles;

  t64_cycles  = meas[last].t[4] - meas[first].t[1];
  /* averaging the measurements before and after on the CPU clock. Assuming that
  a 64 bit data type won't overflow on a monotonic clock. */
  cpu_cycles  = meas[last].t[5] - meas[first].t[2];
  cpu_cycles += meas[last].t[3] - meas[first].t[0];
  cpu_cycles /= 2;
  bl_assert (meas[last].t[0] - meas[first].t[5] < cpu_cycles ); /* overflow */

  double ratio   = ((double) cpu_cycles) / ((double) t64_cycles);
  s->tcpu_freq = (bl_timept64) (((double) bl_timept64_get_freq()) * ratio);

  /* calculating the ns diff from a non context-switched sample */
  bl_timeoft64 t64 =
    bl_time_extras_interpolate (meas[first].t[1],  meas[first].t[4]);
  bl_assert (t64 >= meas[first].t[1]); /* overflow */
  bl_timeoft64 tcpu =
    bl_time_extras_interpolate (meas[first].t[0], meas[first].t[5]);
  bl_assert (tcpu >= meas[first].t[0]); /* overflow */
  t64 = bl_timept64_to_nsec (t64);
  /* unable to use "bl_cpu_timept_to_nsec" yet */
  bl_u64 sec = tcpu / s->tcpu_freq;
  bl_u64 rem = tcpu % s->tcpu_freq;
  tcpu = (bl_timeoft64)
    ((sec * bl_nsec_in_sec) + ((rem * bl_nsec_in_sec) / s->tcpu_freq));
  s->tcpu_to_timept64_ns = tcpu - t64;
#endif /* #if BL_HAS_CPU_TIMEPT */

  bl_timept64 sysclock64_latency = 0; --sysclock64_latency;
  /* calculate sysclock64 latency*/
  for (int i = 0; i < bl_arr_elems (meas); ++i) {
    meas[i].t[0] = bl_fast_timept_get();
    meas[i].t[1] = bl_sysclock64_get();
    meas[i].t[2] = bl_fast_timept_get();
    meas[i].l[0] = meas[i].t[2] - meas[i].t[0];
    sysclock64_latency = bl_min (sysclock64_latency, meas[i].l[0]);
  }
  s->sysclock64_latency = sysclock64_latency;
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_err bl_time_extras_init (void)
{
  bl_u32 exp = bl_textras_unitialized;
  if (!bl_atomic_u32_strong_cas(
    &state.init_flag,
    &exp,
    bl_textras_initializing,
    bl_mo_acquire,
    bl_mo_relaxed
    )) {
    /* someone else initialized or is initializing */
    if (exp == bl_textras_initialized) {
      return bl_mkok();
    }
    bl_u32 flag;
    while (true) {
      flag = bl_atomic_u32_load (&state.init_flag, bl_mo_acquire);
      if (flag != bl_textras_initializing) {
        break;
      }
      bl_thread_yield();
    }
    return bl_mkerr (flag == bl_textras_initialized ? bl_ok : bl_timeout);
  }
  /* inititialization attempt */
  bl_err err = bl_time_extras_get_ro_data (&state.syncdata);
  if (err.own) {
    bl_atomic_u32_store(
      &state.init_flag, bl_textras_unitialized, bl_mo_release
      );
    return err;
  }
  /* at this point we are only getting an initial value, so
  "bl_cpu_timept_to_sysclock64_diff_ns" can return something in case in can
  do an accurate measurement. */
  bl_timeoft64 t64_to_sys;
  err = bl_time_extras_get_t64_to_sys64 (&t64_to_sys, &state.syncdata);
  if (err.own) {
    bl_atomic_u32_store(
      &state.init_flag, bl_textras_unitialized, bl_mo_release
      );
    return err;
  }
  bl_atomic_i64_store_rlx (&state.timept64_to_sysclock64_ns, t64_to_sys);
  bl_atomic_u32_store (&state.init_flag, bl_textras_initialized, bl_mo_release);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT void bl_time_extras_destroy (void)
{
  /* as of now doing nothing */
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_timeoft64 bl_timept64_to_sysclock64_diff_ns (void)
{
  bl_timeoft64 t64_to_sys64;
  bl_err err = bl_time_extras_get_t64_to_sys64 (&t64_to_sys64, &state.syncdata);
  if (!err.own) {
    bl_atomic_i64_store_rlx (&state.timept64_to_sysclock64_ns, t64_to_sys64);
    return t64_to_sys64;
  }
  else {
    /* better to return an old value than to return nothing */
    return bl_atomic_i64_load_rlx (&state.timept64_to_sysclock64_ns);
  }
}
/*----------------------------------------------------------------------------*/
#if BL_HAS_CPU_TIMEPT
BL_TIME_EXTRAS_EXPORT bl_timept64 bl_cpu_timept_get_freq (void)
{
  return state.syncdata.tcpu_freq;
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_timeoft64 bl_cpu_timept_to_sysclock64_diff_ns (void)
{
  return
    bl_timept64_to_sysclock64_diff_ns() + state.syncdata.tcpu_to_timept64_ns;
}
/*----------------------------------------------------------------------------*/
#endif /* #ifdef BL_HAS_CPU_TIMEPT */
