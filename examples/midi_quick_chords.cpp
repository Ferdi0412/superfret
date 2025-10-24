/**
 * @file midi_quick_chords.cpp
 * @brief Shows some simple ways of playing chords
 * @note Note "60" is middle C - "C4", each increment of 1 is a halfstep
 */
#include <midi.h>

#include <thread>
#include <chrono>
#include <vector>

int main() {
    std::vector<unsigned int> c_major = {60, 66, 69};
    std::vector<unsigned int> c_minor = {60, 65, 69};
    MidiOut out(0);
    
    // Trigger notes via. MIDI note-on
    for ( auto note: c_major )
        out << note;

    // Let chord play for 2 seconds
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    // End notes via. MIDI note-off
    for ( auto note: c_major )
        out >> note;

    // Repeat for C-Minor
    for ( auto note: c_minor )
        out << note;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    for ( auto note: c_minor )
        out >> note;
}