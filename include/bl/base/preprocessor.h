#ifndef __BL_PREPROCESSOR_H__
#define __BL_PREPROCESSOR_H__

/*
This header is a simple adaptation of the file linked below to my own 
formatting and with some personal add-ons:

https://github.com/18sg/uSHET/blob/master/lib/cpp_magic.h

The author of this header seems to be Jonathan Heathcote:

http://jhnet.co.uk/

The license of that file was:

uSHET Library
=============

Copyright (c) 2014 Thomas Nixon, Jonathan Heathcote

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT pp_not LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/*---------------------------------------------------------------------------*/
/**
 This header file contains a library of advanced C Pre-Processor (CPP) macros
 which implement various useful functions, such as iteration, in the
 pre-processor.

 Though the file name (quite validly) labels this as magic, there should be
 enough documentation in the comments for a reader only casually familiar
 with the CPP to be able to understand how everything works.

 The majority of the magic tricks used in this file are based on those
 described by pfultz2 in his "Cloak" library:

  https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms

 Major differences are a greater level of detailed explanation in this
 implementation and also a refusal to include any macros which require a O(N)
 macro definitions to handle O(N) arguments (with the exception of pp_defern).
/
/*---------------------------------------------------------------------------*/
#include <bl/base/preprocessor_basic.h>
/*---------------------------------------------------------------------------*/
/*
 Force the pre-processor to expand the macro a large number of times. Usage:

   pp_eval(expression)

 This is useful when you have a macro which evaluates to a valid macro
 expression which is not subsequently expanded in the same pass. A contrived,
 but easy to understand, example of such a macro follows. Note that though
 this example is contrived, this behaviour is abused to implement bounded
 recursion in macros such as FOR.

   #define A(x) x+1
   #define pp_empty
   #define pp_not_QUITE_RIGHT(x) A pp_empty (x)
   pp_not_QUITE_RIGHT(999)

 Here's what happens inside the C preprocessor:

 1. It sees a macro "pp_not_QUITE_RIGHT" and performs a single macro expansion
    pass on its arguments. Since the argument is "999" and this isn't a macro,
    this is a boring step resulting in no change.
 2. The pp_not_QUITE_RIGHT macro is substituted for its definition giving "A
    pp_empty() (x)".
 3. The expander moves from left-to-right trying to expand the macro:
    The first token, A, cannot be expanded since there are no brackets
    immediately following it. The second token pp_empty(), however, can be
    expanded (recursively in this manner) and is replaced with "".
 4. Expansion continues from the start of the substituted test (which in this
    case is just empty), and sees "(999)" but since no macro name is present,
    nothing is done. This results in a final expansion of "A (999)".

 Unfortunately, this doesn't quite meet expectations since you may expect that
 "A (999)" would have been expanded into "999+1". Unfortunately this requires
 a second expansion pass but luckily we can force the macro processor to make
 more passes by abusing the first step of macro expansion: the preprocessor
 expands arguments in their own pass. If we define a macro which does nothing
 except produce its arguments e.g.:

   #define pp_pass_THROUGH(...) __VA_ARGS__

 We can now do "pp_pass_THROUGH(pp_not_QUITE_RIGHT(999))" causing 
 "pp_not_QUITE_RIGHT" to be expanded to "A (999)", as described above, when the
 arguments are expanded.
 Now when the body of pp_pass_THROUGH is expanded, "A (999)" gets expanded to
 "999+1".

 The pp_eval defined below is essentially equivalent to a large nesting of
 "pp_pass_THROUGH(pp_pass_THROUGH(pp_pass_THROUGH(..." which results in the
 preprocessor making a large number of expansion passes over the given
 expression.
*/
/*---------------------------------------------------------------------------*/
#define pp_eval_1(...)    __VA_ARGS__
#define pp_eval_2(...)    pp_eval_1    (pp_eval_1    (__VA_ARGS__))
#define pp_eval_4(...)    pp_eval_2    (pp_eval_2    (__VA_ARGS__))
#define pp_eval_8(...)    pp_eval_4    (pp_eval_4    (__VA_ARGS__))
#define pp_eval_16(...)   pp_eval_8    (pp_eval_8    (__VA_ARGS__))
#define pp_eval_32(...)   pp_eval_16   (pp_eval_16   (__VA_ARGS__))
#define pp_eval_64(...)   pp_eval_32   (pp_eval_32   (__VA_ARGS__))
#define pp_eval_128(...)  pp_eval_64   (pp_eval_64   (__VA_ARGS__))
#define pp_eval_256(...)  pp_eval_128  (pp_eval_128  (__VA_ARGS__))
#define pp_eval_512(...)  pp_eval_256  (pp_eval_256  (__VA_ARGS__))
#define pp_eval_1024(...) pp_eval_512  (pp_eval_512  (__VA_ARGS__))
#define pp_eval_2048(...) pp_eval_1024 (pp_eval_1024 (__VA_ARGS__))

