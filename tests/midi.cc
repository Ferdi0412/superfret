#include <gtest/gtest.h>
#include "midi.h"

#ifndef _WIN32
    #error "Built for Windows"
#endif 

TEST(MidiOutTest, windows_std_out) {
    MidiOut out(0);
    EXPECT_TRUE(out.connected());
    EXPECT_THROW(MidiOut(0), MidiAllocated);
    EXPECT_THROW(MidiOut(MidiOut::count()), MidiNotFound);
}

TEST(MidiOutTest, unallocated) {
    MidiOut out;
    EXPECT_THROW(out.external(), MidiUnconnected);
    EXPECT_THROW(out.name(), MidiUnconnected);
    EXPECT_THROW(out.notes(), MidiUnconnected);
    EXPECT_THROW(out.channel_mask(), MidiUnconnected);
    EXPECT_THROW(out << 0, MidiUnconnected);
    EXPECT_THROW(out >> 0, MidiUnconnected);
}