#include <stdio.h>
#include <bl/nonblock/mpmc_b_test.h>
#include <bl/nonblock/mpmc_bt_test.h>
#include <bl/nonblock/mpsc_i_test.h>

int main (void)
{
  int failed = 0;
  if (mpmc_bt_tests() != 0) { ++failed; }
  if (mpmc_b_tests() != 0)  { ++failed; }
  if (mpsc_i_tests() != 0)  { ++failed; }
  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}
