#ifndef __BL_PRINTF_INT_H__
#define __BL_PRINTF_INT_H__

#include <bl/base/platform.h>

#define FMT_U8 "hhu"
#define FMT_I8 "hh"
#define FMT_U16 "h"
#define FMT_I16 "hu"
/*FMT_x32 may require fixing for 16-bit compilers, good enough for now*/
#define FMT_U32 "u"
#define FMT_I32 "d"

#if defined (BL_GCC) && (BL_WORDSIZE == 64)
  #define FMT_U64 "lu"
  #define FMT_I64 "l"
#else
  /*FMTx64 may require fixing for other compilers depending on theirs stdint.h,
    tweak as needed*/
  #define FMT_U64 "llu"
  #define FMT_I64 "ll"
#endif

#if BL_WORDSIZE == 16
  #define FMT_UWORD FMT_U16
  #define FMT_WORD FMT_I16
#elif BL_WORDSIZE == 32
  #define FMT_UWORD FMT_U32
  #define FMT_WORD FMT_I32
#elif BL_WORDSIZE == 64
  #define FMT_UWORD FMT_U64
  #define FMT_WORD FMT_I64
#else
  #error "Implement this"
#endif

#define FMT_W FMT_WORD
#define FMT_UW FMT_UWORD

#endif /*__BL_PRINTF_INT_H__*/