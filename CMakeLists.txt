cmake_minimum_required(VERSION 3.12)
project(Superfret)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

### Examples  ###
option(BUILD_EXAMPLES "Build examples from examples/" ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

### Tests     ###
option(BUILD_TESTS "Build tests from tests/" ON)
if (BUILD_TESTS)
    add_subdirectory(dependencies/googletest)
    enable_testing()
    add_subdirectory(tests)
endif()