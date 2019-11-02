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
    #define BL_WORDSIZE     64
    #define BL_WORDSIZE_MAX 64
  #elif defined (_WIN16)
    #define BL_WORDSIZE     16
    #define BL_WORDSIZE_MAX 64
  #else
    #define BL_WORDSIZE     32
    #define BL_WORDSIZE_MAX 64
  #endif

  #if defined (_ARM)
    #define BL_ARM 1
  #endif

  #if defined (_M_IX86) || defined (_M_AMD64) || defined (_M_IA64)
    #define BL_INTEL_AMD_PC 1
  #endif

  #define BL_MSC              _MSC_VER

  #define BL_VSTUDIO_2015     1900
  #define BL_VSTUDIO_2013     1800
  #define BL_VSTUDIO_2012     1700
  #define BL_VSTUDIO_2010     1600
  #define BL_VSTUDIO_2008     1500
  #define BL_VSTUDIO_2005     1400
  #define BL_VSTUDIO_NET_2003 1310
  #define BL_VSTUDIO_NET      1300
  #define BL_VC_6             1200
  #define BL_VC_5             1100

  #define BL_COMPILER         BL_MSC

  #define BL_HAS_C11_STDALIGN(bl_compiler) 0
  #define BL_HAS_C11_ATOMICS(bl_compiler) 0

  #define BL_VISIBILITY_DEFAULT
  #define BL_VISIBILITY_HIDDEN
  #define BL_PRINTF_FORMAT(string_idx, va_args_idx)

  #define BL_HAS_ERRNO_H 0 /*for very backwards compatibility, can be improved*/

#endif
/*---------------------------------------------------------------------------*/
#if defined (__GNUC__) || defined (GCC) || defined (__clang__)

  #if !defined (__clang__)
    #define BL_GCC_VER(major, minor, patchlevel)\
      ((major * 10000) + (minor * 100) + patchlevel)
    #define BL_GCC BL_GCC_VER (__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
  #else
    #define BL_CLANG 1
  #endif

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
      #define BL_WORDSIZE     64
      #define BL_WORDSIZE_MAX 64
    #else
      #define BL_WORDSIZE     32
      #define BL_WORDSIZE_MAX 64
    #endif
  #endif

  #if defined (__i386__)
    #define BL_INTEL_AMD_PC 1
    #define BL_WORDSIZE     32
    #define BL_WORDSIZE_MAX 64
  #endif

  #if defined (__arm__)
    #define BL_ARM 1
    #if defined (__aarch64__)
      #define BL_WORDSIZE     64
      #define BL_WORDSIZE_MAX 64
    #else
      #define BL_WORDSIZE     32
      #define BL_WORDSIZE_MAX 64
    #endif
  #endif

#if 0 /*not sure about how to test on gcc (depends on glib)*/
  #if BL_GCC >= BL_GCC_VER (4, 9, 0)
    #define BL_HAS_C11_THREAD
  #endif
#endif

  #if !defined (__clang__)
    #define BL_COMPILER BL_GCC

    #define BL_HAS_C11_STDALIGN(bl_compiler)\
      bl_compiler >= BL_GCC_VER (4, 7, 0)
    #define BL_HAS_C11_ATOMICS(bl_compiler)\
      bl_compiler >= BL_GCC_VER (4, 9, 0)

    #if BL_GCC >= BL_GCC_VER (4, 0, 0)
      #define BL_VISIBILITY_DEFAULT __attribute__ ((visibility ("default")))
      #define BL_VISIBILITY_HIDDEN __attribute__ ((visibility ("hidden")))
    #else
      #define BL_VISIBILITY_DEFAULT
      #define BL_VISIBILITY_HIDDEN
    #endif
  #else
    #define BL_COMPILER BL_CLANG

    #define BL_VISIBILITY_DEFAULT __attribute__ ((visibility ("default")))
    #define BL_VISIBILITY_HIDDEN __attribute__ ((visibility ("hidden")))

    #define BL_HAS_C11_STDALIGN(bl_compiler) 1
    #define BL_HAS_C11_ATOMICS(bl_compiler) 1
  #endif

  #define BL_PRINTF_FORMAT(string_idx, va_args_idx) \
    __attribute__ ((format (printf, string_idx, va_args_idx)))

  #define BL_HAS_ERRNO_H 1

#endif
/*---------------------------------------------------------------------------*/
#if !defined (BL_COMPILER)
  #error "unknown compiler or platform (you may just need to update this file)"
#endif
/*---------------------------------------------------------------------------*/
#define BL_ARCH_LITTLE_ENDIAN 1 /*good enough for now*/
/*---------------------------------------------------------------------------*/
#include <bl/base/static_assert.h>
/*---------------------------------------------------------------------------*/
#ifndef __cplusplus
  #include <stdbool.h>
  #include <stddef.h>
  #define nullptr NULL
#endif
/*---------------------------------------------------------------------------*/
bl_static_assert_outside_func ((bool) 1 == true, "bool type check failed");
bl_static_assert_outside_func ((bool) 0 == false, "bool type check failed");

#if BL_WORDSIZE <= 32
  bl_static_assert_outside_func ((int) nullptr == 0, "nullptr check failed");
#else
  bl_static_assert_outside_func ((long) nullptr == 0, "nullptr check failed");
#endif
/*---------------------------------------------------------------------------*/
#if defined (BL_WINDOWS)
  #define BL_SCHED_TMIN_US 1000
#elif defined (BL_LINUX)
  #define BL_SCHED_TMIN_US 100
#else
  #error "define this for your the os"
#endif

#if !defined (BL_WORDSIZE) || !defined (BL_WORDSIZE_MAX)
  #error "define this for your platform"
#endif

#if BL_WORDSIZE > BL_WORDSIZE_MAX
  #error "the bl_word size can't be bigger than the maximum bl_word size"
#endif

#endif /*EVK_PLATFORM_H_*/
