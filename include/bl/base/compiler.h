#ifndef _BL_COMPILER_H_
#define _BL_COMPILER_H_

#include <bl/base/version_formats.h>

/* adapted from:

https://www.boost.org/doc/libs/1_72_0/boost/predef/compiler/

Copyright Rene Rivera
Distributed under the Boost Software License, Version 1.0.
*/

/*
defines BL_COMPILER, BL_COMPILER_NAME and BL_COMPILER_VER

To check for a compiler:
  #if BL_COMPILER == BL_COMPILER_GCC

To check for a revision:
  #if BL_COMPILER_VER > BL_VER_GET (4, 7, 0)

Or:
  #if BL_VER_MAJOR_GET (BL_COMPILER_VER) == 5
*/

#define BL_COMPILER_BORLAND      100
#define BL_COMPILER_CLANG        200
#define BL_COMPILER_COMEAU       300
#define BL_COMPILER_COMPAQ       400
#define BL_COMPILER_DIAB         500
#define BL_COMPILER_DIGITALMARS  600
#define BL_COMPILER_DIGNUS       700
#define BL_COMPILER_EDG          800
#define BL_COMPILER_EKOPATH      900
#define BL_COMPILER_GCC          1000
#define BL_COMPILER_GCC_XML      1100
#define BL_COMPILER_GREENHILLS   1200
#define BL_COMPILER_HP_ACC       1300
#define BL_COMPILER_IAR          1400
#define BL_COMPILER_IBM          1500
#define BL_COMPILER_INTEL        1600
#define BL_COMPILER_KAI          1700
#define BL_COMPILER_LLVM         1800
#define BL_COMPILER_METAWARE     1900
#define BL_COMPILER_METROVERKS   2000
#define BL_COMPILER_MICROTEC     2100
#define BL_COMPILER_MPW          2200
#define BL_COMPILER_NVCC         2300
#define BL_COMPILER_PALM         2400
#define BL_COMPILER_PGI          2500
#define BL_COMPILER_SGI_MIPSPRO  2600
#define BL_COMPILER_SUNPRO       2700
#define BL_COMPILER_TENDRA       2800
#define BL_COMPILER_MICROSOFT_VC 2900
#define BL_COMPILER_WATCOM       3000

#define BL_COMPILER_IS(comp) (BL_COMPILER == BL_COMPILER_##comp)

#define BL_COMPILER_VER_IS_GE(major, minor, rev)\
   (BL_COMPILER_VER >= BL_VER_GET ((major), (minor), (rev)))

