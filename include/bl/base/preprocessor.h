#ifndef __BL_PREPROCESSOR_H__
#define __BL_PREPROCESSOR_H__

/*
This header is a simple adaptation of the file linked below to my own
formatting and with some personal add-ons:

https://github.com/18sg/uSHET/blob/master/lib/cbl_pp_magic.h

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
IMPLIED, INCLUDING BUT bl_pp_not LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
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
 macro definitions to handle O(N) arguments (with the exception of
 bl_pp_defern).
*/
/*---------------------------------------------------------------------------*/
#include <bl/base/preprocessor_basic.h>
#if BL_COMPILER_IS (MICROSOFT_VC)
  #error "Microsoft VC preprocessor is broken. This header hasn't been ported"
#endif
/*---------------------------------------------------------------------------*/
/*
 Force the pre-processor to expand the macro a large number of times. Usage:

   bl_pp_eval(expression)

 This is useful when you have a macro which evaluates to a valid macro
 expression which is not subsequently expanded in the same pass. A contrived,
 but easy to understand, example of such a macro follows. Note that though
 this example is contrived, this behaviour is abused to implement bounded
 recursion in macros such as FOR.

   #define A(x) x+1
   #define bl_pp_empty
   #define bl_pp_not_QUITE_RIGHT(x) A bl_pp_empty (x)
   bl_pp_not_QUITE_RIGHT(999)

 Here's what happens inside the C preprocessor:

 1. It sees a macro "bl_pp_not_QUITE_RIGHT" and performs a single macro
    expansion pass on its arguments. Since the argument is "999" and this isn't
    a macro, this is a boring step resulting in no change.
 2. The bl_pp_not_QUITE_RIGHT macro is substituted for its definition giving "A
    bl_pp_empty() (x)".
 3. The expander moves from left-to-right trying to expand the macro:
    The first token, A, cannot be expanded since there are no brackets
    immediately following it. The second token bl_pp_empty(), however, can be
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

   #define bl_pp_pass_THROUGH(...) __VA_ARGS__

 We can now do "bl_pp_pass_THROUGH(bl_pp_not_QUITE_RIGHT(999))" causing
 "bl_pp_not_QUITE_RIGHT" to be expanded to "A (999)", as described above, when the
 arguments are expanded.
 Now when the body of bl_pp_pass_THROUGH is expanded, "A (999)" gets expanded to
 "999+1".

 The bl_pp_eval defined below is essentially equivalent to a large nesting of
 "bl_pp_pass_THROUGH(bl_pp_pass_THROUGH(bl_pp_pass_THROUGH(..." which results in
 the preprocessor making a large number of expansion passes over the given
 expression.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_eval_1(...)    __VA_ARGS__
#define bl_pp_eval_2(...)    bl_pp_eval_1    (bl_pp_eval_1    (__VA_ARGS__))
#define bl_pp_eval_4(...)    bl_pp_eval_2    (bl_pp_eval_2    (__VA_ARGS__))
#define bl_pp_eval_8(...)    bl_pp_eval_4    (bl_pp_eval_4    (__VA_ARGS__))
#define bl_pp_eval_16(...)   bl_pp_eval_8    (bl_pp_eval_8    (__VA_ARGS__))
#define bl_pp_eval_32(...)   bl_pp_eval_16   (bl_pp_eval_16   (__VA_ARGS__))
#define bl_pp_eval_64(...)   bl_pp_eval_32   (bl_pp_eval_32   (__VA_ARGS__))
#define bl_pp_eval_128(...)  bl_pp_eval_64   (bl_pp_eval_64   (__VA_ARGS__))
#define bl_pp_eval_256(...)  bl_pp_eval_128  (bl_pp_eval_128  (__VA_ARGS__))
#define bl_pp_eval_512(...)  bl_pp_eval_256  (bl_pp_eval_256  (__VA_ARGS__))
#define bl_pp_eval_1024(...) bl_pp_eval_512  (bl_pp_eval_512  (__VA_ARGS__))
#define bl_pp_eval_2048(...) bl_pp_eval_1024 (bl_pp_eval_1024 (__VA_ARGS__))

#ifndef BL_PP_MAX_RECURSION
  #define BL_PP_MAX_RECURSION 32
#endif

#define bl_pp_eval(...) \
  bl_pp_tokconcat (bl_pp_eval_, BL_PP_MAX_RECURSION) (__VA_ARGS__)
/*---------------------------------------------------------------------------*/
/**
 Causes a function-style macro to require an additional pass to be expanded.

 This is useful, for example, when trying to implement recursion since the
 recursive step must not be expanded in a single pass as the pre-processor
 will catch it and prevent it.

 Usage:

   bl_pp_defer1(IN_NEXT_bl_pp_pass)(args, to, the, macro)

 How it works:

 1. When bl_pp_defer1 is expanded, first its arguments are expanded which are
    simply IN_NEXT_bl_pp_pass. Since this is a function-style macro and it has
    no arguments, nothing will happen.
 2. The body of bl_pp_defer1 will now be expanded resulting in bl_pp_empty()
    being deleted. This results in "IN_NEXT_bl_pp_pass (args, to, the macro)".
    Note that since the macro expander has already passed IN_NEXT_bl_pp_pass by
    the time it expands bl_pp_empty() and so it won't spot that the brackets
    which remain can be applied to IN_NEXT_bl_pp_pass.
 3. At this point the macro expansion completes. If one more pass is made,
    IN_NEXT_bl_pp_pass(args, to, the, macro) will be expanded as desired.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_defer1(id) id bl_pp_empty()
/*
 As with bl_pp_defer1 except here n additional passes are required for
 bl_pp_defer.

 The mechanism is analogous.

 Note that there doesn't appear to be a way of combining bl_pp_defern macros in
 order to achieve exponentially increasing defers e.g. as is done by bl_pp_eval.
*/
#define bl_pp_defer2(id) id bl_pp_empty bl_pp_empty()()
#define bl_pp_defer3(id) id bl_pp_empty bl_pp_empty bl_pp_empty()()()
#define bl_pp_defer4(id) \
  id bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty()()()()
