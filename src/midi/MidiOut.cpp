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
#include "MidiError.cpp"

/** === System Specific APIs === */
/// @note Provides the following methods:
///
/// std::string midi_out_name(t port)
/// @returns "" if not a real port, or unavailable
///
/// bool midi_out_open(*out, port)
/// @param out should be NULL, and will be set to a valid port
/// @note does not actually check for NULL at the moment,
///       nor automatically close it if open 
/// @return false in case of error, true on success
///
/// bool midi_out_close(*out)
/// @param out is set to NULL on success, assumes is valid output
/// @return false in case of error, true on success

#ifdef _WIN32
    /// @note Windows MultiMedia implementation
std::string midi_out_name(UINT port) {
    char buffer[4096];
    MIDIOUTCAPS moc;
    midi_out_error(midiOutGetDevCaps(port, &moc, sizeof(moc)), "midi_out_name");
    if ( snprintf(buffer, sizeof(buffer), "%s", moc.szPname) >= sizeof(buffer) )
        throw MidiRuntimeError("midi_out_name: buffer overflow");
    return std::string(buffer);
}

bool midi_out_external(UINT port) {
    MIDIOUTCAPS moc;
    midi_out_error(midiOutGetDevCaps(port, &moc, sizeof(moc)), "midi_out_external");
    return moc.wTechnology == MOD_MIDIPORT;
}

size_t midi_out_notes(UINT port) {
    MIDIOUTCAPS moc;
    midi_out_error(midiOutGetDevCaps(port, &moc, sizeof(moc)), "midi_out_notes");
    return moc.wNotes;
}

uint16_t midi_out_channel_mask(UINT port) {
    MIDIOUTCAPS moc;
    midi_out_error(midiOutGetDevCaps(port, &moc, sizeof(moc)), "midi_out_channel_mask");
    return moc.wChannelMask;
}

bool midi_out_open(HMIDIOUT* out, UINT port) {
    midi_out_error(midiOutOpen(out, port, 0, 0, CALLBACK_NULL), "midiOutOpen");
    return true;
}

bool midi_out_close(HMIDIOUT* out) {
    midi_out_error(midiOutReset(*out), "midiOutReset");
    midi_out_error(midiOutClose(*out), "midiOutClose");
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

    midi_out_error(midiOutShortMsg(out, msg.w), "midiOutShortMsg");
    return true;
}
#else
    #error "Only implemented for WIN32 at the moment!"
#endif // _WIN32

/** === MidiOut Impl === */
/**
 * @class MidiOut::Impl
 * @brief Hides the Implementations of the MIDI port
 * 
 * The class is currently based on the following @b assumption:
 *   The MIDI implementation is similar to the @b WIN32 implementation,
 *   with a separate "port" value identifying the specific obejct,
 *   and handles for the "connection" established to the port
 * 
 * Importantly implements the following methods that Info needs
 * 
 * std::string name() const;
 * size_t notes() const;
 * uint16_t channels() const;
 */
struct MidiOut::Impl {
    #ifndef _WIN32
        #warning "Assumes MIDI implementation has separate 'port' and 'connection handle'"
    #endif
    /// @brief Create "shallow instance" - not actually connected to the port 
    Impl(UINT p): port(p), out(NULL) {}

    /// @brief Create a "shallow" copy of self - utility for later 
    std::unique_ptr<Impl> shallow_copy() const {
        return std::make_unique<Impl>(port);
    }

    bool external() const {
        return midi_out_external(port);
    }

    std::string name() const {
        return midi_out_name(port);
    }

    size_t notes() const {
        return midi_out_notes(port);
    }

    uint16_t channel_mask() const {
        return midi_out_channel_mask(port);
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
        #ifdef _WIN32
            UINT     port;
            HMIDIOUT out;
        #endif
};

/** === Info Methods === */
MidiOut::Info::Info(std::unique_ptr<Impl>&& pimpl): _pimpl(std::move(pimpl)) {};
MidiOut::Info::~Info() = default;

MidiOut::Info::Info(const Info& o): _pimpl(o._pimpl->shallow_copy()) {};

MidiOut::Info& MidiOut::Info::operator=(const Info& o) {
    _pimpl = o._pimpl->shallow_copy();
    return *this;
};

MidiOut::Info::Info(Info&&) = default;
MidiOut::Info& MidiOut::Info::operator=(Info&&) = default;

// The MidiOut::Info will never have a NULL _pimpl field
bool MidiOut::Info::external() const {
    return _pimpl->external();
}

std::string MidiOut::Info::name() const {
    return _pimpl->name();
}

size_t MidiOut::Info::notes() const {
    return _pimpl->notes();
}

uint16_t MidiOut::Info::channel_mask() const {
    return _pimpl->channel_mask();
}

/** === MidiOut Methods === */
std::vector<MidiOut::Info> MidiOut::discover() {
    std::vector<Info> found;
    for ( size_t i = 0; i < midi_out_count(); i++ )
        found.emplace_back(std::make_unique<Impl>(i));
    return found;
}

size_t MidiOut::count() {
    return midi_out_count();
}

bool MidiOut::connected() const {
    if ( _pimpl )
        return _pimpl->connected();
    return false;
}

bool MidiOut::external() const {
    if ( _pimpl )
        return _pimpl->external();
    throw MidiUnconnected("MidiOut::external - Must connect first!");
}

std::string MidiOut::name() const {
    if ( _pimpl )
        return _pimpl->name();
    throw MidiUnconnected("MidiOut::name - Must connect first!");
}

size_t MidiOut::notes() const {
    if ( _pimpl )
        return _pimpl->notes();
    throw MidiUnconnected("MidiOut::notes - Must connect first!");
}

uint16_t MidiOut::channel_mask() const {
    if ( _pimpl )
        return _pimpl->channel_mask();
    throw MidiUnconnected("MidiOut::channel_mask - Must connect first!");
}

MidiOut::MidiOut() = default;
MidiOut::~MidiOut() = default;

MidiOut::MidiOut(size_t port): _pimpl(std::make_unique<Impl>(port)) {
    if ( !_pimpl->connect() )
        _pimpl = nullptr;
}

MidiOut& MidiOut::operator=(const Info& out) {
    _pimpl = out._pimpl->shallow_copy();
    _pimpl->connect();
    return *this;
}

MidiOut& MidiOut::set_velocity(uint8_t vel) {
    _vel = 0x7F & vel;
    return *this;
}

MidiOut& MidiOut::operator<<(uint8_t note_on) {
    if ( _pimpl )
        _pimpl->send(0x90, note_on, 120, 0);
    else
        throw MidiUnconnected("MidiOut << - Must connect first!");
    return *this;
}

MidiOut& MidiOut::operator<<(std::pair<uint8_t, uint8_t> note_n_vel) {
    if ( _pimpl )
        _pimpl->send(0x90, note_n_vel.first, note_n_vel.second, 0);
    else
        throw MidiUnconnected("MidiOut << - Must connect first!");
    return *this;
}

MidiOut& MidiOut::operator>>(uint8_t note_off) {
    if ( _pimpl )
        _pimpl->send(0x80, note_off, 120, 0);
    else
        throw MidiUnconnected("MidiOut >> - Must connect first!");
    return *this;
}

MidiOut& MidiOut::operator>>(std::pair<uint8_t, uint8_t> note_n_vel) {
    if ( _pimpl )
        _pimpl->send(0x80, note_n_vel.first, note_n_vel.second, 0);
    else
        throw MidiUnconnected("MidiOut >> - Must connect first!");
    return *this;
}