/*These are the FreeBSD trees slightly reformatted and in lower case*/


/*  $NetBSD: tree.h,v 1.8 2004/03/28 19:38:30 provos Exp $  */
/*  $OpenBSD: tree.h,v 1.7 2002/10/17 21:51:54 art Exp $  */
/* $FreeBSD$ */

/*-
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef  BL_BSD_SYS_TREE_H_
#define  BL_BSD_SYS_TREE_H_

#include <bl/base/platform.h>

/*---------------------------------------------------------------------------*/
/* manpage copy (remember that we use lower case)

DESCRIPTION
  These macros define data structures for different types of trees: splay
  trees and red-black trees.

  In the macro definitions, TYPE is the name tag of a user defined structure
  that must contain a field of type SPLAY_ENTRY, or RB_ENTRY, named ENTRYNAME.
  The argument HEADNAME is the name tag of a user defined structure that must be
  declared using the macros SPLAY_HEAD(), or RB_HEAD(). The argument NAME has
  to be a unique name prefix for every tree that is defined.

  The function prototypes are declared with SPLAY_PROTOTYPE(), RB_PROTOTYPE(), or
  RB_PROTOTYPE_STATIC().  The function bodies are generated with
  SPLAY_GENERATE(), RB_GENERATE(), or RB_GENERATE_STATIC().  See the examples
  below for further explanation ofhow these macros are used.

SPLAY TREES
  A splay tree is a self-organizing data structure.	Every operation on the tree
  causes a splay to happen.  The splay moves the requested node to the root of
  the tree and partly rebalances it.

  This has the benefit that request locality causes faster lookups as the
  requested nodes move to the top of the tree.  On the other hand, every lookup
  causes memory writes.

  The Balance Theorem bounds the total access time for m operations and n
  inserts on an initially empty tree as O((m + n)lg n).  The amortized cost for
  a sequence of m accesses to a splay tree is O(lg n).

  A splay tree is headed by a structure defined by the SPLAY_HEAD() macro.  A
  structure is declared as follows:

	   SPLAY_HEAD(HEADNAME, TYPE) head;

  where HEADNAME is the name of the structure to be defined, and struct TYPE is
  the type of the elements to be inserted into the tree.

  The SPLAY_ENTRY() macro declares a structure that allows elements to be
  connected in the tree.

  In order to use the functions that manipulate the tree structure, their
  prototypes need to be declared with the SPLAY_PROTOTYPE() macro, where NAME is
  a unique identifier for this particular tree.  The TYPE argument is the type
  of the structure that is being managed by the tree.	The FIELD argument is the
  name of the element defined by SPLAY_ENTRY().

  The function bodies are generated with the SPLAY_GENERATE() macro.  It takes
  the same arguments as the SPLAY_PROTOTYPE() macro, but should be used only
  once.

  Finally, the CMP argument is the name of a function used to compare tree nodes
  with each other.  The function takes two arguments of type struct TYPE *.  If
  the first argument is smaller than the second, the function returns a value
  smaller than zero. If they are equal, the function returns zero.  Otherwise,
  it should return a value greater than zero.  The compare function defines the
  order of the tree elements.

  The SPLAY_INIT() macro initializes the tree referenced by head.

  The splay tree can also be initialized statically by using the
  SPLAY_INITIALIZER() macro like this:

	   SPLAY_HEAD(HEADNAME, TYPE) head = SPLAY_INITIALIZER(&head);

  The SPLAY_INSERT() macro inserts the new element elm into the tree.

  The SPLAY_REMOVE() macro removes the element elm from the tree pointed by
    head.

  The SPLAY_FIND() macro can be used to find a particular element in the tree.

	   struct TYPE find, *res;
	   find.key = 30;
	   res = SPLAY_FIND(NAME, head, &find);

  The SPLAY_ROOT(), SPLAY_MIN(), SPLAY_MAX(), and SPLAY_NEXT() macros can be
  used to traverse the tree:

	   for(
      np = SPLAY_MIN(NAME, &head);
      np != NULL; np =
      SPLAY_NEXT(NAME, &head, np)
      )

  Or, for simplicity, one can use the SPLAY_FOREACH() macro:

	   SPLAY_FOREACH(np, NAME, head)

  The SPLAY_EMPTY() macro should be used to check whether a splay tree is empty.

RED-BLACK TREES
  A red-black tree is a binary search tree with the node color as an extra
  attribute.  It fulfills a set of conditions:

	   1.	Every search path from the root to a leaf consists of the same number of
       black nodes.

	   2.	Each red node (except for the root) has a black parent.

	   3.	Each leaf node is black.

  Every operation on a red-black tree is bounded as O(lg n).  The maximum height
  of a red-black tree is 2lg(n + 1).

  A red-black tree is headed by a structure defined by the RB_HEAD() macro.	A
  structure is declared as follows:

	   RB_HEAD(HEADNAME, TYPE) head;

  where HEADNAME is the name of the structure to be defined, and struct TYPE is
  the type of the elements to be inserted into the tree.

  The RB_ENTRY() macro declares a structure that allows elements to be connected
  in the tree.

  In order to use the functions that manipulate the tree structure, their
  prototypes need to be declared with the RB_PROTOTYPE() or
  RB_PROTOTYPE_STATIC() macro, where NAME is a unique identifier for this
  particular tree.  The TYPE argument is the type of the structure that is
  being managed by the tree.  The FIELD argument is the name of the element
  defined by RB_ENTRY().  Individual prototypes can be declared with
  RB_PROTOTYPE_INSERT(), RB_PROTOTYPE_INSERT_COLOR(), RB_PROTOTYPE_REMOVE(),
  RB_PROTOTYPE_REMOVE_COLOR(), RB_PROTOTYPE_FIND(), RB_PROTOTYPE_NFIND(),
  RB_PROTOTYPE_NEXT(), RB_PROTOTYPE_PREV(), and RB_PROTOTYPE_MINMAX() in case
  not all functions are required.  The individual prototype macros expect NAME,
  TYPE, and ATTR arguments.  The ATTR argument must be empty for global
  functions or static for static functions.

  The function bodies are generated with the RB_GENERATE() or
  RB_GENERATE_STATIC() macro. These macros take the same arguments as the
  RB_PROTOTYPE() and RB_PROTOTYPE_STATIC() macros, but should be used only once.
  As an alternative individual function bodies are generated with the
  RB_GENERATE_INSERT(), RB_GENERATE_INSERT_COLOR(), RB_GENERATE_REMOVE(),
  RB_GENERATE_REMOVE_COLOR(), RB_GENERATE_FIND(), RB_GENERATE_NFIND(),
  RB_GENERATE_NEXT(), RB_GENERATE_PREV(), and RB_GENERATE_MINMAX() macros.

  Finally, the CMP argument is the name of a function used to compare tree nodes
  with each other.  The function takes two arguments of type struct TYPE *.  If
  the first argument is smaller than the second, the function returns a value
  smaller than zero.  If they are equal, the function returns zero.  Otherwise,
  it should return a value greater than zero.  The compare function defines the
  order of the tree elements.

  The RB_INIT() macro initializes the tree referenced by head.

  The red-black tree can also be initialized statically by using the
  RB_INITIALIZER() macro like this:

	   RB_HEAD(HEADNAME, TYPE) head = RB_INITIALIZER(&head);

  The RB_INSERT() macro inserts the new element elm into the tree.

  The RB_REMOVE() macro removes the element elm from the tree pointed by head.

  The RB_FIND() and RB_NFIND() macros can be used to find a particular element
  in the tree.

	   struct TYPE find, *res;
	   find.key = 30;
	   res = RB_FIND(NAME, head, &find);

  The RB_ROOT(), RB_MIN(), RB_MAX(), RB_NEXT(), and RB_PREV() macros can be used
  to traverse the tree:

	   for (np = RB_MIN(NAME, &head); np != NULL; np = RB_NEXT(NAME, &head, np))

  Or, for simplicity, one can use the RB_FOREACH() or RB_FOREACH_REVERSE()
  macro:

	   RB_FOREACH(np, NAME, head)

  The macros RB_FOREACH_SAFE() and RB_FOREACH_REVERSE_SAFE() traverse the tree
  referenced by head in a forward or reverse direction respectively, assigning
  each element in turn to np. However, unlike their unsafe counterparts, they
  permit both the removal of np as well as freeing it from within the loop
  safely without interfering with the traversal.

  Both RB_FOREACH_FROM() and RB_FOREACH_REVERSE_FROM() may be used to continue
  an interrupted traversal in a forward or reverse direction respectively. The
  head pointer is not required. The pointer to the node from where to resume the
  traversal should be passed as their last argument, and will be overwritten to
  provide safe traversal.

  The RB_EMPTY() macro should be used to check whether a red-black tree is empty.

NOTES
  Trying to free a tree in the following way is a common error:

	   SPLAY_FOREACH(var, NAME, head) {
		   SPLAY_REMOVE(NAME, head, var);
		   free(var);
	   }
	   free(head);

  Since var is freed, the FOREACH() macro refers to a pointer that may have been
  reallocated already.  Proper code needs a second variable.

	   for (var = SPLAY_MIN(NAME, head); var != NULL; var = nxt) {
		   nxt = SPLAY_NEXT(NAME, head, var);
		   SPLAY_REMOVE(NAME, head, var);
		   free(var);
	   }

  Both RB_INSERT() and SPLAY_INSERT() return NULL if the element was inserted in
  the tree successfully, otherwise they return a pointer to the element with the
  colliding key.

  Accordingly, RB_REMOVE() and SPLAY_REMOVE() return the pointer to the removed
  element otherwise they return NULL to indicate an error.
*/
/*---------------------------------------------------------------------------*/
#define bl_splay_head(name, type)\
struct name {\
  struct type *sph_root; /* root of the tree */\
}

