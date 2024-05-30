cmake_minimum_required(VERSION 3.10)

find_package(Qt5 REQUIRED COMPONENTS Core)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/users)
include_directories(${CMAKE_SOURCE_DIR}/src/flashcardmanagement)

add_executable(
    tests 
    test_main.cc 
    flashcard_test.cc 
    set_test.cc
    user_test.cc
    ../src/flashcardmanagement/Flashcard.cc
    ../src/flashcardmanagement/Set.cc
    ../src/users/User.cc

  )

target_link_libraries(tests gtest_main Qt5::Core ${LIBPQXX_LIBRARIES} pq)

include(GoogleTest)
gtest_discover_tests(tests)