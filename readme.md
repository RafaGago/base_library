Description
===========

A project where I will place generic base utilities for C programming on an
"as needed" basis. The idea is that everything should be very simple and 
statically linkable.

Now the library is divided in some very tiny sublibraries to avoid bloating.

1. (base) Base library
----------------------

Depends on: -

This is the basic utilities libraries, it contains:

* Platform detection (very primitive, will get bigger).

* Static integer math.

* Integer math.

* Timestamp wrappers.

* Humble fixed-size basic data structures (mostly for microcontrollers) as a 
 ringbuffer, ordered ringbuffer and array based linked lists
 (to save size on pointers)

* The famous BSD data structures (lists and trees).

* A futex + atomics semaphore based on the Monotonic clock for Linux (POSIX
  doesn't provide this).

* Wrappers for atomics.

* Wrappers for thread manipulation (adding as needed). As of now mutexes and
  condition variables aren't wrapped (I personally try to code multithreaded 
  applications either by message passing (through lock-free queues) or by 
  using cooperative scheduling).

Everything tries to be very lightweight and comprised of "static inline" func-
tions and generating macros, but unfortunately on some instances this isn't
possible.

It depends on the platform which functions aren't "static inline". It is 
recommended to use this library as a static library.

2. (noblock) Non (OS) blocking queues 
-------------------------------------

Depends on: base

Some non blocking queues (in the sense that never aquire OS locks (mutexes,
semaphores ...)

* The beautiful and famous Dmitry Djukov's MPMC lockfree queue with some addons:
  1. Can be used as MPSC and SPMC.
  2. Uses the high part of the CAS'ed counter as a broadcast signaling mechanism
    that can communicate with producers (on MP mode) or consumers (on MC mode)
    almost for free.


3. (task_queue) MPSC task queue with delayed/scheduled tasks
------------------------------------------------------------

Depends on: base, noblock

A queue for worker/event dispatcher threads.

4. (serial) Serial port thin wrapper
-------------------------------------

Depends on: base

Current status
==============

Under development.

-Linux: 

 All the tests pass on Linux. 
 
 Serial is untested.

-Windows: 

 All the tests pass on Windows. 
 
 Serial is untested.
 
Compilers
=================

-Visual Studio 2013
-GCC 4.8.1
   
Folders
=======

* eclipse: Eclipse projects, unneeded to build. I sometimes use Eclipse as a gdb
front end.
* dependencies: External dependencies, as of now cmocka for the unit tests and
 premake5-alpha for building.
* build/premake: The build script.
* build/state: Place where the generated binaries are located after compilation.
* build/*platform*: Folder where premake generates the makefiles, VS solutions
, etc.
* "include": -
* "test": -
* "src": -

Build on Linux (GCC)
=================

1. If you are planning to run the unit tests you need CMake, as cmocka uses it
as its build system. It's mostly sure that every distro has a package for it.

2. Go to the "dependencies" folder and run the "prepare_dependencies.sh" script.
Check that everything builds.

3. Go to the "build/premake" folder and execute "./premake5 gmake".
This generates the makefile.

4. The makefile is now on the "build/linux" folder. If you specify
"config=debug" yo will build debug binaries. Binaries are placed under
"build/stage/linux".

Build on Windows
===============

1. If you are planning to run the unit tests you need CMake, as cmocka uses it
as its build system.

2. Go to the "dependencies" folder and follow the instructions on the
"prepare_dependencies_windows.txt" document.

3. Go to the "build/premake" folder and execute "./premake5 vs2013".
This generates the visual studio solution on "build/premake/windows".

4. You can build either through the solution or through the
"build/premake/build_vs2013.bat" script. Binaries are placed under
"build/stage/windows".
