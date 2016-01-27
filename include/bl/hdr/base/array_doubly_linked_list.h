#ifndef __BL_ARRAY_DOUBLY_LINKED_LIST_H__
#define __BL_ARRAY_DOUBLY_LINKED_LIST_H__

/*---------------------------------------------------------------------------*/
/*
  A doubly-linked list with node references as array indexes to an embedded
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
#include <bl/hdr/base/impl/array_doubly_linked_list_private.h>

typedef uword adlnls_it;
/*---------------------------------------------------------------------------*/
#define adlnls_nodes(type_ptr)            ((type_ptr)->nodes)
#define adlnls_capacity(type_ptr)         arr_elems (adlnls_nodes (type_ptr))
/*---------------------------------------------------------------------------*/
#define adlnls_it_end(type_ptr)            adlnls_capacity (type_ptr)
#define adlnls_it_rend(type_ptr)           adlnls_it_end (type_ptr)

#define adlnls_it_in_range(type_ptr, it)   (it < adlnls_it_end (type_ptr))
#define adlnls_it_in_range_r(type_ptr, it) adlnls_it_in_range (type_ptr, it)

#define adlnls_it_begin(type_ptr)  (adlnls_it_begin_priv (type_ptr) + 0)
#define adlnls_it_rbegin(type_ptr) (adlnls_it_rbegin_priv (type_ptr) + 0)

#define adlnls_it_next(type_ptr, it) (adlnls_it_next_priv (type_ptr, it) + 0)
#define adlnls_it_prev(type_ptr, it) (adlnls_it_prev_priv (type_ptr, it) + 0)
/*---------------------------------------------------------------------------*/
#define adlnls_is_empty(type_ptr)\
  (adlnls_it_begin (type_ptr) == adlnls_it_end (type_ptr))
/*---------------------------------------------------------------------------*/
#define adlnls_node_is_free(type_ptr, it)\
  (adlnls_it_next (type_ptr, it) == adlnls_nfree_val_priv (it))

#define adlnls_node_acquire_unsafe(type_ptr, it)\
do {\
  bl_assert (it < adlnls_capacity (type_ptr));\
  bl_assert (adlnls_node_is_free (type_ptr, it));\
  adlnls_set_node_insert_ready_priv (type_ptr, it);\
} while (0)

#define adlnls_node_release(type_ptr, it)\
do {\
  bl_assert (it < adlnls_capacity (type_ptr));\
  bl_assert (adlnls_is_node_insert_ready_priv (type_ptr, it));\
  adlnls_it_next_priv (type_ptr, it) = adlnls_nfree_val_priv (it);\
  adlnls_it_prev_priv (type_ptr, it) = adlnls_nfree_val_priv (it);\
} while (0)
/*---------------------------------------------------------------------------*/
#define adlnls_foreach(type_ptr, it_var)\
  for (adlnls_it adlnls_priv_curr_it = adlnls_it_begin ((type_ptr)),\
         it_var = adlnls_priv_curr_it;\
       adlnls_it_in_range ((type_ptr), adlnls_priv_curr_it) &&\
         (adlnls_priv_curr_it =\
            adlnls_it_next ((type_ptr), adlnls_priv_curr_it), 1);\
       it_var = adlnls_priv_curr_it\
       )

#define adlnls_foreach_read_only(type_ptr, it_var)\
  for (it_var = adlnls_it_begin ((type_ptr));\
       adlnls_it_in_range ((type_ptr), it_var);\
       it_var = adlnls_it_next ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_foreach_r(type_ptr, it_var)\
  for (adlnls_it adlnls_priv_curr_it = adlnls_it_rbegin ((type_ptr)),\
         it_var = adlnls_priv_curr_it;\
       adlnls_it_in_range ((type_ptr), adlnls_priv_curr_it) &&\
         (adlnls_priv_curr_it =\
            adlnls_it_prev ((type_ptr), adlnls_priv_curr_it), 1);\
       it_var = adlnls_priv_curr_it\
       )

#define adlnls_foreach_r_read_only(type_ptr, it_var)\
  for (it_var = adlnls_it_rbegin ((type_ptr));\
       adlnls_it_in_range ((type_ptr), it_var);\
       it_var = adlnls_it_prev ((type_ptr), it_var)\
       )
/*---------------------------------------------------------------------------*/
#define adlnls_define_types(\
  new_adlnls_type_basename,\
  index_uint_type,\
  node_count\
  )\
