#ifndef __BL_ALLOCATOR_H__
#define __BL_ALLOCATOR_H__

#include <stdlib.h>
/*---------------------------------------------------------------------------*/
/*
  "bl_alloc_tbl" convention:

  -Objects that don't allocate memory dynamically never receive an "bl_alloc_tbl"
   pointer (obvious).

  -Objects that only allocate at initialization time ("init" function) don't
   store the "bl_alloc_tbl" pointer. Consequence of this is that they require
   the "bl_alloc_tbl" pointer on the "destroy" function.

  -Objects that allocate at initialization time and may allocate at runtime
   store the "bl_alloc_tbl" pointer. They don't require it on the "destroy"
   function.

  With this simple convention we accomplish two things: we can save
  unnecessary struct members and we can tell by just looking at its interface
  which of the three memory allocation strategies an object follows .

  An "bl_alloc_tbl" must ALWAYS be stored by reference (pointer). This is delive-
  rate for two reasons:

  -It forces the higher-order object to keep the "bl_alloc_tbl" object alive
  for the whole lifetime of the lower-order objects.

  -It is strictly necessary to store by reference; an allocator implementation
   may hipotetically use it to access the parent allocator object by offseting
   the "bl_alloc_tbl" pointer (offsetoff).
*/
/*---------------------------------------------------------------------------*/
typedef struct bl_alloc_tbl bl_alloc_tbl;
/*---------------------------------------------------------------------------*/
typedef void* (*bl_alloc_signature) (size_t bytes, bl_alloc_tbl const* invoker);
typedef void* (*rebl_alloc_signature)(
  void* mem, size_t new_size, bl_alloc_tbl const* invoker
  );
typedef void (*bl_dealloc_signature)(
  void const* mem, bl_alloc_tbl const* invoker
  );
/*---------------------------------------------------------------------------*/
typedef struct bl_alloc_tbl {
  bl_alloc_signature   alloc;
  rebl_alloc_signature realloc;
  bl_dealloc_signature dealloc;
}
bl_alloc_tbl;
/*---------------------------------------------------------------------------*/
#define bl_alloc(bl_alloc_tbl_ptr, bytes)\
  (bl_alloc_tbl_ptr)->alloc ((bytes), (bl_alloc_tbl_ptr))
/*---------------------------------------------------------------------------*/
#define bl_realloc(bl_alloc_tbl_ptr, void_ptr_mem, bytes)\
  (bl_alloc_tbl_ptr)->realloc ((void_ptr_mem), (bytes), (bl_alloc_tbl_ptr))
/*---------------------------------------------------------------------------*/
#define bl_dealloc(bl_alloc_tbl_ptr, void_ptr_mem)\
  (bl_alloc_tbl_ptr)->dealloc ((void_ptr_mem), (bl_alloc_tbl_ptr))
/*---------------------------------------------------------------------------*/

#endif /* __BL_ALLOCATOR_H__ */

