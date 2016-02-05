#ifndef BL_ERROR_H
#define BL_ERROR_H

#include <bl/hdr/base/integer.h>

/* TODO ifdef for each platform and try to assing OS values (e.g. errno on
   unix derivatives) */
/*---------------------------------------------------------------------------*/
typedef enum bl_err_e {
  bl_ok = 0,
  /*generic error*/
  bl_error,
  /*it might be done in the future but not now (EBUSY)*/
  bl_busy,
  bl_empty,  
  /* (ETIMEDOUT) */
  bl_timeout,
  /*resource is locked*/
  bl_locked, 
  /*the operation is not allowed given the current internal state*/
  bl_not_allowed,
  /*the function is unsupported or implemented*/
  bl_not_supported,
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
  /*operation cancelled externally or by shutdown (ECANCEL)*/
  bl_cancelled,
  /*operation interrupted e.g. by a signal (spuriosly woken up) (EINTR)*/
  bl_interrupted, 
  bl_alloc,
  bl_err_for_free_use_first = 256
}
bl_err_e;
/*---------------------------------------------------------------------------*/
typedef uword bl_err;
/*---------------------------------------------------------------------------*/

#endif /*BL_ERROR_H*/
