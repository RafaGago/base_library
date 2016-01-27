#ifndef __BL_ARRAY_LINKED_LIST_H__
#define __BL_ARRAY_LINKED_LIST_H__

/*---------------------------------------------------------------------------*/
/*
  A singly-linked list with node references as array indexes to an embedded
  fixed sized array.

  Note that there is no data storage. You can use the iterators directly to
  access another array containing the data you want to associate with each
  entry.

  This is a very specialized data structure just for memory constrained 
  environments. Not a general use one.

  For further usage reference see the unit tests for this type.
*/
/*---------------------------------------------------------------------------*/
#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/impl/array_linked_list_private.h>

typedef uword alnls_it; 
/*---------------------------------------------------------------------------*/
#define alnls_nodes(type_ptr)            ((type_ptr)->nodes)
#define alnls_capacity(type_ptr)         arr_elems (alnls_nodes (type_ptr))
/*---------------------------------------------------------------------------*/
#define alnls_it_end(type_ptr)          alnls_capacity (type_ptr)
#define alnls_it_in_range(type_ptr, it) (it < alnls_it_end (type_ptr))
#define alnls_it_next(type_ptr, it)     (alnls_it_next_priv (type_ptr, it) + 0)
#define alnls_it_begin(type_ptr)        (alnls_it_begin_priv (type_ptr) + 0)
/*---------------------------------------------------------------------------*/
#define alnls_is_empty(type_ptr)\
  (alnls_it_begin (type_ptr) == alnls_it_end (type_ptr))
/*---------------------------------------------------------------------------*/
#define alnls_node_is_free(type_ptr, it)\
  (alnls_it_next (type_ptr, it) == alnls_nfree_val_priv (it))

#define alnls_node_acquire_unsafe(type_ptr, it)\
do {\
  bl_assert (it < alnls_capacity (type_ptr));\
  bl_assert (alnls_node_is_free (type_ptr, it));\
  alnls_it_next_priv (type_ptr, it) = alnls_it_end (type_ptr);\
} while (0)

#define alnls_node_release(type_ptr, it)\
do {\
  bl_assert (it < alnls_capacity (type_ptr));\
  alnls_it_next_priv (type_ptr, it) = alnls_nfree_val_priv (it);\
} while (0)
/*---------------------------------------------------------------------------*/
#define alnls_foreach(type_ptr, it_var)\
  for (alnls_it alnls_priv_curr_it = alnls_it_begin ((type_ptr)),\
         it_var = alnls_priv_curr_it;\
       alnls_it_in_range ((type_ptr), alnls_priv_curr_it) &&\
         (alnls_priv_curr_it =\
            alnls_it_next ((type_ptr), alnls_priv_curr_it), 1);\
       it_var = alnls_priv_curr_it\
       )

#define alnls_foreach_read_only(type_ptr, it_var)\
  for (it_var = alnls_it_begin ((type_ptr));\
       alnls_it_in_range ((type_ptr), it_var);\
       it_var = alnls_it_next ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define alnls_define_types(\
  new_alnls_type_basename,\
  index_uint_type,\
  node_count\
  )\
\
alnls_define_types_asserts_private(\
  new_alnls_type_basename, index_uint_type, (node_count)\
  )\
\
typedef struct new_alnls_type_basename {\
  index_uint_type head;\
  index_uint_type nodes[(node_count)];\
}\
new_alnls_type_basename;
/*---------------------------------------------------------------------------*/
#define alnls_declare_funcs(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
func_prefix void new_alnls_type_basename##_find_two_previous_nodes_private(\
  new_alnls_type_basename* l, alnls_it* prev, alnls_it* prev2, alnls_it val\
  );\
\
func_prefix void new_alnls_type_basename##_init(\
  new_alnls_type_basename* l\
  );\
\
\
func_prefix alnls_it new_alnls_type_basename##_try_acquire_a_node(\
  new_alnls_type_basename* l\
  );\
func_prefix alnls_it new_alnls_type_basename##_try_acquire_node(\
  new_alnls_type_basename* l, alnls_it n\
  );\
\
func_prefix void new_alnls_type_basename##_insert_head(\
  new_alnls_type_basename* l, alnls_it n\
  );\
\
func_prefix void new_alnls_type_basename##_insert_tail(\
  new_alnls_type_basename* l, alnls_it n\
  );\
\
func_prefix alnls_it new_alnls_type_basename##_drop(\
  new_alnls_type_basename* l, alnls_it n\
  );\
\
func_prefix alnls_it new_alnls_type_basename##_drop_head(\
  new_alnls_type_basename* l\
  );\
\
func_prefix alnls_it new_alnls_type_basename##_drop_tail(\
  new_alnls_type_basename* l\
  );
/*---------------------------------------------------------------------------*/
#define alnls_define_funcs(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
alnls_dummy_size_accessors_private(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
alnls_define_funcs_private(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )
/*---------------------------------------------------------------------------*/
/* Same list than the above one but this one keeps a size counter            */
/*---------------------------------------------------------------------------*/
#define alnls_sz_define_types(\
  new_alnls_type_basename,\
  index_uint_type,\
  node_count\
  )\
\
alnls_define_types_asserts_private(\
  new_alnls_type_basename, index_uint_type, (node_count)\
  )\
