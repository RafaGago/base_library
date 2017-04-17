#ifndef __BL_STRING_H__
#define __BL_STRING_H__

#include <bl/base/libexport.h>
#include <bl/base/allocator.h>
#include <stdarg.h>

/*----------------------------------------------------------------------------*/
extern BL_EXPORT int bl_vasprintf(
  char **str, alloc_tbl const* alloc, const char *format, va_list args
  );
/*----------------------------------------------------------------------------*/
extern BL_EXPORT int bl_asprintf(
  char **str, alloc_tbl const* alloc, const char *format, ...
  );
/*----------------------------------------------------------------------------*/

#endif /* __BL_STRING_H__ */