#ifndef BL_PP_MAX_RECURSION
  #define BL_PP_MAX_RECURSION 32
#endif

#define pp_eval(...) pp_tokconcat (pp_eval_, BL_PP_MAX_RECURSION) (__VA_ARGS__)
/*---------------------------------------------------------------------------*/
/**
 Causes a function-style macro to require an additional pass to be expanded.

 This is useful, for example, when trying to implement recursion since the
 recursive step must not be expanded in a single pass as the pre-processor
 will catch it and prevent it.

 Usage:

   pp_defer1(IN_NEXT_pp_pass)(args, to, the, macro)

 How it works:

 1. When pp_defer1 is expanded, first its arguments are expanded which are
    simply IN_NEXT_pp_pass. Since this is a function-style macro and it has no
    arguments, nothing will happen.
 2. The body of pp_defer1 will now be expanded resulting in pp_empty() being
    deleted. This results in "IN_NEXT_pp_pass (args, to, the macro)". Note that
    since the macro expander has already passed IN_NEXT_pp_pass by the time it
    expands pp_empty() and so it won't spot that the brackets which remain can
    be applied to IN_NEXT_pp_pass.
 3. At this point the macro expansion completes. If one more pass is made,
    IN_NEXT_pp_pass(args, to, the, macro) will be expanded as desired.
*/
/*---------------------------------------------------------------------------*/
#define pp_defer1(id) id pp_empty()
/*
 As with pp_defer1 except here n additional passes are required for pp_defern.

 The mechanism is analogous.

 Note that there doesn't appear to be a way of combining pp_defern macros in
 order to achieve exponentially increasing defers e.g. as is done by pp_eval.
*/
#define pp_defer2(id) id pp_empty pp_empty()()
#define pp_defer3(id) id pp_empty pp_empty pp_empty()()()
#define pp_defer4(id) id pp_empty pp_empty pp_empty pp_empty()()()()
#define pp_defer5(id) id pp_empty pp_empty pp_empty pp_empty pp_empty()()()()()
#define pp_defer6(id) id\
  pp_empty pp_empty pp_empty pp_empty pp_empty pp_empty()()()()()()
#define pp_defer7(id) id\
  pp_empty pp_empty pp_empty pp_empty pp_empty pp_empty pp_empty()()()()()()()
#define pp_defer8(id) id\
  pp_empty pp_empty pp_empty pp_empty pp_empty pp_empty pp_empty pp_empty\
  ()()()()()()()()
