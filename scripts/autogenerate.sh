#!/bin/bash -e

PROJROOT=$(dirname "$(readlink -f "$0")")/..

scripts/autogenerator.sh -f source-generation/array_linked_list.gen.h -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64
scripts/autogenerator.sh -f source-generation/array_doubly_linked_list.gen.h -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64
scripts/autogenerator.sh -f source-generation/timepoint_funcs_arbitrary_base.gen.h -r BITS=32 -r BITS=64 -r BITS=
scripts/autogenerator.sh -f source-generation/timepoint_funcs_microsecond_base.gen.h -r BITS=32 -r BITS=
scripts/autogenerator.sh -f source-generation/timepoint_funcs_nanosecond_base.gen.h -r BITS=64 -r BITS=
scripts/autogenerator.sh -f source-generation/timepoint_sysclock_funcs_second_base.gen.h -r BITS=32 -r BITS=
scripts/autogenerator.sh -f source-generation/timepoint_sysclock_funcs_nanosecond_base.gen.h -r BITS=64 -r BITS=