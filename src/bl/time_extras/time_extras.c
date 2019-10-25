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
#define IS_MAGIC_NAN(v) ((u64)(v) == (u64) MAGIC_NAN)
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
static double find_most_repeated (double* v, uword vcount, double window)
{
  struct sample_data{
    double total;
    double winstart;
    uword  count;
  };
  qsort (v, vcount, sizeof v[0], doublecmp);

  struct sample_data cur = { v[0], v[0], 1 };
  struct sample_data max = cur;

  for (uword i = 1; i < vcount; ++i) {
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
static tstamp t_get_noinline (void)
{
  return bl_get_tstamp();
}
/*----------------------------------------------------------------------------*/
static toffset t_to_nsec_noinline (tstamp t)
{
  return bl_tstamp_to_nsec (t);
}
/*----------------------------------------------------------------------------*/
typedef struct sysclockdiff {
  atomic_u64 to_sys_ns;
  tstamp (*get) (void);
  toffset (*to_nsec) (tstamp);
}
sysclockdiff;
/*----------------------------------------------------------------------------*/
/* atomics on this lib won't compile for non lock free implementations of
and atomic types of different sizes than the basic type size */
/*----------------------------------------------------------------------------*/
static sysclockdiff timestamp_sysdata = {
  (atomic_u64) MAGIC_NAN, &t_get_noinline, &t_to_nsec_noinline
};
/*----------------------------------------------------------------------------*/
#ifdef BL_HAS_CPU_TSTAMP
/*----------------------------------------------------------------------------*/
static tstamp cpu_t_get_noinline (void)
{
  return bl_get_cputstamp();
}
/*----------------------------------------------------------------------------*/
static sysclockdiff cputimestamp_sysdata = {
  (atomic_u64) MAGIC_NAN, &cpu_t_get_noinline, &bl_cputstamp_to_nsec
};
/*----------------------------------------------------------------------------*/
atomic_u64 cpu_tstamp_freq = (atomic_u64) 0;
/*----------------------------------------------------------------------------*/
static bl_err bl_cputstamp_freq_set()
{
  if (bl_cputstamp_get_freq() != 0) {
    /* already initialized */
    return bl_mkok();
  }
  tstamp tprev;
  tstamp cpuprev;
  double ratio[BL_TIME_QSAMPLES];
  bl_thread_yield(); /* context switching in-between minimization attempt.*/

  tprev   = bl_get_tstamp();
  cpuprev = bl_get_cputstamp();

  for (uword i = 0; i < arr_elems (ratio); ++i) {
    bl_thread_usleep(5000);
    tstamp t   = bl_get_tstamp();
    tstamp cpu = bl_get_cputstamp();
    ratio[i]   = (double) (cpu - cpuprev);
    ratio[i]  /= (double) (t - tprev);
    tprev      = t;
    cpuprev    = cpu;
  }
  double avg = find_most_repeated (ratio, arr_elems (ratio), CPUFREQ_WINDOW);
  if (IS_MAGIC_NAN (avg)) {
    /*clock jitter couldn't match the defined criteria */
    return bl_mkerr (bl_error);

  }
  double tfreq = (double) bl_tstamp_get_freq();
  atomic_u64_store_rlx (&cpu_tstamp_freq, (u64) (tfreq * avg));
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
static bl_err timestamp_cpu_init (void)
{
  /* calculate cpu timestamp frequency using the monotonic clock */
  uword attempts = -1;
  bl_err err;
  do {
    ++attempts;
    err = bl_cputstamp_freq_set();
  }
  while (err.bl && attempts < BL_TIME_INIT_RETRY_ROUNDS);
  if (err.bl) {
    /*insane clock. unable to proceed */
    return err;
  }
  /* calculate initial difference between cpu timestamp clock and sysclock */
  attempts = 0;
  while(
    IS_MAGIC_NAN (atomic_u64_load_rlx (&cputimestamp_sysdata.to_sys_ns)) &&
    attempts < BL_TIME_INIT_RETRY_ROUNDS
    )
  {
    /*trying to set an initial value on "ts_to_sys_ns"*/
    bl_cputstamp_to_sysclock_diff_ns();
    ++attempts;
  }
  if (IS_MAGIC_NAN (atomic_u64_load_rlx (&cputimestamp_sysdata.to_sys_ns))) {
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
  uword attempts = 0;
  /* calculate initial difference between monotonic and sysclock */
  while(
    IS_MAGIC_NAN (atomic_u64_load_rlx (&timestamp_sysdata.to_sys_ns)) &&
    attempts < BL_TIME_INIT_RETRY_ROUNDS
    )
  {
    /*trying to set an initial value on "ts_to_sys_ns"*/
    bl_tstamp_to_sysclock_diff_ns();
    ++attempts;
  }
  if (IS_MAGIC_NAN (atomic_u64_load_rlx (&timestamp_sysdata.to_sys_ns))) {
    /*insane clock. unable to proceed */
    return bl_mkerr (bl_error);
  }
#ifdef BL_HAS_CPU_TSTAMP
  return timestamp_cpu_init();
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
static toffset bl_tstamp_to_sysclock_diff_ns_impl(sysclockdiff* sc)
{
  double diff[BL_TIME_QSAMPLES];
  double sys_to_ns =
    (double) nsec_in_sec / (double) bl_sysclock_tstamp_get_freq();

  bl_thread_yield(); /* context switching in-between minimization attempt.*/
  for (uword i = 0; i < arr_elems (diff); ++i) {
    tstamp t = sc->get();
    tstamp s = bl_get_sysclock_tstamp();
    processor_pause();
    diff[i]  = (((double) s) * sys_to_ns) - ((double) sc->to_nsec (t));
  }
  double avg = find_most_repeated(
    diff, arr_elems (diff), (double) BL_TIME_TO_SYSCLOCK_MAX_SAMPLE_DRIFT_NS
    );
  if (!IS_MAGIC_NAN (avg)) {
    /*good enough dataset. success.*/
    atomic_u64_store_rlx(&sc->to_sys_ns, (u64) avg);
  }
  return (toffset) atomic_u64_load_rlx (&sc->to_sys_ns);
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT toffset bl_tstamp_to_sysclock_diff_ns (void)
{
  return bl_tstamp_to_sysclock_diff_ns_impl (&timestamp_sysdata);
}
/*----------------------------------------------------------------------------*/
#ifdef BL_HAS_CPU_TSTAMP
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT toffset bl_cputstamp_to_nsec (tstamp t)
{
  return (toffset)
    (((double) t * (double) nsec_in_sec) / (double) bl_cputstamp_get_freq());
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT u64 bl_cputstamp_get_freq (void)
{
  return atomic_u64_load_rlx (&cpu_tstamp_freq);
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT toffset bl_cputstamp_to_sysclock_diff_ns (void)
{
  return bl_tstamp_to_sysclock_diff_ns_impl (&cputimestamp_sysdata);
}
/*----------------------------------------------------------------------------*/
#endif /* #ifdef BL_HAS_CPU_TSTAMP */
