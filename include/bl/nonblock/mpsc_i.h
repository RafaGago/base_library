/*---------------------------------------------------------------------------*/
#ifndef __BL_NONBLOCK_MPSC_I_H__
#define __BL_NONBLOCK_MPSC_I_H__

#include <bl/nonblock/libexport.h>
#include <bl/base/assert.h>
#include <bl/base/platform.h>
#include <bl/base/cache.h>
#include <bl/base/error.h>
#include <bl/base/integer_manipulation.h>
#include <bl/base/atomic.h>

/* D.Vyukov: Intrusive MPSC node-based queue with tagged pointers and
   multiproduce.
  http://www.1024cores.net/home/lock-free-algorithms/queues/intrusive-mpsc-node-based-queue
*/

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
typedef struct mpsc_i_node {
  atomic_uword next;
}
mpsc_i_node;
/*----------------------------------------------------------------------------*/
static inline void* mpsc_i_node_get_next (mpsc_i_node* n, uword tag_bits)
{
  return (void*) (atomic_uword_load_rlx (&n->next) & ~u_lsb_set (tag_bits));
}
/*----------------------------------------------------------------------------*/
static inline uword mpsc_i_node_get_tag (mpsc_i_node* n, uword tag_bits)
{
  return atomic_uword_load_rlx (&n->next) & u_lsb_set (tag_bits);
}
/*----------------------------------------------------------------------------*/
static inline void mpsc_i_node_set(
  mpsc_i_node* n, void* next, uword tag, uword tag_bits
  )
{
  bl_assert ((((uword) next) & ~u_lsb_set (tag_bits)) == ((uword) next)) ;
  bl_assert ((tag & u_lsb_set (tag_bits)) == tag) ;
  atomic_uword_store_rlx (&n->next, ((uword) next) | tag);
}
/*----------------------------------------------------------------------------*/
typedef struct mpsc_i {
  declare_cache_pad_member;
  atomic_uword              tail;
  declare_cache_pad_member;
  mpsc_i_node*              head;
  mpsc_i_node               stable_node;
  declare_cache_pad_member;
}
mpsc_i;
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT void mpsc_i_init (mpsc_i* q);
/*------------------------------------------------------------------------------
tag_bits: if the nodes use tagged pointers, the number of bits used by it.

first: first node of the batch, all the nodes from first to last for a linked
  list.

last: last node of the batch, which already has the "next" value pointing to
  "null" and its tag set;

returns: A hint (that can give false positives) saying if the queue was empty.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT bool mpsc_i_produce_many(
  mpsc_i* q, mpsc_i_node* first, mpsc_i_node* last, uword tag_bits
  );
/*------------------------------------------------------------------------------
tag_bits: if the nodes use tagged pointers, the number of bits used by it.

n: a node which already has the "next" value pointing to "null" and its tag
  set;

returns: A hint (that can give false positives) saying if the queue was empty.
------------------------------------------------------------------------------*/
static inline bool mpsc_i_produce (mpsc_i* q, mpsc_i_node* n, uword tag_bits)
{
  return mpsc_i_produce_many (q, n, n, tag_bits);
}
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT bl_err mpsc_i_consume(
  mpsc_i* q, mpsc_i_node** n, uword tag_bits
  );
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCK_MPSC_I_H__ */
