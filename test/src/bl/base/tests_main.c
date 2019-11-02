#include <stdio.h>

extern int bl_dstr_tests (void);
extern int bl_ringb_tests (void);
extern int integer_manipulation_tests (void);
extern int integer_math_tests (void);
extern int bl_alnls_tests (void);
extern int bl_adlnls_tests (void);
extern int bl_dynarray_tests (void);
extern int ac_bl_dynarray_tests (void);
extern int bl_autoarray_tests (void);
extern int bl_oringb_tests (void);
extern int semaphore_not_totally_broken_tests (void);
extern int time_tests (void);
extern int bl_flat_deadlines_tests (void);
extern int hex_string_tests (void);
extern int bl_string_tests (void);

int main (void)
{
  int failed = 0;
  if (bl_ringb_tests() != 0)                     { ++failed; }
  if (integer_manipulation_tests() != 0)         { ++failed; }
  if (integer_math_tests() != 0)                 { ++failed; }
  if (bl_alnls_tests() != 0)                     { ++failed; }
  if (bl_adlnls_tests() != 0)                    { ++failed; }
  if (bl_dynarray_tests() != 0)                  { ++failed; }
  if (ac_bl_dynarray_tests() != 0)               { ++failed; }
  if (bl_autoarray_tests() != 0)                 { ++failed; }
  if (bl_oringb_tests() != 0)                    { ++failed; }
  if (semaphore_not_totally_broken_tests() != 0) { ++failed; }
  if (time_tests() != 0)                         { ++failed; }
  if (bl_flat_deadlines_tests() != 0)            { ++failed; }
  if (hex_string_tests() != 0)                   { ++failed; }
  if (bl_dstr_tests() != 0)                      { ++failed; }
  if (bl_string_tests() != 0)                    { ++failed; }

  printf ("\n[SUITE ERR ] %d suite(s)\n", failed);
  return failed;
}
