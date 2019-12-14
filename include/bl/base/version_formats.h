#ifndef _BL_VERSION_FORMATS_H_
#define _BL_VERSION_FORMATS_H_
/* adapted from:

https://www.boost.org/doc/libs/1_72_0/boost/predef/make.h/

Copyright Rene Rivera
Distributed under the Boost Software License, Version 1.0.

These set of macros decompose common vendor version number
macros which are composed version, revision, and patch digits.
The naming convention indicates:

* The base of the specified version number. "`BL_VER_FMT_0X`" for
  hexadecimal digits, and "`BL_VER_FMT_10`" for decimal digits.
* The format of the vendor version number. Where "`V`" indicates the version
  digits,"`R`" indicates the revision digits, "`P`" indicates the patch digits,
  and "`0`" indicates an ignored digit.
*/

#define BL_VER_GET(major, minor, rev) \
  ((((major) % 100) * 10000000) + \
   (((minor) % 100) * 100000) + \
   ((rev) % 100000) )

#define BL_VER_MAJOR_GET(bl_ver) (((bl_ver) / 10000000) % 100)
#define BL_VER_MINOR_GET(bl_ver) (((bl_ver) / 100000) % 100 )
#define BL_VER_REV_GET(bl_ver) ((bl_ver) % 100000)
#define BL_VER_UNAVAILABLE BL_VER_GET (0, 0, 0)

#define BL_VER_FMT_0X_VRP(v) \
  BL_VER_GET ((v & 0xF00) >> 8, (v & 0xF0) >> 4, (v & 0xF))

#define BL_VER_FMT_0X_VVRP(v) \
  BL_VER_GET ((v & 0xFF00) >> 8, (v & 0xF0) >> 4, (v & 0xF))

#define BL_VER_FMT_0X_VRPP(v) \
  BL_VER_GET ((v & 0xF000) >> 12, (v & 0xF00) >> 8, (v & 0xFF))

#define BL_VER_FMT_0X_VVRR(v) \
  BL_VER_GET ((v & 0xFF00) >> 8, (v & 0xFF), 0)

#define BL_VER_FMT_0X_VRRPPPP(v) \
  BL_VER_GET ((v & 0xF000000) >> 24, (v & 0xFF0000) >> 16, (v & 0xFFFF))

#define BL_VER_FMT_0X_VVRRP(v) \
  BL_VER_GET ((v & 0xFF000) >> 12, (v & 0xFF0) >> 4, (v & 0xF))

#define BL_VER_FMT_0X_VRRPP000(v) \
  BL_VER_GET( \
    (v & 0xF0000000) >> 28, (v & 0xFF00000) >> 20, (v & 0xFF000) >> 12 \
    )
#define BL_VER_FMT_0X_VVRRPP(v) \
  BL_VER_GET ((v & 0xFF0000) >> 16, (v & 0xFF00) >> 8, (v & 0xFF))

#define BL_VER_FMT_10_VPPP(v) \
  BL_VER_GET (((v) / 1000) % 10, 0, (v) % 1000)

#define BL_VER_FMT_10_VR0(v) \
  BL_VER_GET (((v) / 100) % 10, ((v) / 10) % 10, 0)

#define BL_VER_FMT_10_VRP(v) \
  BL_VER_GET (((v) / 100) % 10, ((v) / 10) % 10, (v) % 10)

#define BL_VER_FMT_10_VRP000(v) \
  BL_VER_GET (((v) / 100000) % 10, ((v) / 10000) % 10, ((v) / 1000) % 10)

#define BL_VER_FMT_10_VRPPPP(v) \
  BL_VER_GET (((v) / 100000) % 10, ((v) / 10000) % 10, (v) % 10000)

#define BL_VER_FMT_10_VRPP(v) \
  BL_VER_GET (((v) / 1000) % 10, ((v) / 100) % 10, (v) % 100)

#define BL_VER_FMT_10_VRR(v) \
  BL_VER_GET (((v) / 100) % 10, (v) % 100, 0)

#define BL_VER_FMT_10_VRRPP(v) \
  BL_VER_GET (((v) / 10000) % 10, ((v) / 100) % 100, (v) % 100)

#define BL_VER_FMT_10_VRR000(v) \
  BL_VER_GET (((v) / 100000) % 10, ((v) / 1000) % 100, 0)

#define BL_VER_FMT_10_VV00(v) \
  BL_VER_GET (((v) / 100) % 100, 0, 0)

#define BL_VER_FMT_10_VVRR(v) \
  BL_VER_GET (((v) / 100) % 100, (v) % 100, 0)

#define BL_VER_FMT_10_VVRRP(v) \
  BL_VER_GET (((v) / 1000) % 100, ((v) / 10) % 100, (v) % 10)

#define BL_VER_FMT_10_VVRRPP(v) \
  BL_VER_GET (((v) / 10000) % 100, ((v) / 100) % 100, (v) % 100)

#define BL_VER_FMT_10_VVRRPPP(v) \
  BL_VER_GET (((v) / 100000) % 100, ((v) / 1000) % 100, (v) % 1000)

#define BL_VER_FMT_10_VVRR0PP00(v) \
  BL_VER_GET (((v) / 10000000) % 100, ((v) / 100000) % 100, ((v) / 100) % 100)

#define BL_VER_FMT_10_VVRR0PPPP(v) \
  BL_VER_GET (((v) / 10000000) % 100, ((v) / 100000) % 100, (v) % 10000)

#define BL_VER_FMT_10_VVRR00PP00(v) \
  BL_VER_GET (((v) / 100000000) % 100, ((v) / 1000000) % 100, ((v) / 100) % 100)

#endif /* _BL_VERSION_FORMATS_H_ */