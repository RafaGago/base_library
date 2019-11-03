#!/bin/bash -e

PROJROOT=$(dirname "$(readlink -f "$0")")/..

scripts/autogenerator.sh -f include/bl/base/array_linked_list.gen.h -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64
scripts/autogenerator.sh -f include/bl/base/array_doubly_linked_list.gen.h -r TYPE=u8 -r TYPE=u16 -r TYPE=u32 -r TYPE=u64
