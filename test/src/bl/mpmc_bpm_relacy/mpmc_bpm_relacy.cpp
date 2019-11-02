#ifdef BL_NONBLOCK_MPMC_ONLY_TEST_ALGORITHM

#include <bl/base/integer_printf_format.h>
#include <relacy/relacy.hpp>
#include <stdint.h>
#include <iostream>
#include <cstdio>

//------------------------------------------------------------------------------
typedef uint8_t   bl_u8;
typedef uint32_t  bl_u32;
typedef int32_t   bl_i32;
typedef uintptr_t bl_uword;
//------------------------------------------------------------------------------
struct slottype {
  rl::atomic<bl_u32> seq;
  rl::var<bl_u32>    v;
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
#define bl_mo_acquire rl::memory_order_acquire
#define bl_mo_release rl::memory_order_release
#define bl_mo_relaxed rl::memory_order_relaxed
#define bl_mo_seq_cst rl::memory_order_seq_cst

//------------------------------------------------------------------------------
struct bl_mpmc_bpm
{
  slottype        cell[SLOTS + SLOTS - 1];
  bl_u32             mask;
  rl::atomic<bl_u32> push_slot;
  rl::atomic<bl_u32> pop_slot;

  /*--------------------------------------------------------------------------*/
  void init()
  {
    mask = SLOTS - 1;
    for (bl_uword i = 0; i < SLOTS; ++i) {
      cell[i].seq.store (i, bl_mo_seq_cst);
      cell[i].v($) = 3000;
    }
    push_slot.store (0, bl_mo_seq_cst);
    pop_slot.store (0, bl_mo_seq_cst);
  }
  /*--------------------------------------------------------------------------*/
  bl_u32 pos_increment (bl_u32 pos, bl_u32 inc)
  {
  /*Increments "pos" position. The variable length produce feature is based on
    having the on-memory slot version counter on the first slot. Other slots
    other than the first use this position to store raw data.

    When wrapping happens (returning from the tail of the memorybuffer to the
    head) the only known position to contains a valid seq/version counter is
    the first one (position 0).

    The tail of the buffer contains space enough to make a contiguous allocation
    on the last element of the queue.*/
    bl_u32 newpos     = pos + inc;
    bl_u32 newpos_lap = newpos & ~mask;
    return (pos & ~mask) == (newpos_lap) ? newpos : newpos_lap;
  }
  /*--------------------------------------------------------------------------*/
  bl_err produce_prepare (slottype** rcell, bl_uword slots)
  {
    if (!slots || slots >= SLOTS) {
      return bl_mkerr (bl_invalid);
    }
    bl_u32 pos = push_slot.load (bl_mo_relaxed);
resync:
    bl_u32 idx  = pos & mask;
    bl_u32 seq  = cell[idx].seq.load (bl_mo_acquire);
    bl_i32 diff = (bl_i32) (seq - pos);
    if (diff < 0) {
      return bl_would_overflow;
    }
    else if (diff > 0) {
      pos = push_slot.load (bl_mo_relaxed);
      goto resync;
    }
    bl_u32 check = bl_min ((bl_u32) slots, mask + 1 - idx);
    for (bl_uword i = 1; i < check; ++i) {
      bl_u32 seqb  = cell[idx + i].seq.load (bl_mo_acquire);
      bl_i32 diffb = (bl_i32) (seqb - (pos + i));
      if (diffb != 0) {
        bl_u32 oldpos = pos;
        pos = push_slot.fetch_add (0, bl_mo_relaxed);
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
      pos, pos + check, bl_mo_relaxed, bl_mo_relaxed
      )) {
      *rcell = &cell[idx];
      return bl_mkok();
    }
    goto resync;
  }
  /*--------------------------------------------------------------------------*/
  void produce_commit (slottype* incell, bl_uword slots)
  {
    bl_u32 seq = incell->seq.load (bl_mo_relaxed);
    incell->seq.store (seq + slots, bl_mo_release);
  }
  /*--------------------------------------------------------------------------*/
  bl_err consume_prepare (slottype** rcell, bl_u32* ticket, bl_uword* slots)
  {
    bl_u32 pos = pop_slot.load (bl_mo_relaxed);
resync:
    bl_u32 idx  = pos & mask;
    bl_u32 seq  = cell[idx].seq.load (bl_mo_acquire);
    bl_i32 diff = (bl_i32) (seq - pos);
    if (diff <= 0) {
      return bl_empty;
    }
    else if (diff > (bl_i32) mask){
      pos = pop_slot.load (bl_mo_relaxed);
      goto resync;
    }
    bl_u32 slots_limit = bl_min ((bl_u32) diff, mask + 1 - idx);
    if (pop_slot.compare_exchange_weak(
        pos, pos + slots_limit, bl_mo_relaxed, bl_mo_relaxed
      )) {
      *slots  = diff;
      *ticket = idx;
      *rcell = &cell[idx];
      return bl_mkok();
    }
    goto resync;
  }
  /*--------------------------------------------------------------------------*/
  void consume_commit (slottype* cell, bl_u32 ticket)
  {
    bl_u32 seq         = cell->seq.load (bl_mo_relaxed);
    bl_u32 slots       = (seq - ticket) & mask;
    bl_u32 slots_limit = bl_min ((bl_u32) slots, mask + 1 - ticket);
    seq             = seq - slots + mask + 1;
    for (bl_uword i = 1; i < slots_limit; ++i) {
      ++seq;
      cell[i].seq.store (seq, bl_mo_release);
    }
    seq -= slots_limit - 1;
    cell[0].seq.store (seq, bl_mo_release);
  }
  /*--------------------------------------------------------------------------*/
};
#define THREADS 16
//------------------------------------------------------------------------------
struct bl_mpmc_bpm_test : rl::test_suite<bl_mpmc_bpm_test, THREADS>
{
  bl_mpmc_bpm q;
  bl_uword produced_slots;
  bl_uword consumed_slots;
  /*--------------------------------------------------------------------------*/
  void before()
  {
    q.init();
    produced_slots = consumed_slots = 0;
    FILE* f = fopen ("relacy-subst.txt", "w");
    fprintf (f, "| sed 's|\\.\\./test/src/bl/bl_mpmc_bpm_relacy/||g' |");
    fprintf (f, "sed 's|0x%lx|push_slot|g' | ", (bl_uword) &q.push_slot);
    fprintf (f, "sed 's|0x%lx|pop_slot|g' | ", (bl_uword) &q.pop_slot);
    fprintf (f, "sed 's|0x%lx|seq[0]|g' | ", (bl_uword) &q.cell[0].seq);
    fprintf (f, "sed 's|0x%lx|seq[1]|g' | ", (bl_uword) &q.cell[1].seq);
    fprintf (f, "sed 's|0x%lx|seq[2]|g' | ", (bl_uword) &q.cell[2].seq);
    fprintf (f, "sed 's|0x%lx|seq[3]|g' | ", (bl_uword) &q.cell[3].seq);
    fprintf (f, "sed 's|0x%lx|seq[4]|g' | ", (bl_uword) &q.cell[4].seq);
    fprintf (f, "sed 's|0x%lx|seq[5]|g' | ", (bl_uword) &q.cell[5].seq);
    fprintf (f, "sed 's|0x%lx|seq[6]|g' | ", (bl_uword) &q.cell[6].seq);
    fprintf (f, "sed 's|0x%lx|seq[7]|g' | ", (bl_uword) &q.cell[7].seq);
    fprintf (f, "sed 's|0x%lx|seq[8]|g' | ", (bl_uword) &q.cell[8].seq);
    fprintf (f, "sed 's|0x%lx|seq[9]|g' | ", (bl_uword) &q.cell[9].seq);
    fprintf (f, "sed 's|0x%lx|seq[10]|g' | ", (bl_uword) &q.cell[10].seq);
    fprintf (f, "sed 's|0x%lx|seq[11]|g' | ", (bl_uword) &q.cell[11].seq);
    fprintf (f, "sed 's|0x%lx|seq[12]|g' | ", (bl_uword) &q.cell[12].seq);
    fprintf (f, "sed 's|0x%lx|seq[13]|g' | ", (bl_uword) &q.cell[13].seq);
    fprintf (f, "sed 's|0x%lx|seq[14]|g' | ", (bl_uword) &q.cell[14].seq);
    fprintf (f, "sed 's|0x%lx|v[0]|g' | ", (bl_uword) &q.cell[0].v);
    fprintf (f, "sed 's|0x%lx|v[1]|g' | ", (bl_uword) &q.cell[1].v);
    fprintf (f, "sed 's|0x%lx|v[2]|g' | ", (bl_uword) &q.cell[2].v);
    fprintf (f, "sed 's|0x%lx|v[3]|g' | ", (bl_uword) &q.cell[3].v);
    fprintf (f, "sed 's|0x%lx|v[4]|g' | ", (bl_uword) &q.cell[4].v);
    fprintf (f, "sed 's|0x%lx|v[5]|g' | ", (bl_uword) &q.cell[5].v);
    fprintf (f, "sed 's|0x%lx|v[6]|g' | ", (bl_uword) &q.cell[6].v);
    fprintf (f, "sed 's|0x%lx|v[7]|g' | ", (bl_uword) &q.cell[7].v);
    fprintf (f, "sed 's|0x%lx|v[8]|g' | ", (bl_uword) &q.cell[8].v);
    fprintf (f, "sed 's|0x%lx|v[9]|g' | ", (bl_uword) &q.cell[9].v);
    fprintf (f, "sed 's|0x%lx|v[10]|g' | ", (bl_uword) &q.cell[10].v);
    fprintf (f, "sed 's|0x%lx|v[11]|g' | ", (bl_uword) &q.cell[11].v);
    fprintf (f, "sed 's|0x%lx|v[12]|g' | ", (bl_uword) &q.cell[12].v);
    fprintf (f, "sed 's|0x%lx|v[13]|g' | ", (bl_uword) &q.cell[13].v);
    fprintf (f, "sed 's|0x%lx|v[14]|g'", (bl_uword) &q.cell[14].v);
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
    slottype* c;
    bl_err err =  q.produce_prepare (&c, slots);
    if (err.bl == bl_ok) {
      c[0].v($) = CONSTANT + slots;
      for (bl_uword i = 1; i < slots; ++i) {
        /*simulating usage of seq as raw data*/
        c[i].seq.store (CONSTANT + slots - i, bl_mo_relaxed);
        c[i].v($) = CONSTANT + slots - i;
      }
      q.produce_commit (c, slots);
      produced_slots += slots;
    }
  }
  /*--------------------------------------------------------------------------*/
  bool run_reader_once()
  {
    slottype* c;
    bl_u32       ticket;
    bl_uword     slots;
    bl_err err = q.consume_prepare (&c, &ticket, &slots);
    if (err.bl == bl_ok) {
      consumed_slots += slots;
      RL_ASSERT (c[0].v($) == CONSTANT + slots);
      for (bl_uword i = 1; i < slots; ++i) {
        RL_ASSERT (c[i].seq.load (bl_mo_relaxed) == (CONSTANT + slots - i));
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
    for (bl_uword i = 0; i < 16; ++i) {
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
      std::printf ("produced slots: %" BL_FMT_UW "\n", produced_slots);
      RL_ASSERT (false && "producer slots not filled");
    }
    while (run_reader_once());
    if (consumed_slots != produced_slots) {
      std::printf(
        "produced slots: %" BL_FMT_UW ", consumed slots: %" BL_FMT_UW "\n",
        produced_slots, consumed_slots
        );
      RL_ASSERT (false && "consumer slots not filled");
    }
  }
  /*--------------------------------------------------------------------------*/
};
#else

#define BL_ATOMIC_USE_RELACY

/*This has always to be included first, it replaces malloc, free, etc.*/
#include <bl/base/integer_printf_format.h>
#include <relacy/relacy.hpp>

#include <bl/base/alignment.h>
#include <bl/base/static_integer_math.h>
#include <bl/nonblock/mpmc_b_common.c>
#include <bl/nonblock/mpmc_bpm.c>

/*Allocator workaround, relacy realloc is broken */
/*---------------------------------------------------------------------------*/
static void* bl_default_alloc_func (size_t bytes, bl_alloc_tbl const* invoker)
{
  return malloc (bytes);
}
/*---------------------------------------------------------------------------*/
static void* bl_default_realloc_func(
  void* mem, size_t new_size, bl_alloc_tbl const* invoker
  )
{
  RL_ASSERT(false);
  return nullptr;
}
/*---------------------------------------------------------------------------*/
static void bl_default_dealloc_func(
  void const* mem, bl_alloc_tbl const* invoker
  )
{
  free ((void*) mem);
}
/*---------------------------------------------------------------------------*/
static inline bl_alloc_tbl bl_get_default_alloc()
{
  bl_alloc_tbl default_alloc;
  default_alloc.alloc   = bl_default_alloc_func;
  default_alloc.realloc = bl_default_realloc_func;
  default_alloc.dealloc = bl_default_dealloc_func;
  return default_alloc;
}
//------------------------------------------------------------------------------
struct slottype {
  rl::atomic<bl_u32> seq;
  rl::var<bl_u32>    v;
};

#define CONSTANT 100000
#define THREADS 16
#define SLOTS 8
//------------------------------------------------------------------------------
struct bl_mpmc_bpm_test : rl::test_suite<bl_mpmc_bpm_test, THREADS>
{
  bl_mpmc_bpm  q;
  bl_alloc_tbl alloc;
  slottype* cell;
  bl_uword     produced_slots;
  bl_uword     consumed_slots;
  /*--------------------------------------------------------------------------*/
  bl_mpmc_bpm_test()
  {
    alloc      = bl_get_default_alloc();
    bl_err err = bl_mpmc_bpm_init(
      &q,
      &alloc,
      SLOTS,
      SLOTS - 1,
      sizeof (slottype),
      bl_alignof (slottype),
      false
      );
    RL_ASSERT (!err.bl);
    cell = (slottype*) q.mem;
    /*giving initial values to the array*/
    for (bl_uword i = 0; i < (SLOTS + SLOTS - 2); ++i) {
      new (&cell[i].seq) bl_atomic_u32();
      new (&cell[i].v) rl::var<bl_u32>();
      cell[i].seq.store ((bl_u32) i, bl_mo_seq_cst, $);
      cell[i].v($) = (bl_u32) ~i;
    }
  }
  /*--------------------------------------------------------------------------*/
  ~bl_mpmc_bpm_test()
  {
    for (bl_uword i = 0; i < (SLOTS + SLOTS - 2); ++i) {
      cell[i].v.~var();
      cell[i].seq.~bl_atomic_u32();
    }
    bl_mpmc_bpm_destroy (&q, &alloc);
  }
  /*--------------------------------------------------------------------------*/
  void before()
  {
    produced_slots = 0;
    consumed_slots = 0;
    /*debug tooling*/
    FILE* f = fopen ("relacy-subst.txt", "w");

    fprintf (f, "| sed 's|\\.\\./test/src/bl/bl_mpmc_bpm_relacy/||g' |");
    fprintf (f, "sed 's|0x%lx|push_slot|g' | ", (bl_uword) &q.push_slot);
    fprintf (f, "sed 's|0x%lx|pop_slot|g' | ", (bl_uword) &q.pop_slot);
    fprintf (f, "sed 's|0x%lx|seq[0]|g' | ", (bl_uword) &cell[0].seq);
    fprintf (f, "sed 's|0x%lx|seq[1]|g' | ", (bl_uword) &cell[1].seq);
    fprintf (f, "sed 's|0x%lx|seq[2]|g' | ", (bl_uword) &cell[2].seq);
    fprintf (f, "sed 's|0x%lx|seq[3]|g' | ", (bl_uword) &cell[3].seq);
    fprintf (f, "sed 's|0x%lx|seq[4]|g' | ", (bl_uword) &cell[4].seq);
    fprintf (f, "sed 's|0x%lx|seq[5]|g' | ", (bl_uword) &cell[5].seq);
    fprintf (f, "sed 's|0x%lx|seq[6]|g' | ", (bl_uword) &cell[6].seq);
    fprintf (f, "sed 's|0x%lx|seq[7]|g' | ", (bl_uword) &cell[7].seq);
    fprintf (f, "sed 's|0x%lx|seq[8]|g' | ", (bl_uword) &cell[8].seq);
    fprintf (f, "sed 's|0x%lx|seq[9]|g' | ", (bl_uword) &cell[9].seq);
    fprintf (f, "sed 's|0x%lx|seq[10]|g' | ", (bl_uword) &cell[10].seq);
    fprintf (f, "sed 's|0x%lx|seq[11]|g' | ", (bl_uword) &cell[11].seq);
    fprintf (f, "sed 's|0x%lx|seq[12]|g' | ", (bl_uword) &cell[12].seq);
    fprintf (f, "sed 's|0x%lx|seq[13]|g' | ", (bl_uword) &cell[13].seq);
    fprintf (f, "sed 's|0x%lx|seq[14]|g' | ", (bl_uword) &cell[14].seq);
    fprintf (f, "sed 's|0x%lx|v[0]|g' | ", (bl_uword) &cell[0].v);
    fprintf (f, "sed 's|0x%lx|v[1]|g' | ", (bl_uword) &cell[1].v);
    fprintf (f, "sed 's|0x%lx|v[2]|g' | ", (bl_uword) &cell[2].v);
    fprintf (f, "sed 's|0x%lx|v[3]|g' | ", (bl_uword) &cell[3].v);
    fprintf (f, "sed 's|0x%lx|v[4]|g' | ", (bl_uword) &cell[4].v);
    fprintf (f, "sed 's|0x%lx|v[5]|g' | ", (bl_uword) &cell[5].v);
    fprintf (f, "sed 's|0x%lx|v[6]|g' | ", (bl_uword) &cell[6].v);
    fprintf (f, "sed 's|0x%lx|v[7]|g' | ", (bl_uword) &cell[7].v);
    fprintf (f, "sed 's|0x%lx|v[8]|g' | ", (bl_uword) &cell[8].v);
    fprintf (f, "sed 's|0x%lx|v[9]|g' | ", (bl_uword) &cell[9].v);
    fprintf (f, "sed 's|0x%lx|v[10]|g' | ", (bl_uword) &cell[10].v);
    fprintf (f, "sed 's|0x%lx|v[11]|g' | ", (bl_uword) &cell[11].v);
    fprintf (f, "sed 's|0x%lx|v[12]|g' | ", (bl_uword) &cell[12].v);
    fprintf (f, "sed 's|0x%lx|v[13]|g' | ", (bl_uword) &cell[13].v);
    fprintf (f, "sed 's|0x%lx|v[14]|g'", (bl_uword) &cell[14].v);
    fclose (f);
    /*Subtitutes the atomic/var addresses by names: run on the shell:

    > bl-nonblock-mpmc-bpm-relacy > relacy.txt
    > CMD="cat relacy.txt $(cat relacy-subst.txt)" && eval $CMD > relacy.subst.txt
    */
  }
  /*--------------------------------------------------------------------------*/
  void run_writer (bl_u32 slots)
  {
    bl_mpmc_b_op op;
    bl_u8*       mem;
    bl_err    err = bl_mpmc_bpm_produce_prepare (&q, &op, &mem, slots);
    if (err.bl == bl_ok) {
      slottype* c = (slottype*) (mem - sizeof (bl_atomic_u32));
      c[0].v($) = CONSTANT + slots;
      for (bl_uword i = 1; i < slots; ++i) {
        /*simulating usage of seq as raw data*/
        c[i].seq.store (CONSTANT + slots - i, bl_mo_relaxed, $);
        c[i].v($) = CONSTANT + slots - i;
      }
      bl_mpmc_bpm_produce_commit (&q, op, mem, slots);
      produced_slots += slots;
    }
  }
  /*--------------------------------------------------------------------------*/
  bool run_reader_once()
  {
    bl_mpmc_b_op op;
    bl_u8*       mem;
    bl_u32       slots;
    bl_err    err = bl_mpmc_bpm_consume_prepare (&q, &op, &mem, &slots);
    if (err.bl == bl_ok) {
      slottype* c = (slottype*) (mem - sizeof (bl_atomic_u32));
      consumed_slots += slots;
      RL_ASSERT (c[0].v($) == CONSTANT + slots);
      for (bl_uword i = 1; i < slots; ++i) {
        RL_ASSERT (c[i].seq.load (bl_mo_relaxed, $) == (CONSTANT + slots - i));
        RL_ASSERT (c[i].v($) == CONSTANT + slots - i);
      }
      bl_mpmc_bpm_consume_commit (&q, op, mem, slots);
      return true;
    }
    return false;
  }
  /*--------------------------------------------------------------------------*/
  void run_reader()
  {
    for (bl_uword i = 0; i < SLOTS; ++i) {
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
      std::printf ("produced slots: %" BL_FMT_UW "\n", produced_slots);
      RL_ASSERT (false && "producer slots not filled");
    }
    while (run_reader_once());
    if (consumed_slots != produced_slots) {
      std::printf(
        "produced slots: %" BL_FMT_UW ", consumed slots: %" BL_FMT_UW "\n",
        produced_slots, consumed_slots
        );
      RL_ASSERT (false && "consumer slots not filled");
    }
  }
  /*--------------------------------------------------------------------------*/
};
#endif
//------------------------------------------------------------------------------
int main (int argc, char const* argv[])
{
  rl::test_params params;
  //params.iteration_count = 100000;
  //params.search_type     = rl::sched_full;
  params.search_type     = rl::sched_bound;
  rl::simulate<bl_mpmc_bpm_test> (params);
  return 0;
}
//------------------------------------------------------------------------------
