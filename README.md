# superfret
A collection of tools I am making because I like practicing guitar, but struggle in some aspects, for example music theory.

## Goals
The goal initially is to create a very basic "Synth" that can be used 
directly in your terminal, and to make a basic program in your terminal
that will display an ASCII-fretboard with various scales and chords.

Down the line I would like to add further features, such as chord/scale
recognition for audio inputs, and some further features for practicing
guitar or similar melodic instruments.

## Building
This project is built using `cmake`, and is currently being worked on 
for Windows. To build the project, [download cmake](https://cmake.org/download/)
and follow any relevant steps, then from the repository root:

```sh
cmake -S . -B build
cmake --build build
```

### Examples
Some examples are provided under `examples/`, use `cmake -S . -B build -DBUILD_EXAMPLES=ON` to build them. This will allow you to run them for 
example using `./build/examples/midi_scale` on unix-based systems or
`.\build\examples\midi_scale` on Windows.

### Tests
Some tests are under `tests/`, and will be built by setting `-DBUILD_TESTS=ON`. They will be built
under `build/tests/`. These are built using **Google Tests**.

### Docs
The documentation is intended to be made using `doxygen`. I am also plan to use
**Doxygen Awesome** to help with the appearance of the documentation.
To build the documentation, [download doxygen](https://www.doxygen.nl/download.html)
and follow any relevant steps, then from the repository root:

```sh
cd docs/
doxygen Doxyfile
```