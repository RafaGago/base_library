#ifndef BL_CMOCKA_PRE_H
#define BL_CMOCKA_PRE_H

#include <bl/hdr/base/platform.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#if !defined (inline)
# define BL_DIDNT_HAVE_INLINE_MACRO 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <cmocka.h>

#ifdef __cplusplus
} /*extern "C" {*/
#endif

#if defined (BL_MS_COMPILER) && \
    defined (__cplusplus) &&\
    defined (inline) && \
    defined (BL_DIDNT_HAVE_INLINE_MACRO)
/*cmocka tries to define the inline keyword as a macro, AFAIK "inline" is a 
  keyword on C99. I'm not sure what they are trying to workaround.*/
# undef inline
# undef BL_DIDNT_HAVE_INLINE_MACRO
#endif

typedef struct CMUnitTest CMUnitTest;

#endif
