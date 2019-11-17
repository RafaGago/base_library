#include <bl/cmocka_pre.h>

#include <bl/base/integer.h>
#include <bl/nonblock/mpsc_i.h>
#include <bl/nonblock/mpsc_i.c>
#include <bl/base/utility.h>
#include <bl/base/integer_math.h>
#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
typedef struct testnode {
  bl_mpsc_i_node n;
  int         v;
}
testnode;
/*---------------------------------------------------------------------------*/
typedef struct bl_mpsc_i_context {
  bl_mpsc_i    q;
  testnode  nodes[8];
}
bl_mpsc_i_context;
/*---------------------------------------------------------------------------*/
static int bl_mpsc_i_test_setup (void **state)
{
  static bl_mpsc_i_context c;
  *state = (void*) &c;
  bl_mpsc_i_init (&c.q);
  return 0;
}
/*---------------------------------------------------------------------------*/
static void bl_mpsc_i_produce_consume (void **state)
{
  static const bl_uword tag_bits = 2;
  bl_mpsc_i_context* c = (bl_mpsc_i_context*) *state;
  bl_uword mask        = bl_u_lsb_set (tag_bits);
  for (bl_uword i = 0; i < bl_arr_elems (c->nodes); ++i) {
    bl_mpsc_i_node_set (&c->nodes[i].n, nullptr, i & mask, tag_bits);
    c->nodes[i].v = i;
    bool ret = bl_mpsc_i_produce (&c->q, &c->nodes[i].n, tag_bits);
    assert_true (ret == (i == 0));
  }
  bl_mpsc_i_node* node;
  bl_err err;
  for (bl_uword i = 0; i < bl_arr_elems (c->nodes); ++i) {
    err = bl_mpsc_i_consume (&c->q, &node, tag_bits);
    assert_true (err.own == bl_ok);
    assert_true (bl_mpsc_i_node_get_tag (node, tag_bits) == (i & mask));
    testnode* tn = bl_to_type_containing (node, n, testnode);
    assert_true (tn->v == i);
  }
  err = bl_mpsc_i_consume (&c->q, &node, tag_bits);
  assert_true (err.own == bl_empty);
}
/*---------------------------------------------------------------------------*/
static void bl_mpsc_i_multiproduce_consume (void **state)
{
  static const bl_uword tag_bits = 2;
  bl_mpsc_i_context* c = (bl_mpsc_i_context*) *state;
  bl_uword mask        = bl_u_lsb_set (tag_bits);
  bl_uword i;
  for (i = 0; i < bl_arr_elems (c->nodes) - 1; ++i) {
    c->nodes[i].v = i;
    bl_mpsc_i_node_set (&c->nodes[i].n, &c->nodes[i + 1].n, i & mask, tag_bits);
  }
  c->nodes[i].v = i;
  bl_mpsc_i_node_set (&c->nodes[i].n, nullptr, i & mask, tag_bits);
  bool ret = bl_mpsc_i_produce_many(
    &c->q, &c->nodes[0].n, &c->nodes[i].n, tag_bits
    );
  assert_true (ret == true);
  bl_mpsc_i_node* node;
  bl_err err;
  for (bl_uword i = 0; i < bl_arr_elems (c->nodes); ++i) {
    err = bl_mpsc_i_consume (&c->q, &node, tag_bits);
    assert_true (err.own == bl_ok);
    assert_true (bl_mpsc_i_node_get_tag (node, tag_bits) == (i & mask));
    testnode* tn = bl_to_type_containing (node, n, testnode);
    assert_true (tn->v == i);
  }
  err = bl_mpsc_i_consume (&c->q, &node, tag_bits);
  assert_true (err.own == bl_empty);
}
/*---------------------------------------------------------------------------*/
static const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup (bl_mpsc_i_produce_consume, bl_mpsc_i_test_setup),
  cmocka_unit_test_setup (bl_mpsc_i_multiproduce_consume, bl_mpsc_i_test_setup),
};
/*---------------------------------------------------------------------------*/
int bl_mpsc_i_tests ()
{
  return cmocka_run_group_tests (tests, nullptr, nullptr);
}
/*---------------------------------------------------------------------------*/
