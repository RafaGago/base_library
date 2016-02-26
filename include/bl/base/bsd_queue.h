/*  $NetBSD: queue.h,v 1.30 2001/06/22 06:18:22 chs Exp $  */

/*
 * Copyright (c) 1991, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *  @(#)queue.h  8.5 (Berkeley) 8/20/94
 */

#ifndef  _BSD_SYS_QUEUE_H_
#define  _BSD_SYS_QUEUE_H_

#include <bl/base/platform.h>

/*---------------------------------------------------------------------------*/
/* Man page copy. Remember that we use lower case
DESCRIPTION
  These macros define and operate on four types of data structures: 
  singly-linked lists, singly-linked tail queues, lists, and tail queues.  All
  four structures support the following functionality:
  1. Insertion of a new entry at the head of the list.
  2. Insertion of a new entry after any element in the list.
  3. O(1) removal of an entry from the head of the list.
  4. Forward traversal through the list.
  5. Swapping the contents of two lists.

  Singly-linked lists are the simplest of the four data structures and support
  only the above functionality.  Singly-linked lists are ideal for applications
  with large datasets and few or no removals, or for implementing a LIFO queue.
  Singly-linked lists add the following functionality:
  1. O(n) removal of any entry in the list.

  Singly-linked tail queues add the following functionality:
  1. Entries can be added at the end of a list.
  2. O(n) removal of any entry in the list.
  3. They may be concatenated.
  However:
  1. All list insertions must specify the head of the list.
  2. Each head entry requires two pointers rather than one.
  3. Code size is about 15% greater and operations run about 20% slower than 
    singly-linked lists.

  Singly-linked tail queues are ideal for applications with large datasets and 
  few or no removals, or for implementing a FIFO queue.

  All doubly linked types of data structures (lists and tail queues) 
  additionally allow:
  1. Insertion of a new entry before any element in the list.
  2. O(1) removal of any entry in the list.
  However:
  1. Each element requires two pointers rather than one.
  2. Code size and execution time of operations (except for removal) is about 
     twice that of the singly-linked data-structures.

  Linked lists are the simplest of the doubly linked data structures.  They add
  the following functionality over the above:
  1. They may be traversed backwards.
  However:
  1. To traverse backwards, an entry to begin the traversal and the list in 
     which it is contained must be specified.

  Tail queues add the following functionality:
  1. Entries can be added at the end of a list.
  2. They may be traversed backwards, from tail to head.
  3. They may be concatenated.
  However:
  1. All list insertions and removals must specify the head of the list.
  2. Each head entry requires two pointers rather than one.
  3. Code size is about 15% greater and operations run about 20% slower than 
    singly-linked lists.

  In the macro definitions, TYPE is the name of a user defined structure, that 
  must contain a field of type SLIST_ENTRY, STAILQ_ENTRY, LIST_ENTRY, or 
  TAILQ_ENTRY, named NAME.  The argument HEADNAME is the name of a user defined
  structure that must be declared using the macros SLIST_HEAD, STAILQ_HEAD, 
  LIST_HEAD, or TAILQ_HEAD.  See the examples below for further explanation of
  how these macros are used.

SINGLY-LINKED LISTS
  A singly-linked list is headed by a structure defined by the SLIST_HEAD macro.
  This structure contains a single pointer to the first element on the list. 
  The elements are singly linked for minimum space and pointer manipulation
  overhead at the expense of O(n) removal for arbitrary elements.  New elements
  can be added to the list after an existing element or at the head of the list.
  An SLIST_HEAD structure is declared as follows:

  SLIST_HEAD(HEADNAME, TYPE) head;

  where HEADNAME is the name of the structure to be defined, and TYPE is the 
  type of the elements to be linked into the list.  A pointer to the head of the
  list can later be declared as:

  struct HEADNAME *headp;

  (The names head and headp are user selectable.)

  The macro SLIST_HEAD_INITIALIZER evaluates to an initializer for the list
  head.

  The macro SLIST_EMPTY evaluates to true if there are no elements in the list.

  The macro SLIST_ENTRY declares a structure that connects the elements in the
  list.

  The macro SLIST_FIRST returns the first element in the list or NULL if the 
  list is empty.

  The macro SLIST_FOREACH traverses the list referenced by head in the forward
  direction, assigning each element in turn to var.

  The macro SLIST_FOREACH_FROM behaves identically to SLIST_FOREACH when var is
  NULL, else it treats var as a previously found SLIST element and begins the
  loop at var instead of the first element in the SLIST referenced by head.

  The macro SLIST_FOREACH_SAFE traverses the list referenced by head in the
  forward direction, assigning each element in turn to var.  However, unlike 
  SLIST_FOREACH() here it is permitted to both remove var as well as free it 
  from within the loop safely without interfering with the traversal.

  The macro SLIST_FOREACH_FROM_SAFE behaves identically to SLIST_FOREACH_SAFE 
  when var is NULL, else it treats var as a previously found SLIST element and
  begins the loop at var instead of the first element in the SLIST referenced 
  by head.

  The macro SLIST_INIT initializes the list referenced by head.

  The macro SLIST_INSERT_HEAD inserts the new element elm at the head of the
  list.

  The macro SLIST_INSERT_AFTER inserts the new element elm after the element 
  listelm.

  The macro SLIST_NEXT returns the next element in the list.

  The macro SLIST_REMOVE_AFTER removes the element after elm from the list.
  Unlike SLIST_REMOVE, this macro does not traverse the entire list.

  The macro SLIST_REMOVE_HEAD removes the element elm from the head of the list.
  For optimum efficiency, elements being removed from the head of the list 
  should explicitly use this macro instead of the generic SLIST_REMOVE macro.

  The macro SLIST_REMOVE removes the element elm from the list.

  The macro SLIST_SWAP swaps the contents of head1 and head2.

SINGLY-LINKED LIST EXAMPLE
  SLIST_HEAD(slisthead, entry) head =
   SLIST_HEAD_INITIALIZER(head);
  struct slisthead *headp;     --> Singly-linked List head.
  struct entry {
    ...
    SLIST_ENTRY(entry) entries;  --> Singly-linked List.
    ...
  } *n1, *n2, *n3, *np;

  SLIST_INIT(&head);    -->  Initialize the list.

  n1 = malloc(sizeof(struct entry));   -->  Insert at the head.
  SLIST_INSERT_HEAD(&head, n1, entries);

  n2 = malloc(sizeof(struct entry));   -->  Insert after.
  SLIST_INSERT_AFTER(n1, n2, entries);

  SLIST_REMOVE(&head, n2, entry, entries);-->  Deletion.
  free(n2);

  n3 = SLIST_FIRST(&head);
  SLIST_REMOVE_HEAD(&head, entries);   -->  Deletion from the head.
  free(n3);
      -->  Forward traversal.
  SLIST_FOREACH(np, &head, entries)
    np-> ...
      -->  Safe forward traversal.
  SLIST_FOREACH_SAFE(np, &head, entries, np_temp) {
    np->do_stuff();
    ...
    SLIST_REMOVE(&head, np, entry, entries);
    free(np);
  }

  while (!SLIST_EMPTY(&head)) {    -->  List Deletion.
    n1 = SLIST_FIRST(&head);
    SLIST_REMOVE_HEAD(&head, entries);
    free(n1);
  }

SINGLY-LINKED TAIL QUEUES
  A singly-linked tail queue is headed by a structure defined by the 
  STAILQ_HEAD macro.  This structure contains a pair of pointers, one to the
  first element in the tail queue and the other to the last element in the tail
  queue. The elements are singly linked for minimum space and pointer 
  manipulation overhead at the expense of O(n) removal for arbitrary elements.
  New elements can be added to the tail queue after an existing element, at the
  head of the tail queue, or at the end of the tail queue.  A STAILQ_HEAD 
  structure is declared as follows:

  STAILQ_HEAD(HEADNAME, TYPE) head;

  where HEADNAME is the name of the structure to be defined, and TYPE is the
  type of the elements to be linked into the tail queue.  A pointer to the head
  of the tail queue can later be declared as:

  struct HEADNAME *headp;

  (The names head and headp are user selectable.)

  The macro STAILQ_HEAD_INITIALIZER evaluates to an initializer for the tail
  queue head.

  The macro STAILQ_CONCAT concatenates the tail queue headed by head2 onto the
  end of the one headed by head1 removing all entries from the former.

  The macro STAILQ_EMPTY evaluates to true if there are no items on the tail
  queue.

  The macro STAILQ_ENTRY declares a structure that connects the elements in the
  tail queue.

  The macro STAILQ_FIRST returns the first item on the tail queue or NULL if the
  tail queue is empty.

  The macro STAILQ_FOREACH traverses the tail queue referenced by head in the
  forward direction, assigning each element in turn to var.

  The macro STAILQ_FOREACH_FROM behaves identically to STAILQ_FOREACH when var
  is NULL, else it treats var as a previously found STAILQ element and begins 
  the loop at var instead of the first element in the STAILQ referenced by head.

  The macro STAILQ_FOREACH_SAFE traverses the tail queue referenced by head in 
  the forward direction, assigning each element in turn to var.  However, unlike
  STAILQ_FOREACH() here it is permitted to both remove var as well as free it
  from within the loop safely without interfering with the traversal.

  The macro STAILQ_FOREACH_FROM_SAFE behaves identically to STAILQ_FOREACH_SAFE
  when var is NULL, else it treats var as a previously found STAILQ element and
  begins the loop at var instead of the first element in the STAILQ referenced
  by head.

  The macro STAILQ_INIT initializes the tail queue referenced by head.

  The macro STAILQ_INSERT_HEAD inserts the new element elm at the head of the
  tail queue.

  The macro STAILQ_INSERT_TAIL inserts the new element elm at the end of the
  tail queue.

  The macro STAILQ_INSERT_AFTER inserts the new element elm after the element
  listelm.

  The macro STAILQ_LAST returns the last item on the tail queue.  If the tail
  queue is empty the return value is NULL.

  The macro STAILQ_NEXT returns the next item on the tail queue, or NULL this
  item is the last.

  The macro STAILQ_REMOVE_AFTER removes the element after elm from the tail
  queue. Unlike STAILQ_REMOVE, this macro does not traverse the entire tail 
  queue.

  The macro STAILQ_REMOVE_HEAD removes the element at the head of the tail
  queue.  For optimum efficiency, elements being removed from the head of the
  tail queue should use this macro explicitly rather than the generic 
  STAILQ_REMOVE macro.

  The macro STAILQ_REMOVE removes the element elm from the tail queue.

  The macro STAILQ_SWAP swaps the contents of head1 and head2.

SINGLY-LINKED TAIL QUEUE EXAMPLE
  STAILQ_HEAD(stailhead, entry) head =
   STAILQ_HEAD_INITIALIZER(head);
  struct stailhead *headp;     -->  Singly-linked tail queue head.
  struct entry {
    ...
    STAILQ_ENTRY(entry) entries;    -->  Tail queue.
    ...
  } *n1, *n2, *n3, *np;

  STAILQ_INIT(&head);      -->  Initialize the queue.

  n1 = malloc(sizeof(struct entry));   -->  Insert at the head.
  STAILQ_INSERT_HEAD(&head, n1, entries);

  n1 = malloc(sizeof(struct entry));   -->  Insert at the tail.
  STAILQ_INSERT_TAIL(&head, n1, entries);

  n2 = malloc(sizeof(struct entry));   -->  Insert after.
  STAILQ_INSERT_AFTER(&head, n1, n2, entries);
      -->  Deletion.
  STAILQ_REMOVE(&head, n2, entry, entries);
  free(n2);
      -->  Deletion from the head.
  n3 = STAILQ_FIRST(&head);
  STAILQ_REMOVE_HEAD(&head, entries);
  free(n3);
      -->  Forward traversal.
  STAILQ_FOREACH(np, &head, entries)
    np-> ...
      -->  Safe forward traversal.
  STAILQ_FOREACH_SAFE(np, &head, entries, np_temp) {
    np->do_stuff();
    ...
    STAILQ_REMOVE(&head, np, entry, entries);
    free(np);
  }
      -->  TailQ Deletion.
  while (!STAILQ_EMPTY(&head)) {
    n1 = STAILQ_FIRST(&head);
    STAILQ_REMOVE_HEAD(&head, entries);
    free(n1);
  }
      -->  Faster TailQ Deletion.
  n1 = STAILQ_FIRST(&head);
  while (n1 != NULL) {
    n2 = STAILQ_NEXT(n1, entries);
    free(n1);
    n1 = n2;
  }
  STAILQ_INIT(&head);

LISTS
  A list is headed by a structure defined by the LIST_HEAD macro.  This 
  structure contains a single pointer to the first element on the list.  The
  elements are doubly linked so that an arbitrary element can be removed without
  traversing the list. New elements can be added to the list after an existing
  element, before an existing element, or at the head of the list. A LIST_HEAD
  structure is declared as follows:

  LIST_HEAD(HEADNAME, TYPE) head;

  where HEADNAME is the name of the structure to be defined, and TYPE is the
  type of the elements to be linked into the list.  A pointer to the head of the
  list can later be declared as:

  struct HEADNAME *headp;

  (The names head and headp are user selectable.)

  The macro LIST_HEAD_INITIALIZER evaluates to an initializer for the list head.

  The macro LIST_EMPTY evaluates to true if there are no elements in the list.

  The macro LIST_ENTRY declares a structure that connects the elements in the
  list.

  The macro LIST_FIRST returns the first element in the list or NULL if the list
  is empty.

  The macro LIST_FOREACH traverses the list referenced by head in the forward 
  direction, assigning each element in turn to var.

  The macro LIST_FOREACH_FROM behaves identically to LIST_FOREACH when var is
  NULL, else it treats var as a previously found LIST element and begins the
  loop at var instead of the first element in the LIST referenced by head.

  The macro LIST_FOREACH_SAFE traverses the list referenced by head in the
  forward direction, assigning each element in turn to var.  However, unlike
  LIST_FOREACH() here it is permitted to both remove var as well as free it from
  within the loop safely without interfering with the traversal.

  The macro LIST_FOREACH_FROM_SAFE behaves identically to LIST_FOREACH_SAFE when
  var is NULL, else it treats var as a previously found LIST element and begins
  the loop at var instead of the first element in the LIST referenced by head.

  The macro LIST_INIT initializes the list referenced by head.

  The macro LIST_INSERT_HEAD inserts the new element elm at the head of the
  list.

  The macro LIST_INSERT_AFTER inserts the new element elm after the element
  listelm.

  The macro LIST_INSERT_BEFORE inserts the new element elm before the element
  listelm.

  The macro LIST_NEXT returns the next element in the list, or NULL if this is
  the last.

  The macro LIST_PREV returns the previous element in the list, or NULL if this
  is the first. List head must contain element elm.

  The macro LIST_REMOVE removes the element elm from the list.

  The macro LIST_SWAP swaps the contents of head1 and head2.

LIST EXAMPLE
  LIST_HEAD(listhead, entry) head =
   LIST_HEAD_INITIALIZER(head);
  struct listhead *headp;      -->  List head.
  struct entry {
    ...
    LIST_ENTRY(entry) entries;   -->  List.
    ...
  } *n1, *n2, *n3, *np, *np_temp;

  LIST_INIT(&head);     -->  Initialize the list.

  n1 = malloc(sizeof(struct entry));   -->  Insert at the head.
  LIST_INSERT_HEAD(&head, n1, entries);

  n2 = malloc(sizeof(struct entry));   -->  Insert after.
  LIST_INSERT_AFTER(n1, n2, entries);

  n3 = malloc(sizeof(struct entry));   -->  Insert before.
  LIST_INSERT_BEFORE(n2, n3, entries);

  LIST_REMOVE(n2, entries);      -->  Deletion.
  free(n2);
      -->  Forward traversal.
  LIST_FOREACH(np, &head, entries)
    np-> ...

      -->  Safe forward traversal.
  LIST_FOREACH_SAFE(np, &head, entries, np_temp) {
    np->do_stuff();
    ...
    LIST_REMOVE(np, entries);
    free(np);
  }

  while (!LIST_EMPTY(&head)) {   -->  List Deletion.
    n1 = LIST_FIRST(&head);
    LIST_REMOVE(n1, entries);
    free(n1);
  }

  n1 = LIST_FIRST(&head);      -->  Faster List Deletion.
  while (n1 != NULL) {
    n2 = LIST_NEXT(n1, entries);
    free(n1);
    n1 = n2;
  }
  LIST_INIT(&head);

TAIL QUEUES
  A tail queue is headed by a structure defined by the TAILQ_HEAD macro.  This
  structure contains a pair of pointers, one to the first element in the tail
  queue and the other to the last element in the tail queue.  The elements are
  doubly linked so that an arbitrary element can be removed without traversing
  the tail queue.  New elements can be added to the tail queue after an existing
  element, before an existing element, at the head of the tail queue, or at the
  end of the tail queue.  A TAILQ_HEAD structure is declared as follows:

  TAILQ_HEAD(HEADNAME, TYPE) head;

  where HEADNAME is the name of the structure to be defined, and TYPE is the
  type of the elements to be linked into the tail queue.  A pointer to the head
  of the tail queue can later be declared as:

  struct HEADNAME *headp;

  (The names head and headp are user selectable.)

  The macro TAILQ_HEAD_INITIALIZER evaluates to an initializer for the tail
  queue head.

  The macro TAILQ_CONCAT concatenates the tail queue headed by head2 onto the
  end of the one headed by head1 removing all entries from the former.

  The macro TAILQ_EMPTY evaluates to true if there are no items on the tail
  queue.

  The macro TAILQ_ENTRY declares a structure that connects the elements in the
  tail queue.

  The macro TAILQ_FIRST returns the first item on the tail queue or NULL if the
  tail queue is empty.

  The macro TAILQ_FOREACH traverses the tail queue referenced by head in the
  forward direction, assigning each element in turn to var.  var is set to NULL
  if the loop completes normally, or if there were no elements.

  The macro TAILQ_FOREACH_FROM behaves identically to TAILQ_FOREACH when var is
  NULL, else it treats var as a previously found TAILQ element and begins the
  loop at var instead of the first element in the TAILQ referenced by head.

  The macro TAILQ_FOREACH_REVERSE traverses the tail queue referenced by head in
  the reverse direction, assigning each element in turn to var.

  The macro TAILQ_FOREACH_REVERSE_FROM behaves identically to
  TAILQ_FOREACH_REVERSE when var is NULL, else it treats var as a previously
  found TAILQ element and begins the reverse loop at var instead of the last
  element in the TAILQ referenced by head.

  The macros TAILQ_FOREACH_SAFE and TAILQ_FOREACH_REVERSE_SAFE traverse the list
  referenced by head in the forward or reverse direction respectively, assigning
  each element in turn to var.  However, unlike their unsafe counterparts,
  TAILQ_FOREACH and TAILQ_FOREACH_REVERSE permit to both remove var as well as
  free it from within the loop safely without interfering with the traversal.

  The macro TAILQ_FOREACH_FROM_SAFE behaves identically to TAILQ_FOREACH_SAFE
  when var is NULL, else it treats var as a previously found TAILQ element and
  begins the loop at var instead of the first element in the TAILQ referenced by
  head.

  The macro TAILQ_FOREACH_REVERSE_FROM_SAFE behaves identically to
  TAILQ_FOREACH_REVERSE_SAFE when var is NULL, else it treats var as a
  previously found TAILQ element and begins the reverse loop at var instead of
  the last element in the TAILQ referenced by head.

  The macro TAILQ_INIT initializes the tail queue referenced by head.

  The macro TAILQ_INSERT_HEAD inserts the new element elm at the head of the
  tail queue.

  The macro TAILQ_INSERT_TAIL inserts the new element elm at the end of the
  tail queue.

  The macro TAILQ_INSERT_AFTER inserts the new element elm after the element
  listelm.

  The macro TAILQ_INSERT_BEFORE inserts the new element elm before the element
  listelm.

  The macro TAILQ_LAST returns the last item on the tail queue.  If the tail
  queue is empty the return value is NULL.

  The macro TAILQ_NEXT returns the next item on the tail queue, or NULL if this
  item is the last.

  The macro TAILQ_PREV returns the previous item on the tail queue, or NULL if
  this item is the first.

  The macro TAILQ_REMOVE removes the element elm from the tail queue.

  The macro TAILQ_SWAP swaps the contents of head1 and head2.

TAIL QUEUE EXAMPLE
  TAILQ_HEAD(tailhead, entry) head =
   TAILQ_HEAD_INITIALIZER(head);
  struct tailhead *headp;      -->  Tail queue head.
  struct entry {
    ...
    TAILQ_ENTRY(entry) entries;  -->  Tail queue.
    ...
  } *n1, *n2, *n3, *np;

  TAILQ_INIT(&head);    -->  Initialize the queue.

  n1 = malloc(sizeof(struct entry));   -->  Insert at the head.
  TAILQ_INSERT_HEAD(&head, n1, entries);

  n1 = malloc(sizeof(struct entry));   -->  Insert at the tail.
  TAILQ_INSERT_TAIL(&head, n1, entries);

  n2 = malloc(sizeof(struct entry));   -->  Insert after.
  TAILQ_INSERT_AFTER(&head, n1, n2, entries);

  n3 = malloc(sizeof(struct entry));   -->  Insert before.
  TAILQ_INSERT_BEFORE(n2, n3, entries);

  TAILQ_REMOVE(&head, n2, entries);    -->  Deletion.
  free(n2);
      -->  Forward traversal.
  TAILQ_FOREACH(np, &head, entries)
    np-> ...
      -->  Safe forward traversal.
  TAILQ_FOREACH_SAFE(np, &head, entries, np_temp) {
    np->do_stuff();
    ...
    TAILQ_REMOVE(&head, np, entries);
    free(np);
  }
      -->  Reverse traversal.
  TAILQ_FOREACH_REVERSE(np, &head, tailhead, entries)
    np-> ...
      -->  TailQ Deletion.
  while (!TAILQ_EMPTY(&head)) {
    n1 = TAILQ_FIRST(&head);
    TAILQ_REMOVE(&head, n1, entries);
    free(n1);
  }
      -->  Faster TailQ Deletion.
  n1 = TAILQ_FIRST(&head);
  while (n1 != NULL) {
    n2 = TAILQ_NEXT(n1, entries);
    free(n1);
    n1 = n2;
  }
  TAILQ_INIT(&head);
*/
/*---------------------------------------------------------------------------*/
#define list_head(name, type)\
struct name {\
  struct type *lh_first;  /* first element */\
}

