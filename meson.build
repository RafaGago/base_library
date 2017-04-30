# TODO: address sanitizer/valgrind

project(
    'base_library',
    'c',
    version: '0.0.1',
    license: 'BSD-3',
    default_options: [
        'c_std=gnu99',
#        'warning_level=1',
        'default_library=static'
        ]
    )

version_minor = '0'
version_major = '0'
version_rev   = '1'
version       = version_minor + '.' + version_major + '.' + version_rev

proj_dir          = meson.current_source_dir()
build_helpers_dir = proj_dir + '/build_helpers'

message ('building cmocka')
res = run_command(
    build_helpers_dir + '/build_cmake_project.sh',
    proj_dir,
    'cmocka',
    'libcmocka.a',
    '-DWITH_STATIC_LIB=on'
    )
if res.returncode() != 0
  error (res.stderr().strip())
endif

proj_dir = meson.current_source_dir()

include_dirs = [
    include_directories ('include'), include_directories ('src')
]
test_include_dirs  = include_dirs
test_include_dirs += [ include_directories ('test/src') ]
test_include_dirs += [ include_directories ('dep/install/include') ]
libcmocka_path     = proj_dir + '/dep/install/lib/libcmocka.a'

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

test_link_args = [ libcmocka_path ]

if host_system == 'windows'
    test_link_args += ['-lwinmm.lib'] # Untested
endif

threads = dependency ('threads')

cdata = configuration_data()
if get_option ('timestamp_64bit')
    cflags += [ '-DBL_TIMESTAMP_64BIT' ]
    cdata.set ('timestamp_64_placeholder', '#define BL_TIMESTAMP_64BIT')
endif

cdata.set ('version', version)
cdata.set ('version_major', version_major)
cdata.set ('version_minor', version_minor)
cdata.set ('version_rev', version_rev)

configure_file(
    input:         'config.h.in',
    output:        'config.h',
    configuration: cdata,
    install_dir:   'include/bl'
    )

#- base ------------------------------------------------------------------------

base_srcs = [
    'src/bl/base/semaphore_win.c',
    'src/bl/base/dynarray.c',
    'src/bl/base/semaphore_linux.c',
    'src/bl/base/autoarray.c',
    'src/bl/base/thread_micro_sleep.c',
    'src/bl/base/hex_string.c',
    'src/bl/base/timestamp_funcs_arbitrary_base.c',
    'src/bl/base/string.c',
    'src/bl/base/dynamic_string.c',
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
    'test/src/bl/base/integer_math_test.c',
    'test/src/bl/base/array_linked_list_test.c',
    'test/src/bl/base/flat_deadlines_test.c',
    'test/src/bl/base/tests_main.c',
    'test/src/bl/base/dynarray_test.c',
    'test/src/bl/base/dynamic_string_test.c',
    'test/src/bl/base/bl_string_test.c',
]
base_sem_stress_srcs = [
    'test/src/bl/semaphore_stress/semaphore_test.c',
]
base_lib = build_target(
    'bl-base',
    base_srcs,
    target_type         : libtype,
    include_directories : include_dirs,
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
        include_directories : test_include_dirs,
        link_with           : base_lib,
        link_args           : test_link_args,
        dependencies        : threads
    ))
executable(
    'bl-base-sem-stress',
    base_sem_stress_srcs,
    include_directories : test_include_dirs,
    link_with           : base_lib,
    dependencies        : threads
    )

#- nonblock --------------------------------------------------------------------

nonblock_srcs =[
    'src/bl/nonblock/mpmc_bt.c',
    'src/bl/nonblock/mpmc_b.c',
    'src/bl/nonblock/mpsc_i.c',
    'src/bl/nonblock/backoff.c',
    'src/bl/nonblock/mpmc_b_common.c',
]
nonblock_test_srcs =[
    'test/src/bl/nonblock/mpmc_bt_test.c',
    'test/src/bl/nonblock/mpmc_b_test.c',
    'test/src/bl/nonblock/mpsc_i_test.c',
    'test/src/bl/nonblock/tests_main.c',
]
nonblock_lib = build_target(
    'bl-nonblock',
    nonblock_srcs,
    target_type         : libtype,
    include_directories : include_dirs,
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
        include_directories : test_include_dirs,
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
taskqueue_stress_test_srcs =[
    'test/src/bl/task_queue_stress/task_queue_stress_test.c',
]
taskqueue_lib = build_target(
    'bl-taskqueue',
    taskqueue_srcs,
    target_type         : libtype,
    include_directories : include_dirs,
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
        include_directories : test_include_dirs,
        link_with           : [ base_lib, nonblock_lib, taskqueue_lib ],
        link_args           : test_link_args,
        dependencies        : threads
    ))
executable(
    'bl-taskqueue-stress',
    taskqueue_stress_test_srcs,
    include_directories : test_include_dirs,
    link_with           : [ base_lib, nonblock_lib ],
    dependencies        : threads
    )
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
    include_directories : include_dirs,
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
    'test/src/bl/getopt/getopt_lib.c',
    'test/src/bl/getopt/getopt_short_test.c',
    'test/src/bl/getopt/tests_main.c',
    'test/src/bl/getopt/getopt_long_test.c',
]
getopt_lib = build_target(
    'bl-getopt',
    getopt_srcs,
    target_type         : libtype,
    include_directories : include_dirs,
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
        include_directories : test_include_dirs,
        link_args           : test_link_args,
    ))
