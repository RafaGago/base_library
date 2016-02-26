#ifndef BL_TO_TYPE_CONTAINING_H
#define BL_TO_TYPE_CONTAINING_H

#include <stddef.h>

#define to_type_containing(ptr_contained, ptr_member_name, type)\
  ((type*)(((char*) (ptr_contained)) - offsetof (type, ptr_member_name)))

#endif /*BL_TO_TYPE_CONTAINING_H*/
