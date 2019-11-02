#include <bl/nonblock/libexport.h>
#include <bl/nonblock/mpsc_i.h>

#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT void bl_mpsc_i_init (bl_mpsc_i* q)
{
  q->head = &q->stable_node;
  bl_atomic_uword_store_rlx (&q->tail, (bl_uword) &q->stable_node);
  bl_atomic_uword_store_rlx (&q->stable_node.next, (bl_uword) nullptr);
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bool bl_mpsc_i_produce_many(
  bl_mpsc_i* q, bl_mpsc_i_node* first, bl_mpsc_i_node* last, bl_uword tag_bits
  )
{
  bl_uword mask = bl_u_lsb_set (tag_bits);
  bl_assert (bl_mpsc_i_node_get_next (last, tag_bits) == nullptr);
  bl_mpsc_i_node* old  = (bl_mpsc_i_node*) bl_atomic_uword_exchange(
    &q->tail, (bl_uword) last, bl_mo_acq_rel
    );
  /* window of inconsistence */
  bl_uword oldv  = bl_atomic_uword_load_rlx (&old->next);
  bl_atomic_uword_store(
    &old->next, (oldv & mask) | ((bl_uword) first), bl_mo_release
    );
  return old == &q->stable_node;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bool bl_mpsc_i_produce_many_notag(
  bl_mpsc_i* q, bl_mpsc_i_node* first, bl_mpsc_i_node* last
  )
{
  bl_assert (bl_mpsc_i_node_get_next (last, 0) == nullptr);
  bl_mpsc_i_node* old  = (bl_mpsc_i_node*) bl_atomic_uword_exchange(
    &q->tail, (bl_uword) last, bl_mo_acq_rel
    );
  /* window of inconsistence */
  bl_atomic_uword_store (&old->next, (bl_uword) first, bl_mo_release);
  return old == &q->stable_node;
}
/*----------------------------------------------------------------------------*/
BL_NONBLOCK_EXPORT bl_err bl_mpsc_i_consume(
  bl_mpsc_i* q, bl_mpsc_i_node** n, bl_uword tag_bits
  )
{
  bl_mpsc_i_node* first  = q->head;
  bl_uword second_raw    = bl_atomic_uword_load (&first->next, bl_mo_acquire);
  bl_mpsc_i_node* second =
    (bl_mpsc_i_node*) (second_raw & ~bl_u_lsb_set (tag_bits));
  if (first == &q->stable_node) {
      if (second)  {
        /* non empty, but a stable node that was used for contention detection
           (save a CAS) is found: skipping it. */
          q->head = second;
          first   = second;
          second  = bl_mpsc_i_node_get_next (second, tag_bits);
      }
      else {
        return bl_mkerr (bl_empty);
      }
  }
  if (second) {
    /* fastpath (visible pushes > 1), the consumer head doesn't touch the
       producer tail. */
    *n      = first;
    q->head = second;
    return bl_mkok();
  }
  /* visible pushes <= 1, consuming collides with the producers  */
  bl_mpsc_i_node* tail = (bl_mpsc_i_node*) bl_atomic_uword_load_rlx (&q->tail);
  if (first != tail) {
    /* if the first node isn't the tail the memory snapshot that we see shows
       that someone is pushing just now. The snapshot is at the window of
       unconsistence of the produce function.*/
    return bl_mkerr (bl_busy);
  }
  /* insert the stable node, so we can consume the last queue element without
     breaking the producers xchg (old will return the stable node for them)*/
  bl_mpsc_i_node_set (&q->stable_node, nullptr, 0, tag_bits);
  bl_mpsc_i_produce (q, &q->stable_node, tag_bits);
  /* remember that "produce" had mem barriers, the view now is consistent. */
  second =  bl_mpsc_i_node_get_next (first, tag_bits);
  if (second) {
    /* "first.next" points to either the stable node or something new that was
       pushed in between.*/
    *n      = first;
    q->head = second;
    return bl_mkok();
  }
  else {
    /* we see a nullptr at ""first.next" next from a produce call that came
       before ours. The stable node ended up after this new node. The snapshot
       that we see is at the produce function window of insconsistency.*/
    return bl_mkerr (bl_busy);
  }
}
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
} /*extern "C" {*/
#endif
