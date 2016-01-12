Current status
==============
Under development. Not tested on Windows.

Description
===========
A project where I will place generic base utilities for C programming as 
needed. This is the result of splitting the utilities part of a project to
its own library so they can evolve independently. This also means that if e.g.
the thread library didn't need condition variables this library isn't having
them until they are needed.

I'm planning to make explicit on each "object" interface if memory allocations/
deallocations happen and where.

As of now everything is just programmed for Linux and should be compilable on 
Windows.

Now the library contains:

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

* Wrappers for atomics and threads (adding as needed).

* The beautiful and famous Dmitry Djukov's MPMC lockfree queue with some addons:
  1. Can be used as MPSC and SPMC.
  2. Uses the high part of the CAS'ed counter as a broadcast signaling mechanism
    that can communicate with producers (on MP mode) or consumers (on MC mode)
    almost for free.
   
Folders
=======
* "include/base_library/hdr":  Code usable as header-only.
* "include/base_library/lib":  May require compilation on some platforms.
* eclipse: Eclipse projects, unneeded to build. I sometimes use Eclipse as a gdb
front end.
* dependencies: External dependencies, as of now cmocka for the unit tests and
 premake5-alpha for building.
* build/premake: The build script.
* build/state: Place where the generated binaries are located after compilation.
* build/*platform*: Folder where premake generates the makefiles, VS solutions
, etc.
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

4. Go to the "build/linux".  Now you can use make for building. If you specify
"config=debug" yo will build debug binaries. Binaries are placed under
"build/stage".

Build on Windows
===============
TODO


