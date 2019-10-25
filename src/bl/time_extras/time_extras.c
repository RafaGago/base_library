#include <bl/base/thread.h>
#include <bl/base/atomic.h>

#include <bl/time_extras/time_extras.h>

#define TIME_UNSET (0x7ff8000000000000UL) /* A NaN value */
#define IS_TIME_UNSET(v) ((u64)(v) == (u64) TIME_UNSET)

#ifndef BL_TIME_QROUNDS
  #define BL_TIME_QROUNDS 10
#endif

#ifndef BL_TIME_INIT_QROUNDS
  #define BL_TIME_INIT_QROUNDS 10
#endif

#ifndef BL_TIME_QSAMPLES
  #define BL_TIME_QSAMPLES 10
#endif

#ifndef BL_TIME_QMAXDRIFT_AVG_NS
  #define BL_TIME_QMAXDRIFT_AVG_NS 30000
#endif

#define MAXDRIFT_NS_POW2 \
  ((double)(BL_TIME_QMAXDRIFT_AVG_NS * BL_TIME_QMAXDRIFT_AVG_NS))
#define QSAMPLES BL_TIME_QSAMPLES

/*----------------------------------------------------------------------------*/
static tstamp t_get_noinline (void)
{
  return bl_get_tstamp();
}
/*----------------------------------------------------------------------------*/
typedef struct sysclockdiff {
  atomic_u64 to_sys_ns;
  tstamp (*tstamp_get) (void);
}
sysclockdiff;
/*----------------------------------------------------------------------------*/
/* atomics on this lib won't compile for non lock free implementations of
and atomic types of different sizes than the basic type size */
static sysclockdiff timestamp_sysdata = {
  (atomic_u64) TIME_UNSET, &t_get_noinline
};
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT bl_err bl_time_extras_init (void)
{
  uword attempts = 0;
  while(
    IS_TIME_UNSET (atomic_u64_load_rlx (&timestamp_sysdata.to_sys_ns)) &&
    attempts < BL_TIME_INIT_QROUNDS
    )
  {
    /*trying to set an initial value on "ts_to_sys_ns"*/
    bl_tstamp_to_sysclock_diff_ns();
    ++attempts;
  }
  if (IS_TIME_UNSET (atomic_u64_load_rlx (&timestamp_sysdata.to_sys_ns))) {
    /*insane clock. unable to proceed */
    return bl_mkerr (bl_error);
  }
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT void bl_time_extras_destroy (void)
{
  /*Now this is a no-op. If something has to be done in the future we will
  need to add thread safety wrt to bl_time_extras_init and to count references.
  */
}
/*----------------------------------------------------------------------------*/
static toffset bl_tstamp_to_sysclock_diff_ns_impl (sysclockdiff* s)
{
  double best_avg    = (double) TIME_UNSET;
  double best_stddev = (double) TIME_UNSET;
  double sys_to_ns =
    (double) nsec_in_sec / (double) bl_sysclock_tstamp_get_freq();

  for (uword r = 0; r < BL_TIME_QROUNDS; ++r) {
    double diff[QSAMPLES];
    double avg;
next:
    avg = 0.;
    for (uword i = 0; i < QSAMPLES; ++i) {
      bl_thread_yield(); /* context switching in-between minimization attempt.*/
      tstamp t = s->tstamp_get();
      tstamp s = bl_get_sysclock_tstamp();
      diff[i]  = (((double) s) * sys_to_ns) - (double) bl_tstamp_to_nsec (t);
      avg     += diff[i];
    }
    avg /= (double) QSAMPLES;
    double stddevpow2 = 0.;
    for (uword i = 0; i < QSAMPLES; ++i) {
      double v     = diff[i] - avg;
      double vpow2 = v * v;
      if (v > MAXDRIFT_NS_POW2) {
        /*very low quality sample, aborting this sample series*/
        goto next; /*better than the non-goto alternative.*/
      }
      stddevpow2 += vpow2;
    }
    stddevpow2 /= (double) QSAMPLES;
    /* Getting the real stddev isn't required, This can avoid "sqrt" as it is
    only used to compare against the minimum.*/
    if (IS_TIME_UNSET (best_stddev) || stddevpow2 < best_stddev) {
      best_avg    = avg;
      best_stddev = stddevpow2;
    }
  }
  /*Considering measurements from contending threads equally good.

  The absolute best implementation of this would function would be to have it
  mutex protected by a mutex that could inform if the thread was free to acquire
  the lock or if it had to wait.

  With that mutex, the thread that could aqquire it directly would make the
  calculation and the ones that waited would know that the cached value
  (ts_to_sys_ns) is recent enough to be returned.

  Unfortunately this function is not enough motivation for me to implement and
  support that mutex between platforms.*/
  if (!IS_TIME_UNSET (best_avg)) {
    atomic_u64_store_rlx (&s->to_sys_ns, (u64) best_avg);
  }
  return (toffset) (double) atomic_u64_load_rlx (&s->to_sys_ns);
}
/*----------------------------------------------------------------------------*/
BL_TIME_EXTRAS_EXPORT toffset bl_tstamp_to_sysclock_diff_ns (void)
{
  return bl_tstamp_to_sysclock_diff_ns_impl (&timestamp_sysdata);
}
/*----------------------------------------------------------------------------*/
