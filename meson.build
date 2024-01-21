project('your_project', 'cpp')

cmake = import('cmake')

libkompute_subproj = cmake.subproject('kompute-0.9.0')

message(libkompute_subproj.target_list())

libkompute_dep = libkompute_subproj.dependency('kompute')
libkompute_dep_fmt = libkompute_subproj.dependency('fmt')
libkompute_dep_logger = libkompute_subproj.dependency('kp_logger')

# Include the lib directory
subdir('lib')

# Include the bin directory and its dependencies
executable('kompute_exe', 'bin/main.cpp', dependencies : [lib_compute_dep, libkompute_dep, libkompute_dep_fmt, libkompute_dep_logger])

