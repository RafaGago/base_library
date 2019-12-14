#ifndef __BL_OS_H__
#define __BL_OS_H__

/* adapted from:

https://www.boost.org/doc/libs/1_72_0/boost/predef/os/

Copyright Rene Rivera
Distributed under the Boost Software License, Version 1.0.
*/

/*
defines BL_OS, BL_OS_NAME, BL_OS_VER and BL_OS_SUBVER

Normally, if available, the version is placed on BL_OS_VER. On the BSD variants
the BSD version is placed in "BL_OS_VER" and the BSD variant version is placed
on BL_OS_SUBVER

To check for an OS:
  #if BL_OS == BL_OS_BSD_OPEN

To check for a revision:
  #if BL_OS_VER > BL_VER_GET (2, 1, 0)

Or:
  #if BL_VER_MAJOR_GET (BL_OS_VER) == 2
*/

#include <bl/base/version_formats.h>

#define BL_OS_AIX           100
#define BL_OS_AMIGAOS       200
#define BL_OS_ANDROID       300
#define BL_OS_BEOS          400
#define BL_OS_BSD_NET       500
#define BL_OS_BSD_FREE      600
#define BL_OS_BSD_I         700
#define BL_OS_BSD_DRAGONFLY 800
#define BL_OS_BSD_OPEN      900
#define BL_OS_BSD_GENERIC   1000
#define BL_OS_CYGWIN        1100
#define BL_OS_HAIKU         1200
#define BL_OS_HPUX          1300
#define BL_OS_IOS           1400
#define BL_OS_IRIX          1500
#define BL_OS_QNX           1600
#define BL_OS_LINUX         1700
#define BL_OS_MACOS         1800
#define BL_OS_OS400         1900
#define BL_OS_SOLARIS       2000
#define BL_OS_VMS           2100
#define BL_OS_WINDOWS       2200

/* Missing: RTEMS, VxWorks*/

/* BSD aliases */
#define BL_OS_NETBSD       BL_OS_BSD_NET
#define BL_OS_FREEBSD      BL_OS_BSD_FREE
#define BL_OS_BSDI         BL_OS_BSD_I
#define BL_OS_DRAGONFLYBSD BL_OS_BSD_DRAGONFLY
#define BL_OS_OPENBSD      BL_OS_BSD_OPEN

/* convenience macros */
#define BL_OS_IS(os) (BL_OS == BL_OS_##os)

#define BL_OS_VER_IS_GE(major, minor, rev)\
   (BL_OS_VER >= BL_VER_GET ((major), (minor), (rev)))

#define BL_OS_SUBVER_IS_GE(major, minor, rev)\
   (BL_OS_SUBVER >= BL_VER_GET ((major), (minor), (rev)))

#define BL_OS_IS_BSD (BL_OS >= BL_OS_BSD_NET && BL_OS <= BL_OS_BSD_GENERIC)

/* needs versions and there are a lot of fine details, but this is good enough
for now*/
#define BL_OS_IS_MOSTLY_POSIX \
  (BL_OS_IS (AIX) || \
  BL_OS_IS (ANDROID) || \
  BL_OS_IS (BEOS) || \
  BL_OS_IS_BSD || \
  BL_OS_IS (HPUX) || \
  BL_OS_IS (IOS) || \
  BL_OS_IS (QNX) || \
  BL_OS_IS (LINUX) || \
  BL_OS_IS (MACOS) || \
  BL_OS_IS (SOLARIS))

/*qnx*/
#if !defined (BL_OS) && (defined (__QNX__) || defined (__QNXNTO__))
  #define BL_OS        BL_OS_QNX
  #define BL_OS_NAME   "QNX"
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE

  #if defined (_NTO_VERSION)
    #define BL_OS_VER BL_VER_FMT_10_VVRR (_NTO_VERSION)
  #elif defined (__QNX__)
    #define BL_OS_VER BL_VER_GET (4,0,0)
  #else
    #define BL_OS_VER BL_VER_UNAVAILABLE
  #endif
#endif

/* os400*/
#if !defined (BL_OS) && defined (__OS400__)
  #define BL_OS        BL_OS_OS400
  #define BL_OS_NAME   "IBM OS/400"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

/*macos (has to be done before BSD)*/
#if !defined (BL_OS) && ( \
    defined (macintosh) || defined (Macintosh) || \
    (defined (__APPLE__) && defined (__MACH__)) \
    )

  #define BL_OS        BL_OS_MACOS
  #define BL_OS_NAME   "Mac OS"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE

  #if defined (__APPLE__) && defined (__MACH__)
    #define BL_OS_VER BL_VER_GET (10,0,0)
  #else
    #define BL_OS_VER BL_VER_GET (9,0,0)
  #endif
