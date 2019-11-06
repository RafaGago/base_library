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
  -r BITS=64,CLOCKNAME=timept64,EXPORT=BL_EXPORT,INCLUDE_HDR=base/libexport\.h,INCLUDE_SRC=base/time\.h,HDR_PREFIX=base/impl/generated/time,SRC_PREFIX=base/generated/time \
  -r BITS=32,CLOCKNAME=timept32,EXPORT=BL_EXPORT,INCLUDE_HDR=base/libexport\.h,INCLUDE_SRC=base/time\.h,HDR_PREFIX=base/impl/generated/time,SRC_PREFIX=base/generated/time \
  -r BITS=64,CLOCKNAME=cpu_timept,EXPORT=BL_TIME_EXTRAS_EXPORT,INCLUDE_HDR=time_extras/libexport\.h,INCLUDE_SRC=time_extras/time_extras\.h,HDR_PREFIX=time_extras/generated,SRC_PREFIX=time_extras/generated

$AUTOGEN -f source-generation/timepoint_funcs_nanosecond_base.gen.h \
  -r BITS=64,CLOCKNAME=timept64 \
  -r BITS=64,CLOCKNAME=sysclock64

$AUTOGEN -f source-generation/timepoint_funcs_microsecond_base.gen.h \
  -r BITS=32,CLOCKNAME=timept32

$AUTOGEN -f source-generation/timepoint_funcs_second_base.gen.h \
  -r BITS=32,CLOCKNAME=sysclock32

$AUTOGEN -f source-generation/timepoint_funcs_forward.gen.h \
  -r SRCCLK=timept32,DSTCLK=timept,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=timept64,DSTCLK=timept,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=sysclock32,DSTCLK=sysclock,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=sysclock64,DSTCLK=sysclock,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=cpu_timept,DSTCLK=fast_timept,HDR_PREFIX=time_extras/generated \
  -r SRCCLK=timept64,DSTCLK=fast_timept,HDR_PREFIX=time_extras/generated

$AUTOGEN -f source-generation/deadline.gen.h \
  -r BITS=32,CLOCKNAME=timept32,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r BITS=64,CLOCKNAME=timept64,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r BITS=32,CLOCKNAME=sysclock32,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r BITS=64,CLOCKNAME=sysclock64,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r BITS=64,CLOCKNAME=cpu_timept,INCLUDE_HDR=time_extras/time_extras\.h,HDR_PREFIX=time_extras/generated

  $AUTOGEN -f source-generation/deadline_forward.gen.h \
  -r SRCCLK=timept32,DSTCLK=timept,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=timept64,DSTCLK=timept,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=sysclock32,DSTCLK=sysclock,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=sysclock64,DSTCLK=sysclock,INCLUDE_HDR=base/time\.h,HDR_PREFIX=base/impl/generated/time \
  -r SRCCLK=cpu_timept,DSTCLK=fast_timept,INCLUDE_HDR=time_extras/time_extras\.h,HDR_PREFIX=time_extras/generated \
  -r SRCCLK=timept64,DSTCLK=fast_timept,INCLUDE_HDR=time_extras/time_extras\.h,HDR_PREFIX=time_extras/generated
