#ifndef __BL_PREPROCESSOR_BASIC_H__
#define __BL_PREPROCESSOR_BASIC_H__

#include <bl/base/compiler.h>

/*---------------------------------------------------------------------------*/
/**
 Convert to string: bl_pp_str(3) => "3"
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_to_str_private(v) #v
#define bl_pp_to_str(v) bl_pp_to_str_private(v)
/*---------------------------------------------------------------------------*/
/**
 Indirection around the standard ## concatenation operator. This simply
 ensures that the arguments are expanded (once) before concatenation.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_concat(a, ...) a ## __VA_ARGS__
#define bl_pp_concat3(a, b, ...) a ## b ## __VA_ARGS__

#define bl_pp_tokconcat(a, ...) bl_pp_concat (a, __VA_ARGS__)
#define bl_pp_tokconcat3(a, b, ...) bl_pp_concat3 (a, __VA_ARGS__)

/*---------------------------------------------------------------------------*/
/**
 Macros which expand to common values
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_pass2(a, b) a b
#define bl_pp_pass(...) __VA_ARGS__
#define bl_pp_empty()
#define bl_pp_comma() ,
#define bl_pp_plus() +
#define bl_pp_zero() 0
#define bl_pp_one() 1
#define bl_pp_colon() :
#define bl_pp_semicolon() ;
#define bl_pp_minus() -
#define bl_pp_asterisk() *
#define bl_pp_slash() /
#define bl_pp_ampersand() &
#define bl_pp_pipe() |
#define bl_pp_lt() <
#define bl_pp_gt() >
#define bl_pp_equal() =

/*---------------------------------------------------------------------------*/
/**
 Get the first argument and ignore the rest.
*/
/*---------------------------------------------------------------------------*/
#if !BL_COMPILER_IS (MICROSOFT_VC)
    #define bl_pp_vargs_first(a, ...) a
#else
    #define bl_pp_vargs_first_impl(a, ...) a
    #define bl_pp_vargs_first(...) \
        bl_pp_pass2 (bl_pp_vargs_first_impl, (__VA_ARGS__, dummy))
#endif
/*---------------------------------------------------------------------------*/
/**
 Ignore the first argument and get the rest.
*/
/*---------------------------------------------------------------------------*/
#if !BL_COMPILER_IS (MICROSOFT_VC)
    #define bl_pp_vargs_ignore_first(a, ...) __VA_ARGS__
#else
    #define bl_pp_vargs_ignore_first_impl(a, ...) __VA_ARGS__
    #define bl_pp_vargs_ignore_first(...) \
        bl_pp_pass2 (bl_pp_vargs_ignore_first_impl, (__VA_ARGS__))
#endif
/*---------------------------------------------------------------------------*/
/**
 Get the second argument and ignore the rest.
*/
/*---------------------------------------------------------------------------*/
#if !BL_COMPILER_IS (MICROSOFT_VC)
    #define bl_pp_vargs_second(a, b, ...) b
#else
    #define bl_pp_vargs_second_impl(a, b, ...) b
    #define bl_pp_vargs_second(...) \
        bl_pp_pass2 (bl_pp_vargs_second_impl, (__VA_ARGS__))
#endif
/*---------------------------------------------------------------------------*/
/**
 Ignore the second argument and get the rest.
*/
/*---------------------------------------------------------------------------*/
#if !BL_COMPILER_IS (MICROSOFT_VC)
    #define bl_pp_vargs_ignore_second(a, b, ...) a, __VA_ARGS__
#else
    #define bl_pp_vargs_ignore_second_impl(a, b, ...) a, __VA_ARGS__
    #define bl_pp_vargs_ignore_second(...) \
        bl_pp_pass2 (bl_pp_vargs_ignore_second_impl, (__VA_ARGS__))
#endif

#define bl_pp_nonzero_vargs_count_impl( \
  a00, a01, a02, a03, a04, a05, a06, a07, a08, a09, \
  a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, \
  a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, \
  a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, \
  a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, \
  a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, \
  a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, \
  a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, \
  a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, \
  a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, \
  size, \
  ... \
  ) size

/* notice: bl_pp_pass2 is only required for MSVC, but I prefer to always add it
to avoid copying the number sequence. Adding the number sequence itself as a
macro would require expansion too as does __VA_ARGS__ on MSVC */
#define bl_pp_nonzero_vargs_count(...) \
  bl_pp_pass2( \
    bl_pp_nonzero_vargs_count_impl, (\
    __VA_ARGS__, \
    99, 98, 97, 96, 95, 94, 93, 92, 91, 90, \
    89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
    79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
    69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
    9,  8,  7,  6,  5,  4,  3,  2,  1, \
    ))

#endif /* __BL_PREPROCESSOR_BASIC_H__ */
