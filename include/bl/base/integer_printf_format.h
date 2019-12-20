#ifndef __BL_PRINTF_INT_H__
#define __BL_PRINTF_INT_H__

/*NOTE: this wheel was initially reinvented when not knowing about the format
  specifiers (e.g. PRIu64) in C99 <inttypes.h> and then proved useful for some
  extra cases.

This still has the advantage that it provides the BL_FMT_Lxx macros, so
literal concatenations can be used (were needed when developing malc). */

#include <bl/base/platform.h>
#include <bl/base/integer.h>

#define BL_FMT_L8           "hh"
#define BL_FMT_U8 BL_FMT_L8 "u"
#define BL_FMT_I8 BL_FMT_L8 "d"
#define BL_FMT_X8 BL_FMT_L8 "x"
#define BL_FMT_O8 BL_FMT_L8 "o"

#define BL_FMT_L16            "h"
#define BL_FMT_U16 BL_FMT_L16 "u"
#define BL_FMT_I16 BL_FMT_L16 "d"
#define BL_FMT_X16 BL_FMT_L16 "x"
#define BL_FMT_O16 BL_FMT_L16 "o"

/*BL_FMT_x32 may require fixing for 16-bit compilers, good enough for now*/
#define BL_FMT_L32         ""
#define BL_FMT_U32 BL_FMT_L32 "u"
#define BL_FMT_I32 BL_FMT_L32 "d"
#define BL_FMT_X32 BL_FMT_L32 "x"
#define BL_FMT_O32 BL_FMT_L32 "o"

#if BL_COMPILER_IS (GCC) && (BL_WORDSIZE == 64)
  #define BL_FMT_L64 "l"
#else
  /*FMTx64 may require fixing for other compilers depending on theirs stdint.h,
    tweak as needed*/
  #define BL_FMT_L64 "ll"
#endif

#define BL_FMT_U64 BL_FMT_L64 "u"
#define BL_FMT_I64 BL_FMT_L64 "d"
#define BL_FMT_X64 BL_FMT_L64 "x"
#define BL_FMT_O64 BL_FMT_L64 "o"

#if BL_WORDSIZE == 16
  #define BL_FMT_LWORD    BL_FMT_L16
  #define BL_FMT_LWORD_D2 BL_FMT_L8
#elif BL_WORDSIZE == 32
  #define BL_FMT_LWORD    BL_FMT_L32
  #define BL_FMT_LWORD_D2 BL_FMT_L16
  #define BL_FMT_LWORD_D4 BL_FMT_L8
#elif BL_WORDSIZE == 64
  #define BL_FMT_LWORD    BL_FMT_L64
  #define BL_FMT_LWORD_D2 BL_FMT_L32
  #define BL_FMT_LWORD_D4 BL_FMT_L16
#else
  #error "Implement this"
#endif

#if BL_WORDSIZE_MAX >= 64
  #define BL_FMT_LBIG    BL_FMT_L64
  #define BL_FMT_LBIG_D2 BL_FMT_L32
  #define BL_FMT_LBIG_D4 BL_FMT_L16
#elif BL_WORDSIZE_MAX >= 32
  #define BL_FMT_LBIG    BL_FMT_L32
  #define BL_FMT_LBIG_D2 BL_FMT_L16
  #define BL_FMT_LBIG_D4 BL_FMT_L8
#elif BL_WORDSIZE_MAX >= 16
  #define BL_FMT_LBIG    BL_FMT_L16
  #define BL_FMT_LBIG_D2 BL_FMT_L8
#else
  #define BL_FMT_LBIG    BL_FMT_L8
#endif

#define BL_FMT_UWORD BL_FMT_LWORD "u"
#define BL_FMT_WORD  BL_FMT_LWORD "d"
#define BL_FMT_XWORD BL_FMT_LWORD "x"
#define BL_FMT_OWORD BL_FMT_LWORD "o"