#define list_head_initializer(head)\
  { nullptr }

#define list_entry(type)\
struct {\
  struct type *le_next;  /* next element */\
  struct type **le_prev;  /* address of previous next element */\
}

/*
 * list functions.
 */
#define queuedebug_list_insert_head(head, elm, field)
#define queuedebug_list_op(elm, field)
#define queuedebug_list_postremove(elm, field)

#define  list_init(head)\
do {\
  (head)->lh_first = nullptr;\
} while (/*constcond*/0)

#define list_insert_after(listelm, elm, field)\
do {\
  queuedebug_list_op((listelm), field)\
  if (((elm)->field.le_next = (listelm)->field.le_next) != nullptr)\
    (listelm)->field.le_next->field.le_prev =\
     &(elm)->field.le_next;\
  (listelm)->field.le_next = (elm);\
  (elm)->field.le_prev = &(listelm)->field.le_next;\
} while (/*constcond*/0)

#define  list_insert_before(listelm, elm, field)\
do {\
  queuedebug_list_op((listelm), field)\
  (elm)->field.le_prev = (listelm)->field.le_prev;\
  (elm)->field.le_next = (listelm);\
  *(listelm)->field.le_prev = (elm);\
  (listelm)->field.le_prev = &(elm)->field.le_next;\
} while (/*constcond*/0)

