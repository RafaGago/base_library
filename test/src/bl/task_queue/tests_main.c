#include <stdio.h>
#include <bl/task_queue/delayed_test.h>
#include <bl/task_queue/task_queue_not_totally_broken_test.h>

int main (void)
{
  int failed = 0;
  if (delayed_tests() != 0)                       { ++failed; }
  if (task_queue_not_totally_broken_tests() != 0) { ++failed; }

  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}

