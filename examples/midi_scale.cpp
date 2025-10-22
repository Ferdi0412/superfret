#include <midi.h>
#include <music.h>

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    Scale c_major = Scale::major("C");
    std::cout << Note(0);
    
    MidiOut out(0);

    for ( auto t: c_major.tones() )
        std::cout << t << std::endl;

    for ( auto n: c_major.range(60, 72) ) {
        std::cout << "Playing " << n << std::endl;
        out << n;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        out >> n;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    std::cout << "C Major Chord" << std::endl;
    for ( auto n: Chord::major_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::major_triad(60) )
        out >> n;
    
    std::cout << "C Minor Chord" << std::endl;
    for ( auto n: Chord::minor_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::minor_triad(60) )
        out >> n;

    std::cout << "C Augmented Triad" << std::endl;
    for ( auto n: Chord::augmented_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::augmented_triad(60) )
        out >> n;

    std::cout << "C Diminished Triad" << std::endl;
    for ( auto n: Chord::diminished_triad(60) )
        out << n;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for ( auto n: Chord::diminished_triad(60) )
        out >> n;

    std::cout << "Done" << std::endl;
}