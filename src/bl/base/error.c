#include <bl/base/error.h>
#include <string.h> /*strerror*/

/*----------------------------------------------------------------------------*/
BL_EXPORT char const* bl_strerror (bl_err e)
{
  if (e.own == bl_ok) {
    return "ok";
  }
  if (e.sys) {
    return strerror (e.sys);
  }
  switch (e.own) {
    case bl_error          : return "generic error";
    case bl_busy           : return "busy";
    case bl_empty          : return "empty";
    case bl_timeout        : return "timed out";
    case bl_locked         : return "resource locked";
    case bl_file           : return "file related error";
    case bl_preconditions  : return "invalid preconditions";
    case bl_invalid        : return "invalid input parameters";
    case bl_unsupported    : return "unsupported/non implemented operation";
    case bl_would_overflow : return "data structure/queue would overflow";
    case bl_nothing_to_do  : return "success, but with no forward progress";
    case bl_unexpected     : return "unexpected state/event";
    case bl_cancelled      : return "operation cancelled";
    case bl_interrupted    : return "(sys)call interrupted";
    case bl_alloc          : return "allocation error";
    case bl_range          : return "value out of range";
    default                : return "unknown error";
  }
}
/*----------------------------------------------------------------------------*/