#if BL_HAS_WORD_D4
  #define BL_FMT_UWORD_D4 BL_FMT_LWORD_D4 "u"
  #define BL_FMT_WORD_D4  BL_FMT_LWORD_D4 "d"
  #define BL_FMT_XWORD_D4 BL_FMT_LWORD_D4 "x"
  #define BL_FMT_OWORD_D4 BL_FMT_LWORD_D4 "o"
#endif

#if BL_HAS_WORD_D2
  #define BL_FMT_UWORD_D2 BL_FMT_LWORD_D2 "u"
  #define BL_FMT_WORD_D2  BL_FMT_LWORD_D2 "d"
  #define BL_FMT_XWORD_D2 BL_FMT_LWORD_D2 "x"
  #define BL_FMT_OWORD_D2 BL_FMT_LWORD_D2 "o"
#endif

#if BL_HAS_BIG_D4
  #define BL_FMT_UBIG_D4 BL_FMT_LBIG_D4 "u"
  #define BL_FMT_BIG_D4  BL_FMT_LBIG_D4 "d"
  #define BL_FMT_XBIG_D4 BL_FMT_LBIG_D4 "x"
  #define BL_FMT_OBIG_D4 BL_FMT_LBIG_D4 "o"
#endif

#if BL_HAS_BIG_D2
  #define BL_FMT_UBIG_D2 BL_FMT_LBIG_D2 "u"
  #define BL_FMT_BIG_D2  BL_FMT_LBIG_D2 "d"
  #define BL_FMT_XBIG_D2 BL_FMT_LBIG_D2 "x"
  #define BL_FMT_OBIG_D2 BL_FMT_LBIG_D2 "o"
#endif

/* shorter aliases */
#define BL_FMT_LW BL_FMT_LWORD
#define BL_FMT_W  BL_FMT_WORD
#define BL_FMT_UW BL_FMT_UWORD
#define BL_FMT_XW BL_FMT_XWORD
#define BL_FMT_OW BL_FMT_OWORD

#if BL_HAS_WORD_D4
  #define BL_FMT_LWD4 BL_FMT_LWORD_D4
  #define BL_FMT_WD4  BL_FMT_WORD_D4
  #define BL_FMT_UWD4 BL_FMT_WORD_D4
  #define BL_FMT_XWD4 BL_FMT_XWORD_D4
  #define BL_FMT_OWD4 BL_FMT_OWORD_D4
#endif

#if BL_HAS_WORD_D2
  #define BL_FMT_LWD2 BL_FMT_LWORD_D2
  #define BL_FMT_WD2  BL_FMT_WORD_D2
  #define BL_FMT_UWD2 BL_FMT_WORD_D2
  #define BL_FMT_XWD2 BL_FMT_XWORD_D2
  #define BL_FMT_OWD2 BL_FMT_OWORD_D2
#endif

#if BL_HAS_BIG_D4
  #define BL_FMT_LBD4 BL_FMT_LBIG
  #define BL_FMT_BD4  BL_FMT_BIG_D4
  #define BL_FMT_UBD4 BL_FMT_UBIG_D4
  #define BL_FMT_XBD4 BL_FMT_XBIG_D4
  #define BL_FMT_OBD4 BL_FMT_OBIG_D4
#endif

#if BL_HAS_BIG_D2
  #define BL_FMT_LBD2 BL_FMT_LBIG
  #define BL_FMT_BD2  BL_FMT_BIG_D2
  #define BL_FMT_UBD2 BL_FMT_UBIG_D2
  #define BL_FMT_XBD2 BL_FMT_XBIG_D2
  #define BL_FMT_OBD2 BL_FMT_OBIG_D2
#endif

#ifdef BL_UNPREFIXED_PRINTF_FORMATS

#define FMT_L8 BL_FMT_L8
#define FMT_U8 BL_FMT_U8
#define FMT_I8 BL_FMT_I8
#define FMT_X8 BL_FMT_X8
#define FMT_O8 BL_FMT_O8