\
adlnls_define_types_base_private(\
  new_adlnls_type_basename,\
  index_uint_type,\
  (node_count)\
  )\
\
typedef struct new_adlnls_type_basename {\
  new_adlnls_type_basename##_hook head;\
  new_adlnls_type_basename##_hook nodes[(node_count)];\
}\
new_adlnls_type_basename;
/*---------------------------------------------------------------------------*/
#define adlnls_sz_define_types(\
  new_adlnls_type_basename,\
  index_uint_type,\
  node_count\
  )\
\
adlnls_define_types_base_private(\
  new_adlnls_type_basename,\
  index_uint_type,\
  (node_count)\
  )\
\
typedef struct new_adlnls_type_basename {\
  new_adlnls_type_basename##_hook head;\
  new_adlnls_type_basename##_hook nodes[(node_count)];\
  index_uint_type                 size;\
}\
new_adlnls_type_basename;
/*---------------------------------------------------------------------------*/
#define adlnls_declare_funcs(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
func_prefix void new_adlnls_type_basename##_init (new_adlnls_type_basename* l);\
\
func_prefix adlnls_it new_adlnls_type_basename##_try_acquire_a_node(\
  new_adlnls_type_basename* l\
  );\
\
func_prefix adlnls_it new_adlnls_type_basename##_try_acquire_node(\
  new_adlnls_type_basename* l\
  );\
\
func_prefix void new_adlnls_type_basename##_insert_head(\
  new_adlnls_type_basename* l, adlnls_it n\
  );\
\
func_prefix void new_adlnls_type_basename##_insert_tail(\
  new_adlnls_type_basename* l, adlnls_it n\
  );\
\
func_prefix adlnls_it new_adlnls_type_basename##_drop(\
  new_adlnls_type_basename* l, adlnls_it n, bool return_previous\
  );\
\
func_prefix adlnls_it new_adlnls_type_basename##_drop_head(\
  new_adlnls_type_basename* l\
  );\
\
func_prefix adlnls_it new_adlnls_type_basename##_drop_tail(\
  new_adlnls_type_basename* l\
  );
/*---------------------------------------------------------------------------*/
/* Same list than the above one but this one keeps a size counter            */
/*---------------------------------------------------------------------------*/
#define adlnls_sz_declare_funcs(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
adlnls_declare_funcs(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
index_uint_type new_adlnls_type_basename##_size(\
  new_adlnls_type_basename* l\
  );