#define list_insert_head(head, elm, field)\
do {\
  queuedebug_list_insert_head((head), (elm), field)\
  if (((elm)->field.le_next = (head)->lh_first) != nullptr)\
    (head)->lh_first->field.le_prev = &(elm)->field.le_next;\
  (head)->lh_first = (elm);\
  (elm)->field.le_prev = &(head)->lh_first;\
} while (/*constcond*/0)

#define list_remove(elm, field)\
do {\
  queuedebug_list_op((elm), field)\
  if ((elm)->field.le_next != nullptr)\
    (elm)->field.le_next->field.le_prev = \
     (elm)->field.le_prev;\
  *(elm)->field.le_prev = (elm)->field.le_next;\
  queuedebug_list_postremove((elm), field)\
} while (/*constcond*/0)

#define list_foreach(var, head, field)\
  for ((var) = ((head)->lh_first);\
    (var);\
    (var) = ((var)->field.le_next))

/*
 * list access methods.
 */
#define  list_empty(head)    ((head)->lh_first == nullptr)
#define  list_first(head)    ((head)->lh_first)
#define  list_next(elm, field)    ((elm)->field.le_next)

/*
 * singly-linked list definitions.
 */
#define slist_head(name, type)\
struct name {\
  struct type *slh_first;  /* first element */\
}

