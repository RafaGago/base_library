#include <stdio.h>
#include <bl/getopt/getopt_test.h>

int main (void)
{
  int failed = 0;
  if (bl_getopt_short_tests() != 0) { ++failed; }
  if (bl_getopt_long_tests() != 0) { ++failed; }

  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}

