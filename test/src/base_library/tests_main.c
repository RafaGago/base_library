#include <stdio.h>
#include <base_library/hdr/ringbuffer_test.h>
#include <base_library/hdr/integer_manipulation_test.h>
#include <base_library/hdr/array_linked_list_test.h>
#include <base_library/hdr/array_doubly_linked_list_test.h>
#include <base_library/hdr/ordered_ringbuffer_test.h>
#include <base_library/lib/semaphore_not_totally_broken_test.h>
#include <base_library/lib/mpmc_b_test.h>

int main (void)
{
  int failed = 0;
  if (ringb_tests() != 0)                        { ++failed; }
  if (mpmc_b_tests() != 0)                       { ++failed; }
  if (integer_manipulation_tests() != 0)         { ++failed; }
  if (alnls_tests() != 0)                        { ++failed; }
  if (adlnls_tests() != 0)                       { ++failed; }
  if (oringb_tests() != 0)                       { ++failed; }
  if (semaphore_not_totally_broken_tests() != 0) { ++failed; }
  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}
