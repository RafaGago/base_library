#ifndef __BL_ARCH_H__
#define __BL_ARCH_H__

/* adapted from:

https://www.boost.org/doc/libs/1_72_0/boost/predef/architecture/

Copyright Rene Rivera
Distributed under the Boost Software License, Version 1.0.
*/

/*
defines BL_ARCH, BL_ARCH_NAME and BL_ARCH_VER

To check for a processor:
  #if BL_ARCH_IS (ARM)

or:
  #if BL_ARCH == BL_ARCH_ARM

To check for a revision:
  #if BL_ARCH_VER > BL_VER_GET (7, 0, 0)

Or:
  #if BL_VER_MAJOR_GET (BL_ARCH_VER) == 7
*/

#include <bl/base/version_formats.h>

#define BL_ARCH_ALPHA    100
#define BL_ARCH_ARM      200
#define BL_ARCH_BLACKFIN 300
#define BL_ARCH_CONVEX   400
#define BL_ARCH_IA64     500
#define BL_ARCH_M68K     600
#define BL_ARCH_MIPS     700
#define BL_ARCH_PPC      800
#define BL_ARCH_PTX      900
#define BL_ARCH_PARISC   1000
#define BL_ARCH_PYRAMID  1100
#define BL_ARCH_RS6000   1200
#define BL_ARCH_SPARC    1300
#define BL_ARCH_SUPERH   1400
#define BL_ARCH_SYS370   1500
#define BL_ARCH_SYS390   1600
#define BL_ARCH_X86      1700
#define BL_ARCH_X86_64   1800
#define BL_ARCH_ZARCH    1900

#define BL_ARCH_IS(arch) (BL_ARCH == BL_ARCH_##arch)

#define BL_ARCH_IS_X64_X86_IA64 \
  (BL_ARCH_IS (X86_64) || BL_ARCH_IS (X86) || BL_ARCH_IS (IA64))

/* x86_64 */
#if !defined (BL_ARCH)
  #if \
    defined (__x86_64) || defined (__x86_64__) || \
    defined (__amd64__) || defined (__amd64) || \
    defined (_M_X64)

    #define BL_ARCH      BL_ARCH_X86_64
    #define BL_ARCH_NAME "x86_64"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* arm */
#if !defined (BL_ARCH)
  #if \
    defined (__ARM_ARCH) || defined (__TARGET_ARCH_ARM) || \
    defined (__TARGET_ARCH_THUMB) || defined (_M_ARM) || \
    defined (__arm__) || defined (__arm64) || defined (__thumb__) || \
    defined (_M_ARM64) || defined (__aarch64__) || defined (__AARCH64EL__) || \
    defined (__ARM_ARCH_7__) || defined (__ARM_ARCH_7A__) || \
    defined (__ARM_ARCH_7R__) || defined (__ARM_ARCH_7M__) || \
    defined (__ARM_ARCH_6K__) || defined (__ARM_ARCH_6Z__) || \
    defined (__ARM_ARCH_6KZ__) || defined (__ARM_ARCH_6T2__) || \
    defined (__ARM_ARCH_5TE__) || defined (__ARM_ARCH_5TEJ__) || \
    defined (__ARM_ARCH_4T__) || defined (__ARM_ARCH_4__)

    #define BL_ARCH      BL_ARCH_ARM
    #define BL_ARCH_NAME "arm"

    #if defined (__ARM_ARCH)
      #define BL_ARCH_VER BL_VER_GET (__ARM_ARCH, 0, 0)

    #elif defined (__TARGET_ARCH_ARM)
      #define BL_ARCH_VER BL_VER_GET (__TARGET_ARCH_ARM, 0, 0)

    #elif defined (__TARGET_ARCH_THUMB)
      #define BL_ARCH_VER BL_VER_GET (__TARGET_ARCH_THUMB, 0, 0)

    #elif defined(_M_ARM)
      #define BL_ARCH_VER BL_VER_GET (_M_ARM, 0, 0)

    #elif \
      defined (__ARM_ARCH_7__) || defined (__ARM_ARCH_7A__) || \
      defined (__ARM_ARCH_7R__) || defined (__ARM_ARCH_7M__)

      #define BL_ARCH_VER BL_VER_GET (7, 0, 0)

    #elif \
      defined (__ARM_ARCH_6K__) || defined (__ARM_ARCH_6Z__) || \
      defined (__ARM_ARCH_6KZ__) || defined (__ARM_ARCH_6T2__) ||

      #define BL_ARCH_VER BL_VER_GET (6, 0, 0)

    #elif defined (__ARM_ARCH_5TE__) || defined (__ARM_ARCH_5TEJ__)
      #define BL_ARCH_VER BL_VER_GET (5, 0, 0)

    #elif defined (__ARM_ARCH_4T__) || defined (__ARM_ARCH_4__)
      #define BL_ARCH_VER BL_VER_GET (4, 0, 0)

    #else
      #define BL_ARCH_VER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