#define bl_splay_initializer(root)\
  { nullptr }

#define bl_splay_init(root)\
do {\
  (root)->sph_root = nullptr;\
} while (/*constcond*/ 0)

#define bl_splay_entry(type)\
struct {\
  struct type *spe_left; /* left element */\
  struct type *spe_right; /* right element */\
}

#define bl_splay_left(elm, field)    (elm)->field.spe_left
#define bl_splay_right(elm, field)    (elm)->field.spe_right
#define bl_splay_root(head)    (head)->sph_root
#define bl_splay_empty(head)    (bl_splay_root(head) == nullptr)

/* splay_rotate_{left,right} expect that tmp hold splay_{right,left} */
#define bl_splay_rotate_right(head, tmp, field)\
do {\
  bl_splay_left((head)->sph_root, field) = bl_splay_right(tmp, field);\
  bl_splay_right(tmp, field) = (head)->sph_root;\
  (head)->sph_root = tmp;\
} while (/*constcond*/ 0)

#define bl_splay_rotate_left(head, tmp, field)\
do {\
  bl_splay_right((head)->sph_root, field) = bl_splay_left(tmp, field);\
  bl_splay_left(tmp, field) = (head)->sph_root;\
  (head)->sph_root = tmp;\
} while (/*constcond*/ 0)

