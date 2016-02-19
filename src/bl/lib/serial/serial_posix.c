/*----------------------------------------------------------------------------*/
/* Thanks to:
   -William Woodall <wjwwood@gmail.com> and 
   -John Harrison <ash@greaterthaninfinity.com> 

   At:
   https://github.com/wjwwood/serial

   A lot of code has been adapted from there. The heavy-duty work was done
   there.
*/
/*----------------------------------------------------------------------------*/
#include <bl/hdr/base/platform.h>

#ifdef BL_POSIX

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

#if defined (BL_LINUX)
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

#include <bl/hdr/base/error.h>
#include <bl/hdr/base/deadline.h>
#include <bl/hdr/base/ringbuffer.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/deadline.h>
#include <bl/hdr/base/static_integer_math.h>

#include <bl/lib/serial/serial.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
define_ringb_types (u8_dq, u8)
define_ringb_funcs (u8_dq, u8, static inline)
declare_ringb_funcs (u8_dq, u8, static inline)
/*----------------------------------------------------------------------------*/
struct bl_serial {
  int   fd;
  u8_dq rq;
};
/*----------------------------------------------------------------------------*/
bl_err BL_SERIAL_EXPORT bl_serial_create(
  bl_serial** s_out, uword read_buffer_min_size, alloc_tbl const* alloc
  )
{
  bl_assert (s_out && alloc);
  read_buffer_min_size = round_next_pow2_u (read_buffer_min_size);
  if (read_buffer_min_size == 0 || !alloc || !s_out) {
    return bl_invalid;
  }
  bl_serial* s = bl_alloc (alloc, sizeof *s + read_buffer_min_size);
  if (!s) {
    return bl_alloc;
  }
  memset (s, 0, sizeof *s);
  s->fd  = -1;
  (void) u8_dq_init_extern(
    &s->rq,
    ((u8*) s) + sizeof *s,
    read_buffer_min_size
    );
  *s_out = s;
  return bl_ok;
}
/*----------------------------------------------------------------------------*/
void BL_SERIAL_EXPORT bl_serial_destroy (bl_serial* s, alloc_tbl const* alloc)
{
  bl_assert (s);
  bl_serial_stop (s);
destroy:
  bl_dealloc (alloc, s);
}
/*----------------------------------------------------------------------------*/
static bool try_get_standard_baudrate (uword baudrate, int* enum_val)
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
static inline bl_err try_set_nonstandard_baudrate (bl_serial* s, uword baudrate)
{
  speed_t bauds = (speed_t) baudrate;
  return ioctl (s->fd, IOSSIOSPEED, &bauds, 1) >= 0) ? bk_ok : bl_invalid;
}
/*----------------------------------------------------------------------------*/
#elif defined (BL_LINUX) && defined (TIOCSSERIAL)
static inline bl_err try_set_nonstandard_baudrate (bl_serial* s, uword baudrate)
{
  struct serial_struct ser;
  if (ioctl (s->fd, TIOCGSERIAL, &ser) < 0) {
    return bl_file;
  }
  ser.custom_divisor = ser.baud_base / baudrate;
  ser.flags         &= ~ASYNC_SPD_MASK;
  ser.flags         |= ASYNC_SPD_CUST;

  return ioctl (s->fd, TIOCSSERIAL, &ser) >= 0 ? bl_ok : bl_invalid;  
} 
/*----------------------------------------------------------------------------*/
#else
static inline bl_err try_set_nonstandard_baudrate (bl_serial* s, uword baudrate)
{
  return bl_invalid;  
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
bl_err BL_SERIAL_EXPORT bl_serial_start(
  bl_serial* s, bl_serial_cfg const* cfg
  )
{
  bl_assert (s && cfg);
  if (s->fd) {
    return bl_not_allowed;
  }
  if (!cfg->device_name) {
    return bl_invalid;
  }

  /*open*/
try_again:
  s->fd = open (cfg->device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (s->fd < 0) {
    if (errno != EINTR) {
      goto try_again; /*interrupted by a signal: recoverable*/
    }
    return bl_file;
  }
  
  /*fixed attributes*/
  struct termios options;
  bl_err err = tcgetattr (s->fd, &options) >= 0 ? bl_ok : bl_file;
  if (err) {
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
    if (err) {
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
    err = bl_invalid;
    goto close;
  }

  /*stop bits*/
  switch (cfg->stop_bits) {
  case bl_stop_bits_one: {
   options.c_cflag &= (tcflag_t) ~(CSTOPB);
   break;
  }
  case bl_stop_bits_one_point_five:
    err = bl_invalid;
    goto close;
  case bl_stop_bits_two: {
    options.c_cflag |=  (CSTOPB);
    break;
  }
  default:
    err = bl_invalid;
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
    err = bl_invalid;
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
    err = bl_invalid;
    goto close;
  }

  /* non blocking reads*/
  options.c_cc[VMIN]  = 0;
  options.c_cc[VTIME] = 0;

  err = tcsetattr (s->fd, TCSANOW, &options) >= 0 ? bl_ok : bl_invalid;
  if (err) {
    goto close;
  }
  return bl_ok;

close:
  bl_serial_stop (s);
  return err;
 
}
/*----------------------------------------------------------------------------*/
void BL_SERIAL_EXPORT bl_serial_stop (bl_serial* s)
{
  if (s->fd >= 0) {
    close (s->fd);
    s->fd = -1;
  }
}
/*----------------------------------------------------------------------------*/
bl_err BL_SERIAL_EXPORT bl_serial_read(
  bl_serial* s, memr rbuff, toffset timeout_us
  )
{
  bl_assert (s);
  bl_assert (s->fd >= 0);
  bl_assert (timeout_us >= 0);

  if (!memr_is_valid (rbuff) || memr_size (rbuff) > u8_dq_capacity (&s->rq)) {
    return bl_invalid;
  }
  /*leftovers from previous reads*/
  uword copied = bl_min (memr_size (rbuff), u8_dq_size (&s->rq));
  if (copied != 0) { 
    uword adjacent = u8_dq_adjacent_elems_from (&s->rq, 0, copied);
    memcpy (memr_beg (rbuff), u8_dq_at_head (&s->rq), adjacent);
    if (adjacent != copied) {
      memcpy(
        memr_at (rbuff, adjacent),
        u8_dq_at (&s->rq, adjacent),
        copied - adjacent
        );
    }
    u8_dq_drop_head_n (&s->rq, copied);
  }
  if (copied == memr_size (rbuff)) {
    return bl_ok;
  }
  
  tstamp deadline = 0; 
  bl_err err      = bl_ok;
  if (timeout_us != 0) {
    err =  deadline_init (&deadline, timeout_us);
    if (err) {
      goto rollback;
    }
  }
  /*read*/
  while (copied < memr_size (rbuff)) {
    uword rd = read(
      s->fd, memr_at (rbuff, copied), memr_size (rbuff) - copied
      );
    copied += rd;
    if (rd != 0) {
      continue;
    }
    tstampdiff diff = (timeout_us != 0) ?
      deadline_compare (deadline, bl_get_tstamp()) : -1;
      /*context switches from here to the pselect will be suboptimal*/
    if (diff <= 0) {
      err = bl_timeout;
      goto rollback;
    }
    uword us_pending = bl_tstamp_to_usec_ceil ((tstamp) diff);

    fd_set fds;
    FD_ZERO (&fds);
    FD_SET (s->fd, &fds);    

    struct timespec t;
    t.tv_sec  = us_pending / usec_in_sec;
    t.tv_nsec = (us_pending - (t.tv_sec * usec_in_sec)) * nsec_in_usec;

    int r = pselect (s->fd + 1, &fds, nullptr, nullptr, &t, nullptr);
    if (r < 0 && errno != EINTR) {
      err = bl_error;
      goto rollback;
    }
  }  
  return bl_ok;

rollback:
  /*unsucessful read*/
  bl_assert (u8_dq_size (&s->rq) == 0);
  u8_dq_set_start_position (&s->rq, 0);
  u8_dq_expand_tail_n (&s->rq, copied);
  memcpy (u8_dq_at_head (&s->rq), memr_beg (rbuff), copied);
  return err;
}
/*----------------------------------------------------------------------------*/
bl_err BL_SERIAL_EXPORT bl_serial_write(
  bl_serial* s, memr wbuff, u32* written, toffset timeout_us
  )
{
  bl_assert (s);
  bl_assert (written);
  bl_assert (s->fd >= 0);
  bl_assert (timeout_us >= 0);
  
  if (!memr_is_valid (wbuff) || !written) {
    return bl_invalid;
  }

  *written        = 0;
  tstamp deadline = 0; 
  bl_err err      = bl_ok;
  if (timeout_us != 0) {
    err = deadline_init (&deadline, timeout_us);
    if (err) {
      return err;
    }
  }

  uword wr = 0;
  while (wr < memr_size (wbuff)) {
    fd_set fds;
    FD_ZERO (&fds);
    FD_SET (s->fd, &fds);
    
    struct timespec t;
    struct timespec* t_ptr = nullptr;   
    if (timeout_us != 0) {
      tstampdiff diff = deadline_compare (deadline, bl_get_tstamp());
      if (diff <= 0) {
        err = bl_timeout;
        goto end;
      }
      uword us_pending = bl_tstamp_to_usec_ceil ((tstamp) diff);
      t.tv_sec  = us_pending / usec_in_sec;
      t.tv_nsec = (us_pending - (t.tv_sec * usec_in_sec)) * nsec_in_usec;
      t_ptr     = &t;
    }
    if (pselect (s->fd + 1, nullptr, &fds, nullptr, t_ptr, nullptr) > 0) {
      bl_assert (FD_ISSET (s->fd, &fds));
      uword now = write (s->fd, memr_at (wbuff, wr), memr_size (wbuff) - wr);
      bl_assert (now <= memr_size (wbuff) - wr);
      if (unlikely (now == 0)) {
        /*device disconnected on Linux*/
        err = bl_error;
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
bl_err BL_SERIAL_EXPORT bl_serial_ioctl_get(
  bl_serial* s, bl_serial_ioctl op, uword* val
  )
{
  bl_assert (s);
  if (s->fd < 0 || !val) {
    return bl_invalid;
  }
  int mask;
  switch (op) {
  case bl_cts:        mask = TIOCM_CTS; break;
  case bl_rts:        return bl_invalid;
  case bl_dtr:        return bl_invalid;      
  case bl_dsr:        mask = TIOCM_DSR; break;       
  case bl_ri:         mask = TIOCM_RI; break;
  case bl_cd:         mask = TIOCM_CD; break;
  case bl_send_break: return bl_invalid; 
  case bl_set_break:  return bl_invalid;
  default:            return bl_invalid;
  }
  int status;
  if (ioctl (s->fd, TIOCMGET, &status) >= 0) {
    *val = (status & mask) != 0;
    return bl_ok;
  }
  return bl_error;
}
/*----------------------------------------------------------------------------*/
bl_err BL_SERIAL_EXPORT bl_serial_ioctl_set(
  bl_serial* s, bl_serial_ioctl op, uword val
  )
{
  bl_assert (s);
  if (s->fd < 0) {
    return bl_invalid;
  }
  int ioctlv;
  int cmd;
  switch (op) {
  case bl_cts:        return bl_invalid;
  case bl_rts:
    cmd    = TIOCM_RTS;
    ioctlv = (val) ? TIOCMBIS : TIOCMBIC;
    break;
  case bl_dtr:
    cmd    = TIOCM_DTR;
    ioctlv = (val) ? TIOCMBIS : TIOCMBIC;
    break;   
  case bl_dsr:        return bl_invalid;
  case bl_ri:         return bl_invalid;
  case bl_cd:         return bl_invalid;
  case bl_send_break: 
    tcsendbreak (s->fd, 0);
    return bl_ok; 
  case bl_set_break:
    ioctlv = (val) ? TIOCSBRK : TIOCCBRK;
    return ioctl (s->fd, ioctlv) >= 0 ? bl_ok : bl_error;
  default:            return bl_invalid;
  }
  return ioctl (s->fd, ioctlv, &cmd) >= 0 ? bl_ok : bl_error;
}
/*----------------------------------------------------------------------------*/
uword BL_SERIAL_EXPORT bl_serial_get_bit_time_ns (bl_serial_cfg const* cfg)
{
  bl_assert (cfg);
  if (!cfg) {
    return 0;
  }
  return (uword) fixp_to_int(
      int_to_fixp (u64, nsec_in_sec, 32) / cfg->baudrate, 32
      );
}
/*----------------------------------------------------------------------------*/
uword BL_SERIAL_EXPORT bl_serial_get_byte_time_ns (bl_serial_cfg const* cfg)
{
  bl_assert (cfg);
  if (!cfg) {
    return 0;
  }
  u64 bit_ns    = int_to_fixp (u64, nsec_in_sec, 32) / cfg->baudrate;
  u64 bits_byte = 1 + 
    cfg->byte_size + 
    (cfg->parity != bl_parity_none) + 
    (cfg->stop_bits == bl_stop_bits_one) ? 1 : 2;
  return (uword) fixp_to_int (bit_ns * bits_byte, 32);
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* #ifdef BL_POSIX */
