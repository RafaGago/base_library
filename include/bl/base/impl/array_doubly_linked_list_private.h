#ifndef __ARRAY_DOUBLY_LINKED_LIST_PRIVATE_H__
#define __ARRAY_DOUBLY_LINKED_LIST_PRIVATE_H__

/*---------------------------------------------------------------------------*/
#define adlnls_it_next_priv(type_ptr, it)\
  (adlnls_nodes (type_ptr)[it].next)
#define adlnls_it_prev_priv(type_ptr, it)\
  (adlnls_nodes (type_ptr)[it].prev)
#define adlnls_it_begin_priv(type_ptr)      ((type_ptr)->head.next)
#define adlnls_it_rbegin_priv(type_ptr)     ((type_ptr)->head.prev)
#define adlnls_nfree_val_priv(it)           (it)

#define adlnls_set_node_insert_ready_priv(type_ptr, it)\
do {\
  adlnls_it_next_priv (type_ptr, it) = adlnls_it_end (type_ptr);\
  adlnls_it_prev_priv (type_ptr, it) = adlnls_it_end (type_ptr);\
} while (0)

#define adlnls_is_node_insert_ready_priv(type_ptr, it)\
  (adlnls_it_next (type_ptr, it) == adlnls_it_end (type_ptr) &&\
   adlnls_it_prev (type_ptr, it) == adlnls_it_end (type_ptr))
/*---------------------------------------------------------------------------*/
#define adlnls_node_type_middle        0
#define adlnls_node_type_tail          1
#define adlnls_node_type_head          2
#define adlnls_node_type_tail_and_head 3

#define adlns_get_node_type(type_ptr, it)\
  (((adlnls_it) adlnls_it_next (l, it) == adlnls_it_end (l)) |\
   (((adlnls_it) adlnls_it_prev (l, it) == adlnls_it_end (l)) << 1))
/*---------------------------------------------------------------------------*/
#define adlnls_define_types_base_private(\
  name,\
  index_uint_type,\
  node_count\
  )\
static_assert_outside_func(\
  utype_max (index_uint_type) > ((node_count) * 2),\
  "the specified index unsigned type can't hold the required number of "\
  "possible indexes + 1 (null)"\
  );\
\
static_assert_outside_func(\
  sizeof (index_uint_type) < (2 * sizeof (adlnls_it)),\
  "the specified unsigned type is bigger than the iterator unsigned type (the "\
  "iterator would overflow). you could be using a regular pointer based linked"\
  " list instead."\
  );\
typedef struct name##_hook {\
  index_uint_type next : type_bits (index_uint_type) / 2;\
  index_uint_type prev : type_bits (index_uint_type) / 2;\
}\
name##_hook;\
/*---------------------------------------------------------------------------*/
#define adlnls_dummy_size_accessors_private(\
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
#define adlnls_sz_size_accessors_private(\
  name,\
  index_uint_type,\
  func_prefix\
  )\
static inline void name##_size_set_private (name* l, index_uint_type v)\
{\
  l->size = v;\
}\
\
static inline index_uint_type name##_size_get_private(name* l)\
{\
  return l->size;\
}
/*---------------------------------------------------------------------------*/
#define adlnls_define_funcs_private(\
  name,\
  index_uint_type,\
  func_prefix\
  )\
