project(
    'base_library',
    'c',
    'cpp', # only for relacy
    version: '0.1.0',
    license: 'BSD-3',
    default_options: [
        'c_std=gnu99',
        'cpp_std=gnu++11',
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

proj_dir = meson.current_source_dir()

include_dirs = [
    include_directories ('include'), include_directories ('src')
]
test_include_dirs  = include_dirs
test_include_dirs += [ include_directories ('test/src') ]

pkg_mod = import ('pkgconfig')
install_subdir ('include/bl', install_dir : 'include')

relacy_path = 'gitmodules/relacy'

cflags = [ '-D_GNU_SOURCE' ]
lib_cflags = []
test_cflags = []

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

if not get_option ('bare')
    cmocka = subproject('cmocka')
    cmocka_dep = cmocka.get_variable ('dep_cmocka')
endif

test_link_args = []
host_system = host_machine.system()
if host_system == 'windows'
    test_link_args += ['-lwinmm.lib'] # Untested
endif

threads = dependency ('threads')

cdata = configuration_data()
if get_option ('timepoint_64bit')
    cflags += [ '-DBL_TIMEPOINT_BITS=64' ]
    cdata.set ('timepoint_placeholder', '#define BL_TIMEPOINT_BITS 64')
else
    cflags += [ '-DBL_TIMEPOINT_BITS=32' ]
    cdata.set ('timepoint_placeholder', '#define BL_TIMEPOINT_BITS 32')
endif

if host_system == 'linux'
    cflags += [ '-DBL_USE_CLOCK_MONOTONIC_RAW' ]
    cdata.set ('clock_placeholder', '#define BL_USE_CLOCK_MONOTONIC_RAW')
else
    cdata.set ('clock_placeholder', '/* #define BL_USE_CLOCK_MONOTONIC_RAW */')
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
    'src/bl/base/dynarray_from_file.c',
    'src/bl/base/semaphore_linux.c',
    'src/bl/base/autoarray.c',
    'src/bl/base/thread_micro_sleep.c',
    'src/bl/base/hex_string.c',
    'src/bl/base/string.c',
    'src/bl/base/dynamic_string.c',
    'src/bl/base/dynamic_string_replace.c',
    'src/bl/base/ringbuffer_basic.c',
    'src/bl/base/ringbuffer_extended.c',
    'src/bl/base/ordered_ringbuffer.c',
    'src/bl/base/flat_deadlines.c',
    'src/bl/base/error.c',
    'src/bl/base/generated/alnls/array_linked_list_u8_basic.c',
    'src/bl/base/generated/alnls/array_linked_list_u8_extended.c',
    'src/bl/base/generated/alnls/array_linked_list_u16_basic.c',
    'src/bl/base/generated/alnls/array_linked_list_u16_extended.c',
    'src/bl/base/generated/alnls/array_linked_list_u32_basic.c',
    'src/bl/base/generated/alnls/array_linked_list_u32_extended.c',
    'src/bl/base/generated/alnls/array_linked_list_u64_basic.c',
    'src/bl/base/generated/alnls/array_linked_list_u64_extended.c',
    'src/bl/base/generated/adlnls/array_doubly_linked_list_u8_basic.c',
    'src/bl/base/generated/adlnls/array_doubly_linked_list_u16_basic.c',
    'src/bl/base/generated/adlnls/array_doubly_linked_list_u32_basic.c',
    'src/bl/base/generated/adlnls/array_doubly_linked_list_u64_basic.c',
    'src/bl/base/generated/time/timept32_funcs_arbitrary_base.c',
    'src/bl/base/generated/time/timept64_funcs_arbitrary_base.c',
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
    'test/src/bl/base/array_concat_dynarray_test.c',
]
base_sem_stress_srcs = [
    'test/src/bl/semaphore_stress/semaphore_test.c',
]
base_lib = build_target(
    'bl-base',
    base_srcs,
    target_type         : libtype,
    include_directories : include_dirs,
    c_args              : cflags + lib_cflags + ['-DBL_COMPILATION'],
    install             : true
    )
pkg_mod.generate(
    base_lib,
    version     : '0.0.1',
    name        : 'libbl-base',
    filebase    : 'bl-base',
    description : 'bl base library commons'
    )
if not get_option ('bare')
    test(
        'bl-base',
        executable(
            'bl-base-test',
            base_test_srcs,
            c_args              : cflags + test_cflags + ['-DBL_COMPILATION'],
            include_directories : test_include_dirs,
            link_with           : base_lib,
            link_args           : test_link_args,
            dependencies        : [threads, cmocka_dep]
        ))
    executable(
        'bl-base-sem-stress',
        base_sem_stress_srcs,
        c_args              : cflags,
        include_directories : test_include_dirs,
        link_with           : base_lib,
        dependencies        : threads
        )
endif

#- nonblock --------------------------------------------------------------------

nonblock_srcs =[
    'src/bl/nonblock/mpmc_bt.c',
    'src/bl/nonblock/mpmc_b.c',
    'src/bl/nonblock/mpmc_bpm.c',
    'src/bl/nonblock/mpsc_i.c',
    'src/bl/nonblock/backoff.c',
    'src/bl/nonblock/mpmc_b_common.c',
]
nonblock_test_srcs =[
    'test/src/bl/nonblock/mpmc_bt_test.c',
    'test/src/bl/nonblock/mpmc_b_test.c',
    'test/src/bl/nonblock/mpmc_bpm_test.c',
    'test/src/bl/nonblock/mpsc_i_test.c',
    'test/src/bl/nonblock/tests_main.c',
]
nonblock_mpmc_bpm_relacy_srcs =[
    'test/src/bl/mpmc_bpm_relacy/mpmc_bpm_relacy.cpp',
]
nonblock_lib = build_target(
    'bl-nonblock',
    nonblock_srcs,
    target_type         : libtype,
    include_directories : include_dirs,
    c_args              : cflags + lib_cflags + ['-DBL_COMPILATION'],
    install             : true
    )
pkg_mod.generate(
    nonblock_lib,
    version     : '0.0.1',
    name        : 'libbl-nonblock',
    filebase    : 'bl-nonblock',
    description : 'bl multithreaded data queues'
    )
if not get_option ('bare')
    test(
        'bl-nonblock',
        executable(
            'bl-nonblock-test',
            nonblock_test_srcs,
            include_directories : test_include_dirs,
            c_args              : cflags + test_cflags + ['-DBL_COMPILATION'],
            link_with           : [ base_lib, nonblock_lib] ,
            link_args           : test_link_args,
            dependencies        : [threads, cmocka_dep]
        ))
    executable(
        'bl-nonblock-mpmc-bpm-relacy',
        nonblock_mpmc_bpm_relacy_srcs,
        include_directories : test_include_dirs +
            [include_directories (relacy_path)],
    #    cpp_args            : [ '-DBL_NONBLOCK_MPMC_ONLY_TEST_ALGORITHM' ],
        dependencies        : threads
        )
endif

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
    c_args              : cflags + lib_cflags + ['-DBL_COMPILATION'],
    install             : true
    )
