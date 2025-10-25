/**
 * @file midi_discover.cpp
 * @brief Shows a simple way of finding the desired MIDI output
 */
#include <midi.h>

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    #ifdef _WIN32
    MidiOut out;

    for ( auto o: MidiOut::discover() )
        if ( o.name() == "Microsoft GS Wavetable Synth" )
            out = o;
    
    if ( !out.connected() )
        exit(-1);

    std::cout << "Playing C-Major on '" << out.name() << "'" << std::endl;
    out << 60;
    out << 64;
    out << 67;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    out >> 60;
    out >> 64;
    out >> 67;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    #else
    for ( auto o: MidiOut::discover() )
        std::cout << o.name() << std::endl;
    #endif
}