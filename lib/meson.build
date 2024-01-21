lib_compute_sources = files([
    'kompute_device.cpp',
])

lib_compute = static_library('lib_compute', lib_compute_sources, dependencies : [libkompute_dep])

# Declare a dependency object for this library
lib_compute_dep = declare_dependency(link_with : lib_compute)
