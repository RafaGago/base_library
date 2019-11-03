/*----------------------------------------------------------------------------*/
/* Thanks to:
   -William Woodall <wjwwood@gmail.com> and
   -John Harrison <ash@greaterthaninfinity.com>

   At:
   https://github.com/wjwwood/serial

   A lot of code has been adapted from there. The heavy-duty work was already
   done by them.
*/
/*----------------------------------------------------------------------------*/
#include <bl/base/platform.h>

#ifdef BL_WINDOWS

#include <string>

#include <bl/base/include_windows.h>
#include <bl/base/error.h>
#include <bl/base/time.h>
#include <bl/base/deadline.h>
#include <bl/base/ringbuffer.h>
#include <bl/base/integer.h>
#include <bl/base/deadline.h>
#include <bl/base/static_integer_math.h>

#include <bl/serial/serial.h>

/*----------------------------------------------------------------------------*/
static inline std::string prefix_port (std::string const& in)
{
  static const char windows_com_port_prefix[] = "\\\\.\\";
  if (in.compare (windows_com_port_prefix) != 0)
  {
    return std::string (windows_com_port_prefix) + in;
  }
  return in;
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
bl_define_ringb_funcs (bl_u8_dq, bl_u8)
/*----------------------------------------------------------------------------*/
struct bl_serial {
  HANDLE fd;
  bl_ringb  rq;
  bl_uword  rtimeout_ms;
  bl_uword  wtimeout_ms;
};
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_init(
  bl_serial** s_out, bl_uword read_buffer_min_size, bl_alloc_tbl const* alloc
  )
{
  bl_assert (s_out && alloc);
  read_buffer_min_size = bl_round_next_pow2_u (read_buffer_min_size);
  if (read_buffer_min_size == 0 || !alloc || !s_out) {
    return bl_mkerr (bl_invalid);
  }
  bl_serial* s;
  s = (bl_serial*) bl_alloc (alloc, sizeof *s + read_buffer_min_size);
  if (!s) {
    return bl_mkerr (bl_alloc);
  }
  memset (s, 0, sizeof *s);
  s->fd = INVALID_HANDLE_VALUE;
  (void) bl_u8_dq_init_extern(
    &s->rq,
    ((bl_u8*) s) + sizeof *s,
    read_buffer_min_size
    );
  *s_out = s;
  return bl_mkok();
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT void
  bl_serial_destroy (bl_serial* s, bl_alloc_tbl const* alloc)
{
  bl_assert (s);
  bl_serial_stop (s);
  bl_dealloc (alloc, s);
}
/*----------------------------------------------------------------------------*/
static bool try_get_standard_baudrate (bl_uword baudrate, int* enum_val)
{
  switch (baudrate) {
#ifdef CBR_0
  case 0: *enum_val = CBR_0; return true;
#endif
#ifdef CBR_50
  case 50: *enum_val = CBR_50; return true;
#endif
#ifdef CBR_75
  case 75: *enum_val = CBR_75; return true;
#endif
#ifdef CBR_110
  case 110: *enum_val = CBR_110; return true;
#endif
#ifdef CBR_134
  case 134: *enum_val = CBR_134; return true;
#endif
#ifdef CBR_150
  case 150: *enum_val = CBR_150; return true;
#endif
#ifdef CBR_200
  case 200: *enum_val = CBR_200; return true;
#endif
#ifdef CBR_300
  case 300: *enum_val = CBR_300; return true;
#endif
#ifdef CBR_600
  case 600: *enum_val = CBR_600; return true;
#endif
#ifdef CBR_1200
  case 1200: *enum_val = CBR_1200; return true;
#endif
#ifdef CBR_1800
  case 1800: *enum_val = CBR_1800; return true;
#endif
#ifdef CBR_2400
  case 2400: *enum_val = CBR_2400; return true;
#endif
#ifdef CBR_4800
  case 4800: *enum_val = CBR_4800; return true;
#endif
#ifdef CBR_7200
  case 7200: *enum_val = CBR_7200; return true;
#endif
#ifdef CBR_9600
  case 9600: *enum_val = CBR_9600; return true;
#endif
#ifdef CBR_14400
  case 14400: *enum_val = CBR_14400; return true;
#endif
#ifdef CBR_19200
  case 19200: *enum_val = CBR_19200; return true;
#endif
#ifdef CBR_28800
  case 28800: *enum_val = CBR_28800; return true;
#endif
#ifdef CBR_38400
  case 38400: *enum_val = CBR_38400; return true;
#endif
#ifdef CBR_57600
  case 57600: *enum_val = CBR_57600; return true;
#endif
#ifdef CBR_76800
  case 76800: *enum_val = CBR_76800; return true;
#endif
#ifdef CBR_115200
  case 115200: *enum_val = CBR_115200; return true;
#endif
#ifdef CBR_128000
  case 128000: *enum_val = CBR_128000; return true;
#endif
#ifdef CBR_153600
  case 153600: *enum_val = CBR_153600; return true;
#endif
#ifdef CBR_230400
  case 230400: *enum_val = CBR_230400; return true;
#endif
#ifdef CBR_256000
  case 256000: *enum_val = CBR_256000; return true;
#endif
#ifdef CBR_460800
  case 460800: *enum_val = CBR_460800; return true;
#endif
#ifdef CBR_576000
  case 576000: *enum_val = CBR_576000; return true;
#endif
#ifdef CBR_921600
  case 921600: *enum_val = CBR_921600; return true;
#endif
  default: *enum_val = baudrate; return false;
  }
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_start(
  bl_serial* s, bl_serial_cfg const* cfg
  )
{
  bl_assert (s && cfg);
  if (s->fd != INVALID_HANDLE_VALUE) {
    return bl_mkerr (bl_not_allowed);
  }
  if (!s || !cfg || !cfg->device_name) {
    return bl_mkerr (bl_invalid);
  }
  s->fd = CreateFile(
    prefix_port (std::string (cfg->device_name)).c_str(),
    GENERIC_READ | GENERIC_WRITE,
    0,
    0,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    0
    );
  if (s->fd == INVALID_HANDLE_VALUE) {
    return bl_mkerr (bl_file);
  }

  DCB options;
  memset (&options, 0, sizeof options);
  options.DCBlength = sizeof options;

  bl_err err = bl_mkok();

  /*baudrate*/
  int br_enum;
  try_get_standard_baudrate (cfg->baudrate, &br_enum);
  options.BaudRate = br_enum;

  /*char length*/
  options.ByteSize = cfg->byte_size;
  if (cfg->byte_size < 5 || cfg->byte_size > 8) {
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /*stop bits*/
  switch (cfg->stop_bits) {
  case bl_stop_bits_one:
    options.StopBits = ONESTOPBIT; break;
  case bl_stop_bits_one_point_five:
    options.StopBits = ONE5STOPBITS; break;
  case bl_stop_bits_two:
    options.StopBits = TWOSTOPBITS; break;
  default:
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /*parity*/
  switch (cfg->parity) {
  case bl_parity_none:
    options.Parity = NOPARITY; break;
  case bl_parity_odd:
    options.Parity = ODDPARITY; break;
  case bl_parity_even:
    options.Parity = EVENPARITY; break;
  case bl_parity_mark:
    options.Parity = MARKPARITY; break;
  case bl_parity_space:
    options.Parity = SPACEPARITY; break;
  default:
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /*flow control*/
  switch (cfg->flow_control) {
  case bl_flow_control_none:
    options.fOutxCtsFlow = false;
    options.fRtsControl  = 0x00;
    options.fOutX        = false;
    options.fInX         = false;
    break;
  case bl_flow_control_software:
    options.fOutxCtsFlow = false;
    options.fRtsControl  = 0x00;
    options.fOutX        = true;
    options.fInX         = true;
    break;
  case bl_flow_control_hardware:
    options.fOutxCtsFlow = true;
    options.fRtsControl  = 0x03;
    options.fOutX        = false;
    options.fInX         = false;
    break;
  default:
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /* activate settings*/
  if (!SetCommState (s->fd, &options)){
    err = bl_mkerr (bl_error);
    goto close;
  }

  s->rtimeout_ms = 1000;
  s->wtimeout_ms = 1000;

  COMMTIMEOUTS timeouts;
  /*both blocking reads and writes*/
  timeouts.ReadIntervalTimeout         = 0;
  timeouts.ReadTotalTimeoutMultiplier  = 0;
  timeouts.ReadTotalTimeoutConstant    = s->rtimeout_ms;
  timeouts.WriteTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant   = s->wtimeout_ms;
  if (!SetCommTimeouts (s->fd, &timeouts)) {
    err = bl_mkerr (bl_error);
    goto close;
  }
  return bl_mkok();

close:
  bl_serial_stop (s);
  return err;

}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT void bl_serial_stop (bl_serial* s)
{
  if (s && s->fd != INVALID_HANDLE_VALUE) {
    CloseHandle (s->fd);
    s->fd = nullptr;
  }
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_read(
  bl_serial* s, bl_memr rbuff, bl_timeoft timeout_us
  )
{
  bl_assert (s);
  bl_assert (s->fd != INVALID_HANDLE_VALUE);
  bl_assert (timeout_us >= 0);

  if (!bl_memr_is_valid (rbuff)
    || bl_memr_size (rbuff) > bl_u8_dq_capacity (&s->rq)
    ) {
    return bl_mkerr (bl_invalid);
  }
  /*leftovers from previous reads*/
  bl_uword copied = bl_min (bl_memr_size (rbuff), bl_u8_dq_size (&s->rq));
  if (copied != 0) {
    bl_uword adjacent = bl_u8_dq_adjacent_elems_from (&s->rq, 0, copied);
    memcpy (bl_memr_beg (rbuff), bl_u8_dq_at_head (&s->rq), adjacent);
    if (adjacent != copied) {
      memcpy(
        bl_memr_at (rbuff, adjacent),
        bl_u8_dq_at (&s->rq, adjacent),
        copied - adjacent
        );
    }
    bl_u8_dq_drop_head_n (&s->rq, copied);
  }
  if (copied == bl_memr_size (rbuff)) {
    return bl_mkok();
  }

  bl_err err = bl_mkok();
  bl_uword t_ms = bl_div_ceil (timeout_us, bl_usec_in_msec);
  if (t_ms != s->rtimeout_ms) {
    COMMTIMEOUTS timeouts;
    if (!GetCommTimeouts (s->fd, &timeouts)) {
      err = bl_mkerr (bl_error);
      goto roll_back;
    }
    timeouts.ReadTotalTimeoutConstant = t_ms;
    if (!SetCommTimeouts (s->fd, &timeouts)) {
      err = bl_mkerr (bl_error);
      goto roll_back;
    }
    s->rtimeout_ms = t_ms;
  }

  DWORD read;
  DWORD toread = bl_memr_size (rbuff) - copied;
  bool rf = ReadFile (s->fd, bl_memr_at (rbuff, copied), toread, &read, nullptr);
  copied += read;
  if (!rf) {
    err = bl_mkerr (bl_error);
    goto roll_back;
  }
  else if (copied != bl_memr_size (rbuff)) {
    err = bl_mkerr (bl_timeout);
    goto roll_back;
  }
  return bl_mkok();
roll_back:
  /*unsucessful read*/
  bl_assert (bl_u8_dq_size (&s->rq) == 0);
  bl_u8_dq_set_start_position (&s->rq, 0);
  bl_u8_dq_expand_tail_n (&s->rq, copied);
  memcpy (bl_u8_dq_at_head (&s->rq), bl_memr_beg (rbuff), copied);
  return err;
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_write(
  bl_serial* s, bl_memr wbuff, bl_u32* written, bl_timeoft timeout_us
  )
{
  bl_assert (s);
  bl_assert (written);
  bl_assert (s->fd != INVALID_HANDLE_VALUE);
  bl_assert (timeout_us >= 0);

  *written = 0;
  if (!bl_memr_is_valid (wbuff) || !written) {
    return bl_mkerr (bl_invalid);
  }
  bl_uword t_ms = (timeout_us != 0) ? bl_div_ceil (timeout_us, bl_usec_in_msec) : 1;
  if (t_ms != s->wtimeout_ms) {
    COMMTIMEOUTS timeouts;
    if (!GetCommTimeouts (s->fd, &timeouts)) {
      return bl_mkerr (bl_error);
    }
    timeouts.WriteTotalTimeoutConstant = t_ms;
    if (!SetCommTimeouts (s->fd, &timeouts)) {
      return bl_mkerr (bl_error);
    }
    s->wtimeout_ms = t_ms;
  }

  DWORD wr;
  bool wf = WriteFile(
    s->fd, bl_memr_beg (wbuff), bl_memr_size (wbuff), &wr, nullptr
    );
  *written = wr;
  if (!wf) {
    return bl_mkerr (bl_error);
  }
  else if (bl_memr_size (wbuff) != wr) {
    return bl_mkerr (bl_timeout);
  }
  else {
    return bl_mkok();
  }
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_ioctl_get(
  bl_serial* s, bl_serial_ioctl op, bl_uword* val
  )
{
  bl_assert (s);
  if (s->fd == INVALID_HANDLE_VALUE || !val) {
    return bl_mkerr (bl_invalid);
  }
  DWORD mask;
  switch (op) {
  case bl_cts:        mask = MS_CTS_ON; break;
  case bl_rts:        return bl_mkerr (bl_invalid);
  case bl_dtr:        return bl_mkerr (bl_invalid);
  case bl_dsr:        mask = MS_DSR_ON; break;
  case bl_ri:         mask = MS_RING_ON; break;
  case bl_cd:         mask = MS_RLSD_ON; break;
  case bl_send_break: return bl_mkerr (bl_invalid);
  case bl_set_break:  return bl_mkerr (bl_invalid);
  default:            return bl_mkerr (bl_invalid);
  }
  DWORD stat;
  if (GetCommModemStatus(s->fd, &stat)) {
    *val = (stat & mask) != 0;
    return bl_mkok();
  }
  return bl_mkerr (bl_error);
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_ioctl_set(
  bl_serial* s, bl_serial_ioctl op, bl_uword val
  )
{
  bl_assert (s);
  if (s->fd == INVALID_HANDLE_VALUE) {
    return bl_mkerr (bl_invalid);
  }
  DWORD fn;
  switch (op) {
  case bl_cts:        return bl_mkerr (bl_invalid);
  case bl_rts:        fn = (val) ? SETRTS : CLRRTS; break;
  case bl_dtr:        fn = (val) ? SETDTR : CLRDTR; break;
  case bl_dsr:        return bl_mkerr (bl_invalid);
  case bl_ri:         return bl_mkerr (bl_invalid);
  case bl_cd:         return bl_mkerr (bl_invalid);
  case bl_send_break: return bl_unsupported;
  case bl_set_break:  fn = (val) ? SETBREAK : CLRBREAK; break;
  default:            return bl_mkerr (bl_invalid);
  }
  return EscapeCommFunction (s->fd, fn) ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_uword bl_serial_get_bit_time_ns (bl_serial_cfg const* cfg)
{
  bl_assert (cfg);
  if (!cfg) {
    return 0;
  }
  return (bl_uword) bl_fixp_to_int(
      bl_int_to_fixp (bl_u64, bl_nsec_in_sec, 32) / cfg->baudrate, 32
      );
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_uword bl_serial_get_byte_time_ns (bl_serial_cfg const* cfg)
{
  bl_assert (cfg);
  if (!cfg) {
    return 0;
  }
  bl_u64 bit_ns    = bl_int_to_fixp (bl_u64, bl_nsec_in_sec, 32) / cfg->baudrate;
  bl_u64 bits_byte = 1 + cfg->byte_size + (cfg->parity != bl_parity_none);
  bits_byte     = bl_int_to_fixp (bl_u64, bits_byte, 32);

  switch (cfg->stop_bits) {
  case bl_stop_bits_one:
    bits_byte += bl_int_to_fixp (bl_u64, 1, 32);
    break;
  case bl_stop_bits_one_point_five:
    bits_byte += bl_fixp_div(
      bl_u64,
      bl_int_to_fixp (bl_u64, 3, 32),
      bl_int_to_fixp (bl_u64, 2, 32),
      32
      );
    break;
  case bl_stop_bits_two:
    bits_byte += bl_int_to_fixp (bl_u64, 2, 32);
    break;
  default:
    break;
  }
  return (bl_uword) bl_fixp_to_int(
    bl_fixp_mul (bl_u64, bit_ns, bits_byte, 32),
    32
    );
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* #ifdef BL_WIN */
