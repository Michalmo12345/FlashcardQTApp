cmake_minimum_required(VERSION 3.10)

add_executable(tests test_main.cc flashcard_test.cc)

target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)