\
func_prefix void name##_init (name* l)\
{\
  adlnls_it_begin_priv (l)  = adlnls_it_end (l);\
  adlnls_it_rbegin_priv (l) = adlnls_it_end (l);\
  for (adlnls_it it = 0; it < adlnls_it_end (l); ++it) {\
    adlnls_it_next_priv (l, it) = adlnls_nfree_val_priv (it);\
    adlnls_it_prev_priv (l, it) = adlnls_nfree_val_priv (it);\
  }\
  name##_size_set_private (l, 0);\
}\
func_prefix adlnls_it name##_try_acquire_node (name* l, adlnls_it n)\
{\
  if (adlnls_node_is_free (l, n)) {\
    adlnls_node_acquire_unsafe (l, n);\
    return n;\
  }\
  return adlnls_it_end (l);\
}\
\
func_prefix adlnls_it name##_try_acquire_a_node (name* l)\
{\
  adlnls_it i;\
  for (i = 0; i < adlnls_it_end (l); ++i) {\
    if (adlnls_node_is_free (l, i)) {\
      adlnls_node_acquire_unsafe (l, i);\
      break;\
    }\
  }\
  return i;\
}\
\
func_prefix void name##_insert_head (name* l, adlnls_it n)\
{\
  bl_assert (adlnls_is_node_insert_ready_priv (l, n));\
  if (!adlnls_is_empty (l)) {\
    adlnls_it_prev_priv (l, adlnls_it_begin (l)) = n;\
    bl_assert (adlnls_it_prev (l, adlnls_it_begin (l)) == n);\
    adlnls_it_next_priv (l, n) = adlnls_it_begin (l);\
  }\
  else {\
    adlnls_it_rbegin_priv (l) = (index_uint_type) n;  \
    bl_assert (adlnls_it_rbegin (l) == n);\
  }\
  adlnls_it_begin_priv (l) = (index_uint_type) n;\
  bl_assert (adlnls_it_begin (l) == n);\
  name##_size_set_private (l, name##_size_get_private (l) + 1);\
}\
\
func_prefix void name##_insert_tail (name* l, adlnls_it n)\
{\
  bl_assert (adlnls_is_node_insert_ready_priv (l, n));\
  if (!adlnls_is_empty (l)) {\
    adlnls_it_next_priv (l, adlnls_it_rbegin (l)) = n;\
    bl_assert (adlnls_it_next (l, adlnls_it_rbegin (l)) == n);\
    adlnls_it_prev_priv (l, n) = adlnls_it_rbegin (l);\
  }\
  else {\
    adlnls_it_begin_priv (l) = (index_uint_type) n;  \
    bl_assert (adlnls_it_begin (l) == n);\
  }\
  adlnls_it_rbegin_priv (l) = (index_uint_type) n;\
  bl_assert (adlnls_it_rbegin (l) == n);\
  name##_size_set_private (l, name##_size_get_private (l) + 1);\
}\
\
func_prefix adlnls_it name##_drop_explicit(\
  name* l, adlnls_it n, bool return_previous\
  )\
{\
  bl_assert (adlnls_it_in_range (l, n));\
  bl_assert (!adlnls_node_is_free (l, n));\
\
  if (adlnls_is_empty (l)) {\
    return adlnls_it_end (l);\
  }\
\
  adlnls_it ret;\
  switch (adlns_get_node_type (l, n)) {\
  case adlnls_node_type_tail_and_head: {\
    ret                       = adlnls_it_end (l);\
    adlnls_it_begin_priv (l)  = adlnls_it_end (l);\
    adlnls_it_rbegin_priv (l) = adlnls_it_end (l);\
    goto do_return;\
  }\
  case adlnls_node_type_tail:{ \
    ret = return_previous ? adlnls_it_prev (l, n) : adlnls_it_end (l);\
    adlnls_it_next_priv (l, adlnls_it_prev (l, n)) = adlnls_it_end (l);\
    adlnls_it_rbegin_priv (l) = adlnls_it_prev (l, n);\
    goto do_return;\
  }\
  case adlnls_node_type_head: {\
    ret = return_previous ? adlnls_it_end (l) : adlnls_it_next (l, n);\
    adlnls_it_prev_priv (l, adlnls_it_next (l, n)) = adlnls_it_end (l);\
    adlnls_it_begin_priv (l) = adlnls_it_next (l, n);\
    goto do_return;\
  }\
  case adlnls_node_type_middle: {\
    ret = return_previous ? adlnls_it_prev (l, n) : adlnls_it_next (l, n);\
    adlnls_it_prev_priv (l, adlnls_it_next (l, n)) = adlnls_it_prev (l, n);\
    adlnls_it_next_priv (l, adlnls_it_prev (l, n)) = adlnls_it_next (l, n);\
    goto do_return;\
  }\
  default:\
    return adlnls_it_end (l);\
  }\
do_return:\
  adlnls_set_node_insert_ready_priv (l, n);\
  name##_size_set_private (l, name##_size_get_private (l) - 1);\
  return ret;\
}\
\
func_prefix adlnls_it name##_drop(name* l, adlnls_it n)\
{\
  return name##_drop_explicit (l, n, true);\
}\
\
func_prefix adlnls_it name##_drop_head(name* l)\
{\
  adlnls_it ret = adlnls_it_begin (l);\
  (void) name##_drop (l, ret);\
  return ret;\
}\
\
func_prefix adlnls_it name##_drop_tail(name* l)\
{\
  adlnls_it ret = adlnls_it_rbegin (l);\
  (void) name##_drop (l, ret);\
  return ret;\
}
/*---------------------------------------------------------------------------*/
#endif /* __ARRAY_DOUBLY_LINKED_LIST_PRIVATE_H__ */