/* x86 */
#if !defined (BL_ARCH)
  #if \
    defined (i386) || defined (__i386__) || \
    defined (__i486__) || defined (__i586__) || \
    defined (__i686__) || defined (__i386) || \
    defined (_M_IX86) || defined (_X86_) || \
    defined (__THW_INTEL__) || defined (__I86__) || \
    defined (__INTEL__)

    #define BL_ARCH      BL_ARCH_X86
    #define BL_ARCH_NAME "x86"

    #if defined (__I86__)
      #define BL_ARCH_VER BL_VER_GET (__I86__, 0, 0)
    #elif defined (_M_IX86)
      #define BL_ARCH_VER BL_VER_FMT_10_VV00 (_M_IX86)
    #elif defined (__i686__)
      #define BL_ARCH_VER BL_VER_GET (6, 0, 0)
    #elif defined (__i586__)
      #define BL_ARCH_VER BL_VER_GET (5, 0, 0)
    #elif defined (__i486__)
      #define BL_ARCH_VER BL_VER_GET (4, 0, 0)
    #elif defined (__i386__)
      #define BL_ARCH_VER BL_VER_GET (3, 0, 0)
    #else
      #define BL_ARCH_VER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

/*PowerPC*/
#if !defined (BL_ARCH)
  #if \
    defined(__powerpc) || defined(__powerpc__) || \
    defined(__POWERPC__) || defined(__ppc__) || \
    defined(_M_PPC) || defined(_ARCH_PPC) || \
    defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || \
    defined(_XENON)

    #define BL_ARCH      BL_ARCH_PPC
    #define BL_ARCH_NAME "PowerPC"

    #if defined (__ppc601__) || defined (_ARCH_601)
      #define BL_ARCH_VER BL_VER_GET (6, 1, 0)
    #elif defined (__ppc603__) || defined (_ARCH_603)
      #define BL_ARCH_VER BL_VER_GET (6, 3, 0)
    #elif defined (__ppc604__) || defined (__ppc604__)
      #define BL_ARCH_VER BL_VER_GET (6, 4, 0)
    #else
      #define BL_ARCH_VER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

/* itanium 64*/
#if !defined (BL_ARCH)
  #if \
    defined (__ia64__) || defined (_IA64) || \
    defined (__IA64__) || defined (__ia64) || \
    defined (_M_IA64) || defined (__itanium__)

    #define BL_ARCH      BL_ARCH_IA64
    #define BL_ARCH_NAME "itanium 64"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* PTX */
#if !defined (BL_ARCH)
  #if defined (__CUDA_ARCH__)
    #define BL_ARCH      BL_ARCH_PTX
    #define BL_ARCH_NAME "PTX"
    #define BL_ARCH_VER  BL_VER_FMT_10_VR0 (__CUDA_ARCH__)
  #endif
#endif

/* blackfin */
#if !defined (BL_ARCH)
  #if \
    defined (__bfin__) || defined (__BFIN__) || \
    defined (bfin) || defined (BFIN)

    #define BL_ARCH      BL_ARCH_BLACKFIN
    #define BL_ARCH_NAME "blackfin"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* m68k */
#if !defined (BL_ARCH)
  #if defined (__m68k__) || defined (M68000)
    #define BL_ARCH      BL_ARCH_M68K
    #define BL_ARCH_NAME "motorola 68k"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* parisc */
#if !defined (BL_ARCH)
  #if defined (__hppa__) || defined (__hppa) || defined (__HPPA__)
    #define BL_ARCH      BL_ARCH_PARISC
    #define BL_ARCH_NAME "PA-RISC"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* sparc */
#if !defined (BL_ARCH)
  #if defined (__sparc__) || defined (__sparc)
    #define BL_ARCH      BL_ARCH_SPARC
    #define BL_ARCH_NAME "sparc"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* system 370 */
#if !defined (BL_ARCH)
  #if defined (__370__) || defined (__THW_370__)
    #define BL_ARCH      BL_ARCH_SYS370
    #define BL_ARCH_NAME "system 370"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* system 390 */
#if !defined (BL_ARCH)
  #if defined (__s390__) || defined (__s390x__)
    #define BL_ARCH      BL_ARCH_SYS390
    #define BL_ARCH_NAME "system 390"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* Z archictecture */