#define slist_head_initializer(head)\
  { nullptr }
 
#define slist_entry(type)\
struct {\
  struct type *sle_next;  /* next element */\
}
 
/*
 * singly-linked list functions.
 */
#define  slist_empty(head)  ((head)->slh_first == nullptr)
#define  slist_first(head)  ((head)->slh_first)
#define  slist_next(elm, field)  ((elm)->field.sle_next)

#define slist_foreach(var, head, field)\
  for((var) = (head)->slh_first; (var); (var) = (var)->field.sle_next)

#define slist_init(head)\
do {\
  (head)->slh_first = nullptr;\
} while (/*constcond*/0)

#define slist_insert_after(slistelm, elm, field)\
do {\
  (elm)->field.sle_next = (slistelm)->field.sle_next;\
  (slistelm)->field.sle_next = (elm);\
} while (/*constcond*/0)

#define slist_insert_head(head, elm, field)\
do {\
  (elm)->field.sle_next = (head)->slh_first;\
  (head)->slh_first = (elm);\
} while (/*constcond*/0)

#define slist_next(elm, field)  ((elm)->field.sle_next)

#define slist_remove_head(head, field)\
do {\
  (head)->slh_first = (head)->slh_first->field.sle_next;\
} while (/*constcond*/0)

#define slist_remove(head, elm, type, field)\
do {\
  if ((head)->slh_first == (elm)) {\
    slist_remove_head((head), field);\
  }\
  else {\
    struct type *curelm = (head)->slh_first;\
    while(curelm->field.sle_next != (elm))\
   curelm = curelm->field.sle_next;\
    curelm->field.sle_next =\
     curelm->field.sle_next->field.sle_next;\
  }\
} while (/*constcond*/0)

