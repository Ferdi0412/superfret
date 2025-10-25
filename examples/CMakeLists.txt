cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(midi_quick_chords midi_quick_chords.cpp)
target_link_libraries(midi_quick_chords PRIVATE midi)

add_executable(midi_scale midi_scale.cpp)
target_link_libraries(midi_scale PRIVATE midi music)

add_executable(midi_discover midi_discover.cpp)
target_link_libraries(midi_discover PRIVATE midi music)