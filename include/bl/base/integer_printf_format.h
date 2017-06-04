#ifndef __BL_PRINTF_INT_H__
#define __BL_PRINTF_INT_H__

/*NOTE: this wheel was initially reinvented when not knowing about the format
  specifiers (e.g. PRIu64) in C99 <inttypes.h> and then proved usefull for some
  extra cases.

This still has the advantage that it provides the FMT_Lxx macros, so
literal concatenations can be used (were needed when developing malc). */

#include <bl/base/platform.h>

#define FMT_L8        "hh"
#define FMT_U8 FMT_L8 "u"
#define FMT_I8 FMT_L8 "d"
#define FMT_X8 FMT_L8 "x"
#define FMT_O8 FMT_L8 "o"

#define FMT_L16         "h"
#define FMT_U16 FMT_L16 "u"
#define FMT_I16 FMT_L16 "d"
#define FMT_X16 FMT_L16 "x"
#define FMT_O16 FMT_L16 "o"

/*FMT_x32 may require fixing for 16-bit compilers, good enough for now*/
#define FMT_L32         ""
#define FMT_U32 FMT_L32 "u"
#define FMT_I32 FMT_L32 "d"
#define FMT_X32 FMT_L32 "x"
#define FMT_O32 FMT_L32 "o"

#if defined (BL_GCC) && (BL_WORDSIZE == 64)
  #define FMT_L64 "l"
#else
  /*FMTx64 may require fixing for other compilers depending on theirs stdint.h,
    tweak as needed*/
  #define FMT_L64 "ll"
#endif

#define FMT_U64 FMT_L64 "u"
#define FMT_I64 FMT_L64 "d"
#define FMT_X64 FMT_L64 "x"
#define FMT_O64 FMT_L64 "o"

#if BL_WORDSIZE == 16
  #define FMT_LWORD FMT_L16
#elif BL_WORDSIZE == 32
  #define FMT_LWORD FMT_L32
#elif BL_WORDSIZE == 64
  #define FMT_LWORD FMT_L64
#else
  #error "Implement this"
#endif

#define FMT_UWORD FMT_LWORD "u"
#define FMT_WORD  FMT_LWORD "d"
#define FMT_XWORD FMT_LWORD "x"
#define FMT_OWORD FMT_LWORD "o"

#define FMT_LW FMT_LWORD
#define FMT_W  FMT_WORD
#define FMT_UW FMT_UWORD
#define FMT_XW FMT_XWORD
#define FMT_OW FMT_OWORD

#endif /*__BL_PRINTF_INT_H__*/