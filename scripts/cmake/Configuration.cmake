
function(pr_init_configuration name base)
    # Flags
    set(CMAKE_C_FLAGS_${name} ${CMAKE_C_FLAGS_${base}} CACHE STRING "Flags used by the C Compiler during ${name} builds." FORCE)
    set(CMAKE_CXX_FLAGS_${name} ${CMAKE_CXX_FLAGS_${base}} CACHE STRING "Flags used by the C++ Compiler during ${name} builds." FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_${name} ${CMAKE_EXE_LINKER_FLAGS_${base}} CACHE STRING "Flags used for linking binaries during ${name} builds." FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_${name} ${CMAKE_SHARED_LINKER_FLAGS_${base}} CACHE STRING "Flags used by the shared libraries linker during ${name} builds." FORCE)

    mark_as_advanced(CMAKE_C_FLAGS_${name} CMAKE_CXX_FLAGS_${name} CMAKE_EXE_LINKER_FLAGS_${name} CMAKE_SHARED_LINKER_FLAGS_${name})
endfunction()
