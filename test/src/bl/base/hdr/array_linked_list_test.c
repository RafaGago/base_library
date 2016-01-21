#include <bl/base/cmocka_pre.h>
#include <bl/base/hdr/array_linked_list_test.h>

#include <bl/hdr/base/array_linked_list.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/to_type_containing.h>

alnls_sz_define_types (alnls_test, u8, 4)
alnls_sz_define_funcs (alnls_test, u8, static inline)

/*---------------------------------------------------------------------------*/
typedef struct alnls_test_context {
  alnls_test  l;
  alnls_it    its[alnls_capacity ((alnls_test*) 0)];
}
alnls_test_context;
/*---------------------------------------------------------------------------*/
static int alnls_test_setup (void **state)
{
  static alnls_test_context c;

  *state = (void*) &c;
  alnls_test_init (&c.l);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void alnls_test_acquire_release_nodes (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    assert_true (c->its[i] != alnls_it_end (&c->l));
  }
  assert_true (alnls_test_try_acquire_a_node (&c->l) == alnls_it_end(&c->l));
  alnls_it release = c->its[arr_elems (c->its) - 1];
  alnls_node_release (&c->l, release);
  assert_true (alnls_test_try_acquire_a_node (&c->l) == release);
}
/*---------------------------------------------------------------------------*/
static void alnls_test_insert_tail_drop_head (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_tail (&c->l, c->its[i]);
    assert_true (alnls_test_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (alnls_test_drop_head (&c->l) == c->its[i]);
    alnls_node_release (&c->l, c->its[i]);
    assert_true (alnls_test_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (alnls_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void alnls_test_insert_head_drop_tail (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_head (&c->l, c->its[i]);
    assert_true (alnls_test_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (alnls_test_drop_tail (&c->l) == c->its[i]);
    alnls_node_release (&c->l, c->its[i]);
    assert_true (alnls_test_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (alnls_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void alnls_iteration (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_tail (&c->l, c->its[i]);
  }
  alnls_it it;
  uword i = 0;
  alnls_foreach_read_only (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_test_middle_removal_no_head_no_tail (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) / 2;
  assert_true(
    alnls_test_drop (&c->l, c->its[removed_pos]) == c->its[removed_pos - 1]
   );
  assert_true (alnls_test_size (&c->l) == arr_elems (c->its) - 1);
  alnls_it it;
  uword i = 0;
  alnls_foreach (&c->l, it)
  {
	if (i == removed_pos) { ++i; }
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_test_middle_removal_tail (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) - 1;
  assert_true(
    alnls_test_drop (&c->l, c->its[removed_pos]) == c->its[removed_pos - 1]
   );
  alnls_it it;
  uword i = 0;
  alnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its) - 1);
}
/*---------------------------------------------------------------------------*/
static void alnls_test_middle_removal_head (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  alnls_it it      = alnls_test_try_acquire_a_node (&c->l);
  alnls_test_insert_tail (&c->l, it);

  assert_true (alnls_test_drop (&c->l, it) == alnls_it_end (&c->l));
  assert_true (alnls_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void alnls_test_iterate_removal (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) / 2;
  alnls_it it;
  uword i = 0;
  alnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      alnls_it prev = alnls_test_drop (&c->l, it);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_test_iterate_removal_head (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = 0;
  alnls_it it;
  uword i = 0;
  alnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      alnls_it noprev = alnls_test_drop (&c->l, it);
      assert_true (noprev == alnls_it_end (&c->l));
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void alnls_test_iterate_removal_tail (void **state)
{
  alnls_test_context* c = (alnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = alnls_test_try_acquire_a_node (&c->l);
    alnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) - 1;
  alnls_it it;
  uword i = 0;
  alnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      alnls_it prev = alnls_test_drop (&c->l, it);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {

  cmocka_unit_test_setup (alnls_test_acquire_release_nodes, alnls_test_setup),
  cmocka_unit_test_setup (alnls_test_insert_tail_drop_head, alnls_test_setup),
  cmocka_unit_test_setup (alnls_test_insert_head_drop_tail, alnls_test_setup),
  cmocka_unit_test_setup (alnls_iteration, alnls_test_setup),
  cmocka_unit_test_setup(
    alnls_test_middle_removal_no_head_no_tail, alnls_test_setup
    ),
  cmocka_unit_test_setup (alnls_test_middle_removal_head, alnls_test_setup),
  cmocka_unit_test_setup (alnls_test_middle_removal_tail, alnls_test_setup),
  cmocka_unit_test_setup (alnls_test_iterate_removal, alnls_test_setup),
  cmocka_unit_test_setup (alnls_test_iterate_removal_head, alnls_test_setup),
  cmocka_unit_test_setup (alnls_test_iterate_removal_tail, alnls_test_setup),
};
/*---------------------------------------------------------------------------*/
int alnls_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
