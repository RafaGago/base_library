#include <stdio.h>

extern int bl_itostr_test (void);

int main (void)
{
  int failed = 0;
  if (bl_itostr_test() != 0) { ++failed; }

  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}

