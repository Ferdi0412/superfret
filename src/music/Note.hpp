#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <cstdint>
#include <string>
#include <iostream>

#include "Tone.hpp"

class Note {
    private:
        uint8_t _note;

    public:
        Note(uint8_t note=60): _note(note & 0x7F) {}
        Note(Tone tone, uint8_t octave): _note(tone.midi(octave)) {}
        Note(const std::string& tone, uint8_t octave): _note(Tone(tone).midi(octave)) {}

        uint8_t note() const {
            return _note;
        }

        std::string name() const {
            return Tone::nameof(_note) + std::to_string(int(_note / 12) - 1);
        }

        Tone tone() const {
            return Tone(_note);
        }

        uint8_t octave() const {
            return _note / 12;
        }

        friend std::ostream& operator<<(std::ostream& os, const Note& note) {
            os << note.name();
            return os;
        }

        /**
         * Allow implicit conversion to uint8_t
         */
        operator uint8_t() const {
            return _note;
        }

        bool operator==(const Note& other) const {
            return _note == other._note;
        }

        bool operator!=(const Note& other) const {
            return _note != other._note;
        }

        bool operator>(const Note& other) const {
            return _note > other._note;
        }

        bool operator<(const Note& other) const {
            return _note < other._note;
        }

        bool operator>=(const Note& other) const {
            return _note >= other._note;
        }

        bool operator<=(const Note& other) const {
            return _note <= other._note;
        }

        int operator-(const Note& other) const {
            return _note - other._note;
        }

        uint8_t operator-(const Tone& other) const {
            return (_note + 12 - other.tone()) % 12;
        }

        Note operator+(uint8_t interval) const {
            return Note(_note + interval);
        }

        Note& operator+=(uint8_t interval) {
            _note += interval;
            _note %= 0x7F;
            return *this;
        }

        Note operator-(uint8_t interval) const {
            if ( _note < interval )
                return Note(0x7F + _note - interval);
            return Note(_note - interval);
        }

        Note& operator-=(uint8_t interval) {
            if ( _note < interval )
                _note = 0x7F - _note + interval;
            else
                _note -= interval;
            return *this;
        }

        friend uint8_t operator-(const Tone& first, const Note& second) {
            return ((first.tone() + 12) - (second._note % 12)) % 12;
        }

        bool between(const Tone& start, const Tone& end) const {
            if ( end < start )
                return end < tone() && tone() <= start;
            return start <= tone() && tone() <= end;
        }
};

#endif // NOTE_HPP_