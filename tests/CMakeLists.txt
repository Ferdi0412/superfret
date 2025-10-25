add_executable(midi_test midi.cc)
target_link_libraries(midi_test GTest::gtest_main midi)

include(GoogleTest)
gtest_discover_tests(midi_test)