#ifndef __BL_ALLOCATOR_H__
#define __BL_ALLOCATOR_H__

#include <stdlib.h>
/*---------------------------------------------------------------------------*/
/*
  "alloc_tbl" convention:

  -Objects that don't allocate memory dynamically never receive an "alloc_tbl"
   pointer (obvious).

  -Objects that only allocate at initialization time ("init" function) don't
   store the "alloc_tbl" pointer. Consequence of this is that they require
   the "alloc_tbl" pointer on the "destroy" function. 

  -Objects that allocate at initialization time and may allocate at runtime 
   store the "alloc_tbl" pointer. They don't require it on the "destroy"
   function.

  With this simple convention we accomplish two things: we can save
  unnecessary struct members and we can tell by just looking at its interface
  which of the three memory allocation strategies an object follows .

  An "alloc_tbl" must ALWAYS be stored by reference (pointer). This is delive-
  rate for two reasons: 

  -It forces the higher-order object to keep the "alloc_tbl" object alive 
  for the whole lifetime of the lower-order objects.

  -It is strictly necessary to store by reference; an allocator implementation
   may hipotetically use it to access the parent allocator object by offseting
   the "alloc_tbl" pointer (offsetoff).
*/
/*---------------------------------------------------------------------------*/
typedef struct alloc_tbl alloc_tbl;
/*---------------------------------------------------------------------------*/
typedef void* (*alloc_signature) (size_t bytes, alloc_tbl const* invoker);
typedef void* (*realloc_signature)(
  void* mem, size_t new_size, alloc_tbl const* invoker
  );
typedef void (*dealloc_signature) (void* mem, alloc_tbl const* invoker);
/*---------------------------------------------------------------------------*/
typedef struct alloc_tbl {
  alloc_signature   alloc;
  realloc_signature realloc;
  dealloc_signature dealloc;
}
alloc_tbl;
/*---------------------------------------------------------------------------*/
#define bl_alloc(alloc_tbl_ptr, bytes)\
  (alloc_tbl_ptr)->alloc ((bytes), (alloc_tbl_ptr))
/*---------------------------------------------------------------------------*/
#define bl_realloc(alloc_tbl_ptr, void_ptr_mem, bytes)\
  (alloc_tbl_ptr)->realloc ((void_ptr_mem), (bytes), (alloc_tbl_ptr))
/*---------------------------------------------------------------------------*/
#define bl_dealloc(alloc_tbl_ptr, void_ptr_mem)\
  (alloc_tbl_ptr)->dealloc ((void_ptr_mem), (alloc_tbl_ptr))
/*---------------------------------------------------------------------------*/

#endif /* __BL_ALLOCATOR_H__ */
