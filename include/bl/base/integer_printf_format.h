#ifndef __BL_PRINTF_INT_H__
#define __BL_PRINTF_INT_H__

/*NOTE: this wheel was reinvented when not knowing about the format specifiers
(e.g. PRIu64) in C99 <inttypes.h>, I kept it to avoid refactoring,. It is
still missing some less used definitions.*/

#include <bl/base/platform.h>

#define FMT_U8 "hhu"
#define FMT_I8 "hh"
#define FMT_X8 "hhx"
#define FMT_O8 "hho"

#define FMT_U16 "hu"
#define FMT_I16 "h"
#define FMT_X16 "hx"
#define FMT_O16 "ho"

/*FMT_x32 may require fixing for 16-bit compilers, good enough for now*/
#define FMT_U32 "u"
#define FMT_I32 "d"
#define FMT_X32 "x"
#define FMT_O32 "o"

#if defined (BL_GCC) && (BL_WORDSIZE == 64)
  #define FMT_U64 "lu"
  #define FMT_I64 "l"
  #define FMT_X64 "lx"
  #define FMT_O64 "lo"
#else
  /*FMTx64 may require fixing for other compilers depending on theirs stdint.h,
    tweak as needed*/
  #define FMT_U64 "llu"
  #define FMT_I64 "ll"
  #define FMT_X64 "llx"
  #define FMT_O64 "llo"
#endif

#if BL_WORDSIZE == 16
  #define FMT_UWORD FMT_U16
  #define FMT_WORD  FMT_I16
  #define FMT_XWORD FMT_X16
  #define FMT_OWORD FMT_O16
#elif BL_WORDSIZE == 32
  #define FMT_UWORD FMT_U32
  #define FMT_WORD  FMT_I32
  #define FMT_XWORD FMT_X32
  #define FMT_OWORD FMT_O32
#elif BL_WORDSIZE == 64
  #define FMT_UWORD FMT_U64
  #define FMT_WORD  FMT_I64
  #define FMT_XWORD FMT_X64
  #define FMT_OWORD FMT_O64
#else
  #error "Implement this"
#endif

#define FMT_W  FMT_WORD
#define FMT_UW FMT_UWORD
#define FMT_XW FMT_XWORD
#define FMT_OW FMT_OWORD

#endif /*__BL_PRINTF_INT_H__*/