/*
 * simple queue definitions.
 */
#define simpleq_head(name, type)\
struct name {\
  struct type *sqh_first;  /* first element */\
  struct type **sqh_last;  /* addr of last next element */\
}

#define simpleq_head_initializer(head)\
  { nullptr, &(head).sqh_first }

#define simpleq_entry(type)\
struct {\
  struct type *sqe_next;  /* next element */\
}

/*
 * simple queue functions.
 */
#define  simpleq_init(head)\
do {\
  (head)->sqh_first = nullptr;\
  (head)->sqh_last = &(head)->sqh_first;\
} while (/*constcond*/0)

#define simpleq_insert_head(head, elm, field)\
do {\
  if (((elm)->field.sqe_next = (head)->sqh_first) == nullptr)\
    (head)->sqh_last = &(elm)->field.sqe_next;\
  (head)->sqh_first = (elm);\
} while (/*constcond*/0)

#define simpleq_insert_tail(head, elm, field)\
do {\
  (elm)->field.sqe_next = nullptr;\
  *(head)->sqh_last = (elm);\
  (head)->sqh_last = &(elm)->field.sqe_next;\
} while (/*constcond*/0)

#define simpleq_insert_after(head, listelm, elm, field)\
do {\
  if (((elm)->field.sqe_next = (listelm)->field.sqe_next) == nullptr)\
    (head)->sqh_last = &(elm)->field.sqe_next;\
  (listelm)->field.sqe_next = (elm);\
} while (/*constcond*/0)

