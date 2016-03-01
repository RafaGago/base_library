#ifndef __BL_NONBLOCK_BACKOFF_H__
#define __BL_NONBLOCK_BACKOFF_H__

/*----------------------------------------------------------------------------*/
#include <bl/base/integer.h>
#include <bl/base/time.h>

#include <bl/nonblock/libexport.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
typedef struct nonblock_backoff {
  uword   spin;
  uword   spin_max;
  uword   yield;
  uword   yield_max;
  toffset sleep_us;
  toffset sleep_us_mul;
  toffset sleep_us_div;
  toffset sleep_us_max;
}
nonblock_backoff;
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT 
  void nonblock_backoff_init(
    nonblock_backoff* nb, 
    uword             spin_max,
    uword             yield_max, 
    toffset           sleep_us_init,
    toffset           sleep_us_mul,
    toffset           sleep_us_div,
    toffset           sleep_us_max
    );
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT 
  void nonblock_backoff_init_default(
    nonblock_backoff* nb, toffset sleep_us_max
    );  
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT 
  void nonblock_backoff_run (nonblock_backoff* nb);
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCK_BACKOFF_H__ */

