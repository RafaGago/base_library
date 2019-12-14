#ifndef BL_PLATFORM_H_
#define BL_PLATFORM_H_
/*---------------------------------------------------------------------------*/
/* Very useful: http://sourceforge.net/p/predef/wiki/Architectures/          */
/*---------------------------------------------------------------------------*/
#include <bl/base/os.h>
#include <bl/base/arch.h>
#include <bl/base/compiler.h>

/* static assertions */
#if BL_COMPILER_IS (GCC)
  #if !defined(__cplusplus)
    #if BL_COMPILER_VER_IS_GE (4, 6, 0)
      #define bl_static_assert_private(c,s) _Static_assert (c, s)
    #endif
  #else
    #define bl_static_assert_private(c,s) static_assert (c, s)
  #endif
#elif BL_COMPILER_IS (MICROSOFT_VC)
  #define bl_static_assert_private(c,s) static_assert (c, s)
#elif BL_COMPILER_IS (CLANG)
  #if !defined(__cplusplus)
    #define bl_static_assert_private(c,s) _Static_assert (c,s)
  #else
    #define bl_static_assert_private(c,s) static_assert (c,s)
  #endif
#endif

#include <bl/base/static_assert.h>

/* BL_WORDSIZE BL_WORDSIZE_MAX */
#if BL_ARCH_IS (X86_64)
  #if defined (__LP64__) || defined (_WIN64)
    #define BL_WORDSIZE     64
    #define BL_WORDSIZE_MAX 64
  #else
    #define BL_WORDSIZE     32
    #define BL_WORDSIZE_MAX 64
  #endif
#elif BL_ARCH_IS (X86)
  #define BL_WORDSIZE     32
  #define BL_WORDSIZE_MAX 64
#elif BL_ARCH_IS (IA64)
  #define BL_WORDSIZE     64
  #define BL_WORDSIZE_MAX 64
#elif BL_ARCH_IS (ARM)
  #if defined (__aarch64__) && (defined ( __LP64__) || defined (_WIN64))
    #define BL_WORDSIZE     64
    #define BL_WORDSIZE_MAX 64
  #else
    #define BL_WORDSIZE     32
    #define BL_WORDSIZE_MAX 64
  #endif
#elif BL_ARCH_IS (POWERPC)
  #if defined (__powerpc64__) || defined (_ARCH_PPC64)
    #define BL_WORDSIZE     64
    #define BL_WORDSIZE_MAX 64
  #else
    #define BL_WORDSIZE     32
    #define BL_WORDSIZE_MAX 64
  #endif
#else
  #error "BL_WORDSIZE not implemented for this architecture"
#endif

/* BL_PRAGMA_MESSAGE */
#if BL_COMPILER_IS (GCC) || BL_COMPILER_IS (CLANG)
  #include <bl/base/preprocessor_basic.h>
  #define BL_PRAGMA_MESSAGE(x) \
    _Pragma (bl_pp_to_str (message (x)))
#elif BL_COMPILER_IS (MICROSOFT_VC)
  #include <bl/base/preprocessor_basic.h>
  #define BL_PRAGMA_MESSAGE(x) \
    __pragma (message (__FILE__ "(" bl_pp_to_str (__LINE__) "): note: " x))
#elif BL_COMPILER_IS (INTEL)
  #include <bl/base/preprocessor_basic.h>
  #define BL_PRAGMA_MESSAGE(x) \
    __pragma (message (__FILE__ "(" bl_pp_to_str (__LINE__) "): note: " x))
#else
  #define BL_PRAGMA_MESSAGE(x)
#endif

/* C11 HEADER_AVAILABILITY (Q'n'D) */
#if BL_COMPILER_IS (GCC) && (__STDC_VERSION__ >= 201112L)
  /* this depends on the used stdlib, but given that 4.9 is very old as of 2019
  and there is no legacy to maintain this might be OK */
  #define BL_HAS_C11_STDALIGN BL_COMPILER_VER_IS_GE (4, 7, 0)
  #define BL_HAS_C11_ATOMICS  BL_COMPILER_VER_IS_GE (4, 9, 0)
  #define BL_HAS_C11_THREAD   BL_COMPILER_VER_IS_GE (4, 9, 0)
#elif __STDC_VERSION__ >= 201112L
  #define BL_HAS_C11_STDALIGN 1
  #define BL_HAS_C11_ATOMICS 1
  #define BL_HAS_C11_THREAD 1
#else
  #define BL_HAS_C11_STDALIGN 0
  #define BL_HAS_C11_ATOMICS 0
  #define BL_HAS_C11_THREAD 0
#endif

/* Regular header avialability*/
#if BL_OS_IS_MOSTLY_POSIX
  #define BL_HAS_ERRNO_H 1
#else
  /* this might have changed for e.g. Visual Studio...*/
  #define BL_HAS_ERRNO_H 0
#endif

/* printf string format checking*/
#if BL_COMPILER_IS (GCC) || BL_COMPILER_IS (CLANG)
  #define BL_PRINTF_FORMAT(string_idx, va_args_idx) \
    __attribute__ ((format (printf, string_idx, va_args_idx)))
#else
  #define BL_PRINTF_FORMAT(string_idx, va_args_idx)
#endif

/* __COUNTER__ avaiability */
#if BL_COMPILER_IS (GCC) || \
  BL_COMPILER_IS (CLANG) || \
  BL_COMPILER_IS (MICROSOFT_VC)

  #define BL_HAS___COUNTER__ 1
#else
  #define BL_HAS___COUNTER__ 0
#endif

/* visibility modifiers */
#if (BL_COMPILER_IS (GCC) && BL_COMPILER_VER_IS_GE (4, 0, 0)) || \
  BL_COMPILER_IS (CLANG)

  #define BL_VISIBILITY_DEFAULT __attribute__ ((visibility ("default")))
  #define BL_VISIBILITY_HIDDEN __attribute__ ((visibility ("hidden")))
#else
  #define BL_VISIBILITY_DEFAULT
  #define BL_VISIBILITY_HIDDEN
#endif

/* bool/null */
#ifndef __cplusplus
  #include <stdbool.h>
  #include <stddef.h>
  #define nullptr NULL
#endif

/* OS scheduling/time-quanta hint. Used on busy-wait loops */
#if !defined (BL_SCHED_TMIN_US)
  #if BL_OS_IS (WINDOWS)
    #define BL_SCHED_TMIN_US 1000
  #elif BL_OS_IS (LINUX)
    #define BL_SCHED_TMIN_US 100
  #endif
#endif
#if !defined (BL_SCHED_TMIN_US)
  #error "define this for this OS or pass it to the C preprocessor"
#endif

#endif /*_PLATFORM_H_*/
