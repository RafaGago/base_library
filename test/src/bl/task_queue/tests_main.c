#include <stdio.h>

extern int task_queue_not_totally_broken_tests (void);

int main (void)
{
  int failed = 0;
  if (task_queue_not_totally_broken_tests() != 0) { ++failed; }

  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}

