# CMake Configuration
cmake_minimum_required(VERSION 2.8)
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/third_party)

# Project Details
project(rock)

# Options
option(BUILD_TESTS "Build Tests" OFF)

# Cmake Modules
include(cmake/ExternalGtest.cmake)
include(cmake/CodeCoverage.cmake)

# Modules Configuration
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
  setup_target_for_coverage(test-coverage ctest test-coverage)
endif()

# Compilation Details
set(CXX_WARNING_FLAGS "-Wall -Wno-format-extra-args -Wformat-security -Wdelete-non-virtual-dtor -Wformat-nonliteral -Wformat=2 -Wpointer-arith -Wcast-align -Wshadow -Wsign-compare -Wno-variadic-macros -Wno-pmf-conversions")

set(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-rtti -pipe -std=gnu++11 ${CXX_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG    "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE  "-O2 -g  -DNDEBUG")
set(CMAKE_CXX_FLAGS_COVERAGE "-O0 -g3 ${COVERAGE_DEFINITIONS}")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -Wl,--hash-style=gnu")

include_directories(include ${GTEST_INCLUDE_DIRS})

# Tests
if (BUILD_TESTS STREQUAL ON)
  enable_testing()
  add_subdirectory(tests)
endif()

# Misc
mark_as_advanced(CMAKE_CXX_FLAGS_COVERAGE)
