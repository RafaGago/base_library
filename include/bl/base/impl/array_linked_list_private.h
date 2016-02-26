#ifndef __ARRAY_LINKED_LIST_PRIVATE_H__
#define __ARRAY_LINKED_LIST_PRIVATE_H__

/*---------------------------------------------------------------------------*/
#define alnls_it_next_priv(type_ptr, it) (alnls_nodes (type_ptr)[it])
#define alnls_it_begin_priv(type_ptr)    ((type_ptr)->head)
#define alnls_nfree_val_priv(it)         (it)
/*---------------------------------------------------------------------------*/
#define alnls_define_types_asserts_private(name, index_uint_type, node_count)\
static_assert_outside_func(\
  utype_max (index_uint_type) > (node_count),\
  "the specified index unsigned type can't hold the required number of "\
  "possible indexes + 1 (null)"\
  );\
\
static_assert_outside_func(\
  sizeof (index_uint_type) < sizeof (alnls_it),\
  "the specified unsigned type is bigger than the iterator unsigned type (the "\
  "iterator would overflow). you could be using a regular pointer based linked"\
  " list instead."\
  );
/*---------------------------------------------------------------------------*/
#define alnls_dummy_size_accessors_private(\
  name,\
  index_uint_type,\
  func_prefix\
  )\
static inline void name##_size_set_private (name* l, index_uint_type v)\
{}\
\
static inline index_uint_type name##_size_get_private (name* l)\
{\
  return 0;\
}\
/*---------------------------------------------------------------------------*/
#define alnls_sz_size_accessors_private(\
  name,\
  index_uint_type,\
  func_prefix\
  )\
static inline void name##_size_set_private (name* l, index_uint_type v)\
{\
  l->size = v;\
}\
\
static inline index_uint_type name##_size_get_private (name* l)\
{\
  return l->size;\
}
/*---------------------------------------------------------------------------*/
#define alnls_define_funcs_private(\
  name,\
  index_uint_type,\
  func_prefix\
  )\
\
func_prefix void name##_find_two_previous_nodes_private(\
  name* l, alnls_it* prev, alnls_it* prev2, alnls_it val\
  )\
{\
  bl_assert (!alnls_is_empty (l));\
  *prev2       = alnls_it_end (l);\
  *prev        = alnls_it_end (l);\
  alnls_it now = alnls_it_begin (l);\
\
  while (now != val) {\
    *prev2 = *prev;\
    *prev  = now;\
    now    = alnls_it_next (l, now);\
  }\
}\
\
func_prefix void name##_init (name* l)\
{\
  alnls_it_begin_priv (l) = alnls_it_end (l);\
  for (alnls_it it = 0; it < alnls_it_end (l); ++it) {\
    alnls_node_release (l, it);\
  }\
  name##_size_set_private (l, 0);\
}\
\
func_prefix alnls_it name##_try_acquire_node (name* l, alnls_it n)\
{\
  if (alnls_node_is_free (l, n)) {\
    alnls_node_acquire_unsafe (l, n);\
    return n;\
  }\
  return alnls_it_end (l);\
}\
\
func_prefix alnls_it name##_try_acquire_a_node (name* l)\
{\
  alnls_it i;\
  for (i = 0; i < alnls_it_end (l); ++i) {\
    if (alnls_node_is_free (l, i)) {\
      alnls_node_acquire_unsafe (l, i);\
      break;\
    }\
  }\
  return i;\
}\
\
func_prefix void name##_insert_head (name* l, alnls_it n)\
{\
  bl_assert (alnls_it_next (l, n) == alnls_it_end (l));\
  alnls_it_next_priv (l, n) = alnls_it_begin (l);\
  alnls_it_begin_priv (l)   = (index_uint_type) n;\
  bl_assert (alnls_it_begin (l) == n);\
  name##_size_set_private(\
    l, name##_size_get_private (l) + 1\
    );\
}\
\
func_prefix void name##_insert_tail (name* l, alnls_it n)\
{\
  bl_assert (alnls_it_next (l, n) == alnls_it_end (l));\
  if (!alnls_is_empty (l)) {\
    alnls_it tail, subtail;\
    name##_find_two_previous_nodes_private(\
      l, &tail, &subtail, alnls_it_end (l)\
      );\
    alnls_it_next_priv (l, tail) = (index_uint_type) n;\
    bl_assert (alnls_it_next (l, tail) == n);\
  }\
  else {\
    alnls_it_begin_priv (l) = (index_uint_type) n;\
    bl_assert (alnls_it_begin (l) == n);\
  }\
  name##_size_set_private(\
    l, name##_size_get_private (l) + 1\
    );\
}\
\
func_prefix alnls_it name##_drop (name* l, alnls_it n)\
{\
  bl_assert (alnls_it_in_range( l, n));\
  bl_assert (!alnls_node_is_free (l, n));\
\
  if (alnls_is_empty (l)) {\
    return alnls_it_end (l);\
  }\
  alnls_it should_be_n, prev, find;\
  find = alnls_it_next (l, n);\
  name##_find_two_previous_nodes_private(\
    l, &should_be_n, &prev, find\
    );\
  bl_assert (should_be_n == n);\
  if (prev != alnls_it_end (l)) {\
    alnls_it_next_priv (l, prev) = find;\
  }\
  else {\
    alnls_it_begin_priv (l) = alnls_it_end (l);\
  }\
  alnls_it_next_priv (l, n) = alnls_it_end (l);\
  name##_size_set_private(\
    l, name##_size_get_private (l) - 1\
    );\
  return prev;\
}\
\
func_prefix alnls_it name##_drop_head (name* l)\
{\
  if (alnls_is_empty (l)) {\
    return alnls_it_end (l);\
  }\
  alnls_it prev_head                = alnls_it_begin (l);\
  alnls_it_begin_priv (l)           = alnls_it_next (l, prev_head);\
  alnls_it_next_priv (l, prev_head) = alnls_it_end (l);\
  name##_size_set_private(\
    l, name##_size_get_private (l) - 1\
    );\
  return prev_head;\
}\
\
func_prefix alnls_it name##_drop_tail (name* l)\
{\
  if (alnls_is_empty (l)) {\
    return alnls_it_end (l);\
  }\
  alnls_it tail, subtail;\
  name##_find_two_previous_nodes_private(\
    l, &tail, &subtail, alnls_it_end (l)\
    );\
  if (subtail != alnls_it_end (l)) {\
    alnls_it_next_priv (l, subtail) = alnls_it_end (l);\
  }\
  else {\
    alnls_it_begin_priv (l) = alnls_it_end (l);\
  }\
  name##_size_set_private(\
    l, name##_size_get_private (l) - 1\
    );\
  return tail;\
}
/*---------------------------------------------------------------------------*/
#endif /* __ARRAY_LINKED_LIST_PRIVATE_H__ */