#define bl_splay_linkleft(head, tmp, field)\
do {\
  bl_splay_left(tmp, field) = (head)->sph_root;\
  tmp = (head)->sph_root;\
  (head)->sph_root = bl_splay_left((head)->sph_root, field);\
} while (/*constcond*/ 0)

#define bl_splay_linkright(head, tmp, field)\
do {\
  bl_splay_right(tmp, field) = (head)->sph_root;\
  tmp = (head)->sph_root;\
  (head)->sph_root = bl_splay_right((head)->sph_root, field);\
} while (/*constcond*/ 0)

#define splay_assemble(head, node, left, right, field)\
do {\
  bl_splay_right(left, field) = bl_splay_left((head)->sph_root, field);\
  bl_splay_left(right, field) = bl_splay_right((head)->sph_root, field);\
  bl_splay_left((head)->sph_root, field) = bl_splay_right(node, field);\
  bl_splay_right((head)->sph_root, field) = bl_splay_left(node, field);\
} while (/*constcond*/ 0)

/* generates prototypes and inline functions */

#define bl_splay_prototype(name, type, field, cmp)\
void name##_splay(struct name *, struct type *);\
void name##_bl_splay_minmax(struct name *, int);\
struct type *name##_bl_splay_insert(struct name *, struct type *);\
struct type *name##_bl_splay_remove(struct name *, struct type *);\
\
/* finds the node with the same key as elm */\
static inline struct type *\
name##_bl_splay_find(struct name *head, struct type *elm)\
{\
  if (bl_splay_empty(head))\
    return(nullptr);\
  name##_splay(head, elm);\
  if ((cmp)(elm, (head)->sph_root) == 0)\
    return (head->sph_root);\
  return (nullptr);\
}\
\
static inline struct type *\
name##_bl_splay_next(struct name *head, struct type *elm)\
{\
  name##_splay(head, elm);\
  if (bl_splay_right(elm, field) != nullptr) {\
    elm = bl_splay_right(elm, field);\
    while (bl_splay_left(elm, field) != nullptr) {\
   elm = bl_splay_left(elm, field);\
    }\
  } else\
    elm = nullptr;\
  return (elm);\
}\
\
static inline struct type *\
name##_bl_splay_min_max(struct name *head, int val)\
{\
  name##_bl_splay_minmax(head, val);\
     return (bl_splay_root(head));\
}

/* main splay operation.
 * moves node close to the key of elm to top
 */
