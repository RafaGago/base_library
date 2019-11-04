#!/bin/bash -e

PROJROOT=$(dirname "$(readlink -f "$0")")/..

AUTOGEN="scripts/autogenerator.sh"

$AUTOGEN -f source-generation/array_linked_list.gen.h \
  -r TYPE=u8 \
  -r TYPE=u16 \
  -r TYPE=u32 \
  -r TYPE=u64
$AUTOGEN -f source-generation/array_doubly_linked_list.gen.h \
  -r TYPE=u8 \
  -r TYPE=u16 \
  -r TYPE=u32 \
  -r TYPE=u64
$AUTOGEN -f source-generation/timepoint_funcs_arbitrary_base.gen.h \
  -r BITS=64,CLOCKNAME=timept64,EXPORT=BL_EXPORT,INCLUDE_HDR=base/libexport\.h,INCLUDE_SRC=base/time\.h,HDR_PREFIX=base/impl,SRC_PREFIX=base \
  -r BITS=32,CLOCKNAME=timept32,EXPORT=BL_EXPORT,INCLUDE_HDR=base/libexport\.h,INCLUDE_SRC=base/time\.h,HDR_PREFIX=base/impl,SRC_PREFIX=base

$AUTOGEN -f source-generation/timepoint_funcs_nanosecond_base.gen.h \
  -r BITS=64,CLOCKNAME=timept64 \
  -r BITS=64,CLOCKNAME=sysclock64

$AUTOGEN -f source-generation/timepoint_funcs_microsecond_base.gen.h \
  -r BITS=32,CLOCKNAME=timept32

$AUTOGEN -f source-generation/timepoint_funcs_second_base.gen.h \
  -r BITS=32,CLOCKNAME=sysclock32

$AUTOGEN -f source-generation/timepoint_funcs_forward.gen.h \
  -r SRCCLK=timept32,DSTCLK=timept \
  -r SRCCLK=timept64,DSTCLK=timept \
  -r SRCCLK=sysclock32,DSTCLK=sysclock \
  -r SRCCLK=sysclock64,DSTCLK=sysclock \