#define FMT_L16 BL_FMT_L16
#define FMT_U16 BL_FMT_U16
#define FMT_I16 BL_FMT_I16
#define FMT_X16 BL_FMT_X16
#define FMT_O16 BL_FMT_O16

#define FMT_L32 BL_FMT_L32
#define FMT_U32 BL_FMT_U32
#define FMT_I32 BL_FMT_I32
#define FMT_X32 BL_FMT_X32
#define FMT_O32 BL_FMT_O32

#define FMT_L64 BL_FMT_L64
#define FMT_U64 BL_FMT_U64
#define FMT_I64 BL_FMT_I64
#define FMT_X64 BL_FMT_X64
#define FMT_O64 BL_FMT_O64

#define FMT_LWORD    BL_FMT_LWORD
#define FMT_LWORD_D2 BL_FMT_LWORD_D2
#define FMT_LWORD_D2 BL_FMT_LWORD_D2
#define FMT_LWORD_D4 BL_FMT_LWORD_D4

#define FMT_UWORD BL_FMT_UWORD
#define FMT_WORD  BL_FMT_WORD
#define FMT_XWORD BL_FMT_XWORD
#define FMT_OWORD BL_FMT_OWORD

#define FMT_UWORD_D4 BL_FMT_UWORD_D4
#define FMT_WORD_D4  BL_FMT_WORD_D4
#define FMT_XWORD_D4 BL_FMT_XWORD_D4
#define FMT_OWORD_D4 BL_FMT_OWORD_D4

#define FMT_UWORD_D2 BL_FMT_UWORD_D2
#define FMT_WORD_D2  BL_FMT_WORD_D2
#define FMT_XWORD_D2 BL_FMT_XWORD_D2
#define FMT_OWORD_D2 BL_FMT_OWORD_D2

#define FMT_LBIG    BL_FMT_LBIG
#define FMT_LBIG_D2 BL_FMT_LBIG_D2
#define FMT_LBIG_D4 BL_FMT_LBIG_D4

#define FMT_UBIG_D4 BL_FMT_UBIG_D4
#define FMT_BIG_D4  BL_FMT_BIG_D4
#define FMT_XBIG_D4 BL_FMT_XBIG_D4
#define FMT_OBIG_D4 BL_FMT_OBIG_D4

/* shorter aliases */
#define FMT_LW BL_FMT_LW
#define FMT_W  BL_FMT_W
#define FMT_UW BL_FMT_UW
#define FMT_XW BL_FMT_XW
#define FMT_OW BL_FMT_OW

#define FMT_LWD2 BL_FMT_LWD2
#define FMT_WD2  BL_FMT_WD2
#define FMT_UWD2 BL_FMT_UWD2
#define FMT_XWD2 BL_FMT_XWD2
#define FMT_OWD2 BL_FMT_OWD2

#define FMT_LWD4 BL_FMT_LWD4
#define FMT_WD4  BL_FMT_WD4
#define FMT_UWD4 BL_FMT_UWD4
#define FMT_XWD4 BL_FMT_XWD4
#define FMT_OWD4 BL_FMT_OWD4

#define FMT_LBD2 BL_FMT_LBD2
#define FMT_BD2  BL_FMT_BD2
#define FMT_UBD2 BL_FMT_UBD2
#define FMT_XBD2 BL_FMT_XBD2
#define FMT_OBD2 BL_FMT_OBD2

#define FMT_LBD4 BL_FMT_LBD4
#define FMT_BD4  BL_FMT_BD4
#define FMT_UBD4 BL_FMT_UBD4
#define FMT_XBD4 BL_FMT_XBD4
#define FMT_OBD4 BL_FMT_OBD4

#endif /* #ifdef BL_UNPREFIXED_PRINTF_FORMATS */

#endif /*__BL_PRINTF_INT_H__*/