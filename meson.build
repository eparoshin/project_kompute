project('your_project', 'cpp')

cmake = import('cmake')

kompute_subproj = cmake.subproject('kompute-0.9.0')

kompute_dep = kompute_subproj.dependency('kompute')

# Include the lib directory
subdir('lib')

# Include the bin directory and its dependencies
executable('kompute_exe', 'bin/main.cpp', dependencies : [lib_kompute_dep])

