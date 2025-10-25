### Libraries ###
### 1) MIDI   ### 
add_library(midi midi/MidiError.cpp midi/MidiOut.cpp)
target_include_directories(midi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/midi)
if(WIN32)
    target_link_libraries(midi PUBLIC winmm)
else()
    message(FATAL_ERROR "Could not find appropriate MIDI libraries")
endif()

### 2) Music  ###
add_library(music INTERFACE)
target_include_directories(music INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/music)
