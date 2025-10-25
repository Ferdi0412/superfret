/**
 * @file MidiOut.hpp
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
 * @brief Class for sending MIDI messages and discovering MIDI out targets
 * 
 * @par Example 1: Simple note sending
 * @code
 * // Windows (and possibly other OS have equivalent) has a default
 * // MIDI output, named "Microsoft GS Wavetable Synth", which will
 * // always be MIDI-output '0'
 * MidiOut out(0);
 * 
 * out << 60; // Turn on note 60 (C4)
 * // Add delay here...
 * out >> 60; // Turn off note 60 (C4)
 * @endcode
 * 
 * @par Example 2: Searching through devices
 * @code
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
 * @endcode
 * 
 * @todo Implement void set_velocity(uint8_t);
 * @todo Implement broader MidiMsg type
 * @todo Implement MidiIn
 */
class MidiOut {
    public:
        // Docs provided in later declaration
        struct Info;

    /** @name Connect
     * These methods allow for connecting to outputs
     * @{
     */
        /// @brief Construct an unconnected instance
        /// Connect using `out = std::move(other)` or `out = info`
        MidiOut(); // Non-functional

        /// @brief Cleans up any allocated resources
        ~MidiOut();

        /// @brief Connect to the desired MIDI port
        MidiOut(size_t port);

        /// @brief Try to connect to the desired MIDI out 
        MidiOut& operator=(const Info& out);
    /**
     * @}
     */

    /** @name Information
     * Get information on the output connected, and status
     * of the connection
     * @{
     */
        /// @brief Whether this @b MidiOut has connected
        /// Does not test that the connection is still valid 
        bool connected() const;

        /// @brief Whether this is an external MIDI device
        bool external() const;

        /// @brief The name of the MIDI out target 
        std::string name() const;

        /// @brief The number of notes that can be played simultaneously
        /// For @b Windows this returns `0` for external devices 
        size_t notes() const;

        /// @brief Mask for channels contained in this device
        /// `0x0001` Means only channl 0 is available
        /// `0xFFFF` Means all 16 MIDI channels are available
        /// For @b Windows this returns `0` for external devices
        uint16_t channel_mask() const;
    /**
     * @}
     */

    /** @name Sending
     * Send MIDI messages to the MIDI out connection
     * @{
     */
        /// @brief Set the default velocity for use with @b << and @b >>
        /// @code
        /// // Send note with velocity 60
        /// out.set_velocity(60) << 60;
        /// out.set_velocity(120) << 60;
        /// @endcode
        MidiOut& set_velocity(uint8_t vel);

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
    /**
     * @}
     */

    protected:
        friend Info;     
        struct Impl;
        std::unique_ptr<Impl> _pimpl;
        uint8_t _vel = 60;

    public:
    /** @name Discovery 
     * Methods for discovering available MIDI outputs
     * @{
     */
        /// @brief Discover all currently connected/available outputs
        /// @memberof midiout_discovery
        static std::vector<Info> discover();

        /// @brief Count the number of currently connected/available outs
        static size_t count();
    /**
     * @}
     */

        /**
         * @struct MidiOut::Info
         * @brief This is provided for use in searching for MIDI outs
         * 
         * @note This is intended to be constructed using `MidiOut::discover`
         * 
         * The Windows MM API treats "internal" and "external"
         * (physical) MIDI outputs slightly differently - see methods
         * for specific implications
         * 
         * @internal
         * @note Consider the following features that the Windows MIDI
         * API also provides:
         * - manufacturer ID
         * - product ID
         * - version
         * - voices
         * - support
         * https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/ns-mmeapi-midioutcaps
         */
        struct Info {
            protected:
                friend MidiOut;
                std::unique_ptr<Impl> _pimpl;

            public:
                /// @brief Returns `true` if this represents is a physical MIDI connection
                bool external() const;

                /// @brief Read the name of the MIDI out 
                std::string name() const;

                /// @brief How many simulataneous notes this can play 
                /// For @b Windows this returns `0` for external devices
                size_t notes() const;

                /// @brief Mask for the channels contained in this device
                /// `0x0001` Means only channel 0 is available
                /// `0xFFFF` Means all 16 MIDI channels are available
                /// For @b Windows this returns `0` for external devices
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