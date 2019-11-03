#include <stdlib.h>

#include <bl/base/thread.h>
#include <bl/base/atomic.h>
#include <bl/base/utility.h>
#include <bl/base/processor_pause.h>

#include <bl/time_extras/time_extras.h>

/*----------------------------------------------------------------------------*/
/* Different clock values on this file are calculated by:

  - taking a dataset of BL_TIME_QSAMPLES.
  - finding the most repeated sample values that are within a dispersion window.
  - averaging the sample values within that window.

  The macros below control the constanst on these calculations.
------------------------------------------------------------------------------*/
/* number of samples to take on each clock measuration function */
#ifndef BL_TIME_QSAMPLES
  #define BL_TIME_QSAMPLES 20
#endif

/* on initialization some functions will retry if they are unable to get stable
enough values for the clock calculations"*/
#ifndef BL_TIME_INIT_RETRY_ROUNDS
  #define BL_TIME_INIT_RETRY_ROUNDS 10
#endif

/* used as dispersion window size for the conversion to sysclock functions.
Values within the range specified here are consided a same group.*/
#ifndef BL_TIME_TO_SYSCLOCK_MAX_SAMPLE_DRIFT_NS
  #define BL_TIME_TO_SYSCLOCK_MAX_SAMPLE_DRIFT_NS 50
#endif

/* used as dispersion window size for calculation of the CPU clock frequency
(when available). This clock is calibrated against the monotonic clock and
calculated as a ratio to it. Hence the parts per million (PPM) units. */
#ifndef BL_TIME_CPUFREQ_MAX_SAMPLE_PPM
  #define BL_TIME_CPUFREQ_MAX_SAMPLE_PPM 30
#endif
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#define MAGIC_NAN (0x7ff8000000000000UL) /* A NaN value */
#define IS_MAGIC_NAN(v) ((bl_u64)(v) == (bl_u64) MAGIC_NAN)
/*----------------------------------------------------------------------------*/
#define CPUFREQ_WINDOW \
  (((double) BL_TIME_CPUFREQ_MAX_SAMPLE_PPM) / 1000000.)