#if !defined (BL_ARCH)
  #if defined (__SYSC_ZARCH__)
    #define BL_ARCH      BL_ARCH_ZARCH
    #define BL_ARCH_NAME "Z archictecture"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* RS6000 */
#if !defined (BL_ARCH)
  #if \
    defined(__THW_RS6000) || defined(_IBMR2) || \
    defined(_POWER) || defined(_ARCH_PWR) || \
    defined(_ARCH_PWR2)

    #define BL_ARCH      BL_ARCH_RS6000
    #define BL_ARCH_NAME "RS6000"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* Convex */
#if !defined (BL_ARCH)
  #if defined(__convex__)
    #define BL_ARCH      BL_ARCH_CONVEX
    #define BL_ARCH_NAME "convex"

    #if defined (__convex_c1__)
      #define BL_ARCH_VER BL_VER_GET (1, 0, 0)
    #elif defined (__convex_c2__)
      #define BL_ARCH_VER BL_VER_GET (2, 0, 0)
    #elif defined (__convex_c32__)
      #define BL_ARCH_VER BL_VER_GET (3, 2, 0)
    #elif defined (__convex_c34__)
      #define BL_ARCH_VER BL_VER_GET (3, 4, 0)
    #elif defined (__convex_c38__)
      #define BL_ARCH_VER BL_VER_GET (3, 8, 0)
    #else
      #define BL_ARCH_VER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

/* Pyramid 9810*/
#if !defined (BL_ARCH)
  #if defined (pyr)
    #define BL_ARCH      BL_ARCH_PYRAMID
    #define BL_ARCH_NAME "pyramid 9810"
    #define BL_ARCH_VER  BL_VER_UNAVAILABLE
  #endif
#endif

/* SuperH */
#if !defined (BL_ARCH)
  #if defined(__sh__)
    #define BL_ARCH      BL_ARCH_SUPERH
    #define BL_ARCH_NAME "SuperH"

    #if defined (__SH5__)
      #define BL_ARCH_VER BL_VER_GET (5, 0, 0)
    #elif defined (__SH4__)
      #define BL_ARCH_VER BL_VER_GET (4, 0, 0)
    #elif defined (__SH3__)
      #define BL_ARCH_VER BL_VER_GET (3, 0, 0)
    #elif defined (__SH2__)
      #define BL_ARCH_VER BL_VER_GET (2, 0, 0)
    #elif defined (__SH1__)
      #define BL_ARCH_VER BL_VER_GET (1, 0, 0)
    #else
      #define BL_ARCH_VER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

/* mips */
#if !defined (BL_ARCH)
  #if defined(__mips__) || defined(__mips) || defined(__MIPS__)
    #define BL_ARCH      BL_ARCH_MIPS
    #define BL_ARCH_NAME "mips"

    #if defined (__mips)
      #define BL_ARCH_VER BL_VER_GET (__mips, 0, 0)
    #elif defined (_MIPS_ISA_MIPS1) || defined (_R3000)
      #define BL_ARCH_VER BL_VER_GET (1, 0, 0)
    #elif \
      defined (_MIPS_ISA_MIPS2) || defined (__MIPS_ISA2__) || defined (_R4000)

      #define BL_ARCH_VER BL_VER_GET (2, 0, 0)
    #elif defined(_MIPS_ISA_MIPS3) || defined(__MIPS_ISA3__)
      #define BL_ARCH_VER BL_VER_GET (3, 0, 0)
    #elif defined(_MIPS_ISA_MIPS4) || defined(__MIPS_ISA4__)
      #define BL_ARCH_VER BL_VER_GET (4, 0, 0)
    #else
      #define BL_ARCH_VER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

/* DEC Alpha */
#if !defined (BL_ARCH)
  #if defined (__alpha__) || defined (__alpha) || defined (_M_ALPHA)
    #define BL_ARCH      BL_ARCH_ALPHA
    #define BL_ARCH_NAME "DEC Alpha"

    #if defined (__alpha_ev6__)
      #define BL_ARCH_VER BL_VER_GET (6, 0, 0)
    #elif defined (__alpha_ev5__)
      #define BL_ARCH_VER BL_VER_GET (5, 0, 0)
    #elif defined (__alpha_ev4__)
      #define BL_ARCH_VER BL_VER_GET (4, 0, 0)
    #else
      #define BL_ARCH_VER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

#if !defined (BL_ARCH)
  #error "Unable to detect the architecture"
#endif

#endif /* __BL_ARCH_H__ */
