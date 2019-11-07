#ifndef __BL_ARRAY_DOUBLY_LINKED_LIST_H__
#define __BL_ARRAY_DOUBLY_LINKED_LIST_H__

/*---------------------------------------------------------------------------*/
/*
  A doubly-linked list with node references as array indexes to an embedded
  fixed sized array.

  Note that there is no data storage. You can use the iterators directly to
  access another array containing the data you want to associate with each
  entry.

  This is a very specialized data structure just for memory constrained
  environments. Not a general use one.

  For further usage reference see the unit tests for this type.
*/
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include <bl/base/impl/generated/array_doubly_linked_list_u8.h>
#include <bl/base/impl/generated/array_doubly_linked_list_u16.h>
#include <bl/base/impl/generated/array_doubly_linked_list_u32.h>
#include <bl/base/impl/generated/array_doubly_linked_list_u64.h>

#ifdef __cplusplus
}
#endif

#endif /* __BL_ARRAY_DOUBLY_LINKED_LIST_H__ */

