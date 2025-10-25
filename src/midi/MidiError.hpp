/**
 * @file MidiError
 * @brief Provides the error classes for @b MidiIn or @b MidiOut calls
 */
#ifndef MIDI_EXCEPTIONS_HPP_
#define MIDI_EXCEPTIONS_HPP_

#include <exception>
#include <string>

/**
 * @class MidiError
 * @brief Base class for catching all other MIDI error classes
 */
class MidiError : public std::exception {
    protected:
        std::string _msg;
    
        explicit MidiError(const std::string& msg): _msg(msg) {}

    public:
        /// @brief Description of the error
        const char* what() const noexcept override { return _msg.c_str(); }

};

/**
 * @class MidiNotFound
 * @brief Usually a user error in that the desired MIDI port is not available
 * @note This can arise if the system detects a port, but disconnects before a connection is made
 */
class MidiNotFound : public MidiError {
    public:
        explicit MidiNotFound(const std::string& msg): MidiError("MidiNotFound: " + msg) {}
};

/**
 * @class MidiUnconnected
 * @brief User error in trying to send/receive on an unconnected instance
 */
class MidiUnconnected : public MidiError {
    public:
        explicit MidiUnconnected(const std::string& msg): MidiError("MidiUnconnected: " + msg) {}
};

/**
 * @class MidiAllocated
 * @brief The port/resource is already in use
 */
class MidiAllocated : public MidiError {
    public:
        explicit MidiAllocated(const std::string& msg): MidiError("MidiAllocated: " + msg) {}
};

/**
 * @class MidiDisconnected
 * @brief System error in that a MIDI connection was lost or otherwise unavailable
 */
class MidiDisconnected : public MidiError {
    public:
        explicit MidiDisconnected(const std::string& msg): MidiError("MidiDisconnected: " + msg) {}
};

/**
 * @class MidiRuntimeError
 * @brief Something unexpected happened - Most likely Ferdi's fault :(
 */
class MidiRuntimeError : public MidiError {
    public:
        explicit MidiRuntimeError(const std::string& msg): MidiError("MidiRuntimeError: " + msg) {}
};

/**
 * @class MidiSysError
 * @brief The system failed to handle the request
 */
class MidiSysError : public MidiError {
    public:
        explicit MidiSysError(const std::string& msg): MidiError("MidiSysError: " + msg) {}
};

#endif // MIDI_EXCEPTIONS_HPP_