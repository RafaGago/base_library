#ifndef __BL_SERIAL_H__
#define __BL_SERIAL_H__

#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/time.h>
#include <bl/base/error.h>
#include <bl/base/memory_range.h>
#include <bl/base/allocator.h>

#include <bl/serial/serial_cfg.h>
#include <bl/serial/libexport.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
enum bl_serial_ioctl_e{
  bl_cts,        /*R. bool level*/
  bl_rts,        /*W. bool level*/
  bl_dtr,        /*W. bool level*/
  bl_dsr,        /*R. bool level*/
  bl_ri,         /*R. bool level*/
  bl_cd,         /*R. bool level*/
  bl_send_break, /*W. - */
  bl_set_break,  /*W. bool level*/
};
typedef uword bl_serial_ioctl;
/*----------------------------------------------------------------------------*/
typedef struct bl_serial bl_serial;
/*----------------------------------------------------------------------------*/
extern bl_err BL_SERIAL_EXPORT bl_serial_create(
  bl_serial** s_out, uword read_buffer_min_size, alloc_tbl const* alloc
  );
/*----------------------------------------------------------------------------*/
/* undefined behaviour is to be expected if some thread is reading or writing */
extern void BL_SERIAL_EXPORT bl_serial_destroy(
  bl_serial* s, alloc_tbl const* alloc
  );
/*----------------------------------------------------------------------------*/
extern bl_err BL_SERIAL_EXPORT bl_serial_start(
  bl_serial* s, bl_serial_cfg const* cfg
  );
/*----------------------------------------------------------------------------*/
/* undefined behaviour is to be expected if some thread is reading or writing */
extern void BL_SERIAL_EXPORT bl_serial_stop (bl_serial* s);
/*----------------------------------------------------------------------------*/
/* This call is to be called from just one thread. 0 = nonblocking. No partial 
  reads are returned in case of a timeout */
extern bl_err BL_SERIAL_EXPORT bl_serial_read(
  bl_serial* s, memr rbuff, toffset timeout_us
  );
/*----------------------------------------------------------------------------*/
/* This call is to be called from just one thread.*/
extern bl_err BL_SERIAL_EXPORT bl_serial_write(
  bl_serial* s, memr wbuff, u32* written, toffset timeout_us
  );
/*----------------------------------------------------------------------------*/
extern bl_err BL_SERIAL_EXPORT bl_serial_ioctl_get(
  bl_serial* s, bl_serial_ioctl op, uword* val
  );
/*----------------------------------------------------------------------------*/
/* undefined behaviour is to be expected if some thread is reading or writing */
extern bl_err BL_SERIAL_EXPORT bl_serial_ioctl_set(
  bl_serial* s, bl_serial_ioctl op, uword val
  );
/*----------------------------------------------------------------------------*/
extern uword BL_SERIAL_EXPORT bl_serial_get_bit_time_ns(
  bl_serial_cfg const* cfg
  );
/*----------------------------------------------------------------------------*/
extern uword BL_SERIAL_EXPORT bl_serial_get_byte_time_ns(
  bl_serial_cfg const* cfg
  );
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_SERIAL_H__ */
