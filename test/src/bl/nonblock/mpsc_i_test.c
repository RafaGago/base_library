#include <bl/cmocka_pre.h>
#include <bl/nonblock/mpsc_i_test.h>

#include <bl/base/integer.h>
#include <bl/nonblock/mpsc_i.h>
#include <bl/nonblock/mpsc_i.c>
#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef struct testnode {
  mpsc_i_node n;
  int         v;
}
testnode;
/*---------------------------------------------------------------------------*/
typedef struct mpsc_i_context {
  mpsc_i    q;
  testnode  nodes[8];
}
mpsc_i_context;
/*---------------------------------------------------------------------------*/
static int mpsc_i_test_setup (void **state)
{
  static mpsc_i_context c;
  *state = (void*) &c;
  mpsc_i_init (&c.q);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void mpsc_i_produce_consume (void **state)
{
  static const uword tag_bits = 2;
  mpsc_i_context* c = (mpsc_i_context*) *state;
  uword mask        = u_lsb_set (tag_bits);
  for (uword i = 0; i < arr_elems (c->nodes); ++i) {
    mpsc_i_node_set (&c->nodes[i].n, nullptr, i & mask, tag_bits);
    c->nodes[i].v = i;
    bool ret = mpsc_i_produce (&c->q, &c->nodes[i].n, tag_bits);
    assert_true (ret == (i == 0));
  }
  mpsc_i_node* node;
  bl_err err;
  for (uword i = 0; i < arr_elems (c->nodes); ++i) {
    err = mpsc_i_consume (&c->q, &node, tag_bits);
    assert_true (err == bl_ok);
    assert_true (mpsc_i_node_get_tag (node, tag_bits) == (i & mask));
    testnode* tn = to_type_containing (node, n, testnode);
    assert_true (tn->v == i);
  }
  err = mpsc_i_consume (&c->q, &node, tag_bits);
  assert_true (err = bl_empty);
}
/*---------------------------------------------------------------------------*/
static void mpsc_i_multiproduce_consume (void **state)
{
  static const uword tag_bits = 2;
  mpsc_i_context* c = (mpsc_i_context*) *state;
  uword mask        = u_lsb_set (tag_bits);
  uword i;
  for (i = 0; i < arr_elems (c->nodes) - 1; ++i) {
    c->nodes[i].v = i;
    mpsc_i_node_set (&c->nodes[i].n, &c->nodes[i + 1].n, i & mask, tag_bits);
  }
  c->nodes[i].v = i;
  mpsc_i_node_set (&c->nodes[i].n, nullptr, i & mask, tag_bits);
  bool ret = mpsc_i_produce_many(
    &c->q, &c->nodes[0].n, &c->nodes[i].n, tag_bits
    );
  assert_true (ret == true);
  mpsc_i_node* node;
  bl_err err;
  for (uword i = 0; i < arr_elems (c->nodes); ++i) {
    err = mpsc_i_consume (&c->q, &node, tag_bits);
    assert_true (err == bl_ok);
    assert_true (mpsc_i_node_get_tag (node, tag_bits) == (i & mask));
    testnode* tn = to_type_containing (node, n, testnode);
    assert_true (tn->v == i);
  }
  err = mpsc_i_consume (&c->q, &node, tag_bits);
  assert_true (err = bl_empty);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (mpsc_i_produce_consume, mpsc_i_test_setup),
  cmocka_unit_test_setup (mpsc_i_multiproduce_consume, mpsc_i_test_setup),
};
/*---------------------------------------------------------------------------*/
int mpsc_i_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
