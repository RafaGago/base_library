# See https://github.com/anholt/libepoxy/blob/master/meson.build
# TODO: version, tests, valgrind, visibility, config generation

project(
    'base_library',
    'c',
    version : '0.0.1',
    license : 'BSD-3',
    default_options : [
        'c_std=gnu99',
#        'warning_level=1',
        'default_library=static'
        ]
    )

run_command(
    'git', 'submodule', 'update', '--init', '--recursive'
    )

message ('building cmocka')
res = run_command(
    'build_helpers/build_cmake_project.sh',
    'cmocka',
    'libcmocka.a',
    '-DWITH_STATIC_LIB=on'
    )
if res.returncode() != 0
  error (res.stderr().strip())
endif

currdir = meson.current_source_dir()

includedirs = [
    include_directories ('include'), include_directories ('src')
]
testincludedirs = includedirs
testincludedirs += [ include_directories ('test/src') ]
testincludedirs += [ include_directories ('dep/install/include') ]
cmockalib_abspath = currdir + '/dep/install/lib/libcmocka.a'

pkg_mod = import ('pkgconfig')
install_subdir ('include/bl', install_dir : 'include')

cflags = []
lib_cflags = []
test_cflags = []
host_system = host_machine.system()
if host_system == 'linux'
    cflags += [ '-DBL_USE_CLOCK_MONOTONIC_RAW' ]
endif

if get_option ('sharedlib')
    libtype = 'shared_library'
else
    libtype = 'static_library'
endif

cc = meson.get_compiler ('c')
if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
    cflags += ['-fvisibility=hidden', '-fno-stack-protector']
    if get_option ('pic_statlibs') and libtype == 'static_library'
        lib_cflags += ['-fPIC']
    endif
endif

test_link_args = [ cmockalib_abspath ]

if host_system == 'windows'
    test_link_args += ['-lwinmm.lib'] # Untested
endif

threads = dependency ('threads')

#- base ------------------------------------------------------------------------

base_srcs = [
    'src/bl/base/semaphore_win.c',
    'src/bl/base/dynarray.c',
    'src/bl/base/semaphore_linux.c',
    'src/bl/base/autoarray.c',
    'src/bl/base/thread_micro_sleep.c',
    'src/bl/base/hex_string.c',
    'src/bl/base/timestamp_funcs_arbitrary_base.c',
]
base_test_srcs = [
    'test/src/bl/base/hex_string_test.c',
    'test/src/bl/base/ringbuffer_test.c',
    'test/src/bl/base/time_test.c',
    'test/src/bl/base/array_doubly_linked_list_test.c',
    'test/src/bl/base/semaphore_not_totally_broken_test.c',
    'test/src/bl/base/autoarray_test.c',
    'test/src/bl/base/ordered_ringbuffer_test.c',
    'test/src/bl/base/integer_manipulation_test.c',
    'test/src/bl/base/array_linked_list_test.c',
    'test/src/bl/base/flat_deadlines_test.c',
    'test/src/bl/base/tests_main.c',
    'test/src/bl/base/dynarray_test.c',
]
base_lib = build_target(
    'bl-base',
    base_srcs,
    target_type         : libtype,
    include_directories : includedirs,
    c_args              : cflags + lib_cflags,
    install             : true
    )
pkg_mod.generate(
    libraries   : base_lib,
    version     : '0.0.1',
    name        : 'libbl-base',
    filebase    : 'bl-base',
    description : 'bl base library commons'
    )
test(
    'bl-base',
    executable(
        'bl-base-test',
        base_test_srcs,
        include_directories : testincludedirs,
        link_with           : base_lib,
        link_args           : test_link_args,
        dependencies        : threads
    ))

#- nonblock --------------------------------------------------------------------

nonblock_srcs =[
    'src/bl/nonblock/mpmc_bt.c',
    'src/bl/nonblock/mpmc_b.c',
    'src/bl/nonblock/backoff.c',
    'src/bl/nonblock/mpmc_b_common.c',
]
nonblock_test_srcs =[
    'test/src/bl/nonblock/mpmc_bt_test.c',
    'test/src/bl/nonblock/mpmc_b_test.c',
    'test/src/bl/nonblock/tests_main.c',
]
nonblock_lib = build_target(
    'bl-nonblock',
    nonblock_srcs,
    target_type         : libtype,
    include_directories : includedirs,
    c_args              : cflags + lib_cflags,
    install             : true
    )
pkg_mod.generate(
    libraries   : nonblock_lib,
    version     : '0.0.1',
    name        : 'libbl-nonblock',
    filebase    : 'bl-nonblock',
    description : 'bl multithreaded data queues'
    )
test(
    'bl-nonblock',
    executable(
        'bl-nonblock-test',
        nonblock_test_srcs,
        include_directories : testincludedirs,
        link_with           : [ base_lib, nonblock_lib] ,
        link_args           : test_link_args,
        dependencies        : threads
    ))

# - taskqueue ------------------------------------------------------------------

taskqueue_srcs =[
    'src/bl/task_queue/delayed.c',
    'src/bl/task_queue/task_queue.c',
]
taskqueue_test_srcs =[
    'test/src/bl/task_queue/task_queue_not_totally_broken_test.c',
    'test/src/bl/task_queue/tests_main.c',
]
taskqueue_lib = build_target(
    'bl-taskqueue',
    taskqueue_srcs,
    target_type         : libtype,
    include_directories : includedirs,
    c_args              : cflags + lib_cflags,
    install             : true
    )
pkg_mod.generate(
    libraries   : taskqueue_lib,
    version     : '0.0.1',
    name        : 'libbl-taskqueue',
    filebase    : 'bl-taskqueue',
    description : 'bl task queue'
    )
test(
    'bl-taskqueue-test',
    executable(
        'bl-taskqueue-test',
        taskqueue_test_srcs,
        include_directories : testincludedirs,
        link_with           : [ base_lib, nonblock_lib, taskqueue_lib ],
        link_args           : test_link_args,
        dependencies        : threads
    ))

# - serial ---------------------------------------------------------------------

serial_srcs = [
    'src/bl/serial/serial_posix.c',
    'src/bl/serial/serial_win.c',
]
serial_lib = build_target(
    'bl-serial',
    serial_srcs,
    target_type         : libtype,
    c_args              : cflags + lib_cflags,
    include_directories : includedirs,
    install             : true
    )
pkg_mod.generate(
    libraries   : serial_lib,
    version     : '0.0.1',
    name        : 'libbl-serial',
    filebase    : 'bl-serial',
    description : 'bl serial port wrapper'
    )

# - getopt ---------------------------------------------------------------------

getopt_srcs = [
    'src/bl/getopt/getopt.c',
]
getopt_test_srcs = [
    'test/src/bl/getopt/getopt.c',
    'test/src/bl/getopt/getopt_short_test.c',
    'test/src/bl/getopt/tests_main.c',
    'test/src/bl/getopt/getopt_long_test.c',
]
getopt_lib = build_target(
    'bl-getopt',
    getopt_srcs,
    target_type         : libtype,
    include_directories : includedirs,
    c_args              : cflags + lib_cflags,
    install             : true
    )
pkg_mod.generate(
    libraries   : getopt_lib,
    version     : '0.0.1',
    name        : 'libbl-getopt',
    filebase    : 'bl-getopt',
    description : 'getopt multiplatform implementation with no globals'
    )
test(
    'bl-getopt-test',
    executable(
        'bl-getopt-test',
        getopt_test_srcs,
        include_directories : testincludedirs,
        link_args           : test_link_args,
    ))