#if defined (__GNUC__)
  #define BL_COMPILER      BL_COMPILER_GCC
  #define BL_COMPILER_NAME "GNU gcc"

  #if defined (__GNUC_PATCHLEVEL__)
    #define BL_COMPILER_VER \
            BL_VER_GET (__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
  #else
    #define BL_COMPILER_VER BL_VER_GET (__GNUC__, __GNUC_MINOR__, 0)
  #endif
#endif

#if defined (__clang__)
  #define BL_COMPILER      BL_COMPILER_CLANG
  #define BL_COMPILER_NAME "Clang"
  #define BL_COMPILER_VER \
    BL_VER_GET (__clang_major__, __clang_minor__, __clang_patchlevel__)
#endif

#if defined (_MSC_VER)
  #define BL_COMPILER      BL_COMPILER_MICROSOFT_VC
  #define BL_COMPILER_NAME "Microsoft Visual C/C++"

  #if !defined (_MSC_FULL_VER)
    #define BL_MSVC_REV_PRIV 0
  #else
        /* how many digits does the build number have? */
    #if _MSC_FULL_VER / 10000 == _MSC_VER
            /* four digits */
      #define BL_MSVC_REV_PRIV (_MSC_FULL_VER % 10000)
    #elif _MSC_FULL_VER / 100000 == _MSC_VER
            /* five digits */
      #define BL_MSVC_REV_PRIV (_MSC_FULL_VER % 100000)
    #else
      #error "Cannot determine build number from _MSC_FULL_VER"
    #endif
  #endif
    /*
    VS2014 was skipped in the release sequence for MS. Which
    means that the compiler and VS product versions are no longer
    in sync. Hence we need to use different formulas for
    mapping from MSC version to VS product version.

    VS2017 is a total nightmare when it comes to version numbers.
    Hence to avoid arguments relating to that both present and
    future.. Any version after VS2015 will use solely the compiler
    version, i.e. cl.exe, as the version number here.
    */
  #if (_MSC_VER > 1900)
    #define BL_COMPILER_VER \
      BL_VER_GET (_MSC_VER / 100, _MSC_VER % 100, BL_MSVC_REV_PRIV)
  #elif (_MSC_VER >= 1900)
    #define BL_COMPILER_VER \
      BL_VER_GET (_MSC_VER / 100 - 5, _MSC_VER % 100, BL_MSVC_REV_PRIV)
  #else
    #define BL_COMPILER_VER \
      BL_VER_GET (_MSC_VER / 100 - 6, _MSC_VER % 100, BL_MSVC_REV_PRIV)
  #endif
#endif

#if defined (__INTEL_COMPILER) || defined (__ICL) || defined (__ICC) || \
    defined (__ECC)

  #define BL_COMPILER      BL_COMPILER_INTEL
  #define BL_COMPILER_NAME "Intel C/C++"
/*
[note Because of an Intel mistake in the release version numbering when
`__INTEL_COMPILER` is `9999` it is detected as version 12.1.0.]
*/
  #if defined (__INTEL_COMPILER) && (__INTEL_COMPILER == 9999)
    #define BL_COMPILER_VER BL_VER_GET (12, 1, 0)

  #elif defined (__INTEL_COMPILER) && \
      defined (__INTEL_COMPILER_UPDATE)

    #define BL_COMPILER_VER  \
      BL_VER_GET( \
        BL_VER_MAJOR_GET (BL_VER_FMT_10_VVRR (__INTEL_COMPILER)), \
        BL_VER_MINOR_GET (BL_VER_FMT_10_VVRR (__INTEL_COMPILER)), \
        __INTEL_COMPILER_UPDATE \
        )
  #elif defined (__INTEL_COMPILER)
    #define BL_COMPILER_VER BL_VER_FMT_10_VVRR (__INTEL_COMPILER)

  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__IAR_SYSTEMS_ICC__)
  #define BL_COMPILER      BL_COMPILER_IAR
  #define BL_COMPILER_NAME "IAR C/C++"
  #define BL_COMPILER_VER  BL_VER_FMT_10_VVRR (__VER__)
#endif

#if defined (_MRI)
  #define BL_COMPILER      BL_COMPILER_MICROTEC
  #define BL_COMPILER_NAME "Microtec C/C++"
  #define BL_COMPILER_VER  BL_VER_UNAVAILABLE
#endif

#if defined (__MRC__) || defined (MPW_C) || defined (MPW_CPLUS)
  #define BL_COMPILER      BL_COMPILER_MPW
  #define BL_COMPILER_NAME "MPW C++"

  #if defined (__MRC__)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VVRR (__MRC__)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__PATHCC__)
  #define BL_COMPILER      BL_COMPILER_EKOPATH
  #define BL_COMPILER_NAME "EKOpath"
  #define BL_COMPILER_VER  \
    BL_VER_GET (__PATHCC__, __PATHCC_MINOR__, __PATHCC_PATCHLEVEL__)
#endif

#if defined (__ghs) || defined (__ghs__)
  #define BL_COMPILER      BL_COMPILER_GREENHILLS
  #define BL_COMPILER_NAME "Green Hills C/C++"

  #if defined (__GHS_VERSION_NUMBER__)
    #define BL_COMPILER_VER BL_VER_FMT_10_VRP (__GHS_VERSION_NUMBER__)
  #elif defined (__ghs)
    #define BL_COMPILER_VER BL_VER_FMT_10_VRP (__ghs)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__BORLANDC__) || defined (__CODEGEARC__)
  #define BL_COMPILER      BL_COMPILER_BORLAND
  #define BL_COMPILER_NAME "Borland C++"

  #if defined (__CODEGEARC__)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VVRP (__CODEGEARC__)
  #else
    #define BL_COMPILER_VER BL_VER_FMT_0X_VVRP (__BORLANDC__)
  #endif
#endif

#if defined (__PGI)
  #define BL_COMPILER      BL_COMPILER_PGI
  #define BL_COMPILER_NAME "Portland Group C/C++"

  #if defined (__PGIC__) && defined (__PGIC_MINOR__) && \
    defined (__PGIC_PATCHLEVEL__)
    #define BL_COMPILER_VER \
      BL_VER_GET (__PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__llvm__)
  #define BL_COMPILER      BL_COMPILER_LLVM
  #define BL_COMPILER_NAME "LLVM"
  #define BL_COMPILER_VER  BL_VER_UNAVAILABLE
#endif

#if defined (__TenDRA__)
  #define BL_COMPILER      BL_COMPILER_TENDRA
  #define BL_COMPILER_NAME "TenDRA C/C++"
  #define BL_COMPILER_VER  BL_VER_UNAVAILABLE
#endif

#if defined (__DMC__)
  #define BL_COMPILER      BL_COMPILER_DIGITALMARS
  #define BL_COMPILER_NAME "Digital Mars"
  #define BL_COMPILER_VER  BL_VER_FMT_0X_VRP (__DMC__)
#endif

#if defined (__WATCOMC__)
  #define BL_COMPILER      BL_COMPILER_WATCOM
  #define BL_COMPILER_NAME "Watcom C++"
  #define BL_COMPILER_VER   BL_VER_FMT_10_VVRR (__WATCOMC__)
#endif

#if defined (__NVCC__)
  #define BL_COMPILER      BL_COMPILER_NVCC
  #define BL_COMPILER_NAME "NVCC"

  #if defined (__CUDACC_VER_MAJOR__) && \
      defined (__CUDACC_VER_MINOR__) && \
      defined (__CUDACC_VER_BUILD__)

    #define BL_COMPILER_VER \
      BL_VER_GET( \
        __CUDACC_VER_MAJOR__, __CUDACC_VER_MINOR__, __CUDACC_VER_BUILD__ \
        )
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__DECC) || defined (__DECCXX)
  #define BL_COMPILER      BL_COMPILER_COMPAQ
  #define BL_COMPILER_NAME "Compaq C/C++"

  #if defined (__DECCXX_VER)
    #define BL_COMPILER_VER BL_VER_FMT_10_VVRR0PP00 (__DECCXX_VER)
  #elif defined (__DECC_VER)
    #define BL_COMPILER_VER BL_VER_FMT_10_VVRR0PP00 (__DECC_VER)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (_PACC_VER)
  #define BL_COMPILER      BL_COMPILER_PALM
  #define BL_COMPILER_NAME "Palm C/C++"
  #define BL_COMPILER_VER  BL_VER_FMT_0X_VRRPP000 (_PACC_VER)
#endif

#if defined (__IBMCPP__) || defined (__xlC__) || defined (__xlc__)
  #define BL_COMPILER      BL_COMPILER_IBM
  #define BL_COMPILER_NAME "IBM XL C/C++"

  #if defined (__COMPILER_VER__)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VRRPPPP (__COMPILER_VER__)
  #elif defined (__xlC__)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VVRR (__xlC__)
  #elif defined (__xlc__)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VVRR (__xlc__)
  #else
    #define BL_COMPILER_VER BL_VER_FMT_10_VRP (__IBMCPP__)
  #endif
#endif

#if defined (__EDG__)
  #define BL_COMPILER      BL_COMPILER_EDG
  #define BL_COMPILER_NAME "EDG C++ Frontend"
  #define BL_COMPILER_VER  BL_VER_FMT_10_VRR (__EDG_VERSION__)
#endif

#if defined (__HIGHC__)
  #define BL_COMPILER      BL_COMPILER_METAWARE
  #define BL_COMPILER_NAME "MetaWare High C/C++"
  #define BL_COMPILER_VER  BL_VER_UNAVAILABLE
#endif