#define bl_pp_defer5(id) \
  id bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty()()()()()
#define bl_pp_defer6(id) id\
  bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty \
  bl_pp_empty()()()()()()
#define bl_pp_defer7(id) id\
  bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty \
  bl_pp_empty()()()()()()()
#define bl_pp_defer8(id) id\
  bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty bl_pp_empty \
  bl_pp_empty bl_pp_empty ()()()()()()()()
/*---------------------------------------------------------------------------*/
/**
 Expects a single input (not containing commas). Returns 1 if the input is
 bl_pp_do_probe() and otherwise returns 0.

 This can be useful as the basis of a bl_pp_not function.

 This macro abuses the fact that bl_pp_do_probe() contains a comma while other
 valid inputs must not.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_is_probed(...) bl_pp_vargs_second (__VA_ARGS__, 0)
#define bl_pp_do_probe() ~, 1
/*---------------------------------------------------------------------------*/
/**
 Logical negation. 0 is defined as false and everything else as true.

 When 0, _bl_pp_not_0 will be found which evaluates to the bl_pp_do_probe. When
 1 (or any other value) is given, an appropriately named macro won't be found
 and the concatenated string will be produced. bl_pp_is_probed then simply
 checks to see if the bl_pp_do_probe was returned, cleanly converting the
 argument into a 1 or 0.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_not_0 bl_pp_do_probe()
#define bl_pp_not(x) bl_pp_is_probed(bl_pp_concat(bl_pp_not_, x))
/*---------------------------------------------------------------------------*/
/**
 Macro version of C's famous "cast to bool" operator (i.e. !!) which takes
 anything and casts it to 0 if it is 0 and 1 otherwise.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_bool(x) bl_pp_not (bl_pp_not(x))
/*---------------------------------------------------------------------------*/
/**
 Logical OR. Simply performs a lookup.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_or_00 0
#define bl_pp_or_01 1
#define bl_pp_or_10 1
#define bl_pp_or_11 1
#define bl_pp_or(a,b) bl_pp_tokconcat3 (bl_pp_or_, a, b)
/*---------------------------------------------------------------------------*/
/**
 Logical AND. Simply performs a lookup.
*/
#define bl_pp_and_00 0
#define bl_pp_and_01 0
#define bl_pp_and_10 0
#define bl_pp_and_11 1
#define bl_pp_and(a,b) bl_pp_tokconcat3 (bl_pp_and_, a, b)
/*---------------------------------------------------------------------------*/
/**
 Macro if statement. Usage:

   bl_pp_if(c)(expansion when true)

 Here's how:

 1. The preprocessor expands the arguments to bl_pp_if_private casting the
    condition to '0'or '1'.
 2. The casted condition is concatencated with bl_pp_if_private_ giving
    bl_pp_if_private_0 or bl_pp_if_private_1.
 3. The bl_pp_if_private_0 and bl_pp_if_private_1 macros either returns the
    argument or doesn't (e.g. they implement the "choice selection" part of the
    macro).
 4. Note that the "true" clause is in the extra set of brackets; thus these
    become the arguments to bl_pp_if_private_0 or bl_pp_if_private_1 and thus a
    selection is made!
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_if_0(...)
#define bl_pp_if_1(...)     __VA_ARGS__
#define bl_pp_if_private(c) bl_pp_concat (bl_pp_if_,c)
#define bl_pp_if(c)         bl_pp_if_private (bl_pp_bool(c))
/*---------------------------------------------------------------------------*/
/**
 Macro if/else statement. Usage:

   bl_pp_if_else(c)( \
     expansion when true, \
     expansion when false \
   )

 The mechanism is analogous to bl_pp_if.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_if_else_0(t,f) f
#define bl_pp_if_else_1(t,f) t
#define bl_pp_if_else_private(c) bl_pp_concat (bl_pp_if_else_,c)
#define bl_pp_if_else(c)         bl_pp_if_else_private (bl_pp_bool(c))
/*---------------------------------------------------------------------------*/
/*
 Macro which checks if it has any arguments. Returns '0' if there are no
 arguments, '1' otherwise.

 Limitation: bl_pp_has_vargs(,1,2,3) returns 0 -- this check essentially only
 checks that the first argument exists.

 This macro works as follows:

 1. bl_pp_vargs_end is concatenated with the first argument.
 2. If the first argument is not present then only "bl_pp_vargs_end" will
    remain, otherwise "bl_pp_vargs_end something_here" will remain.
 3. In the former case, the bl_pp_vargs_end() macro expands to a
    0 when it is expanded. In the latter, a non-zero result remains.
 4. bl_pp_bool is used to force non-zero results into 1 giving the clean 0 or 1
    output required.
*/
/*---------------------------------------------------------------------------*/
#if 0 /*original*/
  #define bl_pp_vargs_end() 0
  #define bl_pp_has_vargs(...) bl_pp_bool (bl_pp_vargs_first \
     (bl_pp_vargs_end __VA_ARGS__)())
