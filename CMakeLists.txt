cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)

project("PreyRun" CXX)

include(Configuration)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_14)

if(MSVC)
  # Warning flags
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    target_compile_options(project_options INTERFACE /W3)
  endif()

  # Disable undesirable warnings
  target_compile_options(project_options INTERFACE /wd4464) # relative include
                                                            # path contains '..'
  target_compile_options(
    project_options INTERFACE /wd4514) # unreferenced inline function has been
                                       # removed
  target_compile_options(project_options INTERFACE /wd4571) # SEH related
  target_compile_options(
    project_options INTERFACE /wd4616
  )# invalid compiler warnings -
   # https://msdn.microsoft.com/en-us/library/t7ab6xtd.aspx
  target_compile_options(
    project_options INTERFACE /wd4619
  )# invalid compiler warnings -
   # https://msdn.microsoft.com/en-us/library/tacee08d.aspx
  target_compile_options(project_options INTERFACE /wd4710) # function not
                                                            # inlined
  target_compile_options(
    project_options INTERFACE /wd4711) # function 'x' selected for automatic
                                       # inline
  # expansion
  target_compile_options(
    project_options INTERFACE /wd4820
  )# 'bytes' bytes padding added after construct 'member_name' -
   # https://msdn.microsoft.com/en-us/library/t7khkyth.aspx

  # Treat warnings as errors when using msvc.
  # target_compile_options(project_options INTERFACE /WX)

  # Optimization flags
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} /O2 /MT /Oi /Ot /Oy /GF /GS- /Gy")

  # Clang settings
elseif(CMAKE_CXX_COMPILER MATCHES ".*clang")
  # Warning flags
  target_compile_options(project_options INTERFACE -Weverything)
  # target_compile_options(project_options INTERFACE -Werror)

  # disable undesirable warnings
  target_compile_options(project_options INTERFACE -Wno-c++1z-extensions)
  target_compile_options(project_options INTERFACE -Wno-c++98-compat)
  target_compile_options(project_options INTERFACE -Wno-c++98-compat-pedantic)
  target_compile_options(project_options
                         INTERFACE -Wno-c++98-c++11-compat-pedantic)
  target_compile_options(project_options
                         INTERFACE -Wno-documentation-unknown-command)
  target_compile_options(project_options INTERFACE -Wno-double-promotion)
  target_compile_options(project_options
                         INTERFACE -Wno-missing-variable-declarations)
  target_compile_options(project_options INTERFACE -Wno-padded)
  target_compile_options(project_options INTERFACE -Wno-unknown-warning-option)
  target_compile_options(project_options INTERFACE -Wno-unused-template)
  target_compile_options(project_options INTERFACE -Wno-weak-vtables)

  # Optimization flags
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

  # processor architecture
    target_compile_options(project_options INTERFACE -m32)
    target_link_options(project_options INTERFACE -m32)

  # GCC settings
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Warning flags
  target_compile_options(project_options INTERFACE -pedantic)
  target_compile_options(project_options INTERFACE -pedantic-errors)
  target_compile_options(project_options INTERFACE -Wall)
  target_compile_options(project_options INTERFACE -Wcast-align)
  target_compile_options(project_options INTERFACE -Wcast-qual)
  target_compile_options(project_options INTERFACE -Wconversion)
  target_compile_options(project_options INTERFACE -Weffc++)
  target_compile_options(project_options INTERFACE -Werror)
  target_compile_options(project_options INTERFACE -Wextra)
  target_compile_options(project_options INTERFACE -Wfloat-equal)
  target_compile_options(project_options INTERFACE -Wformat=2)
  target_compile_options(project_options INTERFACE -Winline)
  target_compile_options(project_options INTERFACE -Winvalid-pch)
  target_compile_options(project_options INTERFACE -Wlogical-op)
  target_compile_options(project_options INTERFACE -Wmissing-declarations)
  target_compile_options(project_options INTERFACE -Wmissing-field-initializers)
  target_compile_options(project_options INTERFACE -Wmissing-include-dirs)
  target_compile_options(project_options INTERFACE -Wmissing-noreturn)
  target_compile_options(project_options INTERFACE -Wold-style-cast)
  target_compile_options(project_options INTERFACE -Wpointer-arith)
  target_compile_options(project_options INTERFACE -Wredundant-decls)
  target_compile_options(project_options INTERFACE -Wshadow)
  target_compile_options(project_options INTERFACE -Wstack-protector)
  target_compile_options(project_options INTERFACE -Wstrict-aliasing=2)
  target_compile_options(project_options INTERFACE -Wstrict-overflow=5)
  target_compile_options(project_options INTERFACE -Wundef)
  target_compile_options(project_options INTERFACE -Wunreachable-code)
  target_compile_options(project_options INTERFACE -Wunused)
  target_compile_options(project_options INTERFACE -Wunused-parameter)

  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.6)
    target_compile_options(project_options INTERFACE -Wnoexcept)
  endif()

  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0)
    target_compile_options(project_options INTERFACE -Wdouble-promotion)
    target_compile_options(project_options INTERFACE -Wtrampolines)
    target_compile_options(project_options INTERFACE -Wuseless-cast)
    target_compile_options(project_options
                           INTERFACE -Wvector-operation-performance)
    target_compile_options(project_options
                           INTERFACE -Wzero-as-null-pointer-constant)
  endif()

  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.0)
    target_compile_options(project_options INTERFACE -Wduplicated-cond)
    target_compile_options(project_options INTERFACE -Wnull-dereference)
    target_compile_options(project_options INTERFACE -Wshift-overflow=2)
  endif()

  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
    target_compile_options(project_options INTERFACE -Walloc-zero)
    target_compile_options(project_options INTERFACE -Walloca)
    target_compile_options(project_options INTERFACE -Wduplicated-branches)
    target_compile_options(project_options INTERFACE -Wrestrict)
  endif()

  # disable undesirable warnings
  target_compile_options(project_options INTERFACE -Wno-unused-function)

  # Optimization flags
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

  # processor architecture
  target_compile_options(project_options INTERFACE -m32)
  target_link_options(project_options INTERFACE -m32)
