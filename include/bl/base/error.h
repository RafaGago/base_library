#ifndef BL_ERROR_H
#define BL_ERROR_H

#include <bl/base/platform.h>
#include <bl/base/integer.h>

#if BL_HAS_ERRNO_H

#include <errno.h>
/*---------------------------------------------------------------------------*/
typedef enum bl_err_e {
  bl_ok = 0,
  /*generic error.
  Hack, it has been given a very unlikely error value, but it still doesn't make
  sense from a POSIX perspective*/
  bl_error = ELIBEXEC,
  /*it might be done in the future but not now (EAGAIN)*/
  bl_busy = EAGAIN,
  bl_empty = ENODATA,
  bl_timeout = ETIME,
  /*resource is locked.*/
  bl_locked = ENOLCK,
  /*file related error (open, close or IO)*/
  bl_file = EACCES,
    /* bl_not_allowed, DEPRECATED: use bl_precondtions*/
  /*the preconditions to run an operation weren't met. typically returned when a
   function specifies that will backoff under certain conditions*/
  bl_preconditions = EPERM,
  /*invalid input parameters (EINVAL)*/
  bl_invalid = EINVAL,
  /*the operation is unsupported or to be implemented*/
  bl_unsupported = ENOSYS,
  /*full. some data structure or data type is at its maximum capacity. */
  bl_would_overflow = EOVERFLOW,
  /*called function couldn't make forward progress*/
  bl_nothing_to_do = ENOMSG,
  /*unexpected input from a source that isn't controlled by the caller (
    socket, protocol, IPC to another process, etc...*/
  bl_unexpected = EBADE,
  /*operation cancelled externally or by shutdown (ECANCELED)*/
  bl_cancelled = ECANCELED,
  /*operation interrupted e.g. by a signal (spuriosly woken up)*/
  bl_interrupted = EINTR,
  bl_alloc = ENOMEM,
  bl_range = ERANGE,
  bl_err_for_free_use_first = 256,
}
bl_err_e;

#else
/*---------------------------------------------------------------------------*/
typedef enum bl_err_e {
  bl_ok = 0,
  bl_error,
  bl_busy,
  bl_empty,
  bl_timeout,
  bl_locked,
  bl_file,
  /* bl_not_allowed, DEPRECATED: use bl_precondtions*/
  bl_preconditions,
  bl_invalid,
  bl_unsupported,
  bl_would_overflow,
  bl_nothing_to_do,
  bl_unexpected,
  bl_cancelled,
  bl_interrupted,
  bl_alloc,
  bl_range,
  bl_err_for_free_use_first = 256
}
bl_err_e;
#endif
/*---------------------------------------------------------------------------*/
typedef uword bl_err;
/*the FMT_ERR printf format macro requires including "integer_printf_format.h"
  for it to be correctly expanded*/
#define FMT_ERR FMT_UWORD
/*---------------------------------------------------------------------------*/
#define declare_bl_err_to_str()\
char const* bl_err_to_str (bl_err e)\
/*---------------------------------------------------------------------------*/
#define define_bl_err_to_str()\
char const* bl_err_to_str (bl_err e)\
{\
  switch (e) {\
    case bl_ok             : return "ok";\
    case bl_error          : return "generic error";\
    case bl_busy           : return "busy";\
    case bl_empty          : return "empty";\
    case bl_timeout        : return "timeout";\
    case bl_locked         : return "locked";\
    case bl_file           : return "file";\
    case bl_preconditions  : return "preconditions";\
    case bl_invalid        : return "invalid";\
    case bl_unsupported    : return "unsupported";\
    case bl_would_overflow : return "would_overflow";\
    case bl_nothing_to_do  : return "nothing_to_do";\
    case bl_unexpected     : return "unexpected";\
    case bl_cancelled      : return "cancelled";\
    case bl_interrupted    : return "interrupted";\
    case bl_alloc          : return "allocation";\
    case bl_range          : return "range";\
    default                : return "unknown error";\
  }\
}
/*---------------------------------------------------------------------------*/
#endif /*BL_ERROR_H*/