\
typedef struct new_alnls_type_basename {\
  index_uint_type head;\
  index_uint_type nodes[(node_count)];\
  index_uint_type size;\
}\
new_alnls_type_basename;
/*---------------------------------------------------------------------------*/
#define alnls_sz_declare_funcs(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
alnls_declare_funcs(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
index_uint_type new_alnls_type_basename##_size(\
  new_alnls_type_basename* l\
  );
/*---------------------------------------------------------------------------*/
#define alnls_sz_define_funcs(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
alnls_sz_size_accessors_private(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
alnls_define_funcs_private(\
  new_alnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
func_prefix index_uint_type new_alnls_type_basename##_size(\
  new_alnls_type_basename* l\
  )\
{\
  return new_alnls_type_basename##_size_get_private (l);\
}
/*---------------------------------------------------------------------------*/
#if 0 

/*
debug test code. Equivalent to the next two macro invocations:

alnls_define_types (alnls_test, u8, 19)
alnls_define_funcs (alnls_test, u8, static inline)
*/

typedef struct alnls_test {
  u8 head;
  u8 nodes[19];
}
alnls_test;
/*---------------------------------------------------------------------------*/
static inline void alnls_test_init (alnls_test* l)
{
  alnls_it_begin_priv (l) = alnls_it_end (l);
  for (alnls_it it = 0; it < alnls_it_end (l); ++it) {
    alnls_node_release (l, it);
  }
}

static inline void alnls_test_find_two_previous_nodes_private(
  alnls_test* l, alnls_it* prev, alnls_it* prev2, alnls_it val
  )
{
  bl_assert (!alnls_is_empty (l));
  *prev2       = alnls_it_end (l);
  *prev        = alnls_it_end (l);
  alnls_it now = alnls_it_begin (l);

  while (now != val) {
    *prev2 = *prev;
    *prev  = now;
    now    = alnls_it_next (l, now);
  }
}

static inline alnls_it alnls_test_try_acquire_node (alnls_test* l, alnls_it n)
{
  if (alnls_node_is_free (l, n)) {
    alnls_node_acquire_unsafe (l, n);
    return n;
  }
  return alnls_it_end (l);
}

static inline alnls_it alnls_test_try_acquire_a_node (alnls_test* l)
{
  alnls_it i;
  for (i = 0; i < alnls_it_end (l); ++i) {
    if (alnls_node_is_free (l, i)) {
      alnls_node_acquire_unsafe (l, i);
      break;
    }
  }
  return i;
}

static inline void alnls_test_insert_head (alnls_test* l, alnls_it n)
{
  bl_assert (alnls_it_next (l, n) == alnls_it_end (l));
  alnls_it_next_priv (l, n) = alnls_it_begin (l);
  alnls_it_begin_priv (l)   = (u8) n;
  bl_assert (alnls_it_begin (l) == n);
}

static inline void alnls_test_insert_tail (alnls_test* l, alnls_it n)
{
  bl_assert (alnls_it_next (l, n) == alnls_it_end (l));
  if (!alnls_is_empty (l)) {
    alnls_it tail, subtail;
    alnls_test_find_two_previous_nodes_private(
      l, &tail, &subtail, alnls_it_end (l)
      );
    alnls_it_next_priv (l, tail) = (u8) n;
    bl_assert (alnls_it_next (l, tail) == n);
  }
  else {
    alnls_it_begin_priv (l) = (u8) n;
    bl_assert (alnls_it_begin (l) == n);
  }
}

static inline alnls_it alnls_test_drop (alnls_test* l, alnls_it n)
{
  bl_assert (alnls_it_in_range( l, n));
  bl_assert (!alnls_node_is_free (l, n));

  if (alnls_is_empty (l)) {
    return alnls_it_end (l);
  }
  alnls_it should_be_n, prev, find;
  find = alnls_it_next (l, n);
  alnls_test_find_two_previous_nodes_private (l, &should_be_n, &prev, find);
  bl_assert (should_be_n == n);
  if (prev != alnls_it_end (l)) {
    alnls_it_next_priv (l, prev) = find;
  }
  else {
    alnls_it_begin_priv (l) = alnls_it_end (l);
  }
  alnls_it_next_priv (l, n) = alnls_it_end (l);
  return prev;
}

static inline alnls_it alnls_test_drop_head (alnls_test* l)
{
  if (alnls_is_empty (l)) {
    return alnls_it_end (l);
  }
  alnls_it prev_head                = alnls_it_begin (l);
  alnls_it_begin_priv (l)           = alnls_it_next (l, prev_head);
  alnls_it_next_priv (l, prev_head) = alnls_it_end (l);
  return prev_head;
}

static inline alnls_it alnls_test_drop_tail (alnls_test* l)
{
  if (alnls_is_empty (l)) {
    return alnls_it_end (l);
  }
  alnls_it tail, subtail;
  alnls_test_find_two_previous_nodes_private(
    l, &tail, &subtail, alnls_it_end (l)
    );
  if (subtail != alnls_it_end (l)) {
    alnls_it_next_priv (l, subtail) = alnls_it_end (l);
  }
  else {
    alnls_it_begin_priv (l) = alnls_it_end (l);
  }
  return tail;
}
#endif

#endif /* __BL_ARRAY_LINKED_LIST_H__ */

