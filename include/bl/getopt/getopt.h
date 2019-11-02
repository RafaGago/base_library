#ifndef __BL_GETOPT_NOGLOBAL__
#define __BL_GETOPT_NOGLOBAL__

#include <bl/getopt/libexport.h>
#include <bl/base/platform.h>

/* A no-globals getopt implementation based on
https://github.com/kimgr/getopt_port */

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
#define bl_no_argument       0
#define bl_required_argument 1
#define bl_optional_argument 2
/*----------------------------------------------------------------------------*/
typedef struct bl_getopt_state {
  char* optarg;
  char* argptr;
  int   optind;
  int   optopt;
}
bl_getopt_state;
/*----------------------------------------------------------------------------*/
typedef struct bl_arg_option {
  const char* name;
  int         has_arg;
  int*        flag;
  int         val;
}
bl_arg_option;
/*----------------------------------------------------------------------------*/
static inline void bl_getopt_state_init (bl_getopt_state* s)
{
  s->argptr = nullptr;
  s->optarg = nullptr;
  s->optind = 1;
  s->optopt = 0;
}
/*----------------------------------------------------------------------------*/
#define BL_GETOPT_STATE_INITIALIZER { nullptr, nullptr, 1, 0 }
/*----------------------------------------------------------------------------*/
static inline const char* bl_getopt_optarg (bl_getopt_state* s)
{
  return s->optarg;
}
/*----------------------------------------------------------------------------*/
static inline int bl_getopt_optind (bl_getopt_state* s)
{
  return s->optind;
}
/*----------------------------------------------------------------------------*/
static inline int bl_getopt_optopt (bl_getopt_state* s)
{
  return s->optopt;
}
/*----------------------------------------------------------------------------*/
BL_GETOPT_EXPORT int bl_getopt(
  bl_getopt_state* s,
  int              argc,
  char* const      argv[],
  char const*      optstring
  );
/*----------------------------------------------------------------------------*/
BL_GETOPT_EXPORT int bl_getopt_long(
  bl_getopt_state*     s,
  int                  argc,
  char* const          argv[],
  char const*          optstring,
  const bl_arg_option* longopts,
  int*                 longindex
  );
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif
