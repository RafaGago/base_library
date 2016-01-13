#ifndef BL_PLATFORM_H_
#define BL_PLATFORM_H_
/*---------------------------------------------------------------------------*/
/* Very useful: http://sourceforge.net/p/predef/wiki/Architectures/          */
/*---------------------------------------------------------------------------*/
#ifdef _MSC_VER
/*  #define BL_X86_64 1*/
  #if defined (_WIN64) || defined (_WIN32) || defined (_WIN16)
    #define BL_WINDOWS 1
  #else
    #error "Unknown windows platform"
  #endif

  #if defined(_WIN64) && !defined(_WIN16)
    #define BL64 1
  #elif defined (_WIN16)
    #define BL16 1
  #else
    #define BL32 1
  #endif

  #if defined (_ARM)
    #define BL_ARM 1
  #endif

  #if defined (_M_IX86) || defined (_M_AMD64) || defined (_M_IA64)
    #define BL_INTEL_AMD_PC 1
  #endif

  #define BL_CPP_COMPILER_FOR_WHOLE_PROJECT 1

  #define likely(x)   (x) /*TODO move to <utility.h> ?*/
  #define unlikely(x) (x)
#endif
/*---------------------------------------------------------------------------*/
#if defined (__GNUC__) || defined (GCC)

  #define BL_GCC_VER(major, minor, patchlevel)\
    ((major * 10000) + (minor * 100) + patchlevel)
  #define BL_GCC BL_GCC_VER (__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)

  #if defined (_WIN64) || defined (_WIN32) || defined (_WIN32)
    #define BL_WINDOWS 1
  #endif
  #if defined (__linux__)
    #define BL_LINUX 1
    #define BL_POSIX 1
  #endif

  #if defined (__x86_64__)
    #define BL_INTEL_AMD_PC 1
    #if defined (__LP64__)
      #define BL64 1
    #else
      #define BL32 1
    #endif
  #endif

  #if defined (__i386__)
    #define BL_INTEL_AMD_PC 1
    #define BL32 1
  #endif

  #if defined (__arm__)
    #define BL_ARM 1
    #if defined (__aarch64__)
      #define BL64 1
    #else
      #define BL32 1
    #endif
  #endif

  #ifndef __cplusplus
    #include <stdbool.h>
    #include <stddef.h>
    #define nullptr NULL
  #endif

  #define likely(x)   __builtin_expect(!!(x), 1) /*TODO move to <utility.h> ?*/
  #define unlikely(x) __builtin_expect(!!(x), 0)
#endif
/*---------------------------------------------------------------------------*/
#if !defined (BL_INTEL_AMD_PC) && !defined (BL_INTEL_AMD_PC)
  #error "unknown compiler or platform (you may just need to update this file)"
#endif
/*---------------------------------------------------------------------------*/
#define BL_ARCH_LITTLE_ENDIAN 1 /*good enough for now*/
/*---------------------------------------------------------------------------*/
#include <base_library/hdr/static_assert.h>
/*---------------------------------------------------------------------------*/
static_assert_outside_func ((bool) 1 == true, "bool type check failed");
static_assert_outside_func ((bool) 0 == false, "bool type check failed");
static_assert_outside_func ((int) nullptr == 0, "nullptr check failed");
/*---------------------------------------------------------------------------*/
#endif /*EVK_PLATFORM_H_*/
