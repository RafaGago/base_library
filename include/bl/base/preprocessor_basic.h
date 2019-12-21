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
    #define bl_pp_msvc_workaround_fwd(fwdmacro, args) \
        fwdmacro args
    #define bl_pp_vargs_first_impl(a, ...) a
    #define bl_pp_vargs_first(...) \
        bl_pp_msvc_workaround_fwd (bl_pp_vargs_first_impl, (__VA_ARGS__, dummy))
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
        bl_pp_msvc_workaround_fwd (bl_pp_vargs_ignore_first_impl, (__VA_ARGS__))
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
        bl_pp_msvc_workaround_fwd (bl_pp_vargs_second_impl, (__VA_ARGS__))
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
        bl_pp_msvc_workaround_fwd( \
            bl_pp_vargs_ignore_second_impl, (__VA_ARGS__) \
            )
#endif

#endif /* __BL_PREPROCESSOR_BASIC_H__ */
