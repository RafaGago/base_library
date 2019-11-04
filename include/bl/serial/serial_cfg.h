#ifndef __BL_SERIAL_CFG_H__
#define __BL_SERIAL_CFG_H__

#include <bl/base/integer.h>

/*----------------------------------------------------------------------------*/
enum bl_serial_stop_bits_e {
  bl_stop_bits_one,
  bl_stop_bits_one_point_five,
  bl_stop_bits_two,
};
typedef bl_u8 bl_serial_stop_bits;
/*----------------------------------------------------------------------------*/
enum bl_serial_parity_e {
  bl_parity_none,
  bl_parity_odd,
  bl_parity_even,
  bl_parity_mark,
  bl_parity_space,
};
typedef bl_u8 bl_serial_parity;
/*----------------------------------------------------------------------------*/
enum bl_serial_flow_control_e{
  bl_flow_control_none,
  bl_flow_control_software,
  bl_flow_control_hardware
};
typedef bl_u8 bl_serial_flow_control;
/*----------------------------------------------------------------------------*/
typedef struct bl_serial_cfg {
  char const*            device_name; /* "/dev/ttyS0", "COM1", etc... */
  bl_u32                 baudrate;
  bl_u8                  byte_size;
  bl_serial_stop_bits    stop_bits;
  bl_serial_flow_control flow_control;
  bl_serial_parity       parity;
}
bl_serial_cfg;
/*----------------------------------------------------------------------------*/
#endif /* __BL_SERIAL_CFG_H__ */

