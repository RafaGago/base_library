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
typedef struct bl_mpsc_i_node {
  bl_atomic_uword next;
}
bl_mpsc_i_node;
/*----------------------------------------------------------------------------*/
static inline void*
  bl_mpsc_i_node_get_next (bl_mpsc_i_node* n, bl_uword tag_bits)
{
  return (void*) (bl_atomic_uword_load_rlx (&n->next) & ~bl_u_lsb_set (tag_bits));
}
/*----------------------------------------------------------------------------*/
static inline bl_uword
  bl_mpsc_i_node_get_tag (bl_mpsc_i_node* n, bl_uword tag_bits)
{
  return bl_atomic_uword_load_rlx (&n->next) & bl_u_lsb_set (tag_bits);
}
/*----------------------------------------------------------------------------*/
static inline void bl_mpsc_i_node_set(
  bl_mpsc_i_node* n, void* next, bl_uword tag, bl_uword tag_bits
  )
{
  bl_assert(
    (((bl_uword) next) & ~bl_u_lsb_set (tag_bits)) == ((bl_uword) next)
    );
  bl_assert ((tag & bl_u_lsb_set (tag_bits)) == tag) ;
  bl_atomic_uword_store_rlx (&n->next, ((bl_uword) next) | tag);
}
/*----------------------------------------------------------------------------*/
typedef struct bl_mpsc_i {
  bl_declare_cache_pad_member;
  bl_atomic_uword              tail;
  bl_declare_cache_pad_member;
  bl_mpsc_i_node*              head;
  bl_mpsc_i_node               stable_node;
  bl_declare_cache_pad_member;
}
bl_mpsc_i;
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT void bl_mpsc_i_init (bl_mpsc_i* q);
/*------------------------------------------------------------------------------
tag_bits: if the nodes use tagged pointers, the number of bits used by it.

first: first node of the batch, all the nodes from first to last for a linked
  list.

last: last node of the batch, which already has the "next" value pointing to
  "null" and its tag set;

returns: A hint (that can give false positives) saying if the queue was empty.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT bool bl_mpsc_i_produce_many(
  bl_mpsc_i* q, bl_mpsc_i_node* first, bl_mpsc_i_node* last, bl_uword tag_bits
  );
/*------------------------------------------------------------------------------
tag_bits: if the nodes use tagged pointers, the number of bits used by it.

n: a node which already has the "next" value pointing to "null" and its tag
  set;

returns: A hint (that can give false positives) saying if the queue was empty.
------------------------------------------------------------------------------*/
static inline bool
  bl_mpsc_i_produce (bl_mpsc_i* q, bl_mpsc_i_node* n, bl_uword tag_bits)
{
  return bl_mpsc_i_produce_many (q, n, n, tag_bits);
}
/*------------------------------------------------------------------------------
The tagged pointer has the effect of requiring a LOAD of the previous node hook
to retrieve the previous tag before overwriting the field. This extra LOAD has
an effect on performance. When using these "_notag" functions use
"bl_mpsc_i_consume" with tag bits set to 0.
------------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT bool bl_mpsc_i_produce_many_notag(
  bl_mpsc_i* q, bl_mpsc_i_node* first, bl_mpsc_i_node* last
  );
/*------------------------------------------------------------------------------
tag_bits: if the nodes use tagged pointers, the number of bits used by it.

n: a node which already has the "next" value pointing to "null" and its tag
  set;

returns: A hint (that can give false positives) saying if the queue was empty.
------------------------------------------------------------------------------*/
static inline bool bl_mpsc_i_produce_notag (bl_mpsc_i* q, bl_mpsc_i_node* n)
{
  return bl_mpsc_i_produce_many_notag (q, n, n);
}
/*----------------------------------------------------------------------------*/
extern BL_NONBLOCK_EXPORT bl_err bl_mpsc_i_consume(
  bl_mpsc_i* q, bl_mpsc_i_node** n, bl_uword tag_bits
  );
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif

#endif /* __BL_NONBLOCK_MPSC_I_H__ */