#define bl_splay_generate(name, type, field, cmp)\
struct type *\
name##_bl_splay_insert(struct name *head, struct type *elm)\
{\
    if (bl_splay_empty(head)) {\
   bl_splay_left(elm, field) = bl_splay_right(elm, field) = nullptr;\
    } else {\
   int __comp;\
   name##_splay(head, elm);\
   __comp = (cmp)(elm, (head)->sph_root);\
   if(__comp < 0) {\
     bl_splay_left(elm, field) = bl_splay_left((head)->sph_root, field);\
     bl_splay_right(elm, field) = (head)->sph_root;\
     bl_splay_left((head)->sph_root, field) = nullptr;\
   } else if (__comp > 0) {\
     bl_splay_right(elm, field) = bl_splay_right((head)->sph_root, field);\
     bl_splay_left(elm, field) = (head)->sph_root;\
     bl_splay_right((head)->sph_root, field) = nullptr;\
   } else\
     return ((head)->sph_root);\
    }\
    (head)->sph_root = (elm);\
    return (nullptr);\
}\
\
struct type *\
name##_bl_splay_remove(struct name *head, struct type *elm)\
{\
  struct type *__tmp;\
  if (bl_splay_empty(head))\
    return (nullptr);\
  name##_splay(head, elm);\
  if ((cmp)(elm, (head)->sph_root) == 0) {\
    if (bl_splay_left((head)->sph_root, field) == nullptr) {\
   (head)->sph_root = bl_splay_right((head)->sph_root, field);\
    } else {\
   __tmp = bl_splay_right((head)->sph_root, field);\
   (head)->sph_root = bl_splay_left((head)->sph_root, field);\
   name##_splay(head, elm);\
   bl_splay_right((head)->sph_root, field) = __tmp;\
    }\
    return (elm);\
  }\
  return (nullptr);\
}\
\
void\
name##_splay(struct name *head, struct type *elm)\
{\
  struct type __node, *__left, *__right, *__tmp;\
  int __comp;\
\
  bl_splay_left(&__node, field) = bl_splay_right(&__node, field) = nullptr;\
  __left = __right = &__node;\
\
  while ((__comp = (cmp)(elm, (head)->sph_root)) != 0) {\
    if (__comp < 0) {\
   __tmp = bl_splay_left((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if ((cmp)(elm, __tmp) < 0){\
     bl_splay_rotate_right(head, __tmp, field);\
     if (bl_splay_left((head)->sph_root, field) == nullptr)\
    break;\
   }\
   bl_splay_linkleft(head, __right, field);\
    } else if (__comp > 0) {\
   __tmp = bl_splay_right((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if ((cmp)(elm, __tmp) > 0){\
     bl_splay_rotate_left(head, __tmp, field);\
     if (bl_splay_right((head)->sph_root, field) == nullptr)\
    break;\
   }\
   bl_splay_linkright(head, __left, field);\
    }\
  }\
  splay_assemble(head, &__node, __left, __right, field);\
}\
\
/* splay with either the minimum or the maximum element\
 * used to find minimum or maximum element in tree.\
 */\
void name##_bl_splay_minmax(struct name *head, int __comp) \
{\
  struct type __node, *__left, *__right, *__tmp;\
\
  bl_splay_left(&__node, field) = bl_splay_right(&__node, field) = nullptr;\
  __left = __right = &__node;\
\
  while (1) {\
    if (__comp < 0) {\
   __tmp = bl_splay_left((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if (__comp < 0){\
     bl_splay_rotate_right(head, __tmp, field);\
     if (bl_splay_left((head)->sph_root, field) == nullptr)\
    break;\
   }\
   bl_splay_linkleft(head, __right, field);\
    } else if (__comp > 0) {\
   __tmp = bl_splay_right((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if (__comp > 0) {\
     bl_splay_rotate_left(head, __tmp, field);\
     if (bl_splay_right((head)->sph_root, field) == nullptr)\
    break;\
   }\
   bl_splay_linkright(head, __left, field);\
    }\
  }\
  splay_assemble(head, &__node, __left, __right, field);\
}

#define bl_splay_neginf  -1
#define bl_splay_inf  1

#define bl_splay_insert(name, x, y)  name##_bl_splay_insert(x, y)
#define bl_splay_remove(name, x, y)  name##_bl_splay_remove(x, y)
#define bl_splay_find(name, x, y)    name##_bl_splay_find(x, y)
#define bl_splay_next(name, x, y)    name##_bl_splay_next(x, y)
#define bl_splay_min(name, x)    (bl_splay_empty(x) ? nullptr\
    : name##_bl_splay_min_max(x, bl_splay_neginf))
#define bl_splay_max(name, x)    (bl_splay_empty(x) ? nullptr\
    : name##_bl_splay_min_max(x, bl_splay_inf))

#define bl_splay_foreach(x, name, head)\
  for ((x) = bl_splay_min(name, head);\
       (x) != nullptr;\
       (x) = bl_splay_next(name, head, x))

/* macros that define a red-black tree */
#define bl_rb_head(name, type)\
struct name {\
  struct type *rbh_root; /* root of the tree */\
}

#define bl_rb_initializer(root)\
  { nullptr }

#define bl_rb_init(root)\
do {\
  (root)->rbh_root = nullptr;\
} while (/*constcond*/ 0)

#define rb_black  0
#define bl_rb_red    1
#define bl_rb_entry(type)\
struct {\
  struct type *rbe_left;    /* left element */\
  struct type *rbe_right;    /* right element */\
  struct type *rbe_parent;  /* parent element */\
  int rbe_color;   /* node color */\
}

#define bl_rb_left(elm, field)    (elm)->field.rbe_left
#define bl_rb_right(elm, field)    (elm)->field.rbe_right
#define bl_rb_parent(elm, field)    (elm)->field.rbe_parent
#define bl_rb_color(elm, field)    (elm)->field.rbe_color
#define bl_rb_root(head)   (head)->rbh_root
#define bl_rb_empty(head)   (bl_rb_root(head) == nullptr)

#define bl_rb_set(elm, parent, field)\
do {\
  bl_rb_parent(elm, field) = parent;\
  bl_rb_left(elm, field) = bl_rb_right(elm, field) = nullptr;\
  bl_rb_color(elm, field) = bl_rb_red;\
} while (/*constcond*/ 0)

#define bl_rb_set_blackred(black, red, field)\
do {\
  bl_rb_color(black, field) = rb_black;\
  bl_rb_color(red, field) = bl_rb_red;\
} while (/*constcond*/ 0)

#ifndef bl_rb_augment
#define bl_rb_augment(x)  do {} while (0)
#endif

#define bl_rb_rotate_left(head, elm, tmp, field)\
do {\
  (tmp) = bl_rb_right(elm, field);\
  if ((bl_rb_right(elm, field) = bl_rb_left(tmp, field)) != nullptr) {\
    bl_rb_parent(bl_rb_left(tmp, field), field) = (elm);\
  }\
  bl_rb_augment(elm);\
  if ((bl_rb_parent(tmp, field) = bl_rb_parent(elm, field)) != nullptr) {\
    if ((elm) == bl_rb_left(bl_rb_parent(elm, field), field))\
   bl_rb_left(bl_rb_parent(elm, field), field) = (tmp);\
    else\
   bl_rb_right(bl_rb_parent(elm, field), field) = (tmp);\
  } else\
    (head)->rbh_root = (tmp);\
  bl_rb_left(tmp, field) = (elm);\
  bl_rb_parent(elm, field) = (tmp);\
  bl_rb_augment(tmp);\
  if ((bl_rb_parent(tmp, field)))\
    bl_rb_augment(bl_rb_parent(tmp, field));\
} while (/*constcond*/ 0)

#define bl_rb_rotate_right(head, elm, tmp, field)\
do {\
  (tmp) = bl_rb_left(elm, field);\
  if ((bl_rb_left(elm, field) = bl_rb_right(tmp, field)) != nullptr) {\
    bl_rb_parent(bl_rb_right(tmp, field), field) = (elm);\
  }\
  bl_rb_augment(elm);\
  if ((bl_rb_parent(tmp, field) = bl_rb_parent(elm, field)) != nullptr) {\
    if ((elm) == bl_rb_left(bl_rb_parent(elm, field), field))\
   bl_rb_left(bl_rb_parent(elm, field), field) = (tmp);\
    else\
   bl_rb_right(bl_rb_parent(elm, field), field) = (tmp);\
  } else\
    (head)->rbh_root = (tmp);\
  bl_rb_right(tmp, field) = (elm);\
  bl_rb_parent(elm, field) = (tmp);\
  bl_rb_augment(tmp);\
  if ((bl_rb_parent(tmp, field)))\
    bl_rb_augment(bl_rb_parent(tmp, field));\
} while (/*constcond*/ 0)

/* generates prototypes and inline functions */
#define  bl_rb_prototype(name, type, field, cmp)\
  bl_rb_prototype_internal(name, type, field, cmp,)
#define  bl_rb_prototype_static(name, type, field, cmp)\
  bl_rb_prototype_internal(name, type, field, cmp, static)
#define bl_rb_prototype_internal(name, type, field, cmp, attr)\
  bl_rb_prototype_insert_color(name, type, attr);\
  bl_rb_prototype_remove_color(name, type, attr);\
  bl_rb_prototype_insert(name, type, attr);\
  bl_rb_prototype_remove(name, type, attr);\
  bl_rb_prototype_find(name, type, attr);\
  bl_rb_prototype_nfind(name, type, attr);\
  bl_rb_prototype_next(name, type, attr);\
  bl_rb_prototype_prev(name, type, attr);\
  bl_rb_prototype_minmax(name, type, attr);
#define bl_rb_prototype_insert_color(name, type, attr)\
  attr void name##_bl_rb_insert_color(struct name *, struct type *)
#define bl_rb_prototype_remove_color(name, type, attr)\
  attr void name##_bl_rb_remove_color(\
    struct name *, struct type *, struct type *+
    )
#define bl_rb_prototype_remove(name, type, attr)\
  attr struct type *name##_bl_rb_remove(struct name *, struct type *)
#define bl_rb_prototype_insert(name, type, attr)\
  attr struct type *name##_bl_rb_insert(struct name *, struct type *)
#define bl_rb_prototype_find(name, type, attr)\
  attr struct type *name##_bl_rb_find(struct name *, struct type *)
#define bl_rb_prototype_nfind(name, type, attr)\
  attr struct type *name##_bl_rb_nfind(struct name *, struct type *)
#define bl_rb_prototype_next(name, type, attr)\
  attr struct type *name##_bl_rb_next(struct type *)
#define bl_rb_prototype_prev(name, type, attr)\
  attr struct type *name##_bl_rb_prev(struct type *)
#define bl_rb_prototype_minmax(name, type, attr)\
  attr struct type *name##_bl_rb_minmax(struct name *, int)

/* main rb operation.
 * moves node close to the key of elm to top
 */
#define  bl_rb_generate(name, type, field, cmp)\
  bl_rb_generate_internal(name, type, field, cmp,)
#define  bl_rb_generate_static(name, type, field, cmp)\
  bl_rb_generate_internal(name, type, field, cmp, static)
#define bl_rb_generate_internal(name, type, field, cmp, attr)\
  bl_rb_generate_insert_color(name, type, field, attr)\
  bl_rb_generate_remove_color(name, type, field, attr)\
  bl_rb_generate_insert(name, type, field, cmp, attr)\
  bl_rb_generate_remove(name, type, field, attr)\
  bl_rb_generate_find(name, type, field, cmp, attr)\
  bl_rb_generate_nfind(name, type, field, cmp, attr)\
  bl_rb_generate_next(name, type, field, attr)\
  bl_rb_generate_prev(name, type, field, attr)\
  bl_rb_generate_minmax(name, type, field, attr)

#define bl_rb_generate_insert_color(name, type, field, attr)\
attr void \
name##_bl_rb_insert_color(struct name *head, struct type *elm)\
{\
  struct type *parent, *gparent, *tmp;\
  while ((parent = bl_rb_parent(elm, field)) != nullptr &&\
   bl_rb_color(parent, field) == bl_rb_red) {\
    gparent = bl_rb_parent(parent, field);\
    if (parent == bl_rb_left(gparent, field)) {\
   tmp = bl_rb_right(gparent, field);\
   if (tmp && bl_rb_color(tmp, field) == bl_rb_red) {\
     bl_rb_color(tmp, field) = rb_black;\
     bl_rb_set_blackred(parent, gparent, field);\
     elm = gparent;\
     continue;\
   }\
   if (bl_rb_right(parent, field) == elm) {\
     bl_rb_rotate_left(head, parent, tmp, field);\
     tmp = parent;\
     parent = elm;\
     elm = tmp;\
   }\
   bl_rb_set_blackred(parent, gparent, field);\
   bl_rb_rotate_right(head, gparent, tmp, field);\
    } else {\
   tmp = bl_rb_left(gparent, field);\
   if (tmp && bl_rb_color(tmp, field) == bl_rb_red) {\
     bl_rb_color(tmp, field) = rb_black;\
     bl_rb_set_blackred(parent, gparent, field);\
     elm = gparent;\
     continue;\
   }\
   if (bl_rb_left(parent, field) == elm) {\
     bl_rb_rotate_right(head, parent, tmp, field);\
     tmp = parent;\
     parent = elm;\
     elm = tmp;\
   }\
   bl_rb_set_blackred(parent, gparent, field);\
   bl_rb_rotate_left(head, gparent, tmp, field);\
    }\
  }\
  bl_rb_color(head->rbh_root, field) = rb_black;\
}

#define bl_rb_generate_remove_color(name, type, field, attr)\
attr void \
name##_bl_rb_remove_color(\
  struct name *head, struct type *parent, struct type *elm\
  ) \
{\
  struct type *tmp;\
  while ((elm == nullptr || bl_rb_color(elm, field) == rb_black) &&\
   elm != bl_rb_root(head)) {\
    if (bl_rb_left(parent, field) == elm) {\
   tmp = bl_rb_right(parent, field);\
   if (bl_rb_color(tmp, field) == bl_rb_red) {\
     bl_rb_set_blackred(tmp, parent, field);\
     bl_rb_rotate_left(head, parent, tmp, field);\
     tmp = bl_rb_right(parent, field);\
   }\
   if ((bl_rb_left(tmp, field) == nullptr ||\
    bl_rb_color(bl_rb_left(tmp, field), field) == rb_black) &&\
    (bl_rb_right(tmp, field) == nullptr ||\
    bl_rb_color(bl_rb_right(tmp, field), field) == rb_black)) {\
     bl_rb_color(tmp, field) = bl_rb_red;\
     elm = parent;\
     parent = bl_rb_parent(elm, field);\
   } else {\
     if (bl_rb_right(tmp, field) == nullptr ||\
      bl_rb_color(bl_rb_right(tmp, field), field) == rb_black) {\
    struct type *oleft;\
    if ((oleft = bl_rb_left(tmp, field)) \
        != nullptr)\
      bl_rb_color(oleft, field) = rb_black;\
    bl_rb_color(tmp, field) = bl_rb_red;\
    bl_rb_rotate_right(head, tmp, oleft, field);\
    tmp = bl_rb_right(parent, field);\
     }\
     bl_rb_color(tmp, field) = bl_rb_color(parent, field);\
     bl_rb_color(parent, field) = rb_black;\
     if (bl_rb_right(tmp, field))\
    bl_rb_color(bl_rb_right(tmp, field), field) = rb_black;\
     bl_rb_rotate_left(head, parent, tmp, field);\
     elm = bl_rb_root(head);\
     break;\
   }\
    } else {\
   tmp = bl_rb_left(parent, field);\
   if (bl_rb_color(tmp, field) == bl_rb_red) {\
     bl_rb_set_blackred(tmp, parent, field);\
     bl_rb_rotate_right(head, parent, tmp, field);\
     tmp = bl_rb_left(parent, field);\
   }\
   if ((bl_rb_left(tmp, field) == nullptr ||\
    bl_rb_color(bl_rb_left(tmp, field), field) == rb_black) &&\
    (bl_rb_right(tmp, field) == nullptr ||\
    bl_rb_color(bl_rb_right(tmp, field), field) == rb_black)) {\
     bl_rb_color(tmp, field) = bl_rb_red;\
     elm = parent;\
     parent = bl_rb_parent(elm, field);\
   } else {\
     if (bl_rb_left(tmp, field) == nullptr ||\
      bl_rb_color(bl_rb_left(tmp, field), field) == rb_black) {\
    struct type *oright;\
    if ((oright = bl_rb_right(tmp, field)) \
        != nullptr)\
      bl_rb_color(oright, field) = rb_black;\
    bl_rb_color(tmp, field) = bl_rb_red;\
    bl_rb_rotate_left(head, tmp, oright, field);\
    tmp = bl_rb_left(parent, field);\
     }\
     bl_rb_color(tmp, field) = bl_rb_color(parent, field);\
     bl_rb_color(parent, field) = rb_black;\
     if (bl_rb_left(tmp, field))\
    bl_rb_color(bl_rb_left(tmp, field), field) = rb_black;\
     bl_rb_rotate_right(head, parent, tmp, field);\
     elm = bl_rb_root(head);\
     break;\
   }\
    }\
  }\
  if (elm)\
    bl_rb_color(elm, field) = rb_black;\
}

#define bl_rb_generate_remove(name, type, field, attr)\
attr struct type *\
name##_bl_rb_remove(struct name *head, struct type *elm)\
{\
  struct type *child, *parent, *old = elm;\
  int color;\
  if (bl_rb_left(elm, field) == nullptr)\
    child = bl_rb_right(elm, field);\
  else if (bl_rb_right(elm, field) == nullptr)\
    child = bl_rb_left(elm, field);\
  else {\
    struct type *left;\
    elm = bl_rb_right(elm, field);\
    while ((left = bl_rb_left(elm, field)) != nullptr)\
   elm = left;\
    child = bl_rb_right(elm, field);\
    parent = bl_rb_parent(elm, field);\
    color = bl_rb_color(elm, field);\
    if (child)\
   bl_rb_parent(child, field) = parent;\
    if (parent) {\
   if (bl_rb_left(parent, field) == elm)\
     bl_rb_left(parent, field) = child;\
   else\
     bl_rb_right(parent, field) = child;\
   bl_rb_augment(parent);\
    } else\
   bl_rb_root(head) = child;\
    if (bl_rb_parent(elm, field) == old)\
   parent = elm;\
    (elm)->field = (old)->field;\
    if (bl_rb_parent(old, field)) {\
   if (bl_rb_left(bl_rb_parent(old, field), field) == old)\
     bl_rb_left(bl_rb_parent(old, field), field) = elm;\
   else\
     bl_rb_right(bl_rb_parent(old, field), field) = elm;\
   bl_rb_augment(bl_rb_parent(old, field));\
    } else\
   bl_rb_root(head) = elm;\
    bl_rb_parent(bl_rb_left(old, field), field) = elm;\
    if (bl_rb_right(old, field))\
   bl_rb_parent(bl_rb_right(old, field), field) = elm;\
    if (parent) {\
   left = parent;\
   do {\
     bl_rb_augment(left);\
   } while ((left = bl_rb_parent(left, field)) != nullptr); \
    }\
    goto color;\
  }\
  parent = bl_rb_parent(elm, field);\
  color = bl_rb_color(elm, field);\
  if (child)\
    bl_rb_parent(child, field) = parent;\
  if (parent) {\
    if (bl_rb_left(parent, field) == elm)\
   bl_rb_left(parent, field) = child;\
    else\
   bl_rb_right(parent, field) = child;\
    bl_rb_augment(parent);\
  } else\
    bl_rb_root(head) = child;\
color:\
  if (color == rb_black)\
    name##_bl_rb_remove_color(head, parent, child);\
  return (old);\
}\

#define bl_rb_generate_insert(name, type, field, cmp, attr)\
/* inserts a node into the rb tree */\
attr struct type *\
name##_bl_rb_insert(struct name *head, struct type *elm)\
{\
  struct type *tmp;\
  struct type *parent = nullptr;\
  int comp = 0;\
  tmp = bl_rb_root(head);\
  while (tmp) {\
    parent = tmp;\
    comp = (cmp)(elm, parent);\
    if (comp < 0)\
   tmp = bl_rb_left(tmp, field);\
    else if (comp > 0)\
   tmp = bl_rb_right(tmp, field);\
    else\
   return (tmp);\
  }\
  bl_rb_set(elm, parent, field);\
  if (parent != nullptr) {\
    if (comp < 0)\
   bl_rb_left(parent, field) = elm;\
    else\
   bl_rb_right(parent, field) = elm;\
    bl_rb_augment(parent);\
  } else\
    bl_rb_root(head) = elm;\
  name##_bl_rb_insert_color(head, elm);\
  return (nullptr);\
}

#define bl_rb_generate_find(name, type, field, cmp, attr)\
/* finds the node with the same key as elm */\
attr struct type *\
name##_bl_rb_find(struct name *head, struct type *elm)\
{\
  struct type *tmp = bl_rb_root(head);\
  int comp;\
  while (tmp) {\
    comp = cmp(elm, tmp);\
    if (comp < 0)\
   tmp = bl_rb_left(tmp, field);\
    else if (comp > 0)\
   tmp = bl_rb_right(tmp, field);\
    else\
   return (tmp);\
  }\
  return (nullptr);\
}

#define bl_rb_generate_nfind(name, type, field, cmp, attr)\
/* finds the first node greater than or equal to the search key */\
attr struct type *\
name##_bl_rb_nfind(struct name *head, struct type *elm)\
{\
  struct type *tmp = bl_rb_root(head);\
  struct type *res = nullptr;\
  int comp;\
  while (tmp) {\
    comp = cmp(elm, tmp);\
    if (comp < 0) {\
   res = tmp;\
   tmp = bl_rb_left(tmp, field);\
    }\
    else if (comp > 0)\
   tmp = bl_rb_right(tmp, field);\
    else\
   return (tmp);\
  }\
  return (res);\
}

#define bl_rb_generate_next(name, type, field, attr)\
/* argsused */\
attr struct type *\
name##_bl_rb_next(struct type *elm)\
{\
  if (bl_rb_right(elm, field)) {\
    elm = bl_rb_right(elm, field);\
    while (bl_rb_left(elm, field))\
   elm = bl_rb_left(elm, field);\
  } else {\
    if (bl_rb_parent(elm, field) &&\
     (elm == bl_rb_left(bl_rb_parent(elm, field), field)))\
   elm = bl_rb_parent(elm, field);\
    else {\
   while (bl_rb_parent(elm, field) &&\
    (elm == bl_rb_right(bl_rb_parent(elm, field), field)))\
     elm = bl_rb_parent(elm, field);\
   elm = bl_rb_parent(elm, field);\
    }\
  }\
  return (elm);\
}

#define bl_rb_generate_prev(name, type, field, attr)\
/* argsused */\
attr struct type *\
name##_bl_rb_prev(struct type *elm)\
{\
  if (bl_rb_left(elm, field)) {\
    elm = bl_rb_left(elm, field);\
    while (bl_rb_right(elm, field))\
   elm = bl_rb_right(elm, field);\
  } else {\
    if (bl_rb_parent(elm, field) &&\
     (elm == bl_rb_right(bl_rb_parent(elm, field), field)))\
   elm = bl_rb_parent(elm, field);\
    else {\
   while (bl_rb_parent(elm, field) &&\
    (elm == bl_rb_left(bl_rb_parent(elm, field), field)))\
     elm = bl_rb_parent(elm, field);\
   elm = bl_rb_parent(elm, field);\
    }\
  }\
  return (elm);\
}

#define bl_rb_generate_minmax(name, type, field, attr)\
attr struct type *\
name##_bl_rb_minmax(struct name *head, int val)\
{\
  struct type *tmp = bl_rb_root(head);\
  struct type *parent = nullptr;\
  while (tmp) {\
    parent = tmp;\
    if (val < 0)\
   tmp = bl_rb_left(tmp, field);\
    else\
   tmp = bl_rb_right(tmp, field);\
  }\
  return (parent);\
}

#define bl_rb_neginf  -1
#define bl_rb_inf  1

#define bl_rb_insert(name, x, y)  name##_bl_rb_insert(x, y)
#define bl_rb_remove(name, x, y)  name##_bl_rb_remove(x, y)
#define bl_rb_find(name, x, y)  name##_bl_rb_find(x, y)
#define bl_rb_nfind(name, x, y)  name##_bl_rb_nfind(x, y)
#define bl_rb_next(name, x, y)  name##_bl_rb_next(y)
#define bl_rb_prev(name, x, y)  name##_bl_rb_prev(y)
#define bl_rb_min(name, x)    name##_bl_rb_minmax(x, bl_rb_neginf)
#define bl_rb_max(name, x)    name##_bl_rb_minmax(x, bl_rb_inf)

#define bl_rb_foreach(x, name, head)\
  for ((x) = bl_rb_min(name, head);\
       (x) != nullptr;\
       (x) = name##_bl_rb_next(x))

#define bl_rb_foreach_from(x, name, y)\
  for ((x) = (y);\
      ((x) != nullptr) && ((y) = name##_bl_rb_next(x), (x) != nullptr);\
       (x) = (y))

#define bl_rb_foreach_safe(x, name, head, y)\
  for ((x) = bl_rb_min(name, head);\
      ((x) != nullptr) && ((y) = name##_bl_rb_next(x), (x) != nullptr);\
       (x) = (y))

#define bl_rb_foreach_reverse(x, name, head)\
  for ((x) = bl_rb_max(name, head);\
       (x) != nullptr;\
       (x) = name##_bl_rb_prev(x))

#define bl_rb_foreach_reverse_from(x, name, y)\
  for ((x) = (y);\
      ((x) != nullptr) && ((y) = name##_bl_rb_prev(x), (x) != nullptr);\
       (x) = (y))

#define bl_rb_foreach_reverse_safe(x, name, head, y)\
  for ((x) = bl_rb_max(name, head);\
      ((x) != nullptr) && ((y) = name##_bl_rb_prev(x), (x) != nullptr);\
       (x) = (y))

#endif  /* _SYS_TREE_H_ */