#endif

/*bsdi*/
#if !defined (BL_OS) && defined (__bsdi__)
  #define BL_OS        BL_OS_BSD_I
  #define BL_OS_NAME   "BSDi BSD/OS"
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

/*freebsd*/
#if !defined (BL_OS) && defined (__FreeBSD__)
  #define BL_OS        BL_OS_BSD_FREE
  #define BL_OS_NAME   "FreeBSD"

  #include <sys/param.h>
  #if defined (__FreeBSD_version)
    #if __FreeBSD_version == 491000
      #define BL_OS_SUBVER BL_VER_GET (4, 10, 0)
    #elif __FreeBSD_version == 492000
      #define BL_OS_SUBVER BL_VER_GET (4, 11, 0)
    #elif __FreeBSD_version < 500000
      #define BL_OS_SUBVER BL_VER_FMT_10_VRPPPP (__FreeBSD_version)
    #else
      #define BL_OS_SUBVER BL_VER_FMT_10_VVRRPPP (__FreeBSD_version)
    #endif
  #else
    #define BL_OS_SUBVER BL_VER_UNAVAILABLE
  #endif
#endif

#if !defined (BL_OS) && defined (__DragonFly__)
  #define BL_OS        BL_OS_BSD_DRAGONFLY
  #define BL_OS_NAME   "DragonFly BSD"
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && defined (__OpenBSD__)

  #define BL_OS        BL_OS_BSD_OPEN
  #define BL_OS_NAME   "OpenBSD"

  #if defined (OpenBSD2_0)
    #define BL_OS_SUBVER BL_VER_GET (2,0,0)
  #elif defined (OpenBSD2_1)
    #define BL_OS_SUBVER BL_VER_GET (2,1,0)
  #elif defined (OpenBSD2_2)
    #define BL_OS_SUBVER BL_VER_GET (2,2,0)
  #elif defined (OpenBSD2_3)
    #define BL_OS_SUBVER BL_VER_GET (2,3,0)
  #elif defined (OpenBSD2_4)
    #define BL_OS_SUBVER BL_VER_GET (2,4,0)
  #elif defined (OpenBSD2_5)
    #define BL_OS_SUBVER BL_VER_GET (2,5,0)
  #elif defined (OpenBSD2_6)
    #define BL_OS_SUBVER BL_VER_GET (2,6,0)
  #elif defined (OpenBSD2_7)
    #define BL_OS_SUBVER BL_VER_GET (2,7,0)
  #elif defined (OpenBSD2_8)
    #define BL_OS_SUBVER BL_VER_GET (2,8,0)
  #elif defined (OpenBSD2_9)
    #define BL_OS_SUBVER BL_VER_GET (2,9,0)
  #elif defined (OpenBSD3_0)
    #define BL_OS_SUBVER BL_VER_GET (3,0,0)
  #elif defined (OpenBSD3_1)
    #define BL_OS_SUBVER BL_VER_GET (3,1,0)
  #elif defined (OpenBSD3_2)
    #define BL_OS_SUBVER BL_VER_GET (3,2,0)
  #elif defined (OpenBSD3_3)
    #define BL_OS_SUBVER BL_VER_GET (3,3,0)
  #elif defined (OpenBSD3_4)
    #define BL_OS_SUBVER BL_VER_GET (3,4,0)
  #elif defined (OpenBSD3_5)
    #define BL_OS_SUBVER BL_VER_GET (3,5,0)
  #elif defined (OpenBSD3_6)
    #define BL_OS_SUBVER BL_VER_GET (3,6,0)
  #elif defined (OpenBSD3_7)
    #define BL_OS_SUBVER BL_VER_GET (3,7,0)
  #elif defined (OpenBSD3_8)
    #define BL_OS_SUBVER BL_VER_GET (3,8,0)
  #elif defined (OpenBSD3_9)
    #define BL_OS_SUBVER BL_VER_GET (3,9,0)
  #elif defined (OpenBSD4_0)
    #define BL_OS_SUBVER BL_VER_GET (4,0,0)
  #elif defined (OpenBSD4_1)
    #define BL_OS_SUBVER BL_VER_GET (4,1,0)
  #elif defined (OpenBSD4_2)
    #define BL_OS_SUBVER BL_VER_GET (4,2,0)
  #elif defined (OpenBSD4_3)
    #define BL_OS_SUBVER BL_VER_GET (4,3,0)
  #elif defined (OpenBSD4_4)
    #define BL_OS_SUBVER BL_VER_GET (4,4,0)
  #elif defined (OpenBSD4_5)
    #define BL_OS_SUBVER BL_VER_GET (4,5,0)
  #elif defined (OpenBSD4_6)
    #define BL_OS_SUBVER BL_VER_GET (4,6,0)
  #elif defined (OpenBSD4_7)
    #define BL_OS_SUBVER BL_VER_GET (4,7,0)
  #elif defined (OpenBSD4_8)
    #define BL_OS_SUBVER BL_VER_GET (4,8,0)
  #elif defined (OpenBSD4_9)
    #define BL_OS_SUBVER BL_VER_GET (4,9,0)
  #elif defined (OpenBSD5_0)
    #define BL_OS_SUBVER BL_VER_GET (5,0,0)
  #elif defined (OpenBSD5_1)
    #define BL_OS_SUBVER BL_VER_GET (5,1,0)
  #elif defined (OpenBSD5_2)
    #define BL_OS_SUBVER BL_VER_GET (5,2,0)
  #elif defined (OpenBSD5_3)
    #define BL_OS_SUBVER BL_VER_GET (5,3,0)
  #elif defined (OpenBSD5_4)
    #define BL_OS_SUBVER BL_VER_GET (5,4,0)
  #elif defined (OpenBSD5_5)
    #define BL_OS_SUBVER BL_VER_GET (5,5,0)
  #elif defined (OpenBSD5_6)
    #define BL_OS_SUBVER BL_VER_GET (5,6,0)
  #elif defined (OpenBSD5_7)
    #define BL_OS_SUBVER BL_VER_GET (5,7,0)
  #elif defined (OpenBSD5_8)
    #define BL_OS_SUBVER BL_VER_GET (5,8,0)
  #elif defined (OpenBSD5_9)
    #define BL_OS_SUBVER BL_VER_GET (5,9,0)
  #elif defined (OpenBSD6_0)
    #define BL_OS_SUBVER BL_VER_GET (6,0,0)
  #elif defined (OpenBSD6_1)
    #define BL_OS_SUBVER BL_VER_GET (6,1,0)
  #elif defined (OpenBSD6_2)
    #define BL_OS_SUBVER BL_VER_GET (6,2,0)
  #elif defined (OpenBSD6_3)
    #define BL_OS_SUBVER BL_VER_GET (6,3,0)
  #elif defined (OpenBSD6_4)
    #define BL_OS_SUBVER BL_VER_GET (6,4,0)
  #elif defined (OpenBSD6_5)
    #define BL_OS_SUBVER BL_VER_GET (6,5,0)
  #elif defined (OpenBSD6_6)
    #define BL_OS_SUBVER BL_VER_GET (6,6,0)
  #elif defined (OpenBSD6_7)
    #define BL_OS_SUBVER BL_VER_GET (6,7,0)
  #elif defined (OpenBSD6_8)
    #define BL_OS_SUBVER BL_VER_GET (6,8,0)
  #elif defined (OpenBSD6_9)
    #define BL_OS_SUBVER BL_VER_GET (6,9,0)
  #else
    #define BL_OS_SUBVER BL_VER_UNAVAILABLE
  #endif