#if defined (__DCC__)
  #define BL_COMPILER      BL_COMPILER_DIAB
  #define BL_COMPILER_NAME "Diab C/C++"
  #define BL_COMPILER_VER  BL_VER_FMT_10_VRPP (__VERSION_NUMBER__)
#endif

#if defined (__SYSC__)
  #define BL_COMPILER      BL_COMPILER_DIGNUS
  #define BL_COMPILER_NAME "Dignus Systems/C++"
  #define BL_COMPILER_VER  BL_VER_FMT_10_VRRPP (__SYSC_VER__)
#endif

#if defined (__MWERKS__) || defined (__CWCC__)
  #define BL_COMPILER      BL_COMPILER_METROVERKS
  #define BL_COMPILER_NAME "Metrowerks CodeWarrior"

  #if defined (__CWCC__)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VRPP (__CWCC__)
  #elif (__MWERKS__ >= 0x4200)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VRPP (__MWERKS__)
  #elif (__MWERKS__ >= 0x3204) // note the "skip": 04->9.3
    #define BL_COMPILER_VER BL_VER_GET (9, (__MWERKS__) % 100 - 1, 0)
  #elif (__MWERKS__ >= 0x3200)
    #define BL_COMPILER_VER BL_VER_GET (9, (__MWERKS__) % 100, 0)
  #elif (__MWERKS__ >= 0x3000)
    #define BL_COMPILER_VER BL_VER_GET (8, (__MWERKS__) % 100, 0)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__COMO__)
  #define BL_COMPILER      BL_COMPILER_COMEAU
  #define BL_COMPILER_NAME "Comeau C++"

  #if defined (__COMO_VERSION__)
    #define BL_COMPILER_VER BL_VER_FMT_0X_VRP (__COMO_VERSION__)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__KCC)
  #define BL_COMPILER      BL_COMPILER_KAI
  #define BL_COMPILER_NAME "Kai C++"
  #define BL_COMPILER_VER  BL_VER_FMT_0X_VRPP (__KCC_VERSION)
#endif

#if defined (__SUNPRO_CC) || defined (__SUNPRO_C)
  #define BL_COMPILER      BL_COMPILER_SUNPRO
  #define BL_COMPILER_NAME "Oracle Solaris Studio"

  #if defined (__SUNPRO_CC)
    #if (__SUNPRO_CC < 0x5100)
      #define BL_COMPILER_VER BL_VER_FMT_0X_VRP (__SUNPRO_CC)
    #else
      #define BL_COMPILER_VER BL_VER_FMT_0X_VVRRP (__SUNPRO_CC)
    #endif
  #elif defined (__SUNPRO_C)
    #if (__SUNPRO_C < 0x5100)
      #define BL_COMPILER_VER BL_VER_FMT_0X_VRP (__SUNPRO_C)
    #else
      #define BL_COMPILER_VER BL_VER_FMT_0X_VVRRP (__SUNPRO_C)
    #endif
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__HP_aCC)
  #define BL_COMPILER      BL_COMPILER_HP_ACC
  #define BL_COMPILER_NAME "HP aC++"

  #if (__HP_aCC > 1)
    #define BL_COMPILER_VER BL_VER_FMT_10_VVRRPP (__HP_aCC)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if defined (__GCCXML__)
  #define BL_COMPILER      BL_COMPILER_GCC_XML
  #define BL_COMPILER_NAME "GCC XML"
  #define BL_COMPILER_VER  BL_VER_UNAVAILABLE
#endif

#if defined (__sgi) || defined (sgi)

  #define BL_COMPILER      BL_COMPILER_SGI_MIPSPRO
  #define BL_COMPILER_NAME "SGI MIPSpro"

  #if defined (_SGI_COMPILER_VERSION)
    #define BL_COMPILER_VER BL_VER_FMT_10_VRP (_SGI_COMPILER_VERSION)
  #elif defined (_COMPILER_VERSION)
    #define BL_COMPILER_VER BL_VER_FMT_10_VRP (_COMPILER_VERSION)
  #else
    #define BL_COMPILER_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if !defined (BL_COMPILER)
  #error "Unable to detect the compiler"
#endif

#endif /* _BL_COMPILER_H_ */