endif()

# Setup configurations
set(CMAKE_CONFIGURATION_TYPES
    # Standard CMake
    Debug
    MinSizeRel
    Release
    RelWithDebInfo
    # Special Prey
    DebugWithInlines
    DebugWithInlinesAndMemoryLog
    DedicatedDebugWithInlines
    DedicatedRelease
    # Special PreyRun
    DebugPreyRun
    CACHE INTERNAL "" FORCE)

set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
  "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel DebugWithInlines DebugWithInlinesAndMemoryLog DedicatedDebugWithInlines DedicatedRelease DebugPreyRun."
  FORCE)

pr_init_configuration(DebugWithInlines Debug)
pr_init_configuration(DebugWithInlinesAndMemoryLog Debug)
pr_init_configuration(DedicatedDebugWithInlines Debug)
pr_init_configuration(DedicatedRelease Release)
pr_init_configuration(DebugPreyRun RelWithDebInfo)

# DebugWithInlines Flags
if (MSVC)
  set(CMAKE_CXX_FLAGS_DebugWithInlines "${CMAKE_CXX_FLAGS_DebugWithInlines} /Ob1 /D \"_INLINEDEBUG\"")
endif()

# DebugWithInlinesAndMemoryLog Flags
if (MSVC)
  set(CMAKE_CXX_FLAGS_DebugWithInlinesAndMemoryLog "${CMAKE_CXX_FLAGS_DebugWithInlinesAndMemoryLog} /Ob1 /D \"_INLINEDEBUG\" /D \"ID_REDIRECT_NEWDELETE\" /D \"ID_DEBUG_MEMORY\" /D \"ID_DEBUG_UNINITIALIZED_MEMORY\"")
endif()

# DedicatedDebugWithInlines Flags
if (MSVC)
  set(CMAKE_CXX_FLAGS_DedicatedDebugWithInlines "${CMAKE_CXX_FLAGS_DedicatedDebugWithInlines} /Ob1 /D \"_INLINEDEBUG\"")
endif()

# DedicatedRelease Flags

# DebugPreyRun Flags
if (MSVC)
  set(CMAKE_CXX_FLAGS_DebugPreyRun "${CMAKE_CXX_FLAGS_DebugPreyRun} /D \"PR_DEBUG\"")
endif()

add_subdirectory(src)
