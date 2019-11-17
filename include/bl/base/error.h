#ifndef BL_ERROR_H
#define BL_ERROR_H

#include <bl/base/platform.h>
#include <bl/base/libexport.h>
#include <bl/base/integer.h>

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE >= 32
/*the BL_FMT_ERR printf format macro requires including "integer_printf_format.h"
  for it to be correctly expanded*/
  typedef bl_uword_d2 bl_err_uint;
  #define BL_FMT_ERR BL_FMT_UWORD_D2
#else
  typedef bl_uword bl_err_uint;
  #define BL_FMT_ERR BL_FMT_UWORD
#endif
#ifdef BL_UNPREFIXED_PRINTF_FORMATS
  #define FMT_ERR BL_FMT_ERR
#endif
/*---------------------------------------------------------------------------*/
typedef enum bl_err_e {
  bl_ok,
  /*generic error.*/
  bl_error,
  /*it might be done in the future but not now (EAGAIN)*/
  bl_busy,
  bl_empty,
  bl_timeout,
  /*resource is locked.*/
  bl_locked,
  /*file related error (open, close or IO), the platform specific error is left
  on the errno variable */
  bl_file,
  /*the preconditions to run an operation weren't met. typically returned when a
   function specifies that will backoff under certain conditions*/
  bl_preconditions,
  /*invalid input parameters (EINVAL)*/
  bl_invalid,
  /*the operation is unsupported or to be implemented*/
  bl_unsupported,
  /*full. some data structure or data type is at its maximum capacity. */
  bl_would_overflow,
  /*called function couldn't make forward progress*/
  bl_nothing_to_do,
  /*unexpected input from a source that isn't controlled by the caller (
    socket, protocol, IPC to another process, etc...*/
  bl_unexpected,
  /*operation cancelled externally or by shutdown (ECANCELED)*/
  bl_cancelled,
  /*operation interrupted e.g. by a signal (spuriosly woken up)*/
  bl_interrupted,
  bl_alloc,
  bl_range,
  bl_err_for_free_use_first =  256,
}
bl_err_e;
/*---------------------------------------------------------------------------*/
struct bl_err {
  bl_err_uint own; /* own error code (on enum "bl_err_e"), it doesn't change
                   between platforms*/
  bl_err_uint sys; /* additional syscall "errno" value in case it was a syscall
                   causing the error. Only valid if own != bl_ok. This error
                   code may change between platforms. It is moslty used by
                   bl_strerror internally. */
};
typedef struct bl_err bl_err;
/*---------------------------------------------------------------------------*/
static inline bl_err bl_mkerr_sys (bl_uword own, bl_uword sys)
{
  bl_err e;
  e.own  = (bl_err_uint) own;
  e.sys = (bl_err_uint) sys;
  return e;
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_mkerr (bl_uword own)
{
  return bl_mkerr_sys (own, 0);
}
/*---------------------------------------------------------------------------*/
static inline bl_err bl_mkok (void)
{
  return bl_mkerr_sys (bl_ok, 0);
}
/*---------------------------------------------------------------------------*/
extern BL_EXPORT char const* bl_strerror (bl_err e);
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} //extern "C" {
#endif

#endif /*BL_ERROR_H*/
