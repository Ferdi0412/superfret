@page getting_started Getting Started

# Getting started with Superfret

### Requirements
- C++17 or after
- CMake 3.12 or after
- Windows (Linux/macOS later)

### Examples

#### MIDI
MIDI is a protocol for sending/receiving messages related to music, most
notably notes to be played. They handle notes similar to a piano, with
a dedicated *note-on* and *note-off* message (like pressing and releasing 
a piano key). MIDI is also directional, meaning a single MIDI connection
can either go *out* of your program to an instrument or similar device,
or come *in* to your program from a MIDI instrument.

\code{.cpp}
#include <midi.h>

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    MidiOut midi_out; // Unconnected instance

    for ( auto o: MidiOut::discover() )
        if ( o.name() == "Microsoft GS Wavetable Synth" )
            midi_out == o;
    
    if ( !midi_out.connected() ) {
        std::cout << "Could not find desired MIDI Out!" << std::endl;
        exit(1);
    }

    std::cout << "Playing middle-C" << std::endl;
    midi_out << 60; // Note on
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    midi_out >> 60; // Note off
}
\endcode