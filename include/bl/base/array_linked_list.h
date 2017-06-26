#ifndef __BL_ARRAY_LINKED_LIST_H__
#define __BL_ARRAY_LINKED_LIST_H__

/*------------------------------------------------------------------------------
  A singly-linked list with node references as array indexes to an embedded
  fixed sized array.

  Note that there is no data storage. You can use the iterators directly to
  access another array containing the data you want to associate with each
  entry.

  This is a very specialized data structure just for memory constrained
  environments. Not a general use one.

  For further usage reference see the unit tests for this type or any of the
  files included below.
------------------------------------------------------------------------------*/
#include <bl/base/impl/array_linked_list_u8.h>
#include <bl/base/impl/array_linked_list_u16.h>
#include <bl/base/impl/array_linked_list_u32.h>
#include <bl/base/impl/array_linked_list_u64.h>

#endif /* __BL_ARRAY_LINKED_LIST_H__ */
