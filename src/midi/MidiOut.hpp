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
 * @todo Implement MidiOut& operator=(const Info&);
 * @todo Implement void set_velocity(uint8_t);
 * @todo Implement broader MidiMsg type
 * @todo Implement MidiIn
 */
class MidiOut {
    public:
        /**
         * @struct MidiOut::Info
         * @brief This is provided for use in searching for MIDI outs.
         * 
         * @note The only useful methods are:
         * bool external() const;
         * std::string name() const;
         * size_t notes() const;
         * uint16_t channels() const;
         */
        struct Info;

    public:
        /// @brief Discover all currently connected/available outputs
        static std::vector<Info> discover();

        /// @brief Count the number of currently connected/available outs
        static size_t count();
    
    public:
        /// @brief Whether this @b MidiOut has connected
        /// @note Does not test that the connection is still valid 
        bool connected() const;

        /// @brief Whether this is an external MIDI device
        bool external() const;

        /// @brief The name of the MIDI out target 
        std::string name() const;

        /// @brief The number of notes that can be played simultaneously
        /// @note For @b Windows this returns 0 for @b external devices 
        size_t notes() const;

        /// @brief Mask for channels contained in this device
        /// 0x0001 Means only channl 0 is available
        /// 0xFFFF Means all 16 MIDI channels are available
        /// @note For @b Windows this returns 0 for @b external devices
        uint16_t channel_mask() const;

    public:
        /// @brief Initialize an unconnected instance
        /// @note Connect using `out = std::move(other)` or `out = info`
        MidiOut(); // Non-functional

        /// @brief Default destructor, hidden due to PIMPL implementation
        ~MidiOut();

        /// @brief Connect to the desired MIDI port
        MidiOut(size_t port);

        /// @brief Try to connect to the desired MIDI out 
        MidiOut& operator=(const Info& out);

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
        friend Info;     
        struct Impl;
        std::unique_ptr<Impl> _pimpl;

    public:
        struct Info {
            /// @note
            /// The Windows MM API treats the "internal" and "external"
            /// MIDI outputs slightly differently
            /// @todo - Document this above
            /// 
            /// The Windows MM API allows additional features to be
            /// retrieved for various MIDI outs:
            /// manufacturer_id
            /// product_id
            /// version (major:minor)
            /// type of device
            /// voices
            /// support
            ///
            /// For more info see: 
            /// https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/ns-mmeapi-midioutcaps
            protected:
                friend MidiOut;
                std::unique_ptr<Impl> _pimpl;

            public:
                /// @brief Returns `true` if this represents is a physical MIDI connection
                bool external() const;

                /// @brief Read the name of the MIDI out 
                std::string name() const;

                /// @brief How many simulataneous notes this can play 
                size_t notes() const;

                /// @brief Mask for the channels contained in this device
                /// 0x0001 Means only channel 0 is available
                /// 0xFFFF Means all 16 MIDI channels are available
                uint16_t channel_mask() const;

            public:
                Info(std::unique_ptr<Impl>&& pimpl);
                ~Info();

                Info(const Info& o);
                Info& operator=(const Info& o);
                
                Info(Info&&);
                Info& operator=(Info&&);
        };
};

#endif // MIDI_HPP_