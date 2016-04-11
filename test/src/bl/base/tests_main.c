#include <stdio.h>

extern int ringb_tests (void);
extern int integer_manipulation_tests (void);
extern int alnls_tests (void);
extern int adlnls_tests (void);
extern int dynarray_tests (void);
extern int autoarray_tests (void);
extern int oringb_tests (void);
extern int semaphore_not_totally_broken_tests (void);
extern int time_tests (void);
extern int flat_deadlines_tests (void);

int main (void)
{
  int failed = 0;
  if (ringb_tests() != 0)                        { ++failed; }
  if (integer_manipulation_tests() != 0)         { ++failed; }
  if (alnls_tests() != 0)                        { ++failed; }
  if (adlnls_tests() != 0)                       { ++failed; }
  if (dynarray_tests() != 0)                     { ++failed; }
  if (oringb_tests() != 0)                       { ++failed; }
  if (semaphore_not_totally_broken_tests() != 0) { ++failed; }
  if (time_tests() != 0)                         { ++failed; }
  if (flat_deadlines_tests() != 0)               { ++failed; }
  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}
