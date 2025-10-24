#ifdef _WIN32
    extern "C" {
        #include <Windows.h>
        #include <mmeapi.h>
    }
#else
    #error "Only Window's MIDI API is implemented!"
#endif

#include <cstdint>

#include "MidiOut.hpp"
#include "MidiError.hpp"

/** === System Specific APIs === */
std::string midi_out_name(UINT port) {
    char buffer[4096];
    MIDIOUTCAPS moc;

    if ( midiOutGetDevCaps(port, &moc, sizeof(moc)) != MMSYSERR_NOERROR )
        return "";

    if ( snprintf(buffer, sizeof(buffer), "%s", moc.szPname) >= sizeof(buffer) )
        return "";

    return std::string(buffer);
}

bool midi_out_open(HMIDIOUT* out, UINT port) {
    if ( midiOutOpen(out, port, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR )
        return false;
    return true;
}

bool midi_out_close(HMIDIOUT* out) {
    if ( midiOutClose(*out) != MMSYSERR_NOERROR )
        return false;
    *out = NULL;
    return true;
}

size_t midi_out_count() {
    return midiOutGetNumDevs();
}

bool midi_out_send(HMIDIOUT out, uint8_t status_byte, uint8_t data0, uint8_t data1) {
    union {
        DWORD w;
        BYTE  b[4];
    } msg;
    msg.b[0] = status_byte;
    msg.b[1] = data0 & 0x7F;
    msg.b[2] = data1 & 0x7F;
    msg.b[3] = 0;

    if ( midiOutShortMsg(out, msg.w) != MMSYSERR_NOERROR )
        return false;
    return true;
}

/** === MidiOut Impl === */
struct MidiOut::Impl {
    // Main method of construction
    Impl(UINT p): port(p), out(NULL) {}

    // Shallow copy will not be connected to the MIDI port!
    std::unique_ptr<Impl> shallow_copy() const {
        return std::make_unique<Impl>(port);
    }

    std::string name() const {
        return midi_out_name(port);
    }

    bool close() {
        if ( connected() )
            return midi_out_close(&out);
        return true;
    }

    bool connect() {
        if ( connected() )
            return true;
        return midi_out_open(&out, port);
    }

    // Returns if this has at any point been connected
    bool connected() const {
        return out != NULL;
    }

    bool send(uint8_t status, uint8_t d0, uint8_t d1, uint8_t channel) {
        status |= channel;
        return midi_out_send(out, status, d0, d1);
    }

    // Check if connection is still good
    // bool test_connection() ...

    Impl(const Impl& o): port(o.port), out(NULL) { }

    Impl& operator=(const Impl& o) {
        close();
        port = o.port;
        return *this;
    }

    ~Impl() {
        close();
    }

    private:
        UINT     port;
        HMIDIOUT out;
};

/** === MidiOutInfo Methods === */
MidiOut::MidiOutInfo::MidiOutInfo(std::unique_ptr<Impl>&& pimpl): _pimpl(std::move(pimpl)) {};
MidiOut::MidiOutInfo::~MidiOutInfo() = default;

MidiOut::MidiOutInfo::MidiOutInfo(const MidiOutInfo& o): _pimpl(o._pimpl->shallow_copy()) {};

MidiOut::MidiOutInfo& MidiOut::MidiOutInfo::operator=(const MidiOutInfo& o) {
    _pimpl = o._pimpl->shallow_copy();
    return *this;
};

MidiOut::MidiOutInfo::MidiOutInfo(MidiOutInfo&&) = default;
MidiOut::MidiOutInfo& MidiOut::MidiOutInfo::operator=(MidiOutInfo&&) = default;

std::string MidiOut::MidiOutInfo::name() const {
    if ( _pimpl )
        return _pimpl->name();
    return "";
}

/** === MidiOut Methods === */
std::vector<MidiOut::MidiOutInfo> MidiOut::discover() {
    std::vector<MidiOutInfo> found;
    for ( size_t i = 0; i < midi_out_count(); i++ )
        found.emplace_back(std::make_unique<Impl>(i));
    return found;
}

size_t MidiOut::count() {
    return midi_out_count();
}

MidiOut::MidiOut() = default;
MidiOut::~MidiOut() = default;

MidiOut::MidiOut(size_t port): _pimpl(std::make_unique<Impl>(port)) {
    if ( !_pimpl->connect() )
        _pimpl = nullptr;
}

MidiOut& MidiOut::operator<<(uint8_t note_on) {
    if ( _pimpl )
        if ( !_pimpl->send(0x90, note_on, 120, 0) )
            ;
    return *this;
}

MidiOut& MidiOut::operator<<(std::pair<uint8_t, uint8_t> note_n_vel) {
    if ( _pimpl )
        if ( !_pimpl->send(0x90, note_n_vel.first, note_n_vel.second, 0) )
            ;
    return *this;
}

MidiOut& MidiOut::operator>>(uint8_t note_off) {
    if ( _pimpl )
        if ( !_pimpl->send(0x80, note_off, 120, 0) )
            ;
    return *this;
}

MidiOut& MidiOut::operator>>(std::pair<uint8_t, uint8_t> note_n_vel) {
    if ( _pimpl )
        if ( !_pimpl->send(0x80, note_n_vel.first, note_n_vel.second, 0) )
            ;
    return *this;
}