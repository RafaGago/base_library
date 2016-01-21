#ifndef __BL_SHAREDLIB_LOAD_H__
#define __BL_SHAREDLIB_LOAD_H__

#include <bl/hdr/base/platform.h>

#ifdef BL_POSIX

#include <dlfcn.h>
/*----------------------------------------------------------------------------*/
typedef void* bl_sharedlib;
/*----------------------------------------------------------------------------*/
static inline bl_sharedlib bl_sharedlib_load (const char* path)
{
  return dlopen (path, RTLD_NOW);
}
/*----------------------------------------------------------------------------*/
static inline bool bl_sharedlib_unload (bl_sharedlib s)
{
    dlclose (s);
    return true;
}
/*----------------------------------------------------------------------------*/
static inline void* bl_sharedlib_loadsym (bl_sharedlib s, const char* symbol)
{
    return dlsym (s, symbol);
}
/*----------------------------------------------------------------------------*/
static inline void bl_sharedlib_clear_past_errors()
{
    dlerror();
}
/*----------------------------------------------------------------------------*/
static inline const char* bl_sharedlib_last_load_error (bl_sharedlib)
{
    return dlerror();
}
/*----------------------------------------------------------------------------*/
static inline const char* bl_sharedlib_last_sym_error (bl_sharedlib)
{
    return dlerror();
}
/*----------------------------------------------------------------------------*/

#elif defined (BL_WINDOWS)

#include <bl/hdr/base/include_windows.h>

/*----------------------------------------------------------------------------*/
typedef HINSTANCE bl_sharedlib;
/*----------------------------------------------------------------------------*/
static inline bl_sharedlib bl_sharedlib_load (const char* path)
{
  return LoadLibraryA (path);
}
/*----------------------------------------------------------------------------*/
static inline bool bl_sharedlib_unload (bl_sharedlib s)
{
  return FreeLibrary (s) ? true : false;
}
/*----------------------------------------------------------------------------*/
static inline void* bl_sharedlib_loadsym (bl_sharedlib s, const char* symbol)
{
  return GetProcAddress (hdlssymbol);
}
/*----------------------------------------------------------------------------*/
static inline void bl_sharedlib_clear_past_errors() {}
/*----------------------------------------------------------------------------*/
static inline const char* bl_sharedlib_last_load_error (bl_sharedlib s)
{
    return s ? nullptr : "library loading failed\n";
}
/*----------------------------------------------------------------------------*/
static inline const char* bl_sharedlib_last_sym_error (bl_sharedlib s)
{
    return s ? nullptr : "symbol loading failed\n";
}
/*----------------------------------------------------------------------------*/

#endif /* BL_WINDOWS */

#endif /* __BL_SHAREDLIB_LOAD_H__ */

