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

#ifndef  _BSD_SYS_TREE_H_
#define  _BSD_SYS_TREE_H_

#include <bl/hdr/base/platform.h>

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
#define splay_head(name, type)\
struct name {\
  struct type *sph_root; /* root of the tree */\
}

#define splay_initializer(root)\
  { nullptr }

#define splay_init(root)\
do {\
  (root)->sph_root = nullptr;\
} while (/*constcond*/ 0)

#define splay_entry(type)\
struct {\
  struct type *spe_left; /* left element */\
  struct type *spe_right; /* right element */\
}

#define splay_left(elm, field)    (elm)->field.spe_left
#define splay_right(elm, field)    (elm)->field.spe_right
#define splay_root(head)    (head)->sph_root
#define splay_empty(head)    (splay_root(head) == nullptr)

/* splay_rotate_{left,right} expect that tmp hold splay_{right,left} */
#define splay_rotate_right(head, tmp, field)\
do {\
  splay_left((head)->sph_root, field) = splay_right(tmp, field);\
  splay_right(tmp, field) = (head)->sph_root;\
  (head)->sph_root = tmp;\
} while (/*constcond*/ 0)

#define splay_rotate_left(head, tmp, field)\
do {\
  splay_right((head)->sph_root, field) = splay_left(tmp, field);\
  splay_left(tmp, field) = (head)->sph_root;\
  (head)->sph_root = tmp;\
} while (/*constcond*/ 0)

#define splay_linkleft(head, tmp, field)\
do {\
  splay_left(tmp, field) = (head)->sph_root;\
  tmp = (head)->sph_root;\
  (head)->sph_root = splay_left((head)->sph_root, field);\
} while (/*constcond*/ 0)

#define splay_linkright(head, tmp, field)\
do {\
  splay_right(tmp, field) = (head)->sph_root;\
  tmp = (head)->sph_root;\
  (head)->sph_root = splay_right((head)->sph_root, field);\
} while (/*constcond*/ 0)

#define splay_assemble(head, node, left, right, field)\
do {\
  splay_right(left, field) = splay_left((head)->sph_root, field);\
  splay_left(right, field) = splay_right((head)->sph_root, field);\
  splay_left((head)->sph_root, field) = splay_right(node, field);\
  splay_right((head)->sph_root, field) = splay_left(node, field);\
} while (/*constcond*/ 0)

/* generates prototypes and inline functions */

#define splay_prototype(name, type, field, cmp)\
void name##_splay(struct name *, struct type *);\
void name##_splay_minmax(struct name *, int);\
struct type *name##_splay_insert(struct name *, struct type *);\
struct type *name##_splay_remove(struct name *, struct type *);\
\
/* finds the node with the same key as elm */\
static inline struct type *\
name##_splay_find(struct name *head, struct type *elm)\
{\
  if (splay_empty(head))\
    return(nullptr);\
  name##_splay(head, elm);\
  if ((cmp)(elm, (head)->sph_root) == 0)\
    return (head->sph_root);\
  return (nullptr);\
}\
\
static inline struct type *\
name##_splay_next(struct name *head, struct type *elm)\
{\
  name##_splay(head, elm);\
  if (splay_right(elm, field) != nullptr) {\
    elm = splay_right(elm, field);\
    while (splay_left(elm, field) != nullptr) {\
   elm = splay_left(elm, field);\
    }\
  } else\
    elm = nullptr;\
  return (elm);\
}\
\
static inline struct type *\
name##_splay_min_max(struct name *head, int val)\
{\
  name##_splay_minmax(head, val);\
     return (splay_root(head));\
}

/* main splay operation.
 * moves node close to the key of elm to top
 */
