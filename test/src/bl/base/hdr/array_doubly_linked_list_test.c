#include <bl/base/cmocka_pre.h>
#include <bl/base/hdr/array_doubly_linked_list_test.h>

#include <bl/hdr/base/array_doubly_linked_list.h>
#include <bl/hdr/base/utility.h>
#include <bl/hdr/base/to_type_containing.h>
/*---------------------------------------------------------------------------*/
adlnls_sz_define_types (adlnls_test, u8, 4)
adlnls_sz_define_funcs (adlnls_test, u8, static inline)
/*---------------------------------------------------------------------------*/
typedef struct adlnls_test_context {
  adlnls_test  l;
  adlnls_it    its[adlnls_capacity ((adlnls_test*) 0)];
}
adlnls_test_context;
/*---------------------------------------------------------------------------*/
static int adlnls_test_setup (void **state)
{
  static adlnls_test_context c;

  *state = (void*) &c;
  adlnls_test_init (&c.l);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_acquire_release_nodes (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    assert_true (c->its[i] != adlnls_it_end (&c->l));
  }
  assert_true (adlnls_test_try_acquire_a_node (&c->l) == adlnls_it_end(&c->l));
  adlnls_it release = c->its[arr_elems (c->its) - 1];
  adlnls_node_release (&c->l, release);
  assert_true (adlnls_test_try_acquire_a_node (&c->l) == release);
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_insert_tail_drop_head (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_tail (&c->l, c->its[i]);
    assert_true (adlnls_test_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (adlnls_test_drop_head (&c->l) == c->its[i]);
    adlnls_node_release (&c->l, c->its[i]);
    assert_true (adlnls_test_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (adlnls_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_insert_head_drop_tail (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_head (&c->l, c->its[i]);
    assert_true (adlnls_test_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (adlnls_test_drop_tail (&c->l) == c->its[i]);
    adlnls_node_release (&c->l, c->its[i]);
    assert_true (adlnls_test_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (adlnls_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void adlnls_iteration (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_tail (&c->l, c->its[i]);
  }
  adlnls_it it;
  uword i = 0;
  adlnls_foreach_read_only (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_reverse_iteration (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_head (&c->l, c->its[i]);
  }
  adlnls_it it;
  uword i = 0;
  adlnls_foreach_r_read_only (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_middle_removal_no_head_no_tail (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) / 2;
  assert_true(
    adlnls_test_drop_explicit (&c->l, c->its[removed_pos], true
    ) == c->its[removed_pos - 1]
   );
  assert_true (adlnls_test_size (&c->l) == arr_elems (c->its) - 1);
  adlnls_it it;
  uword i = 0;
  adlnls_foreach (&c->l, it)
  {
	if (i == removed_pos) { ++i; }
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_middle_removal_tail (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) - 1;
  assert_true(
    adlnls_test_drop_explicit (&c->l, c->its[removed_pos], true
      ) == c->its[removed_pos - 1]
   );
  adlnls_it it;
  uword i = 0;
  adlnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its) - 1);
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_middle_removal_head (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  adlnls_it it      = adlnls_test_try_acquire_a_node (&c->l);
  adlnls_test_insert_tail (&c->l, it);

  assert_true(
    adlnls_test_drop_explicit (&c->l, it, true) == adlnls_it_end (&c->l)
    );
  assert_true (adlnls_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_iterate_removal (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) / 2;
  adlnls_it it;
  uword i = 0;
  adlnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_it prev = adlnls_test_drop_explicit (&c->l, it, true);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_iterate_removal_head (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = 0;
  adlnls_it it;
  uword i = 0;
  adlnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_it noprev = adlnls_test_drop_explicit (&c->l, it, true);
      assert_true (noprev == adlnls_it_end (&c->l));
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_iterate_removal_tail (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) - 1;
  adlnls_it it;
  uword i = 0;
  adlnls_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_it prev = adlnls_test_drop_explicit (&c->l, it, true);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_iterate_removal_reverse (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_head (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) / 2;
  adlnls_it it;
  uword i = 0;
  adlnls_foreach_r (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_it next = adlnls_test_drop_explicit (&c->l, it, false);
      assert_true (next == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_iterate_removal_reverse_head (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_head (&c->l, c->its[i]);
  }
  uword remove_pos = 0;
  adlnls_it it;
  uword i = 0;
  adlnls_foreach_r (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_it no_next = adlnls_test_drop_explicit (&c->l, it, false);
      assert_true (no_next == adlnls_it_end (&c->l));
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_test_iterate_removal_reverse_tail (void **state)
{
  adlnls_test_context* c = (adlnls_test_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_test_try_acquire_a_node (&c->l);
    adlnls_test_insert_head (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) - 1;
  adlnls_it it;
  uword i = 0;
  adlnls_foreach_r (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_it next = adlnls_test_drop_explicit (&c->l, it, false);
      assert_true (next == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {

  cmocka_unit_test_setup (adlnls_test_acquire_release_nodes, adlnls_test_setup),
  cmocka_unit_test_setup(
    adlnls_test_insert_tail_drop_head, adlnls_test_setup
    ),
  cmocka_unit_test_setup(
    adlnls_test_insert_head_drop_tail, adlnls_test_setup
    ),
  cmocka_unit_test_setup (adlnls_iteration, adlnls_test_setup),
  cmocka_unit_test_setup (adlnls_reverse_iteration, adlnls_test_setup),
  cmocka_unit_test_setup(
    adlnls_test_middle_removal_no_head_no_tail, adlnls_test_setup
    ),
  cmocka_unit_test_setup (adlnls_test_middle_removal_head, adlnls_test_setup),
  cmocka_unit_test_setup (adlnls_test_middle_removal_tail, adlnls_test_setup),
  cmocka_unit_test_setup (adlnls_test_iterate_removal, adlnls_test_setup),
  cmocka_unit_test_setup (adlnls_test_iterate_removal_head, adlnls_test_setup),
  cmocka_unit_test_setup (adlnls_test_iterate_removal_tail, adlnls_test_setup),
  cmocka_unit_test_setup(
    adlnls_test_iterate_removal_reverse, adlnls_test_setup
	),
  cmocka_unit_test_setup(
   adlnls_test_iterate_removal_reverse_head, adlnls_test_setup
    ),
  cmocka_unit_test_setup(
    adlnls_test_iterate_removal_reverse_tail, adlnls_test_setup
    ),
};
/*---------------------------------------------------------------------------*/
int adlnls_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
