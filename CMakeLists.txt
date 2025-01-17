cmake_minimum_required(VERSION 3.0)

project(FlashcardApp)

include(cmake/FindGoogleTest.cmake)

fetch_git_lib(
        GoogleTest
        https://github.com/google/googletest/
        main
)

set(QT_FEATURE_xcb ON)
find_library(LIBPQXX_LIBRARIES NAMES pqxx REQUIRED)

find_package(Qt5Widgets COMPONENTS Core Gui Widgets REQUIRED)



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++20 -Wall -Wextra -g)

option(BUILD_TESTS "Build tests" OFF)

if(NOT BUILD_TESTS)
    add_executable(${PROJECT_NAME} 
            src/main.cc
            src/flashcardmanagement/Flashcard.cc
            src/flashcardmanagement/Set.cc
            src/qt_ui/mainwindow.cc
            src/qt_ui/loginwindow.cc
            src/qt_ui/userdialog.cc
            src/qt_ui/newnamedialog.cc
            src/users/User.cc
        )
    target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets ${LIBPQXX_LIBRARIES} pqxx pq)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${LIBPQXX_INCLUDE_DIRS})
    
else()
    enable_testing()
    add_subdirectory(tests)
endif()

