#ifndef __BL_NONBLOCK_BACKOFF_H__
#define __BL_NONBLOCK_BACKOFF_H__

/*----------------------------------------------------------------------------*/
#include <bl/hdr/base/integer.h>
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
void nonblock_backoff_init_default (nonblock_backoff* nb, toffset sleep_us_max);  
/*----------------------------------------------------------------------------*/
void nonblock_backoff_run (nonblock_backoff* nb);
/*----------------------------------------------------------------------------*/

#endif /* __BL_NONBLOCK_BACKOFF_H__ */