#endif

#if !defined (BL_OS) && (defined (__NETBSD__) || defined (__NetBSD__))
  #define BL_OS       BL_OS_BSD_NET
  #define BL_OS_NAME "NetBSD"

  #if defined (__NETBSD__)
    #if defined (__NETBSD_version)
      #if __NETBSD_version < 500000
        #define BL_OS_SUBVER BL_VER_FMT_10_VRP000 (__NETBSD_version)
      #else
        #define BL_OS_SUBVER BL_VER_FMT_10_VRR000 (__NETBSD_version)
      #endif
    #else
      #define BL_OS_SUBVER BL_VER_UNAVAILABLE
    #endif
  #elif defined (__NetBSD__)
    #if defined (NetBSD0_8)
      #define BL_OS_SUBVER BL_VER_GET (0,8,0)
    #elif defined (NetBSD0_9)
      #define BL_OS_SUBVER BL_VER_GET (0,9,0)
    #elif defined (NetBSD1_0)
      #define BL_OS_SUBVER BL_VER_GET (1,0,0)
    #elif defined (__NetBSD_Version)
      #define BL_OS_SUBVER BL_VER_FMT_10_VVRR00PP00 (__NetBSD_Version)
    #else
        #define BL_OS_SUBVER BL_VER_UNAVAILABLE
    #endif
  #endif
#endif

