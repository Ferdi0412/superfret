/**
 * @brief Provides `MidiOut` class for easy MIDI message sending
 * @author Ferdinand Tonby-Strandborg
 */
#ifndef MIDI_HPP_
#define MIDI_HPP_

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <utility>

/**
 * @class MidiOut
 * @brief This allows MIDI messages to be sent
 * 
 * @example
 * ```
 * // Windows (and possibly other OS have equivalent) has a default
 * // MIDI output, named "Microsoft GS Wavetable Synth", which will
 * // always be MIDI-output '0'
 * MidiOut out(0);
 * 
 * out << 60; // Turn on note 60 (C4)
 * // Add delay here...
 * out >> 60; // Turn off note 60 (C4)
 * ```
 * 
 * @example
 * ```
 * MidiOut out;
 * for ( auto midi_dev: MidiOut::discover() )
 *     if ( midi_dev.name() == "Microsoft GS Wavetable Synth" ) {
 *         out = midi_dev;
 *         break;
 *     }
 * 
 * if ( !out.connected() )
 *     exit(-1);
 * 
 * out << 60;
 * // Add delay here...
 * out >> 60;
 * ```
 * 
 * @todo Implement MidiOut& operator=(const MidiOutInfo&);
 * @todo Implement void set_velocity(uint8_t);
 * @todo Implement broader MidiMsg type
 * @todo Implement MidiIn
 */
class MidiOut {
    public:
        /**
         * @struct MidiOut::MidiOutInfo
         * @brief This is provided for use in searching for MIDI outs.
         * 
         * @note The only useful method is `std::string name() const`
         */
        struct MidiOutInfo;

    public:
        /// @brief Discover all currently connected/available outputs
        static std::vector<MidiOutInfo> discover();

        /// @brief Count the number of currently connected/available outs
        static size_t count();

    public:
        /// @brief Initialize an unconnected instance
        /// @note Connect using `out = std::move(other)` or `out = info`
        MidiOut(); // Non-functional

        /// @brief Default destructor, hidden due to PIMPL implementation
        ~MidiOut();

        /// @brief Connect to a desired MIDI port
        MidiOut(size_t port);

    public:
        /// @brief Turn on note with default velocity 
        MidiOut& operator<<(uint8_t note_on);

        /// @brief Turn on note with desired velocity
        /// @param note_n_vel {note_on, velocity}
        MidiOut& operator<<(std::pair<uint8_t, uint8_t> note_n_vel);

        /// @brief Turn off note with default velocity
        MidiOut& operator>>(uint8_t note_off);

        /// @brief Turn off note with desired velocity
        /// @param note_n_vel {note_off, velocity}
        MidiOut& operator>>(std::pair<uint8_t, uint8_t> note_n_vel);

    protected:
        friend MidiOutInfo;     
        struct Impl;
        std::unique_ptr<Impl> _pimpl;

    public:
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
};

#endif // MIDI_HPP_