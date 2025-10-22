cmake_minimum_required(VERSION 3.12)
project(Superfret)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Libraries ###
### 1) MIDI   ### 
add_library(midi midi/MidiOut.cpp)
target_include_directories(midi PUBLIC midi)
if(WIN32)
    target_link_libraries(midi PUBLIC winmm)
else()
    message(FATAL_ERROR "Could not find appropriate MIDI libraries")
endif()

### 2) Music  ###
add_library(music INTERFACE)
target_include_directories(music INTERFACE music)

### Examples  ###
option(BUILD_EXAMPLES "Build examples from examples/" ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()