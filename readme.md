Description
===========

A project where I will place generic base utilities for C programming on an
"as needed" basis. The idea is that everything should be very pretty simple,
statically linkable by modules and compilable with GCC and Visual Studio.

Some Visual Sudio versions have no C99-C11 support, so sometimes I may fall back
to C++ to emulate some C99-C11 features.

Now the library is divided in some very tiny sublibraries to avoid bloating. The
intention is to don't make a big monolithic library for these basic things and
to allow the linker to easily remove unused sections.

1. (base) Base library
----------------------

Depends on: -

This is the basic utilities library, mostly header-based and heavily relying on
"static inline" and macros. It contains:

* Platform detection (very primitive, will get bigger).

* Static integer math.

* Integer math.

* Timestamp wrappers.

* Humble fixed-size basic data structures (mostly for 16-bit microcontrollers)
  as a ringbuffer, ordered ringbuffer and array based linked lists (to save size
  on pointers)

* The famous BSD data structures (lists and trees).

* A futex + atomics timed semaphore based on the Monotonic clock for Linux
  (POSIX doesn't provide this).

* Wrappers for atomics.

* Wrappers for thread manipulation. As of now just thread creation and thread
  local storage.

* A naive dynamic string accepting printf format setters.

It depends on the platform which functions are or aren't "static inline", so it
might happen that in some platform some things compile as header only and in
another one it does need some definitions on .c files. It's recommended to use
this library as a static library on all platforms.

2. (noblock) Non (OS) blocking queues
-------------------------------------

Depends on: base

Some non blocking queues (in the sense that never aquire OS locks like mutexes,
semaphores, etc), the algorithms may not be lock-free.

* The beautiful and famous Dmitry Djukov's MPMC lockfree queue with some addons:
  1. It's broken in prepare and commit actions, so you can get direct access to
     the internal array memory to avoid memory copying.
  2. Can be used as MPMC, MPSC, SPMC and SPSC.
  3. Uses some bits of the CAS counters as a broadcast signaling/tagging
     mechanism that can communicate with producers (on MP mode) or consumers
     (on MC mode).
  3. Allows a fetch-and-add based produce (instead of CAS) that can be used when
     it's known that a producer will block on the queue when it's full. This has
     the desirable side-effect of having fair FIFO order when many producers are
     contending.
  4. Allows blocking the producer access to the queue. Useful for termination
     scenarios.
  5. Provides a wrapper with a simpler interface.

* A custom modification of the MSMC queue which is suitable for variable-length
  element storage:
  1. It's broken in prepare and commit actions, so you can get direct access to
     the internal array memory to avoid memory copying.
  2. Allows variable-size producers only using the same underlying array (the
     algorithm is as cache-friendly as the original one).
  3. Uses some bits of the CAS counters as a broadcast signaling/tagging
     mechanism that can communicate with producers (on MP mode) or consumers
     (on MC mode).

* The equally beautiful and famous Dmitry Djukov's MPSC intrusive queue with
  some addons:
  1. Multiple node single push.
  2. Tagged pointers.

3. (task_queue) MPSC task queue with delayed/scheduled tasks
------------------------------------------------------------

Depends on: base, nonblock

A queue for worker/event dispatcher threads. Based on wrapping timepoints orde-
red in an array based associative container, some lockfree queues and a
semaphore.

4. (serial) Serial port thin wrapper
-------------------------------------

Depends on: base

5. (getopt)
-----------

Depends on: base headers.

A non-standard implementation of "getopt" that removes the global variables and
avoids some segfaults found in some (major) POSIX implementations.

6. (time-extras) Additional time functions
------------------------------------------

Additional time functions that make no sense as header-only because they require
logic and state. Only enabled when the "timepoint_64bit" meson option is true.
It adds:

- Conversion from monotonic to calendar time.
- A CPU specific high performance monotonic clock implementation.

Depends on: base.

Current status
==============

Under development.

-Linux:

 All the tests pass on Linux.

 Serial is untested.

-Windows:

 Buildsystem moved to meson, untested.

 Serial is untested.

 Getopt is untested (but it contanins no platform specific code...)

 time_extras is untested

Credits and Used resources
==========================

* Dmitry Djukov for making its queue algorithms public.
  [http://www.1024cores.net/]
* William Woodall and John Harrison for their serial code at
  [https://github.com/wjwwood/serial]
* Thomas Nixon, Jonathan Heathcote for the macro hackery at
  [https://github.com/18sg/uSHET]
* Kim Gräsman for his getopt port. [https://github.com/kimgr/getopt_port]
* My former employer Diadrom AB. Some parts of this library were written while
  working there.

Build on Linux
=================

To install "meson" on debian based systems:

> sudo apt install python3 python3-pip ninja-build cmake
> pip3 install --user meson
> git submodule update --init --recursive

To install to a intermediate directory

> mkdir ninja_build
> meson ninja_build --prefix=/ --buildtype=release
> ninja -C ninja_build
> ninja -C ninja_build test
> DESTDIR=$(pwd)/install ninja -C ninja_build install

To install on your system directories

> mkdir ninja_build
> meson ninja_build --buildtype=release
> ninja -C ninja_build
> ninja -C ninja_build test
> sudo ninja -C ninja_build install

Build on Windows
===============

If you are planning to run the unit tests you need CMake, as cmocka uses it
as its build system.

2. TODO (meson untested on Windows)