#else
  /*this is an improvement over the original, which failed if the first argument
  contained parentheses. The changes have been contributed back. E.g. with this
  input failed:

  > (void*) value

  This would result in:

  > bl_pp_vargs_end (void*) value

  And the preprocessor complaining that the function takes one parameter.

  Now there are two addons:

  -bl_pp_vargs_end is variadic too and casts to bool the first passed argument.
  -The invocation to bl_pp_vargs_end adds a 0 as an argument.

  This results in any invocation with a (0) between parentheses succeeding.

  Note that this function will break in presence of code like the next, as it's
  code is valid C:

  > (0) + variable_name

  In practice IMO that code should be rare, it requires arithmetic to a zero
  between parentheses.
  */
  #define bl_pp_vargs_end(...) bl_pp_bool (bl_pp_vargs_first (__VA_ARGS__))
  #define bl_pp_has_vargs(...)\
    bl_pp_bool (bl_pp_vargs_first (bl_pp_vargs_end __VA_ARGS__)(0))
#endif
/*---------------------------------------------------------------------------*/
/**
 Macro bl_pp_apply/list comprehension. Usage:

   bl_pp_apply(op, sep, ...)

 Produces a 'sep()'-separated list of the result of op(arg) for each arg.

 Example Usage:

   #define MAKE_HAPPY(x) happy_##x
   #define bl_pp_comma() ,
   bl_pp_apply(MAKE_HAPPY, bl_pp_comma, 1,2,3)

 Which expands to:

    happy_1 , happy_2 , happy_3

 How it works:

 1. The bl_pp_apply macro simply bl_pp_applys the inner bl_pp_apply_private
    function in a bl_pp_eval which forces it to be expanded a large number of
    times, thus enabling many steps of iteration (see step 6).
 2. The bl_pp_apply_private macro is substituted for its body.
 3. In the body, op(cur_val) is substituted giving the value for this
    iteration.
 4. The bl_pp_if macro expands according to whether further iterations are
    required.This expansion either produces bl_pp_if_0 or bl_pp_if_1.
 5. Since the bl_pp_if is followed by a set of brackets containing the "if
    true"clause, these become the argument to bl_pp_if_0 or bl_pp_if_1. At this
    point, the macro in the brackets will be expanded giving the separator
    followed by bl_pp_apply_private_name bl_pp_empty()()(op, sep, __VA_ARGS__).
 5. If the bl_pp_if was not taken, the above will simply be discarded and
    everything stops. If the bl_pp_if is taken, The expression is then processed
    a second time yielding "bl_pp_apply_private_name()(op, sep, __VA_ARGS__)".
    Note that this call looks very similar to the  essentially the same as the
    original call except the first argument has been dropped.
 6. At this point expansion of bl_pp_apply_private will terminate. However,
    since we can force more rounds of expansion using bl_pp_eval1. In the
    argument-expansion pass of the bl_pp_eval1, bl_pp_apply_private_name() is
    expanded to bl_pp_apply_private which is then expanded using the arguments
    which follow it as in step 2-5. This is followed by a second expansion pass
    as the substitution of bl_pp_eval1() is expanded executing 2-5 a second
    time. This results in up to two iterations occurring. Using many nested
    bl_pp_eval1 macros, i.e. the very-deeply-nested bl_pp_eval macro, will in
    this manner produce further iterations, hence the outer bl_pp_apply macro
    doing this for us.

 Important tricks used:

 * If we directly produce "bl_pp_apply_private" in an expansion of
   bl_pp_apply_private, a special case in the preprocessor will prevent it being
   expanded in the future, even if we bl_pp_eval.  As a result, the
   bl_pp_apply_private macro carefully only expands to something containing
   "bl_pp_apply_private_name()" which requires a further expansion step to
   invoke bl_pp_apply_private and thus implementing the recursion.
 * To prevent bl_pp_apply_private_name being expanded within the macro we must
   first defer its expansion during its initial pass as an argument to
   bl_pp_if_0 or bl_pp_if_1. We must then defer its expansion a second time as
   part of the body of the bl_pp_if_0. As a result hence the bl_pp_defer2.
 * bl_pp_apply_private_name seemingly gets away with producing itself because it
   actually only
   produces bl_pp_apply_private. It just happens that when
   bl_pp_apply_private_name() is expanded in this case it is followed by some
   arguments which get consumed by bl_pp_apply_private and produce a
   bl_pp_apply_private_name.  As such, the macro expander never marks
   bl_pp_apply_private_name as expanding to itself and thus it will still be
   expanded in future productions of itself.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_apply_private_name() bl_pp_apply_private

#define bl_pp_apply_private(op,sep,cur_val, ...) \
  op (cur_val) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))( \
    sep() bl_pp_defer2 (bl_pp_apply_private_name) () (op, sep, ##__VA_ARGS__) \
    )

#define bl_pp_apply(...) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__)) \
    (bl_pp_eval (bl_pp_apply_private (__VA_ARGS__)))
/*---------------------------------------------------------------------------*/
/**
 This is a variant of the bl_pp_apply macro which also includes as an argument
 to the operation a valid C variable name which is different for each iteration.

 Usage:
   bl_pp_apply_wid(op, sep, ...)

 Where op is a macro op(val, id) which takes a list value and an ID. This ID
 will simply be a unary number using the digit "I", that is, I, II, III, IIII,
 and so on.

 Example:

 #define MAKE_STATIC_VAR(type, name) static type name;
 bl_pp_apply_wid(MAKE_STATIC_VAR, bl_pp_empty, int, int, int, bool, char)

 Which expands to:

 static int I; static int II; static int III; static bool IIII; ...

 The mechanism is analogous to the bl_pp_apply macro.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_apply_wid(op,sep,...) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))\
   (bl_pp_eval (bl_pp_apply_wid_private (op, sep, I, ##__VA_ARGS__)))

#define bl_pp_apply_wid_private_name() bl_pp_apply_wid_private

#define bl_pp_apply_wid_private(op,sep,id,cur_val, ...) \
  op (cur_val,id) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))( \
    sep() bl_pp_defer2 (bl_pp_apply_wid_private_name)()\
     (op, sep, bl_pp_concat (id,I), ##__VA_ARGS__) \
    )
/*---------------------------------------------------------------------------*/
/**
 This is a variant of the bl_pp_apply macro which iterates over pairs rather
 than singletons.

 Usage:
   bl_pp_apply_pairs(op, sep, ...)

 Where op is a macro op(val_1, val_2) which takes two list values.

 Example:

   #define MAKE_STATIC_VAR(type, name) static type name;
   bl_pp_apply_pairs(MAKE_STATIC_VAR, bl_pp_empty, char, my_char, int, my_int)

 Which expands to:

   static char my_char; static int my_int;

 The mechanism is analogous to the bl_pp_apply macro.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_apply_pairs_private_name() bl_pp_apply_pairs_private

#define bl_pp_apply_pairs_private(op,sep,cur_val_1, cur_val_2, ...) \
  op (cur_val_1, cur_val_2) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))( \
    sep() bl_pp_defer2 (bl_pp_apply_pairs_private_name)() (op, sep, __VA_ARGS__) \
    )

#define bl_pp_apply_pairs(op,sep,...) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))\
    (bl_pp_eval (bl_pp_apply_pairs_private (op,sep,__VA_ARGS__)))
/*---------------------------------------------------------------------------*/
/**
 This is a variant of the bl_pp_apply macro which iterates over a two-element
 sliding window.

 Usage:
   bl_pp_apply_slide(op, last_op, sep, ...)

 Where op is a macro op(val_1, val_2) which takes the two list values
 currently in the window. last_op is a macro taking a single value which is
 called for the last argument.

 Example:

   #define SIMON_SAYS_OP(simon, next) bl_pp_if(bl_pp_not(simon()))(next)
   #define SIMON_SAYS_LAST_OP(val) last_but_not_least_##val
   #define SIMON_SAYS() 0

   bl_pp_apply_slide(
    SIMON_SAYS_OP,
    SIMON_SAYS_LAST_OP,
    bl_pp_empty, wiggle, SIMON_SAYS, dance, move, SIMON_SAYS, boogie, stop
    )

 Which expands to:

   dance boogie last_but_not_least_stop

 The mechanism is analogous to the bl_pp_apply macro.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_apply_slide_private_name() bl_pp_apply_slide_private

#define bl_pp_apply_slide_private(op,last_op,sep,cur_val, ...) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))\
   (op (cur_val,bl_pp_vargs_first (__VA_ARGS__))) \
  bl_pp_if (bl_pp_not (bl_pp_has_vargs (__VA_ARGS__)))(last_op (cur_val)) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))( \
    sep() bl_pp_defer2 (bl_pp_apply_slide_private_name)()\
      (op, last_op, sep, __VA_ARGS__) \
  )

#define bl_pp_apply_slide(op,last_op,sep,...) \
  bl_pp_if (bl_pp_has_vargs (__VA_ARGS__))\
   (bl_pp_eval (bl_pp_apply_slide_private (op,last_op,sep,__VA_ARGS__)))
/*---------------------------------------------------------------------------*/
/**
 Strip any excess commas from a set of arguments.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_remove_trailing_commas(...) \
	bl_pp_apply (bl_pp_pass, bl_pp_comma, __VA_ARGS__)
/*---------------------------------------------------------------------------*/
/*END OF BORROWED "uSHET" code. Own addons*/
/*---------------------------------------------------------------------------*/
#define bl_pp_vargs_count(...) \
  bl_pp_if_else (bl_pp_has_vargs (__VA_ARGS__))(\
    bl_pp_eval (bl_pp_nonzero_vargs_count (__VA_ARGS__)),\
    0\
    )