#define simpleq_remove_head(head, elm, field)\
do {\
  if (((head)->sqh_first = (elm)->field.sqe_next) == nullptr)\
    (head)->sqh_last = &(head)->sqh_first;\
} while (/*constcond*/0)

#define simpleq_foreach(var, head, field)\
  for ((var) = ((head)->sqh_first);\
    (var);\
    (var) = ((var)->field.sqe_next))

/*
 * simple queue access methods.
 */
#define  simpleq_empty(head)    ((head)->sqh_first == nullptr)
#define  simpleq_first(head)    ((head)->sqh_first)
#define  simpleq_next(elm, field)  ((elm)->field.sqe_next)

/*
 * tail queue definitions.
 */
#define tailq_head(name, type)\
struct name {\
  struct type *tqh_first;  /* first element */\
  struct type **tqh_last;  /* addr of last next element */\
}

#define tailq_head_initializer(head)\
  { nullptr, &(head).tqh_first }

#define tailq_entry(type)\
struct {\
  struct type *tqe_next;  /* next element */\
  struct type **tqe_prev;  /* address of previous next element */\
}

/*
 * tail queue functions.
 */
#define queuedebug_tailq_insert_head(head, elm, field)
#define queuedebug_tailq_insert_tail(head, elm, field)
#define queuedebug_tailq_op(elm, field)
#define queuedebug_tailq_postremove(elm, field)