#define splay_generate(name, type, field, cmp)\
struct type *\
name##_splay_insert(struct name *head, struct type *elm)\
{\
    if (splay_empty(head)) {\
   splay_left(elm, field) = splay_right(elm, field) = nullptr;\
    } else {\
   int __comp;\
   name##_splay(head, elm);\
   __comp = (cmp)(elm, (head)->sph_root);\
   if(__comp < 0) {\
     splay_left(elm, field) = splay_left((head)->sph_root, field);\
     splay_right(elm, field) = (head)->sph_root;\
     splay_left((head)->sph_root, field) = nullptr;\
   } else if (__comp > 0) {\
     splay_right(elm, field) = splay_right((head)->sph_root, field);\
     splay_left(elm, field) = (head)->sph_root;\
     splay_right((head)->sph_root, field) = nullptr;\
   } else\
     return ((head)->sph_root);\
    }\
    (head)->sph_root = (elm);\
    return (nullptr);\
}\
\
struct type *\
name##_splay_remove(struct name *head, struct type *elm)\
{\
  struct type *__tmp;\
  if (splay_empty(head))\
    return (nullptr);\
  name##_splay(head, elm);\
  if ((cmp)(elm, (head)->sph_root) == 0) {\
    if (splay_left((head)->sph_root, field) == nullptr) {\
   (head)->sph_root = splay_right((head)->sph_root, field);\
    } else {\
   __tmp = splay_right((head)->sph_root, field);\
   (head)->sph_root = splay_left((head)->sph_root, field);\
   name##_splay(head, elm);\
   splay_right((head)->sph_root, field) = __tmp;\
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
  splay_left(&__node, field) = splay_right(&__node, field) = nullptr;\
  __left = __right = &__node;\
\
  while ((__comp = (cmp)(elm, (head)->sph_root)) != 0) {\
    if (__comp < 0) {\
   __tmp = splay_left((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if ((cmp)(elm, __tmp) < 0){\
     splay_rotate_right(head, __tmp, field);\
     if (splay_left((head)->sph_root, field) == nullptr)\
    break;\
   }\
   splay_linkleft(head, __right, field);\
    } else if (__comp > 0) {\
   __tmp = splay_right((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if ((cmp)(elm, __tmp) > 0){\
     splay_rotate_left(head, __tmp, field);\
     if (splay_right((head)->sph_root, field) == nullptr)\
    break;\
   }\
   splay_linkright(head, __left, field);\
    }\
  }\
  splay_assemble(head, &__node, __left, __right, field);\
}\
\
/* splay with either the minimum or the maximum element\
 * used to find minimum or maximum element in tree.\
 */\
void name##_splay_minmax(struct name *head, int __comp) \
{\
  struct type __node, *__left, *__right, *__tmp;\
\
  splay_left(&__node, field) = splay_right(&__node, field) = nullptr;\
  __left = __right = &__node;\
\
  while (1) {\
    if (__comp < 0) {\
   __tmp = splay_left((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if (__comp < 0){\
     splay_rotate_right(head, __tmp, field);\
     if (splay_left((head)->sph_root, field) == nullptr)\
    break;\
   }\
   splay_linkleft(head, __right, field);\
    } else if (__comp > 0) {\
   __tmp = splay_right((head)->sph_root, field);\
   if (__tmp == nullptr)\
     break;\
   if (__comp > 0) {\
     splay_rotate_left(head, __tmp, field);\
     if (splay_right((head)->sph_root, field) == nullptr)\
    break;\
   }\
   splay_linkright(head, __left, field);\
    }\
  }\
  splay_assemble(head, &__node, __left, __right, field);\
}

#define splay_neginf  -1
#define splay_inf  1

#define splay_insert(name, x, y)  name##_splay_insert(x, y)
#define splay_remove(name, x, y)  name##_splay_remove(x, y)
#define splay_find(name, x, y)    name##_splay_find(x, y)
#define splay_next(name, x, y)    name##_splay_next(x, y)
#define splay_min(name, x)    (splay_empty(x) ? nullptr\
    : name##_splay_min_max(x, splay_neginf))
#define splay_max(name, x)    (splay_empty(x) ? nullptr\
    : name##_splay_min_max(x, splay_inf))

#define splay_foreach(x, name, head)\
  for ((x) = splay_min(name, head);\
       (x) != nullptr;\
       (x) = splay_next(name, head, x))

/* macros that define a red-black tree */
#define rb_head(name, type)\
struct name {\
  struct type *rbh_root; /* root of the tree */\
}

#define rb_initializer(root)\
  { nullptr }

#define rb_init(root)\
do {\
  (root)->rbh_root = nullptr;\
} while (/*constcond*/ 0)

#define rb_black  0
#define rb_red    1
#define rb_entry(type)\
struct {\
  struct type *rbe_left;    /* left element */\
  struct type *rbe_right;    /* right element */\
  struct type *rbe_parent;  /* parent element */\
  int rbe_color;   /* node color */\
}

#define rb_left(elm, field)    (elm)->field.rbe_left
#define rb_right(elm, field)    (elm)->field.rbe_right
#define rb_parent(elm, field)    (elm)->field.rbe_parent
#define rb_color(elm, field)    (elm)->field.rbe_color
#define rb_root(head)   (head)->rbh_root
#define rb_empty(head)   (rb_root(head) == nullptr)

#define rb_set(elm, parent, field)\
do {\
  rb_parent(elm, field) = parent;\
  rb_left(elm, field) = rb_right(elm, field) = nullptr;\
  rb_color(elm, field) = rb_red;\
} while (/*constcond*/ 0)

#define rb_set_blackred(black, red, field)\
do {\
  rb_color(black, field) = rb_black;\
  rb_color(red, field) = rb_red;\
} while (/*constcond*/ 0)

#ifndef rb_augment
#define rb_augment(x)  do {} while (0)
#endif

#define rb_rotate_left(head, elm, tmp, field)\
do {\
  (tmp) = rb_right(elm, field);\
  if ((rb_right(elm, field) = rb_left(tmp, field)) != nullptr) {\
    rb_parent(rb_left(tmp, field), field) = (elm);\
  }\
  rb_augment(elm);\
  if ((rb_parent(tmp, field) = rb_parent(elm, field)) != nullptr) {\
    if ((elm) == rb_left(rb_parent(elm, field), field))\
   rb_left(rb_parent(elm, field), field) = (tmp);\
    else\
   rb_right(rb_parent(elm, field), field) = (tmp);\
  } else\
    (head)->rbh_root = (tmp);\
  rb_left(tmp, field) = (elm);\
  rb_parent(elm, field) = (tmp);\
  rb_augment(tmp);\
  if ((rb_parent(tmp, field)))\
    rb_augment(rb_parent(tmp, field));\
} while (/*constcond*/ 0)

#define rb_rotate_right(head, elm, tmp, field)\
do {\
  (tmp) = rb_left(elm, field);\
  if ((rb_left(elm, field) = rb_right(tmp, field)) != nullptr) {\
    rb_parent(rb_right(tmp, field), field) = (elm);\
  }\
  rb_augment(elm);\
  if ((rb_parent(tmp, field) = rb_parent(elm, field)) != nullptr) {\
    if ((elm) == rb_left(rb_parent(elm, field), field))\
   rb_left(rb_parent(elm, field), field) = (tmp);\
    else\
   rb_right(rb_parent(elm, field), field) = (tmp);\
  } else\
    (head)->rbh_root = (tmp);\
  rb_right(tmp, field) = (elm);\
  rb_parent(elm, field) = (tmp);\
  rb_augment(tmp);\
  if ((rb_parent(tmp, field)))\
    rb_augment(rb_parent(tmp, field));\
} while (/*constcond*/ 0)

/* generates prototypes and inline functions */
#define  rb_prototype(name, type, field, cmp)\
  rb_prototype_internal(name, type, field, cmp,)
#define  rb_prototype_static(name, type, field, cmp)\
  rb_prototype_internal(name, type, field, cmp, static)
#define rb_prototype_internal(name, type, field, cmp, attr)\
  rb_prototype_insert_color(name, type, attr);\
  rb_prototype_remove_color(name, type, attr);\
  rb_prototype_insert(name, type, attr);\
  rb_prototype_remove(name, type, attr);\
  rb_prototype_find(name, type, attr);\
  rb_prototype_nfind(name, type, attr);\
  rb_prototype_next(name, type, attr);\
  rb_prototype_prev(name, type, attr);\
  rb_prototype_minmax(name, type, attr);
#define rb_prototype_insert_color(name, type, attr)\
  attr void name##_rb_insert_color(struct name *, struct type *)
#define rb_prototype_remove_color(name, type, attr)\
  attr void name##_rb_remove_color(struct name *, struct type *, struct type *)
#define rb_prototype_remove(name, type, attr)\
  attr struct type *name##_rb_remove(struct name *, struct type *)
#define rb_prototype_insert(name, type, attr)\
  attr struct type *name##_rb_insert(struct name *, struct type *)
#define rb_prototype_find(name, type, attr)\
  attr struct type *name##_rb_find(struct name *, struct type *)
#define rb_prototype_nfind(name, type, attr)\
  attr struct type *name##_rb_nfind(struct name *, struct type *)
#define rb_prototype_next(name, type, attr)\
  attr struct type *name##_rb_next(struct type *)
#define rb_prototype_prev(name, type, attr)\
  attr struct type *name##_rb_prev(struct type *)
#define rb_prototype_minmax(name, type, attr)\
  attr struct type *name##_rb_minmax(struct name *, int)

/* main rb operation.
 * moves node close to the key of elm to top
 */
#define  rb_generate(name, type, field, cmp)\
  rb_generate_internal(name, type, field, cmp,)
#define  rb_generate_static(name, type, field, cmp)\
  rb_generate_internal(name, type, field, cmp, static)
#define rb_generate_internal(name, type, field, cmp, attr)\
  rb_generate_insert_color(name, type, field, attr)\
  rb_generate_remove_color(name, type, field, attr)\
  rb_generate_insert(name, type, field, cmp, attr)\
  rb_generate_remove(name, type, field, attr)\
  rb_generate_find(name, type, field, cmp, attr)\
  rb_generate_nfind(name, type, field, cmp, attr)\
  rb_generate_next(name, type, field, attr)\
  rb_generate_prev(name, type, field, attr)\
  rb_generate_minmax(name, type, field, attr)

#define rb_generate_insert_color(name, type, field, attr)\
attr void \
name##_rb_insert_color(struct name *head, struct type *elm)\
{\
  struct type *parent, *gparent, *tmp;\
  while ((parent = rb_parent(elm, field)) != nullptr &&\
   rb_color(parent, field) == rb_red) {\
    gparent = rb_parent(parent, field);\
    if (parent == rb_left(gparent, field)) {\
   tmp = rb_right(gparent, field);\
   if (tmp && rb_color(tmp, field) == rb_red) {\
     rb_color(tmp, field) = rb_black;\
     rb_set_blackred(parent, gparent, field);\
     elm = gparent;\
     continue;\
   }\
   if (rb_right(parent, field) == elm) {\
     rb_rotate_left(head, parent, tmp, field);\
     tmp = parent;\
     parent = elm;\
     elm = tmp;\
   }\
   rb_set_blackred(parent, gparent, field);\
   rb_rotate_right(head, gparent, tmp, field);\
    } else {\
   tmp = rb_left(gparent, field);\
   if (tmp && rb_color(tmp, field) == rb_red) {\
     rb_color(tmp, field) = rb_black;\
     rb_set_blackred(parent, gparent, field);\
     elm = gparent;\
     continue;\
   }\
   if (rb_left(parent, field) == elm) {\
     rb_rotate_right(head, parent, tmp, field);\
     tmp = parent;\
     parent = elm;\
     elm = tmp;\
   }\
   rb_set_blackred(parent, gparent, field);\
   rb_rotate_left(head, gparent, tmp, field);\
    }\
  }\
  rb_color(head->rbh_root, field) = rb_black;\
}

#define rb_generate_remove_color(name, type, field, attr)\
attr void \
name##_rb_remove_color(struct name *head, struct type *parent, struct type *elm) \
{\
  struct type *tmp;\
  while ((elm == nullptr || rb_color(elm, field) == rb_black) &&\
   elm != rb_root(head)) {\
    if (rb_left(parent, field) == elm) {\
   tmp = rb_right(parent, field);\
   if (rb_color(tmp, field) == rb_red) {\
     rb_set_blackred(tmp, parent, field);\
     rb_rotate_left(head, parent, tmp, field);\
     tmp = rb_right(parent, field);\
   }\
   if ((rb_left(tmp, field) == nullptr ||\
    rb_color(rb_left(tmp, field), field) == rb_black) &&\
    (rb_right(tmp, field) == nullptr ||\
    rb_color(rb_right(tmp, field), field) == rb_black)) {\
     rb_color(tmp, field) = rb_red;\
     elm = parent;\
     parent = rb_parent(elm, field);\
   } else {\
     if (rb_right(tmp, field) == nullptr ||\
      rb_color(rb_right(tmp, field), field) == rb_black) {\
    struct type *oleft;\
    if ((oleft = rb_left(tmp, field)) \
        != nullptr)\
      rb_color(oleft, field) = rb_black;\
    rb_color(tmp, field) = rb_red;\
    rb_rotate_right(head, tmp, oleft, field);\
    tmp = rb_right(parent, field);\
     }\
     rb_color(tmp, field) = rb_color(parent, field);\
     rb_color(parent, field) = rb_black;\
     if (rb_right(tmp, field))\
    rb_color(rb_right(tmp, field), field) = rb_black;\
     rb_rotate_left(head, parent, tmp, field);\
     elm = rb_root(head);\
     break;\
   }\
    } else {\
   tmp = rb_left(parent, field);\
   if (rb_color(tmp, field) == rb_red) {\
     rb_set_blackred(tmp, parent, field);\
     rb_rotate_right(head, parent, tmp, field);\
     tmp = rb_left(parent, field);\
   }\
   if ((rb_left(tmp, field) == nullptr ||\
    rb_color(rb_left(tmp, field), field) == rb_black) &&\
    (rb_right(tmp, field) == nullptr ||\
    rb_color(rb_right(tmp, field), field) == rb_black)) {\
     rb_color(tmp, field) = rb_red;\
     elm = parent;\
     parent = rb_parent(elm, field);\
   } else {\
     if (rb_left(tmp, field) == nullptr ||\
      rb_color(rb_left(tmp, field), field) == rb_black) {\
    struct type *oright;\
    if ((oright = rb_right(tmp, field)) \
        != nullptr)\
      rb_color(oright, field) = rb_black;\
    rb_color(tmp, field) = rb_red;\
    rb_rotate_left(head, tmp, oright, field);\
    tmp = rb_left(parent, field);\
     }\
     rb_color(tmp, field) = rb_color(parent, field);\
     rb_color(parent, field) = rb_black;\
     if (rb_left(tmp, field))\
    rb_color(rb_left(tmp, field), field) = rb_black;\
     rb_rotate_right(head, parent, tmp, field);\
     elm = rb_root(head);\
     break;\
   }\
    }\
  }\
  if (elm)\
    rb_color(elm, field) = rb_black;\
}

#define rb_generate_remove(name, type, field, attr)\
attr struct type *\
name##_rb_remove(struct name *head, struct type *elm)\
{\
  struct type *child, *parent, *old = elm;\
  int color;\
  if (rb_left(elm, field) == nullptr)\
    child = rb_right(elm, field);\
  else if (rb_right(elm, field) == nullptr)\
    child = rb_left(elm, field);\
  else {\
    struct type *left;\
    elm = rb_right(elm, field);\
    while ((left = rb_left(elm, field)) != nullptr)\
   elm = left;\
    child = rb_right(elm, field);\
    parent = rb_parent(elm, field);\
    color = rb_color(elm, field);\
    if (child)\
   rb_parent(child, field) = parent;\
    if (parent) {\
   if (rb_left(parent, field) == elm)\
     rb_left(parent, field) = child;\
   else\
     rb_right(parent, field) = child;\
   rb_augment(parent);\
    } else\
   rb_root(head) = child;\
    if (rb_parent(elm, field) == old)\
   parent = elm;\
    (elm)->field = (old)->field;\
    if (rb_parent(old, field)) {\
   if (rb_left(rb_parent(old, field), field) == old)\
     rb_left(rb_parent(old, field), field) = elm;\
   else\
     rb_right(rb_parent(old, field), field) = elm;\
   rb_augment(rb_parent(old, field));\
    } else\
   rb_root(head) = elm;\
    rb_parent(rb_left(old, field), field) = elm;\
    if (rb_right(old, field))\
   rb_parent(rb_right(old, field), field) = elm;\
    if (parent) {\
   left = parent;\
   do {\
     rb_augment(left);\
   } while ((left = rb_parent(left, field)) != nullptr); \
    }\
    goto color;\
  }\
  parent = rb_parent(elm, field);\
  color = rb_color(elm, field);\
  if (child)\
    rb_parent(child, field) = parent;\
  if (parent) {\
    if (rb_left(parent, field) == elm)\
   rb_left(parent, field) = child;\
    else\
   rb_right(parent, field) = child;\
    rb_augment(parent);\
  } else\
    rb_root(head) = child;\
color:\
  if (color == rb_black)\
    name##_rb_remove_color(head, parent, child);\
  return (old);\
}\

#define rb_generate_insert(name, type, field, cmp, attr)\
/* inserts a node into the rb tree */\
attr struct type *\
name##_rb_insert(struct name *head, struct type *elm)\
{\
  struct type *tmp;\
  struct type *parent = nullptr;\
  int comp = 0;\
  tmp = rb_root(head);\
  while (tmp) {\
    parent = tmp;\
    comp = (cmp)(elm, parent);\
    if (comp < 0)\
   tmp = rb_left(tmp, field);\
    else if (comp > 0)\
   tmp = rb_right(tmp, field);\
    else\
   return (tmp);\
  }\
  rb_set(elm, parent, field);\
  if (parent != nullptr) {\
    if (comp < 0)\
   rb_left(parent, field) = elm;\
    else\
   rb_right(parent, field) = elm;\
    rb_augment(parent);\
  } else\
    rb_root(head) = elm;\
  name##_rb_insert_color(head, elm);\
  return (nullptr);\
}

#define rb_generate_find(name, type, field, cmp, attr)\
/* finds the node with the same key as elm */\
attr struct type *\
name##_rb_find(struct name *head, struct type *elm)\
{\
  struct type *tmp = rb_root(head);\
  int comp;\
  while (tmp) {\
    comp = cmp(elm, tmp);\
    if (comp < 0)\
   tmp = rb_left(tmp, field);\
    else if (comp > 0)\
   tmp = rb_right(tmp, field);\
    else\
   return (tmp);\
  }\
  return (nullptr);\
}

#define rb_generate_nfind(name, type, field, cmp, attr)\
/* finds the first node greater than or equal to the search key */\
attr struct type *\
name##_rb_nfind(struct name *head, struct type *elm)\
{\
  struct type *tmp = rb_root(head);\
  struct type *res = nullptr;\
  int comp;\
  while (tmp) {\
    comp = cmp(elm, tmp);\
    if (comp < 0) {\
   res = tmp;\
   tmp = rb_left(tmp, field);\
    }\
    else if (comp > 0)\
   tmp = rb_right(tmp, field);\
    else\
   return (tmp);\
  }\
  return (res);\
}

#define rb_generate_next(name, type, field, attr)\
/* argsused */\
attr struct type *\
name##_rb_next(struct type *elm)\
{\
  if (rb_right(elm, field)) {\
    elm = rb_right(elm, field);\
    while (rb_left(elm, field))\
   elm = rb_left(elm, field);\
  } else {\
    if (rb_parent(elm, field) &&\
     (elm == rb_left(rb_parent(elm, field), field)))\
   elm = rb_parent(elm, field);\
    else {\
   while (rb_parent(elm, field) &&\
    (elm == rb_right(rb_parent(elm, field), field)))\
     elm = rb_parent(elm, field);\
   elm = rb_parent(elm, field);\
    }\
  }\
  return (elm);\
}

#define rb_generate_prev(name, type, field, attr)\
/* argsused */\
attr struct type *\
name##_rb_prev(struct type *elm)\
{\
  if (rb_left(elm, field)) {\
    elm = rb_left(elm, field);\
    while (rb_right(elm, field))\
   elm = rb_right(elm, field);\
  } else {\
    if (rb_parent(elm, field) &&\
     (elm == rb_right(rb_parent(elm, field), field)))\
   elm = rb_parent(elm, field);\
    else {\
   while (rb_parent(elm, field) &&\
    (elm == rb_left(rb_parent(elm, field), field)))\
     elm = rb_parent(elm, field);\
   elm = rb_parent(elm, field);\
    }\
  }\
  return (elm);\
}

#define rb_generate_minmax(name, type, field, attr)\
attr struct type *\
name##_rb_minmax(struct name *head, int val)\
{\
  struct type *tmp = rb_root(head);\
  struct type *parent = nullptr;\
  while (tmp) {\
    parent = tmp;\
    if (val < 0)\
   tmp = rb_left(tmp, field);\
    else\
   tmp = rb_right(tmp, field);\
  }\
  return (parent);\
}

#define rb_neginf  -1
#define rb_inf  1

#define rb_insert(name, x, y)  name##_rb_insert(x, y)
#define rb_remove(name, x, y)  name##_rb_remove(x, y)
#define rb_find(name, x, y)  name##_rb_find(x, y)
#define rb_nfind(name, x, y)  name##_rb_nfind(x, y)
#define rb_next(name, x, y)  name##_rb_next(y)
#define rb_prev(name, x, y)  name##_rb_prev(y)
#define rb_min(name, x)    name##_rb_minmax(x, rb_neginf)
#define rb_max(name, x)    name##_rb_minmax(x, rb_inf)

#define rb_foreach(x, name, head)\
  for ((x) = rb_min(name, head);\
       (x) != nullptr;\
       (x) = name##_rb_next(x))

#define rb_foreach_from(x, name, y)\
  for ((x) = (y);\
      ((x) != nullptr) && ((y) = name##_rb_next(x), (x) != nullptr);\
       (x) = (y))

#define rb_foreach_safe(x, name, head, y)\
  for ((x) = rb_min(name, head);\
      ((x) != nullptr) && ((y) = name##_rb_next(x), (x) != nullptr);\
       (x) = (y))

#define rb_foreach_reverse(x, name, head)\
  for ((x) = rb_max(name, head);\
       (x) != nullptr;\
       (x) = name##_rb_prev(x))

#define rb_foreach_reverse_from(x, name, y)\
  for ((x) = (y);\
      ((x) != nullptr) && ((y) = name##_rb_prev(x), (x) != nullptr);\
       (x) = (y))

#define rb_foreach_reverse_safe(x, name, head, y)\
  for ((x) = rb_max(name, head);\
      ((x) != nullptr) && ((y) = name##_rb_prev(x), (x) != nullptr);\
       (x) = (y))

#endif  /* _SYS_TREE_H_ */