#if BL_OS_IS_BSD && (defined (BSD) || defined (_SYSTYPE_BSD))
  #include <sys/param.h>
  /* version of the BSD flavour is already detected on BL_OS_SUBVER */
  #if defined (BSD4_4)
    #define BL_OS_VER BL_VER_GET (4,4,0)
  #elif defined (BSD4_3)
    #define BL_OS_VER BL_VER_GET (4,3,0)
  #elif defined (BSD4_2)
    #define BL_OS_VER BL_VER_GET (4,2,0)
  #elif defined (BSD)
    #define BL_OS_VER BL_VER_FMT_10_VVRR (BSD)
  #else
    #define BL_OS_VER BL_VER_UNAVAILABLE
  #endif
#endif


#if !defined (BL_OS) && defined (__ANDROID__)
  #define BL_OS        BL_OS_ANDROID
  #define BL_OS_NAME   "Android"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if defined (unix) || defined (__unix) || \
    defined (_XOPEN_SOURCE) || defined (_POSIX_SOURCE)
  #define BL_OS_IS_UNIX 1
#else
  #define BL_OS_IS_UNIX 0
#endif

#if defined (__sysv__) || defined (__SVR4) || \
    defined (__svr4__) || defined (_SYSTYPE_SVR4)
  #define BL_OS_IS_SVR4 1
else
  #define BL_OS_IS_SVR4 0
#endif


#if !defined (BL_OS) && ( \
    defined (__APPLE__) && defined (__MACH__) && \
    defined (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) \
    )

  #define BL_OS        BL_OS_IOS
  #define BL_OS_NAME   "iOS"
  #define BL_OS_VER   (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ * 1000)
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && ( \
    defined (_WIN32) || defined (_WIN64) || \
    defined (__WIN32__) || defined (_WIN16) || defined (__TOS_WIN__) || \
    defined (__WINDOWS__) \
    )
  #define BL_OS        BL_OS_WINDOWS
  #define BL_OS_NAME   "Microsoft Windows"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && (defined (AMIGA) || defined (__amigaos__))
  #define BL_OS        BL_OS_AMIGAOS
  #define BL_OS_NAME   "AmigaOS"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && (defined (hpux) || defined (_hpux) || defined (__hpux))
  #define BL_OS        BL_OS_HPUX
  #define BL_OS_NAME   "HP-UX"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && (defined (sun) || defined (__sun))
  #define BL_OS        BL_OS_SOLARIS
  #define BL_OS_NAME   "Solaris"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && defined (__HAIKU__)
  #define BL_OS        BL_OS_HAIKU
  #define BL_OS_NAME   "Haiku"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && (defined (_AIX) || defined (__TOS_AIX__))
  #define BL_OS        BL_OS_AIX
  #define BL_OS_NAME   "IBM_AIX"
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE

  #if defined (_AIX43)
    #define BL_OS_VER BL_VER_GET (4,3,0)
  #elif defined (_AIX41)
    #define BL_OS_VER BL_VER_GET (4,1,0)
  #elif defined (_AIX32)
    #define BL_OS_VER BL_VER_GET (3,2,0)
  #elif defined (_AIX3)
    #define BL_OS_VER BL_VER_GET (3,0,0)
  #else
    #define BL_OS_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if !defined (BL_OS) && defined (__CYGWIN__)
  #include <cygwin/version.h>
  #define BL_OS        BL_OS_CYGWIN
  #define BL_OS_NAME   "Cygwin"
  #define BL_OS_VER \
    BL_VER_GET (CYGWIN_VERSION_API_MAJOR, CYGWIN_VERSION_API_MINOR, 0)
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && ( \
    defined (linux) || defined (__linux) || \
    defined (__linux__) || defined (__gnu_linux__) \
    )
  #define BL_OS        BL_OS_LINUX
  #define BL_OS_NAME   "Linux"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && (defined (sgi) || defined (__sgi))
  #define BL_OS        BL_OS_IRIX
  #define BL_OS_NAME   "IRIX"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS) && (defined (VMS) || defined (__VMS))
  #define BL_OS        BL_OS_VMS
  #define BL_OS_NAME   "VMS"
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE

  #if defined (__VMS_VER)
    #define BL_OS_VER BL_VER_FMT_10_VVRR00PP00 (__VMS_VER)
  #else
    #define BL_OS_VER BL_VER_UNAVAILABLE
  #endif
#endif

#if !defined (BL_OS) && (defined (__BEOS__))
  #define BL_OS        BL_OS_BEOS
  #define BL_OS_NAME   "BeOS"
  #define BL_OS_VER    BL_VER_UNAVAILABLE
  #define BL_OS_SUBVER BL_VER_UNAVAILABLE
#endif

#if !defined (BL_OS)
  #error "Unable to detect the OS"
#endif

#endif /* __BL_OS_H__ */
