#include <bl/cmocka_pre.h>

#include <bl/base/array_linked_list.h>
#include <string.h>

#include <bl/base/utility.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef struct bl_alnls_u8_context {
  bl_alnls_u8    l;
  bl_alnls_u8_it nodes[4];
  bl_alnls_u8_it its[4];
}
bl_alnls_u8_context;
/*---------------------------------------------------------------------------*/
static int bl_alnls_u8_setup (void **state)
{
  static bl_alnls_u8_context c;

  *state = (void*) &c;
  memset (&c, 0, sizeof c);
  bl_alnls_u8_init (&c.l, c.nodes);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_acquire_release_nodes (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    assert_true (c->its[i] != bl_alnls_u8_it_end (&c->l));
  }
  assert_true(
    bl_alnls_u8_try_acquire_a_node (&c->l) == bl_alnls_u8_it_end(&c->l)
    );
  bl_alnls_u8_it release = c->its[bl_arr_elems (c->its) - 1];
  bl_alnls_u8_node_release (&c->l, release);
  assert_true (bl_alnls_u8_try_acquire_a_node (&c->l) == release);
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_insert_tail_drop_head (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_tail (&c->l, c->its[i]);
    assert_true (bl_alnls_u8_size (&c->l) == i + 1);
  }
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    assert_true (bl_alnls_u8_drop_head (&c->l) == c->its[i]);
    bl_alnls_u8_node_release (&c->l, c->its[i]);
    assert_true (bl_alnls_u8_size (&c->l) == bl_arr_elems (c->its) - i - 1);
  }
  assert_true (bl_alnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_insert_head_drop_tail (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_head (&c->l, c->its[i]);
    assert_true (bl_alnls_u8_size (&c->l) == i + 1);
  }
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    assert_true (bl_alnls_u8_drop_tail (&c->l) == c->its[i]);
    bl_alnls_u8_node_release (&c->l, c->its[i]);
    assert_true (bl_alnls_u8_size (&c->l) == bl_arr_elems (c->its) - i - 1);
  }
  assert_true (bl_alnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_iteration (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  bl_alnls_u8_it it;
  bl_uword i = 0;
  bl_alnls_u8_foreach_read_only (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == bl_arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_middle_removal_no_head_no_tail (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  bl_uword removed_pos = bl_arr_elems (c->its) / 2;
  assert_true(
    bl_alnls_u8_drop (&c->l, c->its[removed_pos]) == c->its[removed_pos - 1]
   );
  assert_true (bl_alnls_u8_size (&c->l) == bl_arr_elems (c->its) - 1);
  bl_uword i = 0;
  bl_alnls_u8_foreach (&c->l, it)
  {
	if (i == removed_pos) { ++i; }
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == bl_arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_middle_removal_tail (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  bl_uword removed_pos = bl_arr_elems (c->its) - 1;
  assert_true(
    bl_alnls_u8_drop (&c->l, c->its[removed_pos]) == c->its[removed_pos - 1]
   );
  bl_uword i = 0;
  bl_alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == bl_arr_elems (c->its) - 1);
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_middle_removal_head (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  bl_alnls_u8_it it      = bl_alnls_u8_try_acquire_a_node (&c->l);
  bl_alnls_u8_insert_tail (&c->l, it);

  assert_true (bl_alnls_u8_drop (&c->l, it) == bl_alnls_u8_it_end (&c->l));
  assert_true (bl_alnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_iterate_removal (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  bl_uword remove_pos = bl_arr_elems (c->its) / 2;
  bl_uword i = 0;
  bl_alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      bl_alnls_u8_it prev = bl_alnls_u8_drop (&c->l, it);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == bl_arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_iterate_removal_head (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  bl_uword remove_pos = 0;
  bl_uword i = 0;
  bl_alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      bl_alnls_u8_it noprev = bl_alnls_u8_drop (&c->l, it);
      assert_true (noprev == bl_alnls_u8_it_end (&c->l));
    }
    ++i;
  }
  assert_true (i == bl_arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void bl_alnls_u8_iterate_removal_tail (void **state)
{
  bl_alnls_u8_context* c = (bl_alnls_u8_context*) *state;
  for (bl_uword i = 0; i < bl_arr_elems (c->its); ++i) {
    c->its[i] = bl_alnls_u8_try_acquire_a_node (&c->l);
    bl_alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  bl_uword remove_pos = bl_arr_elems (c->its) - 1;
  bl_uword i = 0;
  bl_alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      bl_alnls_u8_it prev = bl_alnls_u8_drop (&c->l, it);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == bl_arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (bl_alnls_u8_acquire_release_nodes, bl_alnls_u8_setup),
  cmocka_unit_test_setup (bl_alnls_u8_insert_tail_drop_head, bl_alnls_u8_setup),
  cmocka_unit_test_setup (bl_alnls_u8_insert_head_drop_tail, bl_alnls_u8_setup),
  cmocka_unit_test_setup (bl_alnls_u8_iteration, bl_alnls_u8_setup),
  cmocka_unit_test_setup(
    bl_alnls_u8_middle_removal_no_head_no_tail, bl_alnls_u8_setup
    ),
  cmocka_unit_test_setup (bl_alnls_u8_middle_removal_head, bl_alnls_u8_setup),
  cmocka_unit_test_setup (bl_alnls_u8_middle_removal_tail, bl_alnls_u8_setup),
  cmocka_unit_test_setup (bl_alnls_u8_iterate_removal, bl_alnls_u8_setup),
  cmocka_unit_test_setup (bl_alnls_u8_iterate_removal_head, bl_alnls_u8_setup),
  cmocka_unit_test_setup (bl_alnls_u8_iterate_removal_tail, bl_alnls_u8_setup),
};
/*---------------------------------------------------------------------------*/
int bl_alnls_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
