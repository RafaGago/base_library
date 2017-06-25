#include <bl/cmocka_pre.h>

#include <bl/base/array_linked_list.h>
#include <string.h>

#include <bl/base/utility.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef struct alnls_u8_context {
  alnls_u8    l;
  alnls_u8_it nodes[4];
  alnls_u8_it its[4];
}
alnls_u8_context;
/*---------------------------------------------------------------------------*/
static int alnls_u8_setup (void **state)
{
  static alnls_u8_context c;

  *state = (void*) &c;
  memset (&c, 0, sizeof c);
  alnls_u8_init (&c.l, c.nodes);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_acquire_release_nodes (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    assert_true (c->its[i] != alnls_u8_it_end (&c->l));
  }
  assert_true (alnls_u8_try_acquire_a_node (&c->l) == alnls_u8_it_end(&c->l));
  alnls_u8_it release = c->its[arr_elems (c->its) - 1];
  alnls_u8_node_release (&c->l, release);
  assert_true (alnls_u8_try_acquire_a_node (&c->l) == release);
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_insert_tail_drop_head (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_tail (&c->l, c->its[i]);
    assert_true (alnls_u8_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (alnls_u8_drop_head (&c->l) == c->its[i]);
    alnls_u8_node_release (&c->l, c->its[i]);
    assert_true (alnls_u8_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (alnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_insert_head_drop_tail (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_head (&c->l, c->its[i]);
    assert_true (alnls_u8_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (alnls_u8_drop_tail (&c->l) == c->its[i]);
    alnls_u8_node_release (&c->l, c->its[i]);
    assert_true (alnls_u8_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (alnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_iteration (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  alnls_u8_it it;
  uword i = 0;
  alnls_u8_foreach_read_only (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_middle_removal_no_head_no_tail (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) / 2;
  assert_true(
    alnls_u8_drop (&c->l, c->its[removed_pos]) == c->its[removed_pos - 1]
   );
  assert_true (alnls_u8_size (&c->l) == arr_elems (c->its) - 1);
  uword i = 0;
  alnls_u8_foreach (&c->l, it)
  {
	if (i == removed_pos) { ++i; }
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_middle_removal_tail (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) - 1;
  assert_true(
    alnls_u8_drop (&c->l, c->its[removed_pos]) == c->its[removed_pos - 1]
   );
  uword i = 0;
  alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its) - 1);
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_middle_removal_head (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  alnls_u8_it it      = alnls_u8_try_acquire_a_node (&c->l);
  alnls_u8_insert_tail (&c->l, it);

  assert_true (alnls_u8_drop (&c->l, it) == alnls_u8_it_end (&c->l));
  assert_true (alnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_iterate_removal (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) / 2;
  uword i = 0;
  alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      alnls_u8_it prev = alnls_u8_drop (&c->l, it);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_iterate_removal_head (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = 0;
  uword i = 0;
  alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      alnls_u8_it noprev = alnls_u8_drop (&c->l, it);
      assert_true (noprev == alnls_u8_it_end (&c->l));
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_u8_iterate_removal_tail (void **state)
{
  alnls_u8_context* c = (alnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_u8_try_acquire_a_node (&c->l);
    alnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) - 1;
  uword i = 0;
  alnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      alnls_u8_it prev = alnls_u8_drop (&c->l, it);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (alnls_u8_acquire_release_nodes, alnls_u8_setup),
  cmocka_unit_test_setup (alnls_u8_insert_tail_drop_head, alnls_u8_setup),
  cmocka_unit_test_setup (alnls_u8_insert_head_drop_tail, alnls_u8_setup),
  cmocka_unit_test_setup (alnls_u8_iteration, alnls_u8_setup),
  cmocka_unit_test_setup(
    alnls_u8_middle_removal_no_head_no_tail, alnls_u8_setup
    ),
  cmocka_unit_test_setup (alnls_u8_middle_removal_head, alnls_u8_setup),
  cmocka_unit_test_setup (alnls_u8_middle_removal_tail, alnls_u8_setup),
  cmocka_unit_test_setup (alnls_u8_iterate_removal, alnls_u8_setup),
  cmocka_unit_test_setup (alnls_u8_iterate_removal_head, alnls_u8_setup),
  cmocka_unit_test_setup (alnls_u8_iterate_removal_tail, alnls_u8_setup),
};
/*---------------------------------------------------------------------------*/
int alnls_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
