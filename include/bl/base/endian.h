#ifndef __BL_ENDIAN_H__
#define __BL_ENDIAN_H__

/* https://www.boost.org/doc/libs/1_72_0/boost/predef/other/endian.h */
#include <stddef.h>

#define BL_ARCH_ENDIANESS_LITTLE_VAL 0
#define BL_ARCH_ENDIANESS_BIG_VAL 1

#if defined (__GLIBC__) || defined (__GNU_LIBRARY__) || defined (__ANDROID__)
  #include <endian.h>
#elif defined (macintosh) || defined (Macintosh) || \
    (defined (__APPLE__) && defined (__MACH__))
  #include <machine/endian.h>
#elif defined (BSD) || defined (_SYSTYPE_BSD)
  #if defined(__OpenBSD__)
    #include <machine/endian.h>
  #else
    #include <sys/endian.h>
  #endif
#endif

#if defined (__BYTE_ORDER)
  #define BL_BYTE_ORDER_TEMP __BYTE_ORDER
#elif defined (_BYTE_ORDER)
  #define BL_BYTE_ORDER_TEMP _BYTE_ORDER
#endif

#if defined(BL_BYTE_ORDER_TEMP)
  #if defined (__BIG_ENDIAN) && (BL_BYTE_ORDER_TEMP == __BIG_ENDIAN)

    #define BL_ARCH_ENDIANESS BL_ARCH_ENDIANESS_BIG_VAL

  #endif
  #if defined (__LITTLE_ENDIAN) && (BL_BYTE_ORDER_TEMP == __LITTLE_ENDIAN)

    #define BL_ARCH_ENDIANESS BL_ARCH_ENDIANESS_LITTLE_VAL

  #endif
  #if defined (__PDP_ENDIAN) && (BL_BYTE_ORDER_TEMP == __PDP_ENDIAN)
    #error "PDP endianess not supported by this library"
  #endif
  #undef BL_BYTE_ORDER_TEMP
#endif

#if !defined (BL_ARCH_ENDIANESS)
  #if \
    (defined (__BIG_ENDIAN__) && !defined (__LITTLE_ENDIAN__)) || \
    (defined (_BIG_ENDIAN) && !defined (_LITTLE_ENDIAN)) || \
    defined (__ARMEB__) || \
    defined (__THUMBEB__) || \
    defined (__AARCH64EB__) || \
    defined (_MIPSEB) || \
    defined (__MIPSEB) || \
    defined (__MIPSEB__)

    #define BL_ARCH_ENDIANESS BL_ARCH_ENDIANESS_BIG_VAL
  #endif
#endif

#if !defined (BL_ARCH_ENDIANESS)
  #if \
    (defined (__LITTLE_ENDIAN__) && !defined (__BIG_ENDIAN__)) || \
    (defined (_LITTLE_ENDIAN) && !defined (_BIG_ENDIAN)) || \
    defined (__ARMEL__) || \
    defined (__THUMBEL__) || \
    defined (__AARCH64EL__) || \
    defined (_MIPSEL) || \
    defined (__MIPSEL) || \
    defined (__MIPSEL__)

    #define BL_ARCH_ENDIANESS BL_ARCH_ENDIANESS_LITTLE_VAL
  #endif
#endif

#if !defined (BL_ARCH_ENDIANESS)
  #include <bl/base/arch.h>
#endif

#if !defined (BL_ARCH_ENDIANESS)
  #if \
    BL_ARCH_IS (M68K) || \
    BL_ARCH_IS (PARISC) || \
    BL_ARCH_IS (SPARC) || \
    BL_ARCH_IS (SYS370) || \
    BL_ARCH_IS (SYS390) || \
    BL_ARCH_IS (ZARCH)

    #define BL_ARCH_ENDIANESS BL_ARCH_ENDIANESS_BIG_VAL

  #elif \
    BL_ARCH_IS (X86_64) || \
    BL_ARCH_IS (X86) || \
    BL_ARCH_IS (ITANIUM_64) || \
    BL_ARCH_IS (BLACKFIN)

    #define BL_ARCH_ENDIANESS BL_ARCH_ENDIANESS_LITTLE_VAL
  #endif
#endif

#if !defined (BL_ARCH_ENDIANESS)
  /* Windows on ARM, if not otherwise detected/specified, is always
  byte-swaped little-endian. */
  #if BL_ARCH_IS (ARM)
    #include <bl/base/os.h>
    #if BL_OS_IS (WINDOWS)
      #define BL_ARCH_ENDIANESS BL_ARCH_ENDIANESS_LITTLE_VAL
    #endif
  #endif
#endif

#if !defined (BL_ARCH_ENDIANESS)
  #error "Unable to detect this platform's endianess"
#elif BL_ARCH_ENDIANESS == BL_ARCH_ENDIANESS_LITTLE_VAL
  #define BL_ARCH_IS_LITTLE_ENDIAN 1
  #define BL_ARCH_IS_BIG_ENDIAN 0
#else
  #define BL_ARCH_IS_LITTLE_ENDIAN 0
  #define BL_ARCH_IS_BIG_ENDIAN 1
#endif

#endif /*__BL_ENDIAN_H__*/
