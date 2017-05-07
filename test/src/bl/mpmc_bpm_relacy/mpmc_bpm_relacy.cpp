#include <relacy/relacy.hpp>
#include <stdint.h>
#include <iostream>
#include <cstdio>

//------------------------------------------------------------------------------
typedef uint8_t   u8;
typedef uint32_t  u32;
typedef int32_t   i32;
typedef uintptr_t uword;
//------------------------------------------------------------------------------
struct celltype {
  rl::atomic<u32> seq;
  rl::var<u32>    v;
};
//------------------------------------------------------------------------------
#define CONSTANT 100000
#define SLOTS 8
//------------------------------------------------------------------------------
enum bl_err {
  bl_ok = 0,
  bl_would_overflow,
  bl_empty,
  bl_invalid,
};
//------------------------------------------------------------------------------
template <class T>
T bl_min (T a, T b)
{
  return a > b ? b : a;
}
//------------------------------------------------------------------------------
#define mo_acquire rl::memory_order_acquire
#define mo_release rl::memory_order_release
#define mo_relaxed rl::memory_order_relaxed
#define mo_seq_cst rl::memory_order_seq_cst

//------------------------------------------------------------------------------
struct mpmpc_bpm
{
  celltype        cell[SLOTS + SLOTS - 1];
  u32             mask;
  rl::atomic<u32> push_slot;
  rl::atomic<u32> pop_slot;