/*---------------------------------------------------------------------------*/
#define adlnls_define_funcs(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
adlnls_dummy_size_accessors_private(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
adlnls_define_funcs_private(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )
/*---------------------------------------------------------------------------*/
#define adlnls_sz_define_funcs(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
adlnls_sz_size_accessors_private(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
\
adlnls_define_funcs_private(\
  new_adlnls_type_basename,\
  index_uint_type,\
  func_prefix\
  )\
func_prefix index_uint_type new_adlnls_type_basename##_size(\
  new_adlnls_type_basename* l\
  )\
{\
  return new_adlnls_type_basename##_size_get_private (l);\
}
/*---------------------------------------------------------------------------*/
#if 0 

/*
debug test code. Equivalent to the next two macro invocations:

alnls_define_types (alnls_test, u8, 19)
alnls_define_funcs (alnls_test, u8, static inline)
*/

/*---------------------------------------------------------------------------*/
typedef struct adlnls_test_hook {
  u8 next : type_bits (u8) / 2;
  u8 prev : type_bits (u8) / 2;
}
adlnls_test_hook;

typedef struct adlnls_test {
  adlnls_test_hook head;
  adlnls_test_hook nodes[4];
}
adlnls_test;
/*---------------------------------------------------------------------------*/
static inline void adlnls_test_init (adlnls_test* l)
{
  adlnls_it_begin_priv (l)  = adlnls_it_end (l);
  adlnls_it_rbegin_priv (l) = adlnls_it_end (l);
  for (adlnls_it it = 0; it < adlnls_it_end (l); ++it) {
    adlnls_it_next_priv (l, it) = adlnls_nfree_val_priv (it);
    adlnls_it_prev_priv (l, it) = adlnls_nfree_val_priv (it);
  }
}

static inline adlnls_it adlnls_test_try_acquire_node(
  adlnls_test* l, adlnls_it n
  )
{
  if (adlnls_node_is_free (l, n)) {
    adlnls_node_acquire_unsafe (l, n);
    return n;
  }
  return adlnls_it_end (l);
}

static inline adlnls_it adlnls_test_try_acquire_a_node (adlnls_test* l)
{
  adlnls_it i;
  for (i = 0; i < adlnls_it_end (l); ++i) {
    if (adlnls_node_is_free (l, i)) {
      adlnls_node_acquire_unsafe (l, i);
      break;
    }
  }
  return i;
}

static inline void adlnls_test_insert_head (adlnls_test* l, adlnls_it n)
{
  bl_assert (adlnls_is_node_insert_ready_priv (l, n));
  if (!adlnls_is_empty (l)) {
    adlnls_it_prev_priv (l, adlnls_it_begin (l)) = n;
    bl_assert (adlnls_it_prev (l, adlnls_it_begin (l)) == n);
    adlnls_it_next_priv (l, n) = adlnls_it_begin (l);
  }
  else {
    adlnls_it_rbegin_priv (l) = (u8) n;  
    bl_assert (adlnls_it_rbegin (l) == n);
  }
  adlnls_it_begin_priv (l) = (u8) n;
  bl_assert (adlnls_it_begin (l) == n);
}

static inline void adlnls_test_insert_tail (adlnls_test* l, adlnls_it n)
{
  bl_assert (adlnls_is_node_insert_ready_priv (l, n));
  if (!adlnls_is_empty (l)) {
    adlnls_it_next_priv (l, adlnls_it_rbegin (l)) = n;
    bl_assert (adlnls_it_next (l, adlnls_it_rbegin (l)) == n);
    adlnls_it_prev_priv (l, n) = adlnls_it_rbegin (l);
  }
  else {
    adlnls_it_begin_priv (l) = (u8) n;  
    bl_assert (adlnls_it_begin (l) == n);
  }
  adlnls_it_rbegin_priv (l) = (u8) n;
  bl_assert (adlnls_it_rbegin (l) == n);
}

static inline adlnls_it adlnls_test_drop(
  adlnls_test* l, adlnls_it n, bool return_previous
  )
{
  bl_assert (adlnls_it_in_range (l, n));
  bl_assert (!adlnls_node_is_free (l, n));

  if (adlnls_is_empty (l)) {
    return adlnls_it_end (l);
  }

  adlnls_it ret;
  switch (adlns_get_node_type (l, n)) {
  case adlnls_node_type_tail_and_head: {
    ret                       = adlnls_it_end (l);
    adlnls_it_begin_priv (l)  = adlnls_it_end (l);
    adlnls_it_rbegin_priv (l) = adlnls_it_end (l);
    break;
  }
  case adlnls_node_type_tail:{ 
    ret = return_previous ? adlnls_it_prev (l, n) : adlnls_it_end (l);
    adlnls_it_next_priv (l, adlnls_it_prev (l, n)) = adlnls_it_end (l);
    adlnls_it_rbegin_priv (l) = adlnls_it_prev (l, n);
    break;
  }
  case adlnls_node_type_head: {
    ret = return_previous ? adlnls_it_end (l) : adlnls_it_next (l, n);
    adlnls_it_prev_priv (l, adlnls_it_next (l, n)) = adlnls_it_end (l);
    adlnls_it_begin_priv (l) = adlnls_it_next (l, n);
    break;
  }
  case adlnls_node_type_middle: {
    ret = return_previous ? adlnls_it_prev (l, n) : adlnls_it_next (l, n);
    adlnls_it_prev_priv (l, adlnls_it_next (l, n)) = adlnls_it_prev (l, n);
    adlnls_it_next_priv (l, adlnls_it_prev (l, n)) = adlnls_it_next (l, n);
    goto do_return;
  }
  default:
    return adlnls_it_end (l);
  }
do_return:
  adlnls_set_node_insert_ready_priv (l, n);
  return ret;
}

static inline adlnls_it adlnls_test_drop_head (adlnls_test* l)
{
  adlnls_it ret = adlnls_it_begin (l);
  (void) adlnls_test_drop (l, ret, true);
  return ret;
}

static inline adlnls_it adlnls_test_drop_tail (adlnls_test* l)
{
  adlnls_it ret = adlnls_it_rbegin (l);
  (void) adlnls_test_drop (l, ret, true);
  return ret;
}

#endif
/*---------------------------------------------------------------------------*/
#endif /* __BL_ARRAY_DOUBLY_LINKED_LIST_H__ */

