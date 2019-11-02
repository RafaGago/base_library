#ifndef __BL_MEMORY_RANGE_H__
#define __BL_MEMORY_RANGE_H__

#include <string.h>
#include <bl/base/platform.h>
#include <bl/base/integer.h>
#include <bl/base/assert.h>
#include <bl/base/preprocessor_basic.h>
/*----------------------------------------------------------------------------*/
#define bl_memr_initializer(addr, size) { addr, size }
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* bl_memr8                                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
typedef struct bl_memr8 {
  void* addr;
  bl_u8    size;
}
bl_memr8;
/*----------------------------------------------------------------------------*/
static inline bl_memr8 bl_memr8_rv (void* addr, bl_u8 size);
/*----------------------------------------------------------------------------*/
static inline void* bl_memr8_beg (bl_memr8 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline bl_u8 bl_memr8_size (bl_memr8 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr8_end (bl_memr8 m)
{
  return ((bl_u8*) bl_memr8_beg (m)) + bl_memr8_size (m);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr8 bl_memr8_resize (bl_memr8 m, bl_u8 new_size)
{
  return bl_memr8_rv (bl_memr8_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr8_at (bl_memr8 m, bl_u8 idx)
{
  bl_assert (idx < bl_memr8_size (m));
  return ((bl_u8*) bl_memr8_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline bl_memr8 bl_memr8_null (void)
{
  return bl_memr8_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr8_is_null (bl_memr8 m)
{
  return bl_memr8_beg (m) == nullptr || bl_memr8_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr8_is_valid (bl_memr8 m)
{
  return bl_memr8_beg (m) != nullptr || bl_memr8_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline bl_memr8 bl_memr8_subrange_beg (bl_memr8 m, bl_u8 offset_bytes)
{
  bl_assert (!bl_memr8_is_null (m));
  /*equal allowed, will return a zero sized bl_memr8 in such case*/
  bl_assert (bl_memr8_size (m) >= offset_bytes);
  return bl_memr8_rv(
    bl_memr8_at (m, offset_bytes), bl_memr8_size (m) - offset_bytes
    );
}
/*----------------------------------------------------------------------------*/
static inline void bl_memr8_set (bl_memr8 m, int v)
{
  bl_assert (bl_memr8_is_valid (m));
  memset (bl_memr8_beg (m), v, bl_memr8_size (m));
}
/*----------------------------------------------------------------------------*/
static inline bl_u8 bl_memr8_copy (bl_memr8 dst, bl_memr8 src)
{
  bl_assert (bl_memr8_is_valid (src));
  bl_assert (bl_memr8_is_valid (dst));
  bl_assert (bl_memr8_size (dst) >= bl_memr8_size (src));
  memcpy (bl_memr8_beg (dst), bl_memr8_beg (src), bl_memr8_size (src));
  return bl_memr8_size (src);
}
/*----------------------------------------------------------------------------*/
static inline bl_u8 bl_memr8_copy_offset(
  bl_memr8 dst, bl_memr8 src, bl_u8 dst_offset_bytes
  )
{
  bl_memr8 dst_off = bl_memr8_subrange_beg (dst, dst_offset_bytes);
  return bl_memr8_copy (dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr8 bl_memr8_rv (void* addr, bl_u8 size)
{
  bl_memr8 ret = bl_memr_initializer (addr, size);
  bl_assert (bl_memr8_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define bl_memr8_rv_array(arr) bl_memr8_rv (arr, sizeof arr)
#define bl_memr8_beg_as(memr_val, type) ((type*) bl_memr8_beg (memr_val))
#define bl_memr8_end_as(memr_val, type) ((type*) bl_memr8_end (memr_val))
#define bl_memr8_at_as(memr_val, idx, type)\
  ((type*) bl_memr8_at ((memr_val), idx * sizeof (type)))
#ifdef NDEBUG
  #define bl_memr8_cast(memr_val)\
    bl_memr8_rv ((memr_val).addr, (bl_u8) (memr_val).size)
#else
  #define bl_memr8_cast(memr_val)\
    bl_memr_rv(\
      (memr_val).addr,\
      (\
        bl_assert (((bl_u8) (memr_val).size) == (memr_val).size),\
        (bl_u8) (memr_val).size\
      )\
    )
#endif
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* bl_memr16                                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE >= 16
typedef struct bl_memr16 {
  void* addr;
  bl_u16   size;
}
bl_memr16;
/*----------------------------------------------------------------------------*/
static inline bl_memr16 bl_memr16_rv (void* addr, bl_u16 size);
/*----------------------------------------------------------------------------*/
static inline void* bl_memr16_beg (bl_memr16 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline bl_u16 bl_memr16_size (bl_memr16 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr16_end (bl_memr16 m)
{
  return ((bl_u8*) bl_memr16_beg (m)) + bl_memr16_size (m);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr16 bl_memr16_resize (bl_memr16 m, bl_u16 new_size)
{
  return bl_memr16_rv (bl_memr16_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr16_at (bl_memr16 m, bl_u16 idx)
{
  bl_assert (idx < bl_memr16_size (m));
  return ((bl_u8*) bl_memr16_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline bl_memr16 bl_memr16_null (void)
{
  return bl_memr16_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr16_is_null (bl_memr16 m)
{
  return bl_memr16_beg (m) == nullptr || bl_memr16_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr16_is_valid (bl_memr16 m)
{
  return bl_memr16_beg (m) != nullptr || bl_memr16_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline bl_memr16 bl_memr16_subrange_beg (bl_memr16 m, bl_u16 offset_bytes)
{
  bl_assert (!bl_memr16_is_null (m));
  /*equal allowed, will return a zero sized bl_memr16 in such case*/
  bl_assert (bl_memr16_size (m) >= offset_bytes);
  return bl_memr16_rv(
    bl_memr16_at (m, offset_bytes), bl_memr16_size (m) - offset_bytes
    );
}
/*----------------------------------------------------------------------------*/
static inline void bl_memr16_set (bl_memr16 m, int v)
{
  bl_assert (bl_memr16_is_valid (m));
  bl_assert ((size_t) bl_memr16_size (m) == bl_memr16_size (m));
  memset (bl_memr16_beg (m), v, (size_t) bl_memr16_size (m));
}
/*----------------------------------------------------------------------------*/
static inline bl_u16 bl_memr16_copy (bl_memr16 dst, bl_memr16 src)
{
  bl_assert (bl_memr16_is_valid (src));
  bl_assert (bl_memr16_is_valid (dst));
  bl_assert (bl_memr16_size (dst) >= bl_memr16_size (src));
  bl_assert ((size_t) bl_memr16_size (src) == bl_memr16_size (src));
  memcpy(
    bl_memr16_beg (dst), bl_memr16_beg (src), (size_t) bl_memr16_size (src)
    );
  return bl_memr16_size (src);
}
/*----------------------------------------------------------------------------*/
static inline bl_u16 bl_memr16_copy_offset(
  bl_memr16 dst, bl_memr16 src, bl_u16 dst_offset_bytes
  )
{
  bl_memr16 dst_off = bl_memr16_subrange_beg (dst, dst_offset_bytes);
  return bl_memr16_copy (dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr16 bl_memr16_rv (void* addr, bl_u16 size)
{
  bl_memr16 ret = bl_memr_initializer (addr, size);
  bl_assert (bl_memr16_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define bl_memr16_rv_array(arr) bl_memr16_rv (arr, sizeof arr)
#define bl_memr16_beg_as(memr_val, type) ((type*) bl_memr16_beg (memr_val))
#define bl_memr16_end_as(memr_val, type) ((type*) bl_memr16_end (memr_val))
#define bl_memr16_at_as(memr_val, idx, type)\
  ((type*) bl_memr16_at ((memr_val), idx * sizeof (type)))
#ifdef NDEBUG
  #define bl_memr16_cast(memr_val)\
    bl_memr16_rv ((memr_val).addr, (bl_u16) (memr_val).size)
#else
  #define bl_memr16_cast(memr_val)\
    bl_memr_rv(\
      (memr_val).addr,\
      (\
        bl_assert (((bl_u16) (memr_val).size) == (memr_val).size),\
        (bl_u16) (memr_val).size\
      )\
    )
#endif
#endif /*#if BL_WORDSIZE >= 16*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* bl_memr32                                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE >= 32
typedef struct bl_memr32 {
  void* addr;
  bl_u32   size;
}
bl_memr32;
/*----------------------------------------------------------------------------*/
static inline bl_memr32 bl_memr32_rv (void* addr, bl_u32 size);
/*----------------------------------------------------------------------------*/
static inline void* bl_memr32_beg (bl_memr32 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline bl_u32 bl_memr32_size (bl_memr32 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr32_end (bl_memr32 m)
{
  return ((bl_u8*) bl_memr32_beg (m)) + bl_memr32_size (m);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr32 bl_memr32_resize (bl_memr32 m, bl_u32 new_size)
{
  return bl_memr32_rv (bl_memr32_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr32_at (bl_memr32 m, bl_u32 idx)
{
  bl_assert (idx < bl_memr32_size (m));
  return ((bl_u8*) bl_memr32_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline bl_memr32 bl_memr32_null (void)
{
  return bl_memr32_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr32_is_null (bl_memr32 m)
{
  return bl_memr32_beg (m) == nullptr || bl_memr32_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr32_is_valid (bl_memr32 m)
{
  return bl_memr32_beg (m) != nullptr || bl_memr32_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline bl_memr32 bl_memr32_subrange_beg (bl_memr32 m, bl_u32 offset_bytes)
{
  bl_assert (!bl_memr32_is_null (m));
  /*equal allowed, will return a zero sized bl_memr32 in such case*/
  bl_assert (bl_memr32_size (m) >= offset_bytes);
  return bl_memr32_rv(
    bl_memr32_at (m, offset_bytes), bl_memr32_size (m) - offset_bytes
    );
}
/*----------------------------------------------------------------------------*/
static inline void bl_memr32_set (bl_memr32 m, int v)
{
  bl_assert (bl_memr32_is_valid (m));
  bl_assert ((size_t) bl_memr32_size (m) == bl_memr32_size (m));
  memset (bl_memr32_beg (m), v, bl_memr32_size (m));
}
/*----------------------------------------------------------------------------*/
static inline bl_u32 bl_memr32_copy (bl_memr32 dst, bl_memr32 src)
{
  bl_assert (bl_memr32_is_valid (src));
  bl_assert (bl_memr32_is_valid (dst));
  bl_assert (bl_memr32_size (dst) >= bl_memr32_size (src));
  bl_assert ((size_t) bl_memr32_size (src) == bl_memr32_size (src));
  memcpy(
    bl_memr32_beg (dst), bl_memr32_beg (src), (size_t) bl_memr32_size (src)
    );
  return bl_memr32_size (src);
}
/*----------------------------------------------------------------------------*/
static inline bl_u32 bl_memr32_copy_offset(
  bl_memr32 dst, bl_memr32 src, bl_u32 dst_offset_bytes
  )
{
  bl_memr32 dst_off = bl_memr32_subrange_beg (dst, dst_offset_bytes);
  return bl_memr32_copy (dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr32 bl_memr32_rv (void* addr, bl_u32 size)
{
  bl_memr32 ret = bl_memr_initializer (addr, size);
  bl_assert (bl_memr32_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define bl_memr32_rv_array(arr) bl_memr32_rv (arr, sizeof arr)
#define bl_memr32_beg_as(memr_val, type) ((type*) bl_memr32_beg (memr_val))
#define bl_memr32_end_as(memr_val, type) ((type*) bl_memr32_end (memr_val))
#define bl_memr32_at_as(memr_val, idx, type)\
  ((type*) bl_memr32_at ((memr_val), idx * sizeof (type)))
#ifdef NDEBUG
  #define bl_memr32_cast(memr_val)\
    bl_memr32_rv ((memr_val).addr, (bl_u32) (memr_val).size)
#else
  #define bl_memr32_cast(memr_val)\
    bl_memr_rv(\
      (memr_val).addr,\
      (\
        bl_assert (((bl_u32) (memr_val).size) == (memr_val).size),\
        (bl_u32) (memr_val).size\
      )\
    )
#endif
#endif /*#if BL_WORDSIZE >= 32*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* bl_memr64                                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE >= 64
typedef struct bl_memr64 {
  void* addr;
  bl_u64   size;
}
bl_memr64;
/*----------------------------------------------------------------------------*/
static inline bl_memr64 bl_memr64_rv (void* addr, bl_u64 size);
/*----------------------------------------------------------------------------*/
static inline void* bl_memr64_beg (bl_memr64 m)
{
  return m.addr;
}
/*----------------------------------------------------------------------------*/
static inline bl_u64 bl_memr64_size (bl_memr64 m)
{
  return m.size;
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr64_end (bl_memr64 m)
{
  return ((bl_u8*) bl_memr64_beg (m)) + bl_memr64_size (m);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr64 bl_memr64_resize (bl_memr64 m, bl_u64 new_size)
{
  return bl_memr64_rv (bl_memr64_beg (m), new_size);
}
/*----------------------------------------------------------------------------*/
static inline void* bl_memr64_at (bl_memr64 m, bl_u64 idx)
{
  bl_assert (idx < bl_memr64_size (m));
  return ((bl_u8*) bl_memr64_beg (m)) + idx;
}
/*----------------------------------------------------------------------------*/
static inline bl_memr64 bl_memr64_null (void)
{
  return bl_memr64_rv (nullptr, 0);
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr64_is_null (bl_memr64 m)
{
  return bl_memr64_beg (m) == nullptr || bl_memr64_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool bl_memr64_is_valid (bl_memr64 m)
{
  return bl_memr64_beg (m) != nullptr || bl_memr64_size (m) == 0;
}
/*----------------------------------------------------------------------------*/
/*subrange from beggining*/
static inline bl_memr64 bl_memr64_subrange_beg (bl_memr64 m, bl_u64 offset_bytes)
{
  bl_assert (!bl_memr64_is_null (m));
  /*equal allowed, will return a zero sized bl_memr64 in such case*/
  bl_assert (bl_memr64_size (m) >= offset_bytes);
  return bl_memr64_rv(
    bl_memr64_at (m, offset_bytes), bl_memr64_size (m) - offset_bytes
    );
}
/*----------------------------------------------------------------------------*/
static inline void bl_memr64_set (bl_memr64 m, int v)
{
  bl_assert (bl_memr64_is_valid (m));
  bl_assert ((size_t) bl_memr64_size (m) == bl_memr64_size (m));
  memset (bl_memr64_beg (m), v, (size_t) bl_memr64_size (m));
}
/*----------------------------------------------------------------------------*/
static inline bl_u64 bl_memr64_copy (bl_memr64 dst, bl_memr64 src)
{
  bl_assert (bl_memr64_is_valid (src));
  bl_assert (bl_memr64_is_valid (dst));
  bl_assert (bl_memr64_size (dst) >= bl_memr64_size (src));
  bl_assert ((size_t) bl_memr64_size (src) == bl_memr64_size (src));
  memcpy (bl_memr64_beg (dst), bl_memr64_beg (src), bl_memr64_size (src));
  return bl_memr64_size (src);
}
/*----------------------------------------------------------------------------*/
static inline bl_u64 bl_memr64_copy_offset(
  bl_memr64 dst, bl_memr64 src, bl_u64 dst_offset_bytes
  )
{
  bl_memr64 dst_off = bl_memr64_subrange_beg (dst, dst_offset_bytes);
  return bl_memr64_copy (dst_off, src);
}
/*----------------------------------------------------------------------------*/
static inline bl_memr64 bl_memr64_rv (void* addr, bl_u64 size)
{
  bl_memr64 ret = bl_memr_initializer (addr, size);
  bl_assert (bl_memr64_is_valid (ret));
  return ret;
}
/*----------------------------------------------------------------------------*/
#define bl_memr64_rv_array(arr) bl_memr64_rv (arr, sizeof arr)
#define bl_memr64_beg_as(memr_val, type) ((type*) bl_memr64_beg (memr_val))
#define bl_memr64_end_as(memr_val, type) ((type*) bl_memr64_end (memr_val))
#define bl_memr64_at_as(memr_val, idx, type)\
  ((type*) bl_memr64_at ((memr_val), idx * sizeof (type)))
#if 1
  #define bl_memr64_cast(memr_val)\
    bl_memr64_rv ((memr_val).addr, (bl_u64) (memr_val).size)
#else
  #define bl_memr64_cast(memr_val)\
    bl_memr_rv(\
      (memr_val).addr,\
      (\
        bl_assert (((bl_u64) (memr_val).size) == (memr_val).size),\
        (bl_u64) (memr_val).size\
      )\
    )
#endif
#endif /*#if BL_WORDSIZE >= 64*/
/*----------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#if BL_WORDSIZE == 8
  typedef bl_memr8 bl_memr;
  #define bl_memr_concat(suffix) bl_pp_tokconcat (bl_memr8_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE == 16
  typedef bl_memr16 bl_memr;
  #define bl_memr_concat(suffix) bl_pp_tokconcat (bl_memr16_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE == 32
  typedef bl_memr32 bl_memr;
  #define bl_memr_concat(suffix) bl_pp_tokconcat (bl_memr32_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#if BL_WORDSIZE == 64
  typedef bl_memr64 bl_memr;
  #define bl_memr_concat(suffix) bl_pp_tokconcat (bl_memr64_, suffix)
#endif
/*----------------------------------------------------------------------------*/
#define bl_memr_beg(m)               bl_memr_concat (beg) (m)
#define bl_memr_size(m)              bl_memr_concat (size) (m)
#define bl_memr_end(m)               bl_memr_concat (end) (m)
#define bl_memr_resize(m, s)         bl_memr_concat (resize) ((m), (s))
#define bl_memr_at(m, i)             bl_memr_concat (at) ((m), (i))
#define bl_memr_null()               bl_memr_concat (null)
#define bl_memr_is_null(m)           bl_memr_concat (is_null) (m)
#define bl_memr_is_valid(m)          bl_memr_concat (is_valid) (m)
#define bl_memr_subrange_beg(m, o)   bl_memr_concat (subrange_beg) ((m), (o))
#define bl_memr_set(m, v)            bl_memr_concat (set) ((m), (v))
#define bl_memr_copy(d, s)           bl_memr_concat (copy) ((d), (s))
#define bl_memr_copy_offset(d, s, o)\
  bl_memr_concat (copy_offset) ((d), (s), (o))
#define bl_memr_rv(a, s)             bl_memr_concat (rv) ((a), (s))
#define bl_memr_rv_array(arr)        bl_memr_concat (rv_array) (arr)
#define bl_memr_beg_as(m, t)         bl_memr_concat (beg_as) ((m), t)
#define bl_memr_end_as(m, t)         bl_memr_concat (end_as) ((m), t)
#define bl_memr_at_as(m, i, t)       bl_memr_concat (at_as) ((m), (i), t)
#define bl_memr_cast(m)              bl_memr_concat (cast) (m)
/*----------------------------------------------------------------------------*/
#endif /* __BL_MEMORY_RANGE_H__ */
