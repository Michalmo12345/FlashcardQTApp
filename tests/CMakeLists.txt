cmake_minimum_required(VERSION 3.10)

add_executable(
    tests 
    test_main.cc 
    flashcard_test.cc 
    set_test.cc
    ../src/flashcardmanagement/Flashcard.cc
    ../src/flashcardmanagement/Set.cc
  )

# target_link_libraries(tests gtest_main ${LIBPQXX_LIBRARIES} pq)
target_link_libraries(tests gtest main)
include(GoogleTest)
gtest_discover_tests(tests)