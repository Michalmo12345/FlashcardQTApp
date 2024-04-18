cmake_minimum_required(VERSION 3.0)
# set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR};${CMAKE_SOURCE_DIR}/libs/qt-5.15.13/lib")
set(Qt5Widgets_DIR "${CMAKE_SOURCE_DIR}/libs/qt-5.15.13/lib/cmake/Qt5Widgets")
# set(LIBPQXX_STATIC_LIB "${CMAKE_SOURCE_DIR}/build-pqxx/src/libpqxx.a")
# set(libpqxx_DIR "${CMAKE_SOURCE_DIR}/libs/libpqxx/cmake")
# set(PostgreSQL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/postgresql-13.2/include")
# set(PostgreSQL_LIBRARY "${CMAKE_SOURCE_DIR}/libs/postgresql-13.2/lib")
# set(LIBPQXX_ROOT "${CMAKE_SOURCE_DIR}/libs/libpqxx")
# set(LIBPQXX_LIBRARIES_DIR "${CMAKE_SOURCE_DIR/libs/libpqxx/cmake}")
# set(LIBPQXX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/libpqxx")
# set(LIBPQXX_LIB_DIR /ścieżka/do/libpqxx/build)
# list(APPEND CMAKE_PREFIX_PATH "${LIBPQXX_INSTALL_DIR}")
#/home/michalmo/Downloads/flashcardapp-tests/libs/qt-5.15.13/lib/cmake/Qt5Widgets/Qt5WidgetsConfig.cmake
project(FlashcardApp)

include(cmake/FindGoogleTest.cmake)

fetch_git_lib(
        GoogleTest
        https://github.com/google/googletest/
        main
)
#include_directories(${LIBPQXX_ROOT}/include)
# link_directories(${LIBPQXX_ROOT}/lib)
# include_directories(${PostgreSQL_INCLUDE_DIR})
# link_directories(${PostgreSQL_LIBRARY})
# include_directories(${CMAKE_SOURCE_DIR}/libs/libpqxx/include)
set(QT_FEATURE_xcb ON)
# find_library(LIBPQXX_LIBRARIES NAMES pqxx REQUIRED)
# find_package(libpqxx REQUIRED)
find_package(Qt5Widgets COMPONENTS Core Gui Widgets REQUIRED)
# find_package(PostgreSQL REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++20 -Wall -Wextra -g)


# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
option(BUILD_TESTS "Build tests" OFF)

if(NOT BUILD_TESTS)
    add_executable(${PROJECT_NAME} 
            src/main.cc
            src/flashcardmanagement/Flashcard.cc
            src/flashcardmanagement/Set.cc
            src/qt_ui/mainwindow.cc
        )
    # add_subdirectory(libs/libpqxx build-pqxx) 
    # target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets ${LIBPQ${LIBPQXX_STATIC_LIBXX_LIBRARIES} pq)
    target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets)
    # target_link_libraries(${PROJECT_NAME} PRIVATE pqxx Qt5::Core Qt5::Gui Qt5::Widgets )
    # target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${LIBPQXX_INCLUDE_DIRS})
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
else()
    enable_testing()
    add_subdirectory(tests)
endif()