/*---------------------------------------------------------------------------*/
/**
 Expects a single input (not containing commas). Returns 1 if the input is
 pp_do_probe() and otherwise returns 0.

 This can be useful as the basis of a pp_not function.

 This macro abuses the fact that pp_do_probe() contains a comma while other
 valid inputs must not.
*/
/*---------------------------------------------------------------------------*/
#define pp_is_probed(...) pp_vargs_second (__VA_ARGS__, 0)
#define pp_do_probe() ~, 1
/*---------------------------------------------------------------------------*/
/**
 Logical negation. 0 is defined as false and everything else as true.

 When 0, _pp_not_0 will be found which evaluates to the pp_do_probe. When 1
 (or any other value) is given, an appropriately named macro won't be found
 and the concatenated string will be produced. pp_is_probed then simply checks
 to see if the pp_do_probe was returned, cleanly converting the argument into 
 a 1 or 0.
*/
/*---------------------------------------------------------------------------*/
#define pp_not_0 pp_do_probe()
#define pp_not(x) pp_is_probed(pp_concat(pp_not_, x))
/*---------------------------------------------------------------------------*/
/**
 Macro version of C's famous "cast to bool" operator (i.e. !!) which takes
 anything and casts it to 0 if it is 0 and 1 otherwise.
*/
/*---------------------------------------------------------------------------*/
#define pp_bool(x) pp_not (pp_not(x))
/*---------------------------------------------------------------------------*/
/**
 Logical OR. Simply performs a lookup.
*/
/*---------------------------------------------------------------------------*/
#define pp_or_00 0
#define pp_or_01 1
#define pp_or_10 1
#define pp_or_11 1
#define pp_or(a,b) pp_tokconcat3 (pp_or_, a, b)
/*---------------------------------------------------------------------------*/
/**
 Logical AND. Simply performs a lookup.
*/
#define pp_and_00 0
#define pp_and_01 0
#define pp_and_10 0
#define pp_and_11 1
#define pp_and(a,b) pp_tokconcat3 (pp_and_, a, b)
/*---------------------------------------------------------------------------*/
/**
 Macro if statement. Usage:

   pp_if(c)(expansion when true)

 Here's how:

 1. The preprocessor expands the arguments to pp_if_private casting the
    condition to '0'or '1'.
 2. The casted condition is concatencated with pp_if_private_ giving 
    pp_if_private_0 or pp_if_private_1.
 3. The pp_if_private_0 and pp_if_private_1 macros either returns the argument
    or doesn't (e.g. they implement the "choice selection" part of the macro).
 4. Note that the "true" clause is in the extra set of brackets; thus these
    become the arguments to pp_if_private_0 or pp_if_private_1 and thus a 
    selection is made!
*/
/*---------------------------------------------------------------------------*/
#define pp_if_0(...)
#define pp_if_1(...)     __VA_ARGS__
#define pp_if_private(c) pp_concat (pp_if_,c)
#define pp_if(c)         pp_if_private (pp_bool(c))
/*---------------------------------------------------------------------------*/
/**
 Macro if/else statement. Usage:

   pp_if_else(c)( \
     expansion when true, \
     expansion when false \
   )

 The mechanism is analogous to pp_if.
*/
/*---------------------------------------------------------------------------*/
#define pp_if_else_0(t,f) f
#define pp_if_else_1(t,f) t
#define pp_if_else_private(c) pp_concat (pp_if_else_,c)
#define pp_if_else(c)         pp_if_else_private (pp_bool(c))
/*---------------------------------------------------------------------------*/
/**
 Macro which checks if it has any arguments. Returns '0' if there are no
 arguments, '1' otherwise.

 Limitation: pp_has_vargs(,1,2,3) returns 0 -- this check essentially only
 checks that the first argument exists.

 This macro works as follows:

 1. pp_vargs_end is concatenated with the first argument.
 2. If the first argument is not present then only "pp_vargs_end" will
    remain, otherwise "pp_vargs_end something_here" will remain.
 3. In the former case, the pp_vargs_end() macro expands to a
    0 when it is expanded. In the latter, a non-zero result remains.
 4. pp_bool is used to force non-zero results into 1 giving the clean 0 or 1
    output required.
*/
/*---------------------------------------------------------------------------*/
#if 0 /*original*/
  #define pp_vargs_end() 0
  #define pp_has_vargs(...) pp_bool (pp_vargs_first (pp_vargs_end __VA_ARGS__)())
