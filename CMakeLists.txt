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

if (MSVC)
    add_compile_options(/W4 /permissive- /Zc:preprocessor /bigobj)
else()
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wconversion -Wnon-virtual-dtor -Woverloaded-virtual -Wunreachable-code -Wno-unused)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wduplicated-cond -Wduplicated-branches)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wlogical-op-parentheses)
    endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
endif(MSVC)

if(BUILD_TESTS)
    find_package(Catch2 CONFIG REQUIRED)
endif(BUILD_TESTS)

add_subdirectory(src)

