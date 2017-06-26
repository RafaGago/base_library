#include <bl/cmocka_pre.h>

#include <bl/base/impl/array_doubly_linked_list_u8.h>

#include <bl/base/utility.h>
#include <bl/base/to_type_containing.h>
/*---------------------------------------------------------------------------*/
typedef struct adlnls_u8_context {
  adlnls_u8    l;
  adlnls_u8_it nodes[4];
  adlnls_u8_it its[4];
}
adlnls_u8_context;
/*---------------------------------------------------------------------------*/
static int adlnls_u8_setup (void **state)
{
  static adlnls_u8_context c;

  *state = (void*) &c;
  adlnls_u8_init (&c.l, c.nodes);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_acquire_release_nodes (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    assert_true (c->its[i] != adlnls_u8_it_end (&c->l));
  }
  assert_true (adlnls_u8_try_acquire_a_node (&c->l) == adlnls_u8_it_end(&c->l));
  adlnls_u8_it release = c->its[arr_elems (c->its) - 1];
  adlnls_u8_node_release (&c->l, release);
  assert_true (adlnls_u8_try_acquire_a_node (&c->l) == release);
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_insert_tail_drop_head (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_tail (&c->l, c->its[i]);
    assert_true (adlnls_u8_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (adlnls_u8_drop_head (&c->l) == c->its[i]);
    adlnls_u8_node_release (&c->l, c->its[i]);
    assert_true (adlnls_u8_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (adlnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_insert_head_drop_tail (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_head (&c->l, c->its[i]);
    assert_true (adlnls_u8_size (&c->l) == i + 1);
  }
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    assert_true (adlnls_u8_drop_tail (&c->l) == c->its[i]);
    adlnls_u8_node_release (&c->l, c->its[i]);
    assert_true (adlnls_u8_size (&c->l) == arr_elems (c->its) - i - 1);
  }
  assert_true (adlnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_iteration (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_tail (&c->l, c->its[i]);
  }
  adlnls_u8_it it;
  uword i = 0;
  adlnls_u8_foreach_read_only (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_reverse_iteration (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_head (&c->l, c->its[i]);
  }
  adlnls_u8_it it;
  uword i = 0;
  adlnls_u8_foreach_r_read_only (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_middle_removal_no_head_no_tail (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) / 2;
  assert_true(
    adlnls_u8_drop_explicit (&c->l, c->its[removed_pos], true
    ) == c->its[removed_pos - 1]
   );
  assert_true (adlnls_u8_size (&c->l) == arr_elems (c->its) - 1);
  uword i = 0;
  adlnls_u8_foreach (&c->l, it)
  {
	if (i == removed_pos) { ++i; }
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_middle_removal_tail (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword removed_pos = arr_elems (c->its) - 1;
  assert_true(
    adlnls_u8_drop_explicit (&c->l, c->its[removed_pos], true
      ) == c->its[removed_pos - 1]
   );
  uword i = 0;
  adlnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    ++i;
  }
  assert_true (i == arr_elems (c->its) - 1);
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_middle_removal_head (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  adlnls_u8_it it      = adlnls_u8_try_acquire_a_node (&c->l);
  adlnls_u8_insert_tail (&c->l, it);

  assert_true(
    adlnls_u8_drop_explicit (&c->l, it, true) == adlnls_u8_it_end (&c->l)
    );
  assert_true (adlnls_u8_is_empty (&c->l));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_iterate_removal (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) / 2;
  uword i = 0;
  adlnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_u8_it prev = adlnls_u8_drop_explicit (&c->l, it, true);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_iterate_removal_head (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = 0;
  uword i = 0;
  adlnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_u8_it noprev = adlnls_u8_drop_explicit (&c->l, it, true);
      assert_true (noprev == adlnls_u8_it_end (&c->l));
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_iterate_removal_tail (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_tail (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) - 1;
  uword i = 0;
  adlnls_u8_foreach (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_u8_it prev = adlnls_u8_drop_explicit (&c->l, it, true);
      assert_true (prev == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_iterate_removal_reverse (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_head (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) / 2;
  uword i = 0;
  adlnls_u8_foreach_r (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_u8_it next = adlnls_u8_drop_explicit (&c->l, it, false);
      assert_true (next == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_iterate_removal_reverse_head (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_head (&c->l, c->its[i]);
  }
  uword remove_pos = 0;
  uword i = 0;
  adlnls_u8_foreach_r (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_u8_it no_next = adlnls_u8_drop_explicit (&c->l, it, false);
      assert_true (no_next == adlnls_u8_it_end (&c->l));
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static void adlnls_u8_iterate_removal_reverse_tail (void **state)
{
  adlnls_u8_context* c = (adlnls_u8_context*) *state;
  for (uword i = 0; i < arr_elems (c->its); ++i) {
    c->its[i] = adlnls_u8_try_acquire_a_node (&c->l);
    adlnls_u8_insert_head (&c->l, c->its[i]);
  }
  uword remove_pos = arr_elems (c->its) - 1;
  uword i = 0;
  adlnls_u8_foreach_r (&c->l, it)
  {
    assert_true (it == c->its[i]);
    if (i == remove_pos) {
      adlnls_u8_it next = adlnls_u8_drop_explicit (&c->l, it, false);
      assert_true (next == c->its[i - 1]);
    }
    ++i;
  }
  assert_true (i == arr_elems (c->its));
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {

  cmocka_unit_test_setup (adlnls_u8_acquire_release_nodes, adlnls_u8_setup),
  cmocka_unit_test_setup(
    adlnls_u8_insert_tail_drop_head, adlnls_u8_setup
    ),
  cmocka_unit_test_setup(
    adlnls_u8_insert_head_drop_tail, adlnls_u8_setup
    ),
  cmocka_unit_test_setup (adlnls_u8_iteration, adlnls_u8_setup),
  cmocka_unit_test_setup (adlnls_u8_reverse_iteration, adlnls_u8_setup),
  cmocka_unit_test_setup(
    adlnls_u8_middle_removal_no_head_no_tail, adlnls_u8_setup
    ),
  cmocka_unit_test_setup (adlnls_u8_middle_removal_head, adlnls_u8_setup),
  cmocka_unit_test_setup (adlnls_u8_middle_removal_tail, adlnls_u8_setup),
  cmocka_unit_test_setup (adlnls_u8_iterate_removal, adlnls_u8_setup),
  cmocka_unit_test_setup (adlnls_u8_iterate_removal_head, adlnls_u8_setup),
  cmocka_unit_test_setup (adlnls_u8_iterate_removal_tail, adlnls_u8_setup),
  cmocka_unit_test_setup(
    adlnls_u8_iterate_removal_reverse, adlnls_u8_setup
	),
  cmocka_unit_test_setup(
   adlnls_u8_iterate_removal_reverse_head, adlnls_u8_setup
    ),
  cmocka_unit_test_setup(
    adlnls_u8_iterate_removal_reverse_tail, adlnls_u8_setup
    ),
};
/*---------------------------------------------------------------------------*/
int adlnls_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
