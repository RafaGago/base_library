#ifndef BL_STRUCT_JOIN_H
#define BL_STRUCT_JOIN_H

#include <bl/base/to_type_containing.h>

/*---------------------------------------------------------------------------*/
/*
  This header is a way of declaring the intentions of using "offsetoff"
  between two structs.

  It just defines a join struct containing two other structs with the name
  "base" and "xtra" and automates the macros to switch between members
  by having fixed names (for offsetoff).

  Note that the naming "base" and "xtra" is arbitrary, they could be named
  struct1 and struct2 perfectly, the point is that most often one of the
  structs is "more important" than the other, so placing base on top
  yields 0 offset (less code).

  This simple method doesn't escalate to big structs with complex hierarchies,
  so use it wisely.
*/
/*---------------------------------------------------------------------------*/
#define bl_sjoin_attach(base_struct_type, extra_struct_type)\
    base_struct_type  base;\
    extra_struct_type xtra;\
/*---------------------------------------------------------------------------*/
#define bl_sjoin_define(class_name, base_struct_type, extra_struct_type)\
  typedef struct class_name {\
    bl_sjoin_attach(base_struct_type, extra_struct_type)\
  } class_name
/*---------------------------------------------------------------------------*/
#define bl_sjoin_extra(class_ptr) (&(class_ptr)->xtra)
/*---------------------------------------------------------------------------*/
#define bl_sjoin_base(class_ptr) (&(class_ptr)->base)
/*---------------------------------------------------------------------------*/
#define bl_sjoin_from_base(class_name, base_ptr)\
  bl_to_type_containing ((base_ptr), base, class_name)
/*---------------------------------------------------------------------------*/
#define bl_sjoin_from_extra(class_name, extra_ptr)\
  bl_to_type_containing ((extra_ptr), xtra, class_name)
/*---------------------------------------------------------------------------*/
#define bl_sjoin_mv_base_extra(class_name, base_ptr)\
  bl_sjoin_extra (bl_sjoin_from_base (class_name, (base_ptr)))
/*---------------------------------------------------------------------------*/
#define bl_sjoin_mv_extra_base(class_name, extra_ptr)\
  bl_sjoin_base (bl_sjoin_from_extra (object_name, (extra_ptr)))
/*---------------------------------------------------------------------------*/
#endif  /*BL_STRUCT_JOIN_H*/
