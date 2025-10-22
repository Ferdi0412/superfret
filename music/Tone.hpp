#ifndef TONE_HPP_
#define TONE_HPP_

#include <array>
#include <string>
#include <cstdint>
#include <algorithm>
#include <iostream>
/**
 * In music, a "Tone" can refer to a note such as "C" independent of
 * it's specific pitch/octave - ie. 440Hz and 220Hz are both "A".
 *
 * Following MIDI conventions, tone "0" is "C", and "11" is "B"
 */
class Tone {
    private:
        uint8_t _tone;

    public:
        static inline constexpr std::array<const char*, 12> tones = {"C", "C#", "D", "D#",
                                                                     "E", "F", "F#", "G",
                                                                     "G#", "A", "A#", "B"};

        /**
         * Returns 12 if not found
         */
        static uint8_t toneof(const std::string& tone) {
            auto it = std::find(Tone::tones.begin(), Tone::tones.end(), tone);
            return std::distance(Tone::tones.begin(), it);
        }

        static std::string nameof(uint8_t tone) {
            return tones[tone % 12];
        }

        Tone(): _tone(0) { }
        Tone(int tone): _tone(uint8_t(tone % 12)) { }
        Tone(uint8_t tone): _tone(tone % 12) { }
        Tone(const char* tone): _tone(toneof(tone)) { }
        Tone(const std::string& tone): _tone(toneof(tone)) { } 

        Tone& operator=(uint8_t tone) {
            _tone = tone % 12;
            return *this;
        }

        Tone& operator=(const std::string& tone) {
            _tone = toneof(tone);
            return *this;
        }

        Tone& operator=(const char* tone) {
            _tone = toneof(tone);
            return *this;
        }

        uint8_t tone() const {
            return _tone;
        }

        std::string name() const {
            return nameof(_tone);
        }

        /**
         * MIDI supports octaves "-1" through "9", however implementing
         * starting at 0 is easier. If octave is too high (eg. 11),
         * will cycle values.
         */
        uint8_t midi(uint8_t octave) const {
            return (_tone + octave * 12) % 0x80;
        }

        Tone& operator+=(uint8_t interval) { 
            _tone += interval; _tone %= 12;
            return *this;
        }
        
        Tone operator+(uint8_t interval) const {
            return Tone(_tone + interval);
        }

        Tone& operator-=(uint8_t interval) { 
            if (interval > _tone) _tone += 12;
            _tone -= interval;
            return *this;
        }
        
        Tone operator-(uint8_t interval) const {
            Tone tone(_tone);
            tone -= interval;
            return tone;
        }

        /**
         * Interaval to get from other to *this
         * eg. Tone("C") - Tone("B") := 1 (1 step from "B" to "C")
         * eg. Tone("B") - Tone("C") := 11 (11 steps from "C" up to "B")
         */
        uint8_t operator-(const Tone& other) const {
            return ((12 + _tone ) - other._tone) % 12;
        }

        bool operator==(const Tone& other) const {
            return _tone == other._tone;
        }

        bool operator!=(const Tone& other) const {
            return _tone != other._tone;
        }

        // For internal use
        bool operator<(const Tone& other) const {
            return _tone < other._tone;
        }

        // For internal use
        bool operator>(const Tone& other) const {
            return _tone > other._tone;
        }

        // For internal use
        bool operator<=(const Tone& other) const {
            return _tone <= other._tone;
        }

        // For internal use
        bool operator>=(const Tone& other) const {
            return _tone >= other._tone;
        }

        friend std::ostream& operator<<(std::ostream& os, const Tone& tone) {
            os << tone.name();
            return os;
        }
};

#endif // TONE_HPP_