/*---------------------------------------------------------------------------*/
/**
Increment/decrement the numeric token value.
*/
/*---------------------------------------------------------------------------*/
#ifndef BL_PP_MAX_INC_DEC
  #define BL_PP_MAX_INC_DEC (BL_PP_MAX_RECURSION * 2)
#endif
#include <bl/base/impl/preprocessor_inc_dec.h>
#define bl_pp_inc(x) bl_pp_tokconcat (bl_pp_inc, x)
#define bl_pp_dec(x) bl_pp_tokconcat (bl_pp_dec, x)
/*---------------------------------------------------------------------------*/
/*
bl_pp_varg_count_arith(...) : Counts va_args e.g.

  bl_pp_varg_count (a, b, c) -> is evaluated to 3.
  bl_pp_varg_count() -> is evaluated to 0.

  This function allows as many arguments as BL_PP_MAX_INC_DEC
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_vargs_count_arith_private_name() bl_pp_vargs_count_arith_private

#define bl_pp_vargs_count_arith_private(cnt,cur_val, ...) \
  bl_pp_if_else (bl_pp_not (bl_pp_has_vargs (__VA_ARGS__)))(\
    (cnt),\
    (bl_pp_defer2 (bl_pp_vargs_count_arith_private_name) () (\
      bl_pp_inc (cnt), __VA_ARGS__) \
      )\
    )

#define bl_pp_vargs_count_arith(...) \
  bl_pp_if_else (bl_pp_has_vargs (__VA_ARGS__))(\
    bl_pp_eval (bl_pp_vargs_count_arith_private (1, __VA_ARGS__)),\
    0\
    )
/*---------------------------------------------------------------------------*/
/*
bl_pp_vargs_last(...) : Gets the last var arg e.g.

  bl_pp_varg_count (empty) -> is evaluated to "empty".
  bl_pp_varg_count(empty, 0, 1, 2) -> is evaluated to 2.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_vargs_last_private_name() bl_pp_vargs_last_private

#define bl_pp_vargs_last_private(cur_val, ...) \
  bl_pp_if_else (bl_pp_not (bl_pp_has_vargs (__VA_ARGS__)))(\
    (cur_val),\
    (bl_pp_defer2 (bl_pp_vargs_last_private_name) () (__VA_ARGS__))\
    )

#define bl_pp_vargs_last(on_empty_val, ...) \
  bl_pp_if_else (bl_pp_has_vargs (__VA_ARGS__))(\
    bl_pp_eval (bl_pp_vargs_last_private (__VA_ARGS__)),\
    (on_empty_val)\
    )
/*---------------------------------------------------------------------------*/
/*
bl_pp_add (x, y) : Positive numeric addition

  bl_pp_add (4, 11) -> is evaluated to 15.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_add_private_name() bl_pp_add_private

#define bl_pp_add_private(x,y) \
  bl_pp_if_else (bl_pp_not (y))(\
    x,\
    bl_pp_defer2 (bl_pp_add_private_name) () (bl_pp_inc (x), bl_pp_dec (y))\
    )

#define bl_pp_add(x, y) bl_pp_eval (bl_pp_add_private (x, y))
/*---------------------------------------------------------------------------*/
/*
bl_pp_add (x, y) : Positive numeric substraction. Result has to be positive too.

  bl_pp_sub (11, 4) -> 7.
*/
/*---------------------------------------------------------------------------*/
#define bl_pp_sub_private_name() bl_pp_sub_private

#define bl_pp_sub_private(x,y) \
  bl_pp_if_else (bl_pp_not (y))(\
    x,\
    bl_pp_defer2 (bl_pp_sub_private_name) () (bl_pp_dec (x), bl_pp_dec (y))\
    )

#define bl_pp_sub(x, y) bl_pp_eval (bl_pp_sub_private (x, y))
/*----------------------------------------------------------------------------*/
#endif /* __BL_PREPROCESSOR_H__ */
