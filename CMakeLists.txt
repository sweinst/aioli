cmake_minimum_required(VERSION 3.26)
project(aioli VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(CODE_COVERAGE "Set up code coverage" OFF)

message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Build examples: ${BUILD_EXAMPLES}")
message(STATUS "Code coverage: ${CODE_COVERAGE}")

if (CODE_COVERAGE)
    if (LINUX)
        add_compile_options(--coverage)
        add_link_options(--coverage)
    else(LINUX)
        message(FATAL_ERROR "Code coverage is not supported on this platform")
    endif(LINUX)
endif(CODE_COVERAGE)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

message(STATUS "Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

if(BUILD_TESTS)
    find_package(Catch2 CONFIG REQUIRED)
endif(BUILD_TESTS)

add_subdirectory(src)