/*----------------------------------------------------------------------------*/
static int doublecmp (const void * aptr, const void * bptr)
{
  double a = *(double*) aptr;
  double b = *(double*) bptr;
  if (a > b) {
    return 1;
  }
  else if (a < b) {
    return -1;
  }
  return 0;
}
/*----------------------------------------------------------------------------*/
static double find_most_repeated (double* v, bl_uword vcount, double window)
{
  struct sample_data{
    double total;
    double winstart;
    bl_uword  count;
  };
  qsort (v, vcount, sizeof v[0], doublecmp);

  struct sample_data cur = { v[0], v[0], 1 };
  struct sample_data max = cur;

  for (bl_uword i = 1; i < vcount; ++i) {
    if ((v[i] - cur.winstart) < window) {
      cur.total += v[i];
      ++cur.count;
      if (cur.count > max.count) {
        max = cur;
      }
    }
    else {
      cur.winstart = v[i];
      cur.total = v[i];
      cur.count = 1;
    }
  }
  return (max.count > 1) ?
    (max.total / (double) max.count) : (double) MAGIC_NAN;
}
/*----------------------------------------------------------------------------*/
static bl_timept t_get_noinline (void)
{
  return bl_timept_get();
}
/*----------------------------------------------------------------------------*/
static bl_timeoft t_to_nsec_noinline (bl_timept t)
{
  return bl_timept_to_nsec (t);
}
/*----------------------------------------------------------------------------*/
typedef struct sysclockdiff {
  bl_atomic_u64 to_sys_ns;
  bl_timept (*get) (void);
  bl_timeoft (*to_nsec) (bl_timept);
}
sysclockdiff;
/*----------------------------------------------------------------------------*/
/* atomics on this lib won't compile for non lock free implementations of
and atomic types of different sizes than the basic type size */
/*----------------------------------------------------------------------------*/
static sysclockdiff timepoint_sysdata = {
  (bl_atomic_u64) MAGIC_NAN, &t_get_noinline, &t_to_nsec_noinline
};
/*----------------------------------------------------------------------------*/
#ifdef BL_HAS_CPU_TSTAMP
/*----------------------------------------------------------------------------*/
static bl_timept cpu_t_get_noinline (void)
{
  return bl_cpu_timept_get();
}
/*----------------------------------------------------------------------------*/
static sysclockdiff cputimepoint_sysdata = {
  (bl_atomic_u64) MAGIC_NAN, &cpu_t_get_noinline, &bl_cpu_timept_to_nsec
};
/*----------------------------------------------------------------------------*/
bl_atomic_u64 cpu_timept_freq = (bl_atomic_u64) 0;
/*----------------------------------------------------------------------------*/
static bl_err bl_cpu_timept_freq_set()
{
  if (bl_cpu_timept_get_freq() != 0) {
    /* already initialized */
    return bl_mkok();
  }
  bl_timept tprev;
  bl_timept cpuprev;
  double ratio[BL_TIME_QSAMPLES];
  bl_thread_yield(); /* context switching in-between minimization attempt.*/

  tprev   = bl_timept_get();
  cpuprev = bl_cpu_timept_get();

  for (bl_uword i = 0; i < bl_arr_elems (ratio); ++i) {
    bl_thread_usleep(5000);
    bl_timept t   = bl_timept_get();
    bl_timept cpu = bl_cpu_timept_get();
    ratio[i]   = (double) (cpu - cpuprev);
    ratio[i]  /= (double) (t - tprev);
    tprev      = t;
    cpuprev    = cpu;
  }
  double avg = find_most_repeated (ratio, bl_arr_elems (ratio), CPUFREQ_WINDOW);
  if (IS_MAGIC_NAN (avg)) {
    /*clock jitter couldn't match the defined criteria */
    return bl_mkerr (bl_error);

  }
  double tfreq = (double) bl_timept_get_freq();
  bl_atomic_u64_store_rlx (&cpu_timept_freq, (bl_u64) (tfreq * avg));
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
static bl_err timepoint_cpu_init (void)
{
  /* calculate cpu timepoint frequency using the monotonic clock */
  bl_uword attempts = -1;
  bl_err err;
  do {
    ++attempts;
    err = bl_cpu_timept_freq_set();
  }
  while (err.bl && attempts < BL_TIME_INIT_RETRY_ROUNDS);
  if (err.bl) {
    /*insane clock. unable to proceed */
    return err;
  }
  /* calculate initial difference between cpu timepoint clock and sysclock */
  attempts = 0;
  while(
    IS_MAGIC_NAN (bl_atomic_u64_load_rlx (&cputimepoint_sysdata.to_sys_ns)) &&
    attempts < BL_TIME_INIT_RETRY_ROUNDS
    )
  {
    /*trying to set an initial value on "ts_to_sys_ns"*/
    bl_cpu_timept_to_sysclock_diff_ns();
    ++attempts;
  }
  if (IS_MAGIC_NAN (bl_atomic_u64_load_rlx (&cputimepoint_sysdata.to_sys_ns))) {
    /*too much jitter in clock.*/
    return bl_mkerr (bl_error);
  }
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
#endif /*#ifdef BL_HAS_CPU_TSTAMP*/
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_err bl_time_extras_init (void)
{
  bl_uword attempts = 0;
  /* calculate initial difference between monotonic and sysclock */
  while(
    IS_MAGIC_NAN (bl_atomic_u64_load_rlx (&timepoint_sysdata.to_sys_ns)) &&
    attempts < BL_TIME_INIT_RETRY_ROUNDS
    )
  {
    /*trying to set an initial value on "ts_to_sys_ns"*/
    bl_timept_to_sysclock_diff_ns();
    ++attempts;
  }
  if (IS_MAGIC_NAN (bl_atomic_u64_load_rlx (&timepoint_sysdata.to_sys_ns))) {
    /*insane clock. unable to proceed */
    return bl_mkerr (bl_error);
  }
#ifdef BL_HAS_CPU_TSTAMP
  return timepoint_cpu_init();
#else
  return bl_mkok();
#endif
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT void bl_time_extras_destroy (void)
{
  /*Now this is a no-op. If something has to be done in the future we will
  need to add thread safety wrt to bl_time_extras_init and to count references.
  */
}
/*----------------------------------------------------------------------------*/
static bl_timeoft bl_timept_to_sysclock_diff_ns_impl(sysclockdiff* sc)
{
  double diff[BL_TIME_QSAMPLES];
  double sys_to_ns =
    (double) bl_nsec_in_sec / (double) bl_timept_sysclock_get_freq();

  bl_thread_yield(); /* context switching in-between minimization attempt.*/
  for (bl_uword i = 0; i < bl_arr_elems (diff); ++i) {
    bl_timept t = sc->get();
    bl_timept s = bl_timept_sysclock_get();
    bl_processor_pause();
    diff[i]  = (((double) s) * sys_to_ns) - ((double) sc->to_nsec (t));
  }
  double avg = find_most_repeated(
    diff, bl_arr_elems (diff), (double) BL_TIME_TO_SYSCLOCK_MAX_SAMPLE_DRIFT_NS
    );
  if (!IS_MAGIC_NAN (avg)) {
    /*good enough dataset. success.*/
    bl_atomic_u64_store_rlx(&sc->to_sys_ns, (bl_u64) avg);
  }
  return (bl_timeoft) bl_atomic_u64_load_rlx (&sc->to_sys_ns);
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_timeoft bl_timept_to_sysclock_diff_ns (void)
{
  return bl_timept_to_sysclock_diff_ns_impl (&timepoint_sysdata);
}
/*----------------------------------------------------------------------------*/
#ifdef BL_HAS_CPU_TSTAMP
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_timeoft bl_cpu_timept_to_nsec (bl_timept t)
{
  return (bl_timeoft)
    (((double) t * (double) bl_nsec_in_sec) / (double) bl_cpu_timept_get_freq());
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_u64 bl_cpu_timept_get_freq (void)
{
  return bl_atomic_u64_load_rlx (&cpu_timept_freq);
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_timeoft bl_cpu_timept_to_sysclock_diff_ns (void)
{
  return bl_timept_to_sysclock_diff_ns_impl (&cputimepoint_sysdata);
}
/*----------------------------------------------------------------------------*/
#endif /* #ifdef BL_HAS_CPU_TSTAMP */
