#include <stdio.h>

extern int bl_mpmc_bt_tests (void);
extern int bl_mpmc_b_tests (void);
extern int bl_mpmc_bpm_tests (void);
extern int bl_mpsc_i_tests (void);

int main (void)
{
  int failed = 0;
  if (bl_mpmc_bt_tests() != 0)  { ++failed; }
  if (bl_mpmc_b_tests() != 0)   { ++failed; }
  if (bl_mpmc_bpm_tests() != 0) { ++failed; }
  if (bl_mpsc_i_tests() != 0)   { ++failed; }
  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}
