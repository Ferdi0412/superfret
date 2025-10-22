#ifndef CHORD_HPP_
#define CHORD_HPP_

#include <vector>
#include <cstdint>

#include "Tone.hpp"
#include "Note.hpp"

class Chord {
    public:
        class Iterator;

    protected:
        friend Iterator;
        std::vector<Note> _notes;

    public:
        Chord() { }
        Chord(std::vector<Note> notes): _notes(std::move(notes)) { }
        Chord(Note root, const std::vector<uint8_t>& degrees) { 
            _notes.push_back(root);
            for ( auto d: degrees )
                _notes.push_back(root + d);
        }

        const std::vector<Note>& notes() const {
            return _notes;
        }

        static Chord major_triad(Note root) {
            return Chord(root, {4, 7});
        }

        static Chord minor_triad(Note root) {
            return Chord(root, {3, 7});
        }

        static Chord diminished_triad(Note root) {
            return Chord(root, {3, 6});
        }

        static Chord augmented_triad(Note root) {
            return Chord(root, {4, 8});
        }

        class Iterator {
            Chord* _chord;
            size_t _pos;

            public:
                Iterator(Chord* chord, size_t pos): _chord(chord), _pos(pos) { }

                Iterator& operator++() {
                    _pos ++;
                    return *this;
                }
                
                bool operator==(const Iterator& other) const {
                    return _pos == other._pos;
                }

                bool operator!=(const Iterator& other) const {
                    return _pos != other._pos;
                }

                Note operator*() const {
                    return _chord->_notes[_pos];
                }
        };

        Iterator begin() {
            return Iterator(this, 0);
        }

        Iterator end() {
            return Iterator(this, _notes.size());
        }
};

#endif // CHORD_HPP_