#include <stdio.h>

extern int mpmc_bt_tests (void);
extern int mpmc_b_tests (void);
extern int mpmc_bpm_tests (void);
extern int mpsc_i_tests (void);

int main (void)
{
  int failed = 0;
  if (mpmc_bt_tests() != 0)  { ++failed; }
  if (mpmc_b_tests() != 0)   { ++failed; }
  if (mpmc_bpm_tests() != 0) { ++failed; }
  if (mpsc_i_tests() != 0)   { ++failed; }
  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}
