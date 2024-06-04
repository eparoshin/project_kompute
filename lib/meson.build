lib_compute_sources = files([
    'vulkan_gate.cpp',
    'vulkan_compute_module.cpp',
    'compute_function.cpp',
])

shader_dir = 'shaders'

subdir(shader_dir)

inc_shaders = include_directories(shader_dir)

lib_compute = static_library('lib_compute', lib_compute_sources, dependencies : [libkompute_dep], include_directories : inc_shaders)

# Declare a dependency object for this library
lib_compute_dep = declare_dependency(link_with : lib_compute, sources : generated_hpp_files)
