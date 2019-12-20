#include <bl/getopt/getopt.h>

#include <stddef.h>
#include <string.h>

/*----------------------------------------------------------------------------*/
/*Implementation based on
  https://github.com/kimgr/getopt_port/blob/master/getopt.c

  Replaced multiple nested conditional levels by gotos. */
/*----------------------------------------------------------------------------*/
BL_GETOPT_EXPORT int bl_getopt(
  bl_getopt_state* s,
  int              argc,
  char* const      argv[],
  char const*      optstring
  )
{
  int         optchar = -1;
  const char* optdecl = nullptr;

  s->optarg = nullptr;
  s->optopt = 0;

  if (!argv || !optstring || s->optind >= argc) {
    goto no_more_optchars;
  }
  char* argptr = argv[s->optind];
  if (!argptr || argptr[0] != '-' || argptr[1] == '\0') {
    /*-1 without changing optind*/
    goto no_more_optchars;
  }
  if (argptr[1] == '-' && argptr[2] == '\0') {
    /*-1 incrementing optind*/
    ++s->optind;
    goto no_more_optchars;
  }
  if (s->argptr == nullptr || *s->argptr == '\0') {
    s->argptr = argptr + 1;
  }
  optchar   = *s->argptr;
  s->optopt = *s->argptr;
  optdecl   = strchr (optstring, optchar);
  if (!optdecl) {
    optchar = '?';
    goto exit_preamble;
  }
  if (optdecl[1] == ':') {
    s->optarg = ++s->argptr;
    s->argptr = nullptr;
    if (*s->optarg != '\0') {
      /*argument found without space in-between*/
      goto exit_preamble;
    }
    /*no argument found, check if it was optional*/
    if (optdecl[2] == ':') {
      s->optarg = nullptr;
      goto exit_preamble;
    }
    /*if it wasn't optional the argument may be space separated, getting next
      argument */
    if (++s->optind < argc) {
      s->optarg = argv[s->optind];
    }
    else {
      s->optarg = nullptr;
      optchar   = (optstring[0] == ':') ? ':' : '?';
    }
  }
exit_preamble:
  if (s->argptr == nullptr) {
    ++s->optind;
    return optchar;
  }
  ++s->argptr;
  if (*s->argptr == '\0') {
    ++s->optind;
  }
  return optchar;

no_more_optchars:
  s->argptr = nullptr;
  return -1;
}
/*----------------------------------------------------------------------------*/
static inline int long_arg_len (char const* arg)
{
  int len = 0;
  for (; arg[len] != '\0' && arg[len] != '='; ++len) {}
  return len;
}
/*----------------------------------------------------------------------------*/
BL_GETOPT_EXPORT int bl_getopt_long(
  bl_getopt_state*     s,
  int                  argc,
  char* const          argv[],
  char const*          optstring,
  const bl_arg_option* longopts,
  int*                 longindex
  )
{
  bl_arg_option const* o       = longopts;
  bl_arg_option const* match   = nullptr;
  int                  matches = 0;
  int                  arglen  = 0;
  char const*          argptr  = nullptr;
  int                  retval  = -1;

  s->optarg = nullptr;
  s->optopt = 0;

  if (s->optind >= argc || !argv) {
    return -1;
  }
  argptr = argv[s->optind];
  arglen = long_arg_len (argptr);
  if (arglen < 3 || argptr[0] != '-' || argptr[1] != '-') {
    return bl_getopt (s, argc, argv, optstring);
  }
  /* It's an option; starts with -- and is longer than two chars. */
  arglen -= 2;
  argptr += 2;
  if (o) {
    for (; o->name; ++o) {
      if (strncmp (o->name, argptr, arglen) == 0) {
        match = o;
        ++matches;
      }
    }
  }
  if (matches != 1) {
    retval = '?';
    goto exit;
  }
  if (longindex) {
    *longindex = (match - longopts);
  }

  if (match->flag) {
    *(match->flag) = match->val;
    retval         = 0;
  }
  else {
    retval = match->val;
  }

  char* value_separator_ptr = (char*) strchr (argptr, '=');
  if (match->has_arg == bl_no_argument) {
    retval = value_separator_ptr ? '?' : retval;
    goto exit;
  }
  /*arguments processing*/
  s->optarg = value_separator_ptr;
  if (s->optarg != nullptr) {
    ++s->optarg;
  }
  if (match->has_arg == bl_required_argument) {
    s->optind += (s->optarg == nullptr);
    if (s->optarg == nullptr && s->optind < argc) {
      s->optarg = argv[s->optind];
    }
    if (s->optarg == nullptr) {
      retval = ':';
    }
  }
exit:
  ++s->optind;
  return retval;
}
/*----------------------------------------------------------------------------*/