#else
/*this is an improvement over the original, which failed if the first argument
  contained parentheses*/
#define pp_vargs_end_fn() 0
#define pp_has_vargs(...)\
  pp_bool (pp_tokconcat (pp_vargs_first (pp_vargs_end __VA_ARGS__), _fn)())
#endif
/*---------------------------------------------------------------------------*/
/**
 Macro pp_apply/list comprehension. Usage:

   pp_apply(op, sep, ...)

 Produces a 'sep()'-separated list of the result of op(arg) for each arg.

 Example Usage:

   #define MAKE_HAPPY(x) happy_##x
   #define pp_comma() ,
   pp_apply(MAKE_HAPPY, pp_comma, 1,2,3)

 Which expands to:

    happy_1 , happy_2 , happy_3

 How it works:

 1. The pp_apply macro simply pp_applys the inner pp_apply_private function in
    a pp_eval which forces it to be expanded a large number of times, thus
    enabling many steps of iteration (see step 6).
 2. The pp_apply_private macro is substituted for its body.
 3. In the body, op(cur_val) is substituted giving the value for this
    iteration.
 4. The pp_if macro expands according to whether further iterations are 
    required.This expansion either produces pp_if_0 or pp_if_1.
 5. Since the pp_if is followed by a set of brackets containing the "if true"
    clause, these become the argument to pp_if_0 or pp_if_1. At this point,
    the macro in the brackets will be expanded giving the separator followed by
    pp_apply_private_name pp_empty()()(op, sep, __VA_ARGS__).
 5. If the pp_if was not taken, the above will simply be discarded and 
    everything stops. If the pp_if is taken, The expression is then processed a
    second time yielding "pp_apply_private_name()(op, sep, __VA_ARGS__)". Note
    that this call looks very similar to the  essentially the same as the
    original call except the first argument has been dropped.
 6. At this point expansion of pp_apply_private will terminate. However, since
    we can force more rounds of expansion using pp_eval1. In the
    argument-expansion pass of the pp_eval1, pp_apply_private_name() is
    expanded to pp_apply_private which is then expanded using the arguments
    which follow it as in step 2-5. This is followed by a second expansion pass
    as the substitution of pp_eval1() is expanded executing 2-5 a second time.
    This results in up to two iterations occurring. Using many nested pp_eval1
    macros, i.e. the very-deeply-nested pp_eval macro, will in this manner
    produce further iterations, hence the outer pp_apply macro doing this for
    us.

 Important tricks used:

 * If we directly produce "pp_apply_private" in an expansion of 
   pp_apply_private, a special case in the preprocessor will prevent it being
   expanded in the future, even if we pp_eval.  As a result, the 
   pp_apply_private macro carefully only expands to something containing   
   "pp_apply_private_name()" which requires a further expansion step to invoke
   pp_apply_private and thus implementing the recursion. 
 * To prevent pp_apply_private_name being expanded within the macro we must
   first defer its expansion during its initial pass as an argument to pp_if_0 or
   pp_if_1. We must then defer its expansion a second time as part of the body
   of the pp_if_0. As a result hence the pp_defer2.
 * pp_apply_private_name seemingly gets away with producing itself because it
   actually only
   produces pp_apply_private. It just happens that when pp_apply_private_name()
   is expanded in this case it is followed by some arguments which get consumed
   by pp_apply_private and produce a pp_apply_private_name.  As such, the macro
   expander never marks pp_apply_private_name as expanding to itself and thus
   it will still be expanded in future productions of itself.
*/
/*---------------------------------------------------------------------------*/
#define pp_apply_private_name() pp_apply_private

