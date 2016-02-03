#ifndef __BL_MEMORY_RANGE_H__
#define __BL_MEMORY_RANGE_H__

#include <string.h>
#include <bl/hdr/base/platform.h>
#include <bl/hdr/base/integer.h>
#include <bl/hdr/base/assert.h>
#include <bl/hdr/base/preprocessor_basic.h>
/*----------------------------------------------------------------------------*/
#define memr_initializer(addr, size) { addr, size }
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* memr8                                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
typedef struct memr8 {
  void* addr;
  u8    size;
}
memr8;
/*----------------------------------------------------------------------------*/
static inline void* memr8_beg (memr8 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline u8 memr8_size (memr8 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* memr8_end (memr8 m)
{
  return ((u8*) memr8_beg (m)) + memr8_size (m);
}
/*----------------------------------------------------------------------------*/
static inline memr8 memr8_resize (memr8 m, u8 new_size)
{
  return memr8_rv (memr8_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* memr8_at (memr8 m, u8 idx)
{
  bl_assert (idx < memr8_size (m));
  return ((u8*) memr8_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline memr8 memr8_null (void)
{
  return memr8_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool memr8_is_null (memr8 m)
{
  return memr8_beg (m) == nullptr || memr8_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool memr8_is_valid (memr8 m)
{
  return memr8_beg (m) != nullptr || memr8_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline memr8 memr8_subrange_beg (memr8 m, u8 offset_bytes)
{
  bl_assert (!memr8_is_null (m));
  /*equal allowed, will return a zero sized memr8 in such case*/
  bl_assert (memr8_size (m) <= beg_offset);
  return memr8_rv (memr8_at (m, offset_bytes), memr8_size (m) - offset_bytes);
}
/*----------------------------------------------------------------------------*/
static inline void memr8_set (memr8 m, int v)
{
  bl_assert (memr8_is_valid (m));
  memset (memr8_beg (m), v, memr8_size (m));
}
/*----------------------------------------------------------------------------*/
static inline u8 memr8_copy (memr8 dst, memr8 src)
{
  bl_assert (memr8_is_valid (src));
  bl_assert (memr8_is_valid (dst));
  bl_assert (memr8_size (dst) >= memr8_size (src));
  memcpy (memr8_beg (dst), memr8_beg (src), memr8_size (src));
}
/*----------------------------------------------------------------------------*/
static inline u8 memr8_copy_offset (memr8 dst, memr8 src, u8 dst_offset_bytes)
{
  memr8 dst_off = memr8_subrange_beg (dst, dst_offset_bytes);
  memr8_copy (&dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline memr8 memr8_rv (void* addr, u8 size)
{
  memr8 ret = memr_initializer (addr, size);
  bl_assert (memr8_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define memr8_rv_array(arr) memr8_rv (arr, sizeof arr)
#define memr8_beg_as (memr_val, type) ((type*) memr8_beg (memr_val))
#define memr8_end_as (memr_val, type) ((type*) memr8_end (memr_val))
#define memr8_at_as (memr_val, idx, type)\
  ((type*) memr8_at ((memr_val), idx * sizeof (type)))
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* memr16                                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
typedef struct memr16 {
  void* addr;
  u16   size;
}
memr16;
/*----------------------------------------------------------------------------*/
static inline void* memr16_beg (memr16 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline u16 memr16_size (memr16 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* memr16_end (memr16 m)
{
  return ((u8*) memr16_beg (m)) + memr16_size (m);
}
/*----------------------------------------------------------------------------*/
static inline memr16 memr16_resize (memr16 m, u16 new_size)
{
  return memr16_rv (memr16_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* memr16_at (memr16 m, u16 idx)
{
  bl_assert (idx < memr16_size (m));
  return ((u8*) memr16_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline memr16 memr16_null (void)
{
  return memr16_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool memr16_is_null (memr16 m)
{
  return memr16_beg (m) == nullptr || memr16_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool memr16_is_valid (memr16 m)
{
  return memr16_beg (m) != nullptr || memr16_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline memr16 memr16_subrange_beg (memr16 m, u16 offset_bytes)
{
  bl_assert (!memr16_is_null (m));
  /*equal allowed, will return a zero sized memr16 in such case*/
  bl_assert (memr16_size (m) <= beg_offset);
  return memr16_rv(
    memr16_at (m, offset_bytes), memr16_size (m) - offset_bytes
    );
}
/*----------------------------------------------------------------------------*/
static inline void memr16_set (memr16 m, int v)
{
  bl_assert (memr16_is_valid (m));
  memset (memr16_beg (m), v, memr16_size (m));
}
/*----------------------------------------------------------------------------*/
static inline u16 memr16_copy (memr16 dst, memr16 src)
{
  bl_assert (memr16_is_valid (src));
  bl_assert (memr16_is_valid (dst));
  bl_assert (memr16_size (dst) >= memr16_size (src));
  memcpy (memr16_beg (dst), memr16_beg (src), memr16_size (src));
}
/*----------------------------------------------------------------------------*/
static inline u16 memr16_copy_offset(
  memr16 dst, memr16 src, u16 dst_offset_bytes
  )
{
  memr16 dst_off = memr16_subrange_beg (dst, dst_offset_bytes);
  memr16_copy (&dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline memr16 memr16_rv (void* addr, u16 size)
{
  memr16 ret = memr_initializer (addr, size);
  bl_assert (memr16_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define memr16_rv_array(arr) memr16_rv (arr, sizeof arr)
#define memr16_beg_as (memr_val, type) ((type*) memr16_beg (memr_val))
#define memr16_end_as (memr_val, type) ((type*) memr16_end (memr_val))
#define memr16_at_as (memr_val, idx, type)\
  ((type*) memr16_at ((memr_val), idx * sizeof (type)))
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* memr32                                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef BL_NOINT32
typedef struct memr32 {
  void* addr;
  u32   size;
}
memr32;
/*----------------------------------------------------------------------------*/
static inline memr32 memr32_rv (void* addr, uword size)
{
  memr32 ret = memr_initializer (addr, size);
  return ret;
}
#define memr32_rv_array(arr) memr32_rv (arr, sizeof arr)
/*----------------------------------------------------------------------------*/
static inline void* memr32_beg (memr32 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline u32 memr32_size (memr32 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* memr32_end (memr32 m)
{
  return ((u8*) memr32_beg (m)) + memr32_size (m);
}
/*----------------------------------------------------------------------------*/
static inline memr32 memr32_resize (memr32 m, u32 new_size)
{
  return memr32_rv (memr32_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* memr32_at (memr32 m, u32 idx)
{
  bl_assert (idx < memr32_size (m));
  return ((u8*) memr32_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline memr32 memr32_null (void)
{
  return memr32_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool memr32_is_null (memr32 m)
{
  return memr32_beg (m) == nullptr || memr32_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool memr32_is_valid (memr32 m)
{
  return memr32_beg (m) != nullptr || memr32_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline memr32 memr32_subrange_beg (memr32 m, u32 offset_bytes)
{
  bl_assert (!memr32_is_null (m));
  /*equal allowed, will return a zero sized memr32 in such case*/
  bl_assert (memr32_size (m) <= beg_offset);
  return memr32_rv(
    memr32_at (m, offset_bytes), memr32_size (m) - offset_bytes
    );
}
/*----------------------------------------------------------------------------*/
static inline void memr32_set (memr32 m, int v)
{
  bl_assert (memr32_is_valid (m));
  memset (memr32_beg (m), v, memr32_size (m));
}
/*----------------------------------------------------------------------------*/
static inline u32 memr32_copy (memr32 dst, memr32 src)
{
  bl_assert (memr32_is_valid (src));
  bl_assert (memr32_is_valid (dst));
  bl_assert (memr32_size (dst) >= memr32_size (src));
  memcpy (memr32_beg (dst), memr32_beg (src), memr32_size (src));
}
/*----------------------------------------------------------------------------*/
static inline u32 memr32_copy_offset(
  memr32 dst, memr32 src, u32 dst_offset_bytes
  )
{
  memr32 dst_off = memr32_subrange_beg (dst, dst_offset_bytes);
  memr32_copy (&dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline memr32 memr32_rv (void* addr, u32 size)
{
  memr32 ret = memr_initializer (addr, size);
  bl_assert (memr32_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define memr32_rv_array(arr) memr32_rv (arr, sizeof arr)
#define memr32_beg_as (memr_val, type) ((type*) memr32_beg (memr_val))
#define memr32_end_as (memr_val, type) ((type*) memr32_end (memr_val))
#define memr32_at_as (memr_val, idx, type)\
  ((type*) memr32_at ((memr_val), idx * sizeof (type)))
#endif /*#ifndef BL_NOINT32*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* memr64                                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#ifndef BL_NOINT64
typedef struct memr64 {
  void* addr;
  u64   size;
}
memr64;
/*----------------------------------------------------------------------------*/
static inline memr64 memr64_rv (void* addr, uword size)
{
  memr64 ret = memr_initializer (addr, size);
  return ret;
}
#define memr64_rv_array(arr) memr64_rv (arr, sizeof arr)
/*----------------------------------------------------------------------------*/
static inline void* memr64_beg (memr64 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline u64 memr64_size (memr64 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* memr64_end (memr64 m)
{
  return ((u8*) memr64_beg (m)) + memr64_size (m);
}
/*----------------------------------------------------------------------------*/
static inline memr64 memr64_resize (memr64 m, u64 new_size)
{
  return memr64_rv (memr64_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* memr64_at (memr64 m, u64 idx)
{
  bl_assert (idx < memr64_size (m));
  return ((u8*) memr64_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline memr64 memr64_null (void)
{
  return memr64_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool memr64_is_null (memr64 m)
{
  return memr64_beg (m) == nullptr || memr64_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool memr64_is_valid (memr64 m)
{
  return memr64_beg (m) != nullptr || memr64_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline memr64 memr64_subrange_beg (memr64 m, u64 offset_bytes)
{
  bl_assert (!memr64_is_null (m));
  /*equal allowed, will return a zero sized memr64 in such case*/
  bl_assert (memr64_size (m) <= beg_offset);
  return memr64_rv(
    memr64_at (m, offset_bytes), memr64_size (m) - offset_bytes
    );
}
/*----------------------------------------------------------------------------*/
static inline void memr64_set (memr64 m, int v)
{
  bl_assert (memr64_is_valid (m));
  memset (memr64_beg (m), v, memr64_size (m));
}
/*----------------------------------------------------------------------------*/
static inline u64 memr64_copy (memr64 dst, memr64 src)
{
  bl_assert (memr64_is_valid (src));
  bl_assert (memr64_is_valid (dst));
  bl_assert (memr64_size (dst) >= memr64_size (src));
  memcpy (memr64_beg (dst), memr64_beg (src), memr64_size (src));
}
/*----------------------------------------------------------------------------*/
static inline u64 memr64_copy_offset(
  memr64 dst, memr64 src, u64 dst_offset_bytes
  )
{
  memr64 dst_off = memr64_subrange_beg (dst, dst_offset_bytes);
  memr64_copy (&dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline memr64 memr64_rv (void* addr, u64 size)
{
  memr64 ret = memr_initializer (addr, size);
  bl_assert (memr64_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define memr64_rv_array(arr) memr64_rv (arr, sizeof arr)
#define memr64_beg_as (memr_val, type) ((type*) memr64_beg (memr_val))
#define memr64_end_as (memr_val, type) ((type*) memr64_end (memr_val))
#define memr64_at_as (memr_val, idx, type)\
  ((type*) memr64_at ((memr_val), idx * sizeof (type)))
#endif /*#ifndef BL_NOINT64*/
/*----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#ifdef BL8
  typedef memr8 memr;
  #define memr_concat(suffix) pp_tokconcat (memr8_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#ifdef BL16
  typedef memr16 memr;
  #define memr_concat(suffix) pp_tokconcat (memr16_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#ifdef BL32
  typedef memr32 memr;
  #define memr_concat(suffix) pp_tokconcat (memr32_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#ifdef BL64
  typedef memr64 memr;
  #define memr_concat(suffix) pp_tokconcat (memr64_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#define memr_beg(m)               memr_concat (beg) (m)
#define memr_size(m)              memr_concat (end) (m)
#define memr_end(m)               memr_concat (size) (m)
#define memr_resize(m, s)         memr_concat (resize) ((m), (s))
#define memr_at(m, i)             memr_concat (at) ((m), (i))
#define memr_null()               memr_concat (null)
#define memr_is_null(m)           memr_concat (is_null) (m)
#define memr_is_valid(m)          memr_concat (is_valid) (m)
#define memr_subrange_beg(m, o)   memr_concat (subrange_beg) ((m), (o))
#define memr_set(m, v)            memr_concat (set) ((m), (v))
#define memr_copy(d, s)           memr_concat (copy) ((d), (s))
#define memr_copy_offset(d, s, o) memr_concat (copy_offset) ((d), (s), (o))
#define memr_rv(a, s)             memr_concat (rv) ((a), (s))
#define memr_rv_array(arr)        memr_concat (rv_array) (arr)
#define memr_beg_as(m, t)         memr_concat (beg_as) ((m), (t))
#define memr_end_as(m, t)         memr_concat (end_as) ((m), (t))
#define memr_at_as(m, i, t)       memr_concat (at_as) ((m), (i), (t))
/*----------------------------------------------------------------------------*/
#ifdef NDEBUG
  #define memr_cast(memr_val)\
    memr_rv ((memr_val).addr, (uword) (memr_val).size)
#else
  /*will make memr_rv assertion to trigger if the size is overflowed*/
  #define memr_cast(memr_val)\
    memr_rv(\
      ((uword) (memr_val).size) == (memr_val).size ? (memr_val).addr : nullptr,\
      (uword) (memr_val).size\
      )
#endif
/*----------------------------------------------------------------------------*/


#endif /* __BL_MEMORY_RANGE_H__ */

