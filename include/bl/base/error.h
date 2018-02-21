#ifndef BL_ERROR_H
#define BL_ERROR_H

#include <bl/base/platform.h>
#include <bl/base/libexport.h>
#include <bl/base/integer.h>

/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE >= 32
/*the FMT_ERR printf format macro requires including "integer_printf_format.h"
  for it to be correctly expanded*/
  typedef uword_d2 bl_err_uint;
  #define FMT_ERR FMT_UWORD_D2
#else
  typedef uword bl_err_uint;
  #define FMT_ERR FMT_UWORD
#endif
/*---------------------------------------------------------------------------*/
#if BL_POSIX || BL_WINDOWS
  #include <errno.h>
  #define BL_ENUMVAL(bl, sys) bl = sys
#else
  #define BL_ENUMVAL(bl, sys) bl
#endif
/*---------------------------------------------------------------------------*/
typedef enum bl_err_e {
  BL_ENUMVAL (bl_ok, 0),
  /*generic error.
  Hack, it has been given a very unlikely error value, but it still doesn't make
  sense from a POSIX perspective*/
  BL_ENUMVAL (bl_error, ELIBEXEC),
  /*it might be done in the future but not now (EAGAIN)*/
  BL_ENUMVAL (bl_busy, EAGAIN),
  BL_ENUMVAL (bl_empty, ENODATA),
  BL_ENUMVAL (bl_timeout, ETIME),
  /*resource is locked.*/
  BL_ENUMVAL (bl_locked, ENOLCK),
  /*file related error (open, close or IO), the platform specific error is left
  on the errno variable */
  BL_ENUMVAL (bl_file, EEXIST),
  /*the preconditions to run an operation weren't met. typically returned when a
   function specifies that will backoff under certain conditions*/
  BL_ENUMVAL (bl_preconditions, EPERM),
  /*invalid input parameters (EINVAL)*/
  BL_ENUMVAL (bl_invalid, EINVAL),
  /*the operation is unsupported or to be implemented*/
  BL_ENUMVAL (bl_unsupported, ENOSYS),
  /*full. some data structure or data type is at its maximum capacity. */
  BL_ENUMVAL (bl_would_overflow, EOVERFLOW),
  /*called function couldn't make forward progress*/
  BL_ENUMVAL (bl_nothing_to_do, ENOMSG),
  /*unexpected input from a source that isn't controlled by the caller (
    socket, protocol, IPC to another process, etc...*/
  BL_ENUMVAL (bl_unexpected, EBADE),
  /*operation cancelled externally or by shutdown (ECANCELED)*/
  BL_ENUMVAL (bl_cancelled, ECANCELED),
  /*operation interrupted e.g. by a signal (spuriosly woken up)*/
  BL_ENUMVAL (bl_interrupted, EINTR),
  BL_ENUMVAL (bl_alloc, ENOMEM),
  BL_ENUMVAL (bl_range, ERANGE),
  BL_ENUMVAL (bl_err_for_free_use_first, 256),
}
bl_err_e;
/*---------------------------------------------------------------------------*/
struct bl_err {
  bl_err_uint bl;  /*bl error code on enum "bl_err_e" */
  bl_err_uint sys; /*additional syscall "errno" value, valid if (.bl != bl_ok)*/
};
typedef struct bl_err bl_err;
/*---------------------------------------------------------------------------*/
static inline bl_err bl_mkerr_sys (uword bl, uword sys)
{
  bl_err e;
  e.bl  = (bl_err_uint) bl;
  e.sys = (bl_err_uint) sys;
  return e;
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_mkerr (uword bl)
{
  return bl_mkerr_sys (bl, 0);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_mkok (void)
{
  return bl_mkerr_sys (bl_ok, 0);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT char const* bl_strerror (bl_err e);
/*---------------------------------------------------------------------------*/
#endif /*BL_ERROR_H*/