  /*--------------------------------------------------------------------------*/
  void init()
  {
    mask = SLOTS - 1;
    for (uword i = 0; i < SLOTS; ++i) {
      cell[i].seq.store (i, mo_seq_cst);
      cell[i].v($) = 3000;
    }
    push_slot.store (0, mo_seq_cst);
    pop_slot.store (0, mo_seq_cst);
  }
  /*--------------------------------------------------------------------------*/
  void print_all_seqs()
  {
    printf(
      "seq[0]:%u,"
      "seq[1]:%u,"
      "seq[2]:%u,"
      "seq[3]:%u,"
      "seq[4]:%u,"
      "seq[5]:%u,"
      "seq[6]:%u,"
      "seq[7]:%u,"
      "seq[8]:%u,"
      "seq[9]:%u,"
      "seq[10]:%u,"
      "seq[11]:%u,"
      "seq[12]:%u,"
      "seq[13]:%u,"
      "seq[14]:%u,",
      cell[0].seq.load (mo_relaxed),
      cell[1].seq.load (mo_relaxed),
      cell[2].seq.load (mo_relaxed),
      cell[3].seq.load (mo_relaxed),
      cell[4].seq.load (mo_relaxed),
      cell[5].seq.load (mo_relaxed),
      cell[6].seq.load (mo_relaxed),
      cell[7].seq.load (mo_relaxed),
      cell[8].seq.load (mo_relaxed),
      cell[9].seq.load (mo_relaxed),
      cell[10].seq.load (mo_relaxed),
      cell[11].seq.load (mo_relaxed),
      cell[12].seq.load (mo_relaxed),
      cell[13].seq.load (mo_relaxed),
      cell[14].seq.load (mo_relaxed)
      );
  }
  /*--------------------------------------------------------------------------*/
  u32 pos_increment (u32 pos, u32 inc)
  {
  /*Increments "pos" position. The variable length produce feature is based on
    having the on-memory slot version counter on the first slot. Other slots
    other than the first use this position to store raw data.

    When wrapping happens (returning from the tail of the memorybuffer to the
    head) the only known position to contains a valid seq/version counter is
    the first one (position 0).

    The tail of the buffer contains space enough to make a contiguous allocation
    on the last element of the queue.*/
    u32 newpos     = pos + inc;
    u32 newpos_lap = newpos & ~mask;
    return (pos & ~mask) == (newpos_lap) ? newpos : newpos_lap;
  }
  /*--------------------------------------------------------------------------*/
  bl_err produce_prepare (celltype** rcell, uword slots)
  {
    if (!slots || slots >= SLOTS) {
      return bl_invalid;
    }
    u32 pos = push_slot.load (mo_relaxed);
resync:
    u32 idx  = pos & mask;
    u32 seq  = cell[idx].seq.load (mo_acquire);
    i32 diff = (i32) (seq - pos);
    if (diff < 0) {
      return bl_would_overflow;
    }
    else if (diff > 0) {
      pos = push_slot.load (mo_relaxed);
      goto resync;
    }
    u32 check = bl_min ((u32) slots, mask + 1 - idx);
    for (uword i = 1; i < check; ++i) {
      u32 seqb  = cell[idx + i].seq.load (mo_acquire);
      i32 diffb = (i32) (seqb - (pos + i));
      if (diffb != 0) {
        u32 oldpos = pos;
        pos = push_slot.fetch_add (0, mo_relaxed);
        if (pos == oldpos) {
          /* no collision with producer: not enough space */
          return bl_would_overflow;
        }
        /* collision with producer: retry */
        goto resync;
      }
      /*if this loop doesn't jump back or returns, it is guaranteed that there
        is no collision with the consumers. The remaining cases only collide
        with other producers and are verified by the "compare_exchange_weak"
        below */
    }
    if (push_slot.compare_exchange_weak(
      pos, pos + check, mo_relaxed, mo_relaxed
      )) {
      *rcell = &cell[idx];
      return bl_ok;
    }
    goto resync;
  }
  /*--------------------------------------------------------------------------*/
  void produce_commit (celltype* incell, uword slots)
  {
    u32 seq = incell->seq.load (mo_relaxed);
    incell->seq.store (seq + slots, mo_release);
  }
  /*--------------------------------------------------------------------------*/
  bl_err consume_prepare (celltype** rcell, u32* ticket, uword* slots)
  {
    u32 pos = pop_slot.load (mo_relaxed);
resync:
    u32 idx  = pos & mask;
    u32 seq  = cell[idx].seq.load (mo_acquire);
    i32 diff = (i32) (seq - pos);
    if (diff <= 0) {
      return bl_empty;
    }
    else if (diff > (i32) mask){
      pos = pop_slot.load (mo_relaxed);
      goto resync;
    }
    u32 slots_limit = bl_min ((u32) diff, mask + 1 - idx);
    if (pop_slot.compare_exchange_weak(
        pos, pos + slots_limit, mo_relaxed, mo_relaxed
      )) {
      *slots  = diff;
      *ticket = idx;
      *rcell = &cell[idx];
      return bl_ok;
    }
    goto resync;
  }
  /*--------------------------------------------------------------------------*/
  void consume_commit (celltype* cell, u32 ticket)
  {
    u32 seq         = cell->seq.load (mo_relaxed);
    u32 slots       = (seq - ticket) & mask;
    u32 slots_limit = bl_min ((u32) slots, mask + 1 - ticket);
    seq             = seq - slots + mask + 1;
    for (uword i = 1; i < slots_limit; ++i) {
      ++seq;
      cell[i].seq.store (seq, mo_release);
    }
    seq -= slots_limit - 1;
    cell[0].seq.store (seq, mo_release);
  }
  /*--------------------------------------------------------------------------*/
};
#define THREADS 16
//------------------------------------------------------------------------------
struct mpmpc_bpm_test : rl::test_suite<mpmpc_bpm_test, THREADS>
{
  mpmpc_bpm q;
  uword produced_slots;
  uword consumed_slots;
  /*--------------------------------------------------------------------------*/
  void before()
  {
    q.init();
    produced_slots = consumed_slots = 0;
    FILE* f = fopen ("relacy-subst.txt", "w");
    fprintf (f, "| sed 's|\\.\\./test/src/bl/mpmc_bpm_relacy/||g' |");
    fprintf (f, "sed 's|0x%lx|push_slot|g' | ", (uword) &q.push_slot);
    fprintf (f, "sed 's|0x%lx|pop_slot|g' | ", (uword) &q.pop_slot);
    fprintf (f, "sed 's|0x%lx|seq[0]|g' | ", (uword) &q.cell[0].seq);
    fprintf (f, "sed 's|0x%lx|seq[1]|g' | ", (uword) &q.cell[1].seq);
    fprintf (f, "sed 's|0x%lx|seq[2]|g' | ", (uword) &q.cell[2].seq);
    fprintf (f, "sed 's|0x%lx|seq[3]|g' | ", (uword) &q.cell[3].seq);
    fprintf (f, "sed 's|0x%lx|seq[4]|g' | ", (uword) &q.cell[4].seq);
    fprintf (f, "sed 's|0x%lx|seq[5]|g' | ", (uword) &q.cell[5].seq);
    fprintf (f, "sed 's|0x%lx|seq[6]|g' | ", (uword) &q.cell[6].seq);
    fprintf (f, "sed 's|0x%lx|seq[7]|g' | ", (uword) &q.cell[7].seq);
    fprintf (f, "sed 's|0x%lx|seq[8]|g' | ", (uword) &q.cell[8].seq);
    fprintf (f, "sed 's|0x%lx|seq[9]|g' | ", (uword) &q.cell[9].seq);
    fprintf (f, "sed 's|0x%lx|seq[10]|g' | ", (uword) &q.cell[10].seq);
    fprintf (f, "sed 's|0x%lx|seq[11]|g' | ", (uword) &q.cell[11].seq);
    fprintf (f, "sed 's|0x%lx|seq[12]|g' | ", (uword) &q.cell[12].seq);
    fprintf (f, "sed 's|0x%lx|seq[13]|g' | ", (uword) &q.cell[13].seq);
    fprintf (f, "sed 's|0x%lx|seq[14]|g' | ", (uword) &q.cell[14].seq);
    fprintf (f, "sed 's|0x%lx|v[0]|g' | ", (uword) &q.cell[0].v);
    fprintf (f, "sed 's|0x%lx|v[1]|g' | ", (uword) &q.cell[1].v);
    fprintf (f, "sed 's|0x%lx|v[2]|g' | ", (uword) &q.cell[2].v);
    fprintf (f, "sed 's|0x%lx|v[3]|g' | ", (uword) &q.cell[3].v);
    fprintf (f, "sed 's|0x%lx|v[4]|g' | ", (uword) &q.cell[4].v);
    fprintf (f, "sed 's|0x%lx|v[5]|g' | ", (uword) &q.cell[5].v);
    fprintf (f, "sed 's|0x%lx|v[6]|g' | ", (uword) &q.cell[6].v);
    fprintf (f, "sed 's|0x%lx|v[7]|g' | ", (uword) &q.cell[7].v);
    fprintf (f, "sed 's|0x%lx|v[8]|g' | ", (uword) &q.cell[8].v);
    fprintf (f, "sed 's|0x%lx|v[9]|g' | ", (uword) &q.cell[9].v);
    fprintf (f, "sed 's|0x%lx|v[10]|g' | ", (uword) &q.cell[10].v);
    fprintf (f, "sed 's|0x%lx|v[11]|g' | ", (uword) &q.cell[11].v);
    fprintf (f, "sed 's|0x%lx|v[12]|g' | ", (uword) &q.cell[12].v);
    fprintf (f, "sed 's|0x%lx|v[13]|g' | ", (uword) &q.cell[13].v);
    fprintf (f, "sed 's|0x%lx|v[14]|g'", (uword) &q.cell[14].v);
    fclose (f);
    /*Subtitutes the atomic/var addresses by names: run on the shell:

    > bl-nonblock-mpmc-bpm-relacy > relacy.txt
    > CMD="cat relacy.txt $(cat relacy-subst.txt)" && eval $CMD > relacy.subst.txt
    */
  }
  /*--------------------------------------------------------------------------*/
  void run_writer (unsigned slots)
  {
#if 0 /*relacy seems to crash here when running either sched_bound or sched_full */
    char str[256];
    snprintf (str, sizeof str, "Producer for %u slots", slots);
    RL_LOG (str);
#endif
    celltype* c;
    bl_err err =  q.produce_prepare (&c, slots);
    if (err == bl_ok) {
      c[0].v($) = CONSTANT + slots;
      for (uword i = 1; i < slots; ++i) {
        /*simulating usage of seq as raw data*/
        c[i].seq.store (CONSTANT + slots - i, mo_relaxed);
        c[i].v($) = CONSTANT + slots - i;
      }
      q.produce_commit (c, slots);
      produced_slots += slots;
    }
  }
  /*--------------------------------------------------------------------------*/
  bool run_reader_once()
  {
    celltype* c;
    u32       ticket;
    uword     slots;
    bl_err err = q.consume_prepare (&c, &ticket, &slots);
    if (err == bl_ok) {
      consumed_slots += slots;
      RL_ASSERT (c[0].v($) == CONSTANT + slots);
      for (uword i = 1; i < slots; ++i) {
        RL_ASSERT (c[i].seq.load (mo_relaxed) == (CONSTANT + slots - i));
        RL_ASSERT (c[i].v($) == CONSTANT + slots - i);
      }
      q.consume_commit (c, ticket);
      return true;
    }
    return false;
  }
  /*--------------------------------------------------------------------------*/
  void run_reader()
  {
    for (uword i = 0; i < 16; ++i) {
      run_reader_once();
    }
  }
  /*--------------------------------------------------------------------------*/
  void thread (unsigned i)
  {
    if (i < (THREADS / 2)) {
      run_writer (i != 0 ? i : 1);
    }
    else {
      run_reader();
    }
  }
  /*--------------------------------------------------------------------------*/
  void after()
  {
    if (produced_slots < SLOTS) {
      std::printf ("produced slots: %u\n", produced_slots);
      RL_ASSERT (false && "producer slots not filled");
    }
    while (run_reader_once());
    if (consumed_slots != produced_slots) {
      std::printf(
        "produced slots: %u, consumed slots: %u\n",
        produced_slots, consumed_slots
        );
      RL_ASSERT (false && "consumer slots not filled");
    }
  }
  /*--------------------------------------------------------------------------*/
};
//------------------------------------------------------------------------------
int main (int argc, char const* argv[])
{
  rl::test_params params;
    //params.iteration_count = 100000;
    //params.search_type     = rl::sched_full;
    params.search_type     = rl::sched_bound;
  rl::simulate<mpmpc_bpm_test> (params);
  return 0;
}
//------------------------------------------------------------------------------