#define pp_apply_private(op,sep,cur_val, ...) \
  op (cur_val) \
  pp_if (pp_has_vargs (__VA_ARGS__))( \
    sep() pp_defer2 (pp_apply_private_name) () (op, sep, ##__VA_ARGS__) \
    )

#define pp_apply(...) \
  pp_if (pp_has_vargs (__VA_ARGS__)) (pp_eval (pp_apply_private (__VA_ARGS__)))
/*---------------------------------------------------------------------------*/
/**
 This is a variant of the pp_apply macro which also includes as an argument to
 the operation a valid C variable name which is different for each iteration.

 Usage:
   pp_apply_wid(op, sep, ...)

 Where op is a macro op(val, id) which takes a list value and an ID. This ID
 will simply be a unary number using the digit "I", that is, I, II, III, IIII,
 and so on.

 Example:

 #define MAKE_STATIC_VAR(type, name) static type name;
 pp_apply_wid(MAKE_STATIC_VAR, pp_empty, int, int, int, bool, char)

 Which expands to:

 static int I; static int II; static int III; static bool IIII; static char IIIII;

 The mechanism is analogous to the pp_apply macro.
*/
/*---------------------------------------------------------------------------*/
#define pp_apply_wid(op,sep,...) \
  pp_if (pp_has_vargs (__VA_ARGS__))\
   (pp_eval (pp_apply_wid_private (op, sep, I, ##__VA_ARGS__)))

#define pp_apply_wid_private_name() pp_apply_wid_private

#define pp_apply_wid_private(op,sep,id,cur_val, ...) \
  op (cur_val,id) \
  pp_if (pp_has_vargs (__VA_ARGS__))( \
    sep() pp_defer2 (pp_apply_wid_private_name)()\
     (op, sep, pp_concat (id,I), ##__VA_ARGS__) \
    )
/*---------------------------------------------------------------------------*/
/**
 This is a variant of the pp_apply macro which iterates over pairs rather than
 singletons.

 Usage:
   pp_apply_pairs(op, sep, ...)

 Where op is a macro op(val_1, val_2) which takes two list values.

 Example:

   #define MAKE_STATIC_VAR(type, name) static type name;
   pp_apply_pairs(MAKE_STATIC_VAR, pp_empty, char, my_char, int, my_int)

 Which expands to:

   static char my_char; static int my_int;

 The mechanism is analogous to the pp_apply macro.
*/
/*---------------------------------------------------------------------------*/
#define pp_apply_pairs_private_name() pp_apply_pairs_private

#define pp_apply_pairs_private(op,sep,cur_val_1, cur_val_2, ...) \
  op (cur_val_1, cur_val_2) \
  pp_if (pp_has_vargs (__VA_ARGS__))( \
    sep() pp_defer2 (pp_apply_pairs_private_name)() (op, sep, __VA_ARGS__) \
    )

#define pp_apply_pairs(op,sep,...) \
  pp_if (pp_has_vargs (__VA_ARGS__))\
    (pp_eval (pp_apply_pairs_private (op,sep,__VA_ARGS__)))
/*---------------------------------------------------------------------------*/
/**
 This is a variant of the pp_apply macro which iterates over a two-element
 sliding window.

 Usage:
   pp_apply_slide(op, last_op, sep, ...)

 Where op is a macro op(val_1, val_2) which takes the two list values
 currently in the window. last_op is a macro taking a single value which is
 called for the last argument.

 Example:

   #define SIMON_SAYS_OP(simon, next) pp_if(pp_not(simon()))(next)
   #define SIMON_SAYS_LAST_OP(val) last_but_not_least_##val
   #define SIMON_SAYS() 0

   pp_apply_slide(
    SIMON_SAYS_OP, 
    SIMON_SAYS_LAST_OP, 
    pp_empty, wiggle, SIMON_SAYS, dance, move, SIMON_SAYS, boogie, stop
    )

 Which expands to:

   dance boogie last_but_not_least_stop

 The mechanism is analogous to the pp_apply macro.
*/
/*---------------------------------------------------------------------------*/
#define pp_apply_slide_private_name() pp_apply_slide_private

#define pp_apply_slide_private(op,last_op,sep,cur_val, ...) \
  pp_if (pp_has_vargs (__VA_ARGS__))\
   (op (cur_val,pp_vargs_first (__VA_ARGS__))) \
  pp_if (pp_not (pp_has_vargs (__VA_ARGS__)))(last_op (cur_val)) \
  pp_if (pp_has_vargs (__VA_ARGS__))( \
    sep() pp_defer2 (pp_apply_slide_private_name)()\
      (op, last_op, sep, __VA_ARGS__) \
  )

#define pp_apply_slide(op,last_op,sep,...) \
  pp_if (pp_has_vargs (__VA_ARGS__))\
   (pp_eval (pp_apply_slide_private (op,last_op,sep,__VA_ARGS__)))
/*---------------------------------------------------------------------------*/
/**
 Strip any excess commas from a set of arguments.
*/
/*---------------------------------------------------------------------------*/
#define pp_remove_trailing_commas(...) \
	pp_apply (pp_pass, pp_comma, __VA_ARGS__)
/*---------------------------------------------------------------------------*/
/*END OF BORROWED "uSHET" code. Own bruteforce addons*/
/*---------------------------------------------------------------------------*/
/**
Increment/decrement the numeric token value.
*/
/*---------------------------------------------------------------------------*/
#ifndef BL_PP_MAX_INC_DEC
  #define BL_PP_MAX_INC_DEC (BL_PP_MAX_RECURSION * 2)
#endif
#include <bl/base/impl/preprocessor_inc_dec.h>
#define pp_inc(x) pp_tokconcat (pp_inc, x)
#define pp_dec(x) pp_tokconcat (pp_dec, x)
/*---------------------------------------------------------------------------*/
/*
pp_varg_count(...) : Counts va_args e.g.

  pp_varg_count (a, b, c) -> is evaluated to 3.
  pp_varg_count() -> is evaluated to 0.
*/
/*---------------------------------------------------------------------------*/
#define pp_vargs_count_private_name() pp_vargs_count_private

#define pp_vargs_count_private(cnt,cur_val, ...) \
  pp_if_else (pp_not (pp_has_vargs (__VA_ARGS__)))(\
    (cnt),\
    (pp_defer2 (pp_vargs_count_private_name) () (pp_inc (cnt), __VA_ARGS__))\
    )

#define pp_vargs_count(...) \
  pp_if_else (pp_has_vargs (__VA_ARGS__))(\
    pp_eval (pp_vargs_count_private (1, __VA_ARGS__)),\
    0\
    )
/*---------------------------------------------------------------------------*/
/*
pp_add (x, y) : Positive numeric addition

  pp_add (4, 11) -> is evaluated to 15.
*/
/*---------------------------------------------------------------------------*/
#define pp_add_private_name() pp_add_private

#define pp_add_private(x,y) \
  pp_if_else (pp_not (y))(\
    x,\
    pp_defer2 (pp_add_private_name) () (pp_inc (x), pp_dec (y))\
    )

#define pp_add(x, y) pp_eval (pp_add_private (x, y))
/*---------------------------------------------------------------------------*/
/*
pp_add (x, y) : Positive numeric substraction. Result has to be positive too.

  pp_sub (11, 4) -> 7.
*/
/*---------------------------------------------------------------------------*/
#define pp_sub_private_name() pp_sub_private

#define pp_sub_private(x,y) \
  pp_if_else (pp_not (y))(\
    x,\
    pp_defer2 (pp_sub_private_name) () (pp_dec (x), pp_dec (y))\
    )

#define pp_sub(x, y) pp_eval (pp_sub_private (x, y))
/*----------------------------------------------------------------------------*/
#endif /* __BL_PREPROCESSOR_H__ */
