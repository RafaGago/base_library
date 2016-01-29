#ifndef __BL_PREPROCESSOR_BASIC_H__
#define __BL_PREPROCESSOR_BASIC_H__

/*---------------------------------------------------------------------------*/
/**
 Indirection around the standard ## concatenation operator. This simply
 ensures that the arguments are expanded (once) before concatenation.
*/
/*---------------------------------------------------------------------------*/
#define pp_concat(a, ...) a ## __VA_ARGS__
#define pp_concat3(a, b, ...) a ## b ## __VA_ARGS__

#define pp_tokconcat(a, ...) pp_concat (a, __VA_ARGS__)
#define pp_tokconcat3(a, b, ...) pp_concat3 (a, __VA_ARGS__)

/*---------------------------------------------------------------------------*/
/**
 Macros which expand to common values
*/
/*---------------------------------------------------------------------------*/
#define pp_pass(...) __VA_ARGS__
#define pp_empty()
#define pp_comma() ,
#define pp_plus() +
#define pp_zero() 0
#define pp_one() 1
/*---------------------------------------------------------------------------*/
/**
 Get the first argument and ignore the rest.
*/
/*---------------------------------------------------------------------------*/
#define pp_vargs_first(a, ...) a
/*---------------------------------------------------------------------------*/
/**
 Ignore the first argument and get the rest.
*/
/*---------------------------------------------------------------------------*/
#define pp_vargs_ignore_first(a, ...) __VA_ARGS__
/*---------------------------------------------------------------------------*/
/**
 Get the second argument and ignore the rest.
*/
/*---------------------------------------------------------------------------*/
#define pp_vargs_second(a, b, ...) b
/*---------------------------------------------------------------------------*/
/**
 Ignore the second argument and get the rest.
*/
/*---------------------------------------------------------------------------*/
#define pp_vargs_ignore_second(a, b, ...) a, __VA_ARGS__

#endif /* __BL_PREPROCESSOR_BASIC_H__ */

