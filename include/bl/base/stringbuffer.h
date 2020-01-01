#ifndef __BL_STRINGBUFFER__
#define __BL_STRINGBUFFER__

#include <stddef.h>

struct bl_alloc_tbl;

/*------------------------------------------------------------------------------
 A structure representing a buffer containing a string.

 Notice that a deallocated buffer is expressed by str = NULL. maxlen = 0 with a
 non-NULL "str" means that "str" has capacity only for the null termination
------------------------------------------------------------------------------*/
typedef struct bl_strbuf {
  char*  str;    /* string */
  size_t len;    /* actual string length (excluding null termination )*/
  size_t maxlen; /* max/allocated length (excluding null termination )*/
}
bl_strbuf;
/*----------------------------------------------------------------------------*/
static inline bl_strbuf bl_strbuf_init (char* str, size_t len, size_t maxlen)
{
  bl_strbuf b = { str, len, maxlen };
  return b;
}
/*------------------------------------------------------------------------------
 A structure representing a buffer containing a string and the allocator that
 generated it.
------------------------------------------------------------------------------*/
typedef struct bl_dstrbuf {
  char*  str;    /* string */
  size_t len;    /* actual string length (excluding null termination )*/
  size_t maxlen; /* max/allocated length (excluding null termination )*/
  struct bl_alloc_tbl const* alloc; /* allocator used to allocate "str" */
}
bl_dstrbuf;
/*----------------------------------------------------------------------------*/
static inline bl_dstrbuf bl_dstrbuf_init(
  char* str, size_t len, size_t maxlen, struct bl_alloc_tbl const* alloc
  )
{
  bl_dstrbuf b = { str, len, maxlen, alloc };
  return b;
}
/*----------------------------------------------------------------------------*/

#endif
