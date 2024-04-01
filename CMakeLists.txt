cmake_minimum_required(VERSION 3.0)

project(FlashcardApp)

find_package(Qt5Widgets COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++20 -Wall -Wextra -g)


# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} 
        src/main.cpp
        src/flashcardmanagement/Flashcard.cpp
        src/qt_ui/mainwindow.cpp
    )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