#define  tailq_init(head)\
do {\
  (head)->tqh_first = nullptr;\
  (head)->tqh_last = &(head)->tqh_first;\
} while (/*constcond*/0)

#define tailq_insert_head(head, elm, field)\
do {\
  queuedebug_tailq_insert_head((head), (elm), field)\
  if (((elm)->field.tqe_next = (head)->tqh_first) != nullptr)\
    (head)->tqh_first->field.tqe_prev =\
     &(elm)->field.tqe_next;\
  else\
    (head)->tqh_last = &(elm)->field.tqe_next;\
  (head)->tqh_first = (elm);\
  (elm)->field.tqe_prev = &(head)->tqh_first;\
} while (/*constcond*/0)

#define tailq_insert_tail(head, elm, field)\
do {\
  queuedebug_tailq_insert_tail((head), (elm), field)\
  (elm)->field.tqe_next = nullptr;\
  (elm)->field.tqe_prev = (head)->tqh_last;\
  *(head)->tqh_last = (elm);\
  (head)->tqh_last = &(elm)->field.tqe_next;\
} while (/*constcond*/0)

#define tailq_insert_after(head, listelm, elm, field)\
do {\
  queuedebug_tailq_op((listelm), field)\
  if (((elm)->field.tqe_next = (listelm)->field.tqe_next) != nullptr)\
    (elm)->field.tqe_next->field.tqe_prev = \
     &(elm)->field.tqe_next;\
  else\
    (head)->tqh_last = &(elm)->field.tqe_next;\
  (listelm)->field.tqe_next = (elm);\
  (elm)->field.tqe_prev = &(listelm)->field.tqe_next;\
} while (/*constcond*/0)

#define  tailq_insert_before(listelm, elm, field)\
do {\
  queuedebug_tailq_op((listelm), field)\
  (elm)->field.tqe_prev = (listelm)->field.tqe_prev;\
  (elm)->field.tqe_next = (listelm);\
  *(listelm)->field.tqe_prev = (elm);\
  (listelm)->field.tqe_prev = &(elm)->field.tqe_next;\
} while (/*constcond*/0)

#define tailq_remove(head, elm, field)\
do {\
  queuedebug_tailq_op((elm), field)\
  if (((elm)->field.tqe_next) != nullptr)\
    (elm)->field.tqe_next->field.tqe_prev = \
     (elm)->field.tqe_prev;\
  else\
    (head)->tqh_last = (elm)->field.tqe_prev;\
  *(elm)->field.tqe_prev = (elm)->field.tqe_next;\
  queuedebug_tailq_postremove((elm), field);\
} while (/*constcond*/0)

