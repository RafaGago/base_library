#ifndef __BL_SERIAL_H__
#define __BL_SERIAL_H__

#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/time.h>
#include <bl/hdr/base/error.h>
#include <bl/hdr/base/memory_range.h>
#include <bl/hdr/base/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
enum bl_serial_stop_bits_e {
  bl_stop_bits_one,
  bl_stop_bits_one_point_five,
  bl_stop_bits_two,
}; 
typedef u8 bl_serial_stop_bits;
/*----------------------------------------------------------------------------*/
enum bl_serial_parity_e {
  bl_parity_none,
  bl_parity_odd,
  bl_parity_even,
  bl_parity_mark,
  bl_parity_space,
};
typedef u8 bl_serial_parity;
/*----------------------------------------------------------------------------*/
enum bl_serial_flow_control_e{
  bl_flow_control_none,
  bl_flow_control_software,
  bl_flow_control_hardware
};
typedef u8 bl_serial_flow_control;
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
typedef struct bl_serial_cfg {
  char const*            device_name; /* "/dev/ttyS0", "COM1", etc... */
  u32                    baudrate;
  u8                     byte_size;
  bl_serial_stop_bits    stop_bits;
  bl_serial_flow_control flow_control;
  bl_serial_parity       parity;
}
bl_serial_cfg;
/*----------------------------------------------------------------------------*/
typedef struct bl_serial bl_serial;
/*----------------------------------------------------------------------------*/
extern bl_err bl_serial_create(
  bl_serial** s_out, uword read_buffer_min_size, alloc_tbl const* alloc
  );
/*----------------------------------------------------------------------------*/
/* undefined behaviour is to be expected if some thread is reading or writing */
extern void bl_serial_destroy (bl_serial* s, alloc_tbl const* alloc);
/*----------------------------------------------------------------------------*/
extern bl_err bl_serial_start (bl_serial* s, bl_serial_cfg const* cfg);
/*----------------------------------------------------------------------------*/
/* undefined behaviour is to be expected if some thread is reading or writing */
extern void bl_serial_stop (bl_serial* s);
/*----------------------------------------------------------------------------*/
/* This call is to be called from just one thread. 0 = nonblocking. No partial 
  reads are returned in case of a timeout */
extern bl_err bl_serial_read (bl_serial* s, memr rbuff, toffset timeout_us); 
/*----------------------------------------------------------------------------*/
/* This call is to be called from just one thread.*/
extern bl_err bl_serial_write(
  bl_serial* s, memr wbuff, u32* written, i32 timeout_us
  );
/*----------------------------------------------------------------------------*/
extern bl_err bl_serial_ioctl_get(
  bl_serial* s, bl_serial_ioctl op, uword* val
  );
/*----------------------------------------------------------------------------*/
/* undefined behaviour is to be expected if some thread is reading or writing */
extern bl_err bl_serial_ioctl_set (bl_serial* s, bl_serial_ioctl op, uword val);
/*----------------------------------------------------------------------------*/
extern uword bl_serial_get_bit_time_ns (bl_serial_cfg const* cfg);
/*----------------------------------------------------------------------------*/
extern uword bl_serial_get_byte_time_ns (bl_serial_cfg const* cfg);
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_SERIAL_H__ */

