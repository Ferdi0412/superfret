#ifndef SCALE_HPP_
#define SCALE_HPP_

#include <vector>
#include <cstdint>

#include "Tone.hpp"

class Scale {
    private:
        std::vector<Tone> _tones;

        void replace(const Tone& root, const std::vector<uint8_t>& degrees) {
            uint8_t last = 0;
            _tones = {root};
            for ( auto d: degrees ) {
                if ( d > 12 )
                    break;
                if ( d > last ) {
                    _tones.push_back(root + d);
                    last = d;
                }
            }
        }

    public:
        Scale() {
            replace(Tone(0), {0,1,2,3,4,5,6,7,8,9,10,11});
        }

        Scale(Tone root, const std::vector<uint8_t>& degrees) {
            replace(root, degrees);
        }

        Scale(const std::vector<Tone>& tones) {
            if ( tones.size() == 0 ) {
                *this = Scale();
                return;
            }
            Tone root = tones[0];
            std::vector<uint8_t> degrees;
            uint8_t last = 0;
            for ( auto t: tones ) {
                uint8_t d = t - root;
                if ( d > last ) {
                    degrees.push_back(d);
                    last = d;
                }
            }
            replace(root, degrees);
        }

        std::vector<Tone> tones() const {
            return _tones;
        }

        Tone root() const {
            return _tones[0];
        }

        size_t length() const {
            return _tones.size();
        }

        const Tone& operator[](size_t i) const {
            return _tones[i % _tones.size()];
        }

        /**
         * Get all notes belonging to a scale, between 2 notes
         * (inclusive of first and end last)
         */
        std::vector<Note> range(Note first, Note last) {
            uint8_t pos_first, pos_last;
            if ( last < first )
                return {};

            for ( size_t i = 0; i < length(); i++ )
                if ( first.between(_tones[i % length()], _tones[i % length()]) )
                    pos_first = i;

            for ( size_t i = 0; i < length(); i++ )
                if ( last.between(_tones[i % length()], _tones[i % length()]) )
                    pos_last = i;

            size_t octaves = last.octave() - first.octave();
            uint8_t base_octave = first.octave();

            if ( pos_last < pos_first )
                octaves -= 1;

            size_t steps = octaves * length() + pos_last + 1 - pos_first;
            std::vector<Note> notes;
            for ( size_t i = 0; i < steps; i++ ) {
                notes.push_back(Note(_tones[i % length()], base_octave + (i / length())));
            }
            return notes;
        }
        // std::vector<Note> between(Note first, Note last) {
        //     if ( start <= end )
        //         return {};
        //     size_t start_pos, end_pos;
        //     uint8_t min_int = 12, max_int = 12;
        //     for ( size_t i = 0; i < length(); i++ ) {
        //         if ( (_notes[i] - first) < min_int ) {
        //             start_pos = i;
        //             min_int = (_notes[i] - first);
        //         }
        //     }
        //     for ( size_t i = 0; i < length(); i++ ) {
        //         if ( (last - _notes[i]) < max_int ) {
        //             end_pos = i;
        //             max_int = (last - _notes[i]);
        //         }
        //     }
        //     std::vector<Note> notes;
        //     size_t octaves = last.octave() - first.octave();
        //     size_t steps;

        //     if ( octaves > 0 && end_pos > start_pos )
        //         steps = octaves * length() + end_pos - start_pos;
        //     else if ( octaves > 0 )
        //         steps = octaves * length() + start_pos - end_pos;
        //     else
        //         steps = 
        //     while ( 
        // }

        static Scale ionian(const Tone& root) { return Scale(root, {0, 2, 4, 5, 7, 9, 11}); }
        static Scale dorian(const Tone& root) { return Scale(root, {0, 2, 3, 5, 7, 9, 10}); }
        static Scale phrygian(const Tone& root) { return Scale(root, {0, 1, 3, 5, 7, 8, 10}); }
        static Scale lydian(const Tone& root) { return Scale(root, {0, 2, 4, 6, 7, 9, 11}); }
        static Scale mixolydian(const Tone& root) { return Scale(root, {0, 2, 4, 5, 7, 9, 10}); }
        static Scale aeolian(const Tone& root) { return Scale(root, {0, 2, 3, 5, 7, 8, 10}); }
        static Scale lorian(const Tone& root) { return Scale(root, {0, 1, 3, 5, 6, 8, 19}); }

        static Scale major(Tone root) { return ionian(root); }
        static Scale minor(Tone root) { return aeolian(root); }

        bool operator==(const Scale& other) const {
            if ( other.length() != length() )
                return false;
            for ( size_t i = 0; i < length(); i++ )
                if ( other[i] != (*this)[i] )
                    return false;
            return true;
        }
};

#endif // SCALE_HPP_