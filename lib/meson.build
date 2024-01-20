lib_kompute_sources = files([
    'vector_operation.cpp',
])

lib_kompute = static_library('lib_kompute', lib_kompute_sources, dependencies : [kompute_dep])

# Declare a dependency object for this library
lib_kompute_dep = declare_dependency(link_with : lib_kompute)