/*
 * tail queue access methods.
 */
#define  tailq_empty(head)    ((head)->tqh_first == nullptr)
#define  tailq_first(head)    ((head)->tqh_first)
#define  tailq_next(elm, field)    ((elm)->field.tqe_next)

#define tailq_last(head, headname) \
  (*(((struct headname *)((head)->tqh_last))->tqh_last))
#define tailq_prev(elm, headname, field) \
  (*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))

#define tailq_foreach(var, head, field)\
  for ((var) = ((head)->tqh_first);\
    (var);\
    (var) = ((var)->field.tqe_next))

#define tailq_foreach_reverse(var, head, headname, field)\
  for ((var) = (*(((struct headname *)((head)->tqh_last))->tqh_last));\
    (var);\
    (var) = (*(((struct headname *)((var)->field.tqe_prev))->tqh_last)))

/*
 * circular queue definitions.
 */
#define circleq_head(name, type)\
struct name {\
  struct type *cqh_first;    /* first element */\
  struct type *cqh_last;    /* last element */\
}

#define circleq_head_initializer(head)\
  { (void *)&head, (void *)&head }

#define circleq_entry(type)\
struct {\
  struct type *cqe_next;    /* next element */\
  struct type *cqe_prev;    /* previous element */\
}

/*
 * circular queue functions.
 */
#define  circleq_init(head)\
do {\
  (head)->cqh_first = (void *)(head);\
  (head)->cqh_last = (void *)(head);\
} while (/*constcond*/0)

#define circleq_insert_after(head, listelm, elm, field)\
do {\
  (elm)->field.cqe_next = (listelm)->field.cqe_next;\
  (elm)->field.cqe_prev = (listelm);\
  if ((listelm)->field.cqe_next == (void *)(head))\
    (head)->cqh_last = (elm);\
  else\
    (listelm)->field.cqe_next->field.cqe_prev = (elm);\
  (listelm)->field.cqe_next = (elm);\
} while (/*constcond*/0)

#define circleq_insert_before(head, listelm, elm, field)\
do {\
  (elm)->field.cqe_next = (listelm);\
  (elm)->field.cqe_prev = (listelm)->field.cqe_prev;\
  if ((listelm)->field.cqe_prev == (void *)(head))\
    (head)->cqh_first = (elm);\
  else\
    (listelm)->field.cqe_prev->field.cqe_next = (elm);\
  (listelm)->field.cqe_prev = (elm);\
} while (/*constcond*/0)

#define circleq_insert_head(head, elm, field)\
do {\
  (elm)->field.cqe_next = (head)->cqh_first;\
  (elm)->field.cqe_prev = (void *)(head);\
  if ((head)->cqh_last == (void *)(head))\
    (head)->cqh_last = (elm);\
  else\
    (head)->cqh_first->field.cqe_prev = (elm);\
  (head)->cqh_first = (elm);\
} while (/*constcond*/0)

#define circleq_insert_tail(head, elm, field)\
do {\
  (elm)->field.cqe_next = (void *)(head);\
  (elm)->field.cqe_prev = (head)->cqh_last;\
  if ((head)->cqh_first == (void *)(head))\
    (head)->cqh_first = (elm);\
  else\
    (head)->cqh_last->field.cqe_next = (elm);\
  (head)->cqh_last = (elm);\
} while (/*constcond*/0)

#define  circleq_remove(head, elm, field)\
do {\
  if ((elm)->field.cqe_next == (void *)(head))\
    (head)->cqh_last = (elm)->field.cqe_prev;\
  else\
    (elm)->field.cqe_next->field.cqe_prev =\
     (elm)->field.cqe_prev;\
  if ((elm)->field.cqe_prev == (void *)(head))\
    (head)->cqh_first = (elm)->field.cqe_next;\
  else\
    (elm)->field.cqe_prev->field.cqe_next =\
     (elm)->field.cqe_next;\
} while (/*constcond*/0)

#define circleq_foreach(var, head, field)\
  for ((var) = ((head)->cqh_first);\
   (var) != (void *)(head);\
   (var) = ((var)->field.cqe_next))

#define circleq_foreach_reverse(var, head, field)\
  for ((var) = ((head)->cqh_last);\
   (var) != (void *)(head);\
   (var) = ((var)->field.cqe_prev))

/*
 * circular queue access methods.
 */
#define  circleq_empty(head)    ((head)->cqh_first == (void *)(head))
#define  circleq_first(head)    ((head)->cqh_first)
#define  circleq_last(head)    ((head)->cqh_last)
#define  circleq_next(elm, field)  ((elm)->field.cqe_next)
#define  circleq_prev(elm, field)  ((elm)->field.cqe_prev)
#endif  /* !_SYS_QUEUE_H_ */

