/**
 * @file midi_scale.cpp
 * @brief Shows some of the Scale and Chord classes' interfaces
 * @note Note "60" is middle C - "C4", each increment of 1 is a halfstep
 */
#include <midi.h>
#include <music.h>

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    /// @note This out is always available on Windows, but maybe not otherwise
    MidiOut out(0);

    /// Part 1 - Working with the @b Scale class
    // Get a C-major scale
    Scale c_major = Scale::major("C");
    
    // Go over the "tones" of the scale - each note-class such as "C" or "D"
    for ( auto t: c_major.tones() )
        std::cout << t << std::endl;

    // Go over the "notes" of the scale starting at "C4" and ending at "C5"
    // The notes are defined as having a specific frequency, while tones do not
    // Print to std-out the name of the note, and play for 1 second
    for ( auto n: c_major.range(60, 72) ) {
        std::cout << "Playing " << n << std::endl;
        out << n;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        out >> n;
    }

    // Minor delay to separate sounds
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    /// Part 2 - Working with the @b Chord class
    // Iterate over the notes in the C-major chord, let them play
    // for 1 second, then turn them off
    std::cout << "C Major Chord" << std::endl;
    for ( auto n: Chord::major_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::major_triad(60) )
        out >> n;
    
    // Do the same for a C-minor chord
    std::cout << "C Minor Chord" << std::endl;
    for ( auto n: Chord::minor_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::minor_triad(60) )
        out >> n;

    // Do the same for an augmented C chord
    std::cout << "C Augmented Triad" << std::endl;
    for ( auto n: Chord::augmented_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::augmented_triad(60) )
        out >> n;

    // Do the same for a diminished C chord
    std::cout << "C Diminished Triad" << std::endl;
    for ( auto n: Chord::diminished_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::diminished_triad(60) )
        out >> n;
}