pkg_mod.generate(
    taskqueue_lib,
    version     : '0.0.1',
    name        : 'libbl-taskqueue',
    filebase    : 'bl-taskqueue',
    description : 'bl task queue'
    )
if not get_option ('bare')
    test(
        'bl-taskqueue-test',
        executable(
            'bl-taskqueue-test',
            taskqueue_test_srcs,
            include_directories : test_include_dirs,
            c_args              : cflags + test_cflags + ['-DBL_COMPILATION'],
            link_with           : [ base_lib, nonblock_lib, taskqueue_lib ],
            link_args           : test_link_args,
            dependencies        : [threads, cmocka_dep]
        ))
    tqs = executable(
        'bl-taskqueue-stress',
        taskqueue_stress_test_srcs,
        include_directories : test_include_dirs,
        c_args              : cflags + ['-DBL_COMPILATION'],
        link_with           : [ base_lib, nonblock_lib ],
        dependencies        : threads
        )
    test ('taskqueue-stress', tqs, args : [ '2' ])
endif

# - serial ---------------------------------------------------------------------

serial_srcs = [
    'src/bl/serial/serial_posix.c',
    'src/bl/serial/serial_win.c',
]
serial_lib = build_target(
    'bl-serial',
    serial_srcs,
    target_type         : libtype,
    c_args              : cflags + lib_cflags + ['-DBL_COMPILATION'],
    include_directories : include_dirs,
    install             : true
    )
pkg_mod.generate(
    serial_lib,
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
    c_args              : cflags + lib_cflags + ['-DBL_COMPILATION'],
    install             : true
    )
pkg_mod.generate(
    getopt_lib,
    version     : '0.0.1',
    name        : 'libbl-getopt',
    filebase    : 'bl-getopt',
    description : 'getopt multiplatform implementation with no globals'
    )
if not get_option ('bare')
    test(
        'bl-getopt-test',
        executable(
            'bl-getopt-test',
            getopt_test_srcs,
            c_args              : cflags + ['-DBL_COMPILATION'],
            include_directories : test_include_dirs,
            link_args           : test_link_args,
            dependencies        : [cmocka_dep]
        ))
endif

# - time_extras-----------------------------------------------------------------
if get_option ('timepoint_64bit')
    t_extras_srcs = [
        'src/bl/time_extras/time_extras.c',
        'src/bl/time_extras/generated/cpu_timept_funcs_arbitrary_base.c',
    ]
    t_extras_test_srcs = [
        'test/src/bl/time_extras/time_extras_test.c',
        'test/src/bl/time_extras/tests_main.c',
    ]
    t_extras_lib = build_target(
        'bl-time-extras',
        t_extras_srcs,
        target_type         : libtype,
        include_directories : include_dirs,
        c_args              : cflags + lib_cflags + ['-DBL_COMPILATION'],
        install             : true
        )
    pkg_mod.generate(
        t_extras_lib,
        version     : '0.0.1',
        name        : 'libbl-time-extras',
        filebase    : 'bl-time-extras',
        description : 'additional time functions'
        )
    if not get_option ('bare')
        test(
            'bl-time-extras-test',
            executable(
                'bl-t_extras-test',
                t_extras_test_srcs,
                c_args              : cflags + ['-DBL_COMPILATION'],
                include_directories : test_include_dirs,
                link_with           : [ base_lib , t_extras_lib ] ,
                link_args           : test_link_args,
                dependencies        : [cmocka_dep]
            ))
    endif
endif
