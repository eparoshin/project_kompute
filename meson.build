project('your_project', 'cpp')

cmake = import('cmake')

libkompute_subproj = cmake.subproject('kompute')

message(libkompute_subproj.target_list())

libkompute_dep = libkompute_subproj.dependency('kompute')
libkompute_dep_fmt = libkompute_subproj.dependency('fmt')
libkompute_dep_logger = libkompute_subproj.dependency('kp_logger')

subdir('lib')

test1 = executable('test1', 'test/test.cpp', dependencies : [lib_compute_dep, libkompute_dep, libkompute_dep_fmt, libkompute_dep_logger])
test2 = executable('test2', 'test/test_fill_plots.cpp', dependencies : [lib_compute_dep, libkompute_dep, libkompute_dep_fmt, libkompute_dep_logger])
test3 = executable('test3', 'test/test_fill_plots1.cpp', dependencies : [lib_compute_dep, libkompute_dep, libkompute_dep_fmt, libkompute_dep_logger])
test4 = executable('test4', 'test/test_fill_plots_user_input.cpp', dependencies : [lib_compute_dep, libkompute_dep, libkompute_dep_fmt, libkompute_dep_logger])

test('test1', test1)
test('test2', test2)
test('test3', test3)
benchmark('test3', test3)
