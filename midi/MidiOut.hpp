#ifndef MIDI_HPP_
#define MIDI_HPP_

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <utility>

class MidiOut {
    public:
        struct Impl;
        
        struct MidiOutInfo {
            protected:
                std::unique_ptr<Impl> _pimpl;

            public:
                std::string name() const;

            public:
                MidiOutInfo(std::unique_ptr<Impl>&& pimpl);
                ~MidiOutInfo();

                MidiOutInfo(const MidiOutInfo& o);
                MidiOutInfo& operator=(const MidiOutInfo& o);
                
                MidiOutInfo(MidiOutInfo&&);
                MidiOutInfo& operator=(MidiOutInfo&&);
        };

    public:
        static std::vector<MidiOutInfo> discover();
        static size_t count();

    public:
        MidiOut(); // Non-functional
        // MidiOut(const MidiOutInfo& option);
        ~MidiOut();

        MidiOut(size_t port);

    public:
    // @todo - add some form of "default channel"
        // Quick access to turn on note
        MidiOut& operator<<(uint8_t note_on);
        MidiOut& operator<<(std::pair<uint8_t, uint8_t> note_n_vel);

        // Quick access to turn off note
        MidiOut& operator>>(uint8_t note_off);
        MidiOut& operator>>(std::pair<uint8_t, uint8_t> note_n_vel);

    protected:
        std::unique_ptr<Impl> _pimpl;
};

#endif // MIDI_HPP_