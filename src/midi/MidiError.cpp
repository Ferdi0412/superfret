#ifndef MIDI_ERROR_CPP_
#define MIDI_ERROR_CPP_

#include "MidiError.hpp"

#ifdef _WIN32
    extern "C" {
        #include <Windows.h>
        #include <mmeapi.h>
    }
#else
    #error "Only Window's MIDI API is implemented!"
#endif

#ifdef _WIN32
void midi_out_error(MMRESULT err, const std::string& method = "") {
    if ( err == MMSYSERR_NOERROR )
        return;

    std::string msg = method.empty() ? "" : method + ": ";

    switch (err) {
        // Device/Driver errors
        case MMSYSERR_BADDEVICEID:
            throw MidiNotFound(msg + "Device ID out of range");
        
        case MMSYSERR_NODRIVER:
            throw MidiSysError(msg + "No device driver present");
        
        case MMSYSERR_ALLOCATED:
            throw MidiAllocated(msg + "Device already in use");
        
        case MMSYSERR_INVALHANDLE:
            throw MidiRuntimeError(msg + "Invalid device handle");
        
        // Parameter/Input errors
        case MMSYSERR_INVALPARAM:
            throw MidiRuntimeError(msg + "Invalid parameter");
        
        case MMSYSERR_INVALFLAG:
            throw MidiRuntimeError(msg + "Invalid flag");
        
        // System resource errors
        case MMSYSERR_NOMEM:
            throw MidiSysError(msg + "Unable to allocate memory");
        
        case MMSYSERR_HANDLEBUSY:
            throw MidiAllocated(msg + "Handle in use on another thread");
        
        // MIDI-specific errors
        case MIDIERR_STILLPLAYING:
            throw MidiRuntimeError(msg + "Cannot close - still playing");
        
        case MIDIERR_NOTREADY:
            throw MidiRuntimeError(msg + "Hardware busy with previous message");
        
        case MIDIERR_NODEVICE:
            throw MidiDisconnected(msg + "Device disconnected");
        
        case MIDIERR_NOMAP:
            throw MidiSysError(msg + "No MIDI port mapper available");
        
        case MIDIERR_INVALIDSETUP:
            throw MidiSysError(msg + "Invalid MIDI setup");
        
        // Generic/Unknown
        case MMSYSERR_ERROR:
            throw MidiSysError(msg + "Unspecified MIDI error (try CoInitializeEx on Win10+)");
        
        default:
            throw MidiSysError(msg + "Unknown MIDI error code: " + std::to_string(err));
    }
}
#endif

#endif // MIDI_ERROR_CPP_