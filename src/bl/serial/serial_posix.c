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

#if BL_OS_IS_MOSTLY_POSIX

#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/select.h>

/*#include <paths.h>
#include <sysexits.h>
#include <sys/param.h>*/

#if BL_OS_IS (LINUX)
# include <linux/serial.h>
#endif


#ifdef __MACH__
#include <AvailabilityMacros.h>
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifndef TIOCINQ
#ifdef FIONREAD
#define TIOCINQ FIONREAD
#else
#define TIOCINQ 0x541B
#endif
#endif

#if defined (MAC_OS_X_VERSION_10_3) &&\
   (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_3)
#include <IOKit/serial/ioss.h>
#endif

#include <bl/base/error.h>
#include <bl/base/deadline.h>
#include <bl/base/ringbuffer.h>
#include <bl/base/integer.h>
#include <bl/base/deadline.h>
#include <bl/base/static_integer_math.h>

#include <bl/serial/serial.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
bl_define_ringb_funcs (bl_u8_dq, bl_u8)
/*----------------------------------------------------------------------------*/
struct bl_serial {
  int   fd;
  bl_ringb rq;
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
  bl_serial* s = bl_alloc (alloc, sizeof *s + read_buffer_min_size);
  if (!s) {
    return bl_mkerr (bl_alloc);
  }
  memset (s, 0, sizeof *s);
  s->fd  = -1;
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
#ifdef B0
  case 0: *enum_val = B0; return true;
#endif
#ifdef B50
  case 50: *enum_val = B50; return true;
#endif
#ifdef B75
  case 75: *enum_val = B75; return true;
#endif
#ifdef B110
  case 110: *enum_val = B110; return true;
#endif
#ifdef B134
  case 134: *enum_val = B134; return true;
#endif
#ifdef B150
  case 150: *enum_val = B150; return true;
#endif
#ifdef B200
  case 200: *enum_val = B200; return true;
#endif
#ifdef B300
  case 300: *enum_val = B300; return true;
#endif
#ifdef B600
  case 600: *enum_val = B600; return true;
#endif
#ifdef B1200
  case 1200: *enum_val = B1200; return true;
#endif
#ifdef B1800
  case 1800: *enum_val = B1800; return true;
#endif
#ifdef B2400
  case 2400: *enum_val = B2400; return true;
#endif
#ifdef B4800
  case 4800: *enum_val = B4800; return true;
#endif
#ifdef B7200
  case 7200: *enum_val = B7200; return true;
#endif
#ifdef B9600
  case 9600: *enum_val = B9600; return true;
#endif
#ifdef B14400
  case 14400: *enum_val = B14400; return true;
#endif
#ifdef B19200
  case 19200: *enum_val = B19200; return true;
#endif
#ifdef B28800
  case 28800: *enum_val = B28800; return true;
#endif
#ifdef B38400
  case 38400: *enum_val = B38400; return true;
#endif
#ifdef B57600
  case 57600: *enum_val = B57600; return true;
#endif
#ifdef B76800
  case 76800: *enum_val = B76800; return true;
#endif
#ifdef B115200
  case 115200: *enum_val = B115200; return true;
#endif
#ifdef B128000
  case 128000: *enum_val = B128000; return true;
#endif
#ifdef B153600
  case 153600: *enum_val = B153600; return true;
#endif
#ifdef B230400
  case 230400: *enum_val = B230400; return true;
#endif
#ifdef B256000
  case 256000: *enum_val = B256000; return true;
#endif
#ifdef B460800
  case 460800: *enum_val = B460800; return true;
#endif
#ifdef B576000
  case 576000: *enum_val = B576000; return true;
#endif
#ifdef B921600
  case 921600: *enum_val = B921600; return true;
#endif
#ifdef B1000000
  case 1000000: *enum_val = B1000000; return true;
#endif
#ifdef B1152000
  case 1152000: *enum_val = B1152000; return true;
#endif
#ifdef B1500000
  case 1500000: *enum_val = B1500000; return true;
#endif
#ifdef B2000000
  case 2000000: *enum_val = B2000000; return true;
#endif
#ifdef B2500000
  case 2500000: *enum_val = B2500000; return true;
#endif
#ifdef B3000000
  case 3000000: *enum_val = B3000000; return true;
#endif
#ifdef B3500000
  case 3500000: *enum_val = B3500000; return true;
#endif
#ifdef B4000000
  case 4000000: *enum_val = B4000000; return true;
#endif
  default: return false;
  }
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifdef _BSD_SOURCE
static inline void set_standard_baudrate(
  struct termios* options, int baudrate_enum_val
  )
{
  cfsetspeed (options, baudrate_enum_val);
}
/*----------------------------------------------------------------------------*/
#else /*_BSD_SOURCE*/
static inline void set_standard_baudrate(
  struct termios* options, int baudrate_enum_val
  )
{
  cfsetispeed (options, baudrate_enum_val);
  cfsetospeed (options, baudrate_enum_val);
}
/*----------------------------------------------------------------------------*/
#endif /*_BSD_SOURCE*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if defined (MAC_OS_X_VERSION_10_4) &&\
    (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
static inline bl_err try_set_nonstandard_baudrate (bl_serial* s, bl_uword baudrate)
{
  speed_t bauds = (speed_t) baudrate;
  return bk_mkerr_sys(
    (ioctl (s->fd, IOSSIOSPEED, &bauds, 1) >= 0) ? bl_ok : bl_invalid, errno
    );
}
/*----------------------------------------------------------------------------*/
#elif BL_OS_IS (LINUX) && defined (TIOCSSERIAL)
static inline bl_err try_set_nonstandard_baudrate (bl_serial* s, bl_uword baudrate)
{
  struct serial_struct ser;
  if (ioctl (s->fd, TIOCGSERIAL, &ser) < 0) {
    return bl_mkerr_sys (bl_file, errno);
  }
  ser.custom_divisor = ser.baud_base / baudrate;
  ser.flags         &= ~ASYNC_SPD_MASK;
  ser.flags         |= ASYNC_SPD_CUST;

  return bl_mkerr_sys(
    ioctl (s->fd, TIOCSSERIAL, &ser) >= 0 ? bl_ok : bl_invalid, errno
    );
}
/*----------------------------------------------------------------------------*/
#else
static inline bl_err try_set_nonstandard_baudrate (bl_serial* s, bl_uword baudrate)
{
  return bl_mkerr (bl_invalid);
}
/*----------------------------------------------------------------------------*/
#endif
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifdef IXANY
#define BL_SER_IXANY_FLAG IXANY
#else
#define BL_SER_IXANY_FLAG 0
#endif
static inline void set_xon_xoff (struct termios* options, bool on)
{
  if (on) {
    options->c_iflag |=  (IXON | IXOFF); /*|BL_SER_IXANY_FLAG)*/
  }
  else {
    options->c_iflag &= (tcflag_t) ~(IXON | IXOFF | BL_SER_IXANY_FLAG);
  }
}
/*----------------------------------------------------------------------------*/
#if defined (CRTSCTS)
  #define BL_SER_CTSRTS CRTSCTS
#elif defined (CNEW_RTSCTS)
  #define BL_SER_CTSRTS CNEW_RTSCTS
#else
#error "No OS CRTSCTS support"
#endif
static inline void set_rts_cts (struct termios* options, bool on)
{
  if (on) {
    options->c_cflag |= (BL_SER_CTSRTS);
  }
  else {
    options->c_cflag &= (unsigned long) ~(BL_SER_CTSRTS);
  }
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_start(
  bl_serial* s, bl_serial_cfg const* cfg
  )
{
  bl_assert (s && cfg);
  if (s->fd) {
    return bl_mkerr (bl_preconditions);
  }
  if (!cfg->device_name) {
    return bl_mkerr (bl_invalid);
  }

  /*open*/
try_again:
  s->fd = open (cfg->device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (s->fd < 0) {
    if (errno != EINTR) {
      goto try_again; /*interrupted by a signal: recoverable*/
    }
    return bl_mkerr_sys (bl_file, errno);
  }

  /*fixed attributes*/
  struct termios options;
  bl_err err = bl_mkerr_sys(
    tcgetattr (s->fd, &options) >= 0 ? bl_ok : bl_file, errno
    );
  if (err.own) {
    goto close;
  }
    /*raw mode / no echo / binary */
  options.c_cflag |= (tcflag_t)  (CLOCAL | CREAD);
  options.c_lflag &= (tcflag_t)
    ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG | IEXTEN);
  options.c_oflag &= (tcflag_t) ~(OPOST);
  options.c_iflag &= (tcflag_t) ~(INLCR | IGNCR | ICRNL | IGNBRK);

  /*baudrate*/
  int baudrate_enum_val;
  if (try_get_standard_baudrate (cfg->baudrate, &baudrate_enum_val)) {
    set_standard_baudrate (&options, baudrate_enum_val);
  }
  else {
    err = try_set_nonstandard_baudrate (s, cfg->baudrate);
    if (err.own) {
      goto close;
    }
  }

  /*char length*/
  options.c_cflag &= (tcflag_t) ~CSIZE;
  switch (cfg->byte_size) {
  case 5: options.c_cflag |= CS5; break;
  case 6: options.c_cflag |= CS6; break;
  case 7: options.c_cflag |= CS7; break;
  case 8: options.c_cflag |= CS8; break;
  default:
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /*stop bits*/
  switch (cfg->stop_bits) {
  case bl_stop_bits_one: {
   options.c_cflag &= (tcflag_t) ~(CSTOPB);
   break;
  }
  case bl_stop_bits_one_point_five:
    err = bl_mkerr (bl_invalid);
    goto close;
  case bl_stop_bits_two: {
    options.c_cflag |=  (CSTOPB);
    break;
  }
  default:
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /*parity*/
  options.c_iflag &= (tcflag_t) ~(INPCK | ISTRIP);
  switch (cfg->parity) {
  case bl_parity_none:
    options.c_cflag &= (tcflag_t) ~(PARENB | PARODD); break;
  case bl_parity_odd:
    options.c_cflag |=  (PARENB | PARODD);
    break;
  case bl_parity_even:
    options.c_cflag &= (tcflag_t) ~(PARODD);
    options.c_cflag |= (PARENB);
    break;
#ifdef CMSPAR
  case bl_parity_mark:
    options.c_cflag |=  (PARENB | CMSPAR | PARODD);
    break;
  case bl_parity_space:
    options.c_cflag |=  (PARENB | CMSPAR);
    options.c_cflag &= (tcflag_t) ~(PARODD);
    break;
#endif /*CMSPAR*/
  default:
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /*flow control*/
  switch (cfg->flow_control) {
  case bl_flow_control_none:
    set_xon_xoff (&options, false);
    set_rts_cts (&options, false);
    break;
  case bl_flow_control_software:
    set_xon_xoff (&options, true);
    set_rts_cts (&options, false);
    break;
  case bl_flow_control_hardware:
    set_xon_xoff (&options, false);
    set_rts_cts (&options, true);
    break;
  default:
    err = bl_mkerr (bl_invalid);
    goto close;
  }

  /* non blocking reads*/
  options.c_cc[VMIN]  = 0;
  options.c_cc[VTIME] = 0;

  err = bl_mkerr_sys(
    tcsetattr (s->fd, TCSANOW, &options) >= 0 ? bl_ok : bl_invalid, errno
    );
  if (err.own) {
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
  if (s->fd >= 0) {
    close (s->fd);
    s->fd = -1;
  }
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_read(
  bl_serial* s, bl_memr rbuff, bl_timeoft timeout_us
  )
{
  bl_assert (s);
  bl_assert (s->fd >= 0);
  bl_assert (timeout_us >= 0);

  if (!bl_memr_is_valid (rbuff) || bl_memr_size (rbuff) > bl_u8_dq_capacity (&s->rq)) {
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

  bl_timept deadline = 0;
  bl_err err      = bl_mkok();
  if (timeout_us != 0) {
    static_assert(BL_TIMEPT_BASE <= BL_MICROSECOND_BASE ,"paco");
    err =  bl_timept_deadline_init_usec (&deadline, timeout_us);
    if (err.own) {
      goto rollback;
    }
  }
  /*read*/
  while (copied < bl_memr_size (rbuff)) {
    bl_uword rd = read(
      s->fd, bl_memr_at (rbuff, copied), bl_memr_size (rbuff) - copied
      );
    copied += rd;
    if (rd != 0) {
      continue;
    }
    bl_timeptdiff diff = (timeout_us != 0) ?
      bl_timept_deadline_compare (deadline, bl_timept_get()) : -1;
      /*context switches from here to the pselect will be suboptimal*/
    if (diff <= 0) {
      err = bl_mkerr (bl_timeout);
      goto rollback;
    }
    bl_uword us_pending = bl_timept_to_usec_ceil ((bl_timept) diff);

    fd_set fds;
    FD_ZERO (&fds);
    FD_SET (s->fd, &fds);

    struct timespec t;
    t.tv_sec  = us_pending / bl_usec_in_sec;
    t.tv_nsec = (us_pending - (t.tv_sec * bl_usec_in_sec)) * bl_nsec_in_usec;

    int r = pselect (s->fd + 1, &fds, nullptr, nullptr, &t, nullptr);
    if (r < 0 && errno != EINTR) {
      err = bl_mkerr_sys (bl_error, errno);
      goto rollback;
    }
  }
  return bl_mkok();

rollback:
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
  bl_assert (s->fd >= 0);
  bl_assert (timeout_us >= 0);

  if (!bl_memr_is_valid (wbuff) || !written) {
    return bl_mkerr (bl_invalid);
  }

  *written        = 0;
  bl_timept deadline = 0;
  bl_err err      = bl_mkok();
  if (timeout_us != 0) {
    err = bl_timept_deadline_init_usec (&deadline, timeout_us);
    if (err.own) {
      return err;
    }
  }

  bl_uword wr = 0;
  while (wr < bl_memr_size (wbuff)) {
    fd_set fds;
    FD_ZERO (&fds);
    FD_SET (s->fd, &fds);

    struct timespec t;
    struct timespec* t_ptr = nullptr;
    if (timeout_us != 0) {
      bl_timeptdiff diff = bl_timept_deadline_compare (deadline, bl_timept_get());
      if (diff <= 0) {
        err = bl_mkerr (bl_timeout);
        goto end;
      }
      bl_uword us_pending = bl_timept_to_usec_ceil ((bl_timept) diff);
      t.tv_sec  = us_pending / bl_usec_in_sec;
      t.tv_nsec = (us_pending - (t.tv_sec * bl_usec_in_sec)) * bl_nsec_in_usec;
      t_ptr     = &t;
    }
    if (pselect (s->fd + 1, nullptr, &fds, nullptr, t_ptr, nullptr) > 0) {
      bl_assert (FD_ISSET (s->fd, &fds));
      bl_uword now = write (s->fd, bl_memr_at (wbuff, wr), bl_memr_size (wbuff) - wr);
      bl_assert (now <= bl_memr_size (wbuff) - wr);
      if (bl_unlikely (now == 0)) {
        /*device disconnected on Linux*/
        err = bl_mkerr (bl_error);
        goto end;
      }
      wr += now;
    }
  }
end:
  *written = wr;
  return err;
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_ioctl_get(
  bl_serial* s, bl_serial_ioctl op, bl_uword* val
  )
{
  bl_assert (s);
  if (s->fd < 0 || !val) {
    return bl_mkerr (bl_invalid);
  }
  int mask;
  switch (op) {
  case bl_cts:        mask = TIOCM_CTS; break;
  case bl_rts:        return bl_mkerr (bl_invalid);
  case bl_dtr:        return bl_mkerr (bl_invalid);
  case bl_dsr:        mask = TIOCM_DSR; break;
  case bl_ri:         mask = TIOCM_RI; break;
  case bl_cd:         mask = TIOCM_CD; break;
  case bl_send_break: return bl_mkerr (bl_invalid);
  case bl_set_break:  return bl_mkerr (bl_invalid);
  default:            return bl_mkerr (bl_invalid);
  }
  int status;
  if (ioctl (s->fd, TIOCMGET, &status) >= 0) {
    *val = (status & mask) != 0;
    return bl_mkok();
  }
  return bl_mkerr_sys (bl_error, errno);
}
/*----------------------------------------------------------------------------*/
BL_SERIAL_EXPORT bl_err bl_serial_ioctl_set(
  bl_serial* s, bl_serial_ioctl op, bl_uword val
  )
{
  bl_assert (s);
  if (s->fd < 0) {
    return bl_mkerr (bl_invalid);
  }
  int ioctlv;
  int cmd;
  switch (op) {
  case bl_cts:        return bl_mkerr (bl_invalid);
  case bl_rts:
    cmd    = TIOCM_RTS;
    ioctlv = (val) ? TIOCMBIS : TIOCMBIC;
    break;
  case bl_dtr:
    cmd    = TIOCM_DTR;
    ioctlv = (val) ? TIOCMBIS : TIOCMBIC;
    break;
  case bl_dsr:        return bl_mkerr (bl_invalid);
  case bl_ri:         return bl_mkerr (bl_invalid);
  case bl_cd:         return bl_mkerr (bl_invalid);
  case bl_send_break:
    tcsendbreak (s->fd, 0);
    return bl_mkok();
  case bl_set_break:
    ioctlv = (val) ? TIOCSBRK : TIOCCBRK;
    return bl_mkerr_sys (ioctl (s->fd, ioctlv) >= 0 ? bl_ok : bl_error, errno);
  default:            return bl_mkerr (bl_invalid);
  }
  return bl_mkerr_sys(
    ioctl (s->fd, ioctlv, &cmd) >= 0 ? bl_ok : bl_error, errno
    );
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
  bl_u64 bits_byte = 1 +
    cfg->byte_size +
    (cfg->parity != bl_parity_none) +
    (cfg->stop_bits == bl_stop_bits_one) ? 1 : 2;
  return (bl_uword) bl_fixp_to_int (bit_ns * bits_byte, 32);
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* #if BL_OS_IS_MOSTLY_POSIX */
