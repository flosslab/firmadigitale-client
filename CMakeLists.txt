cmake_minimum_required(VERSION 3.0)
project(fdotool)

if (UNIX)
    include(GNUInstallDirs)
endif ()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QT5_PREFIX "$ENV{QTDIR}" CACHE PATH "Qt5 prefix path")
find_program(QT5_QMAKE_BIN
        qmake
        PATHS "${QT5_PREFIX}/bin"
        NO_CMAKE_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH)

if (NOT QT5_QMAKE_BIN)
    message(FATAL_ERROR "Wrong Qt5 directory: ${QT5_PREFIX}")
endif ()

set(CMAKE_PREFIX_PATH "${QT5_PREFIX}/lib/cmake")

enable_testing()

include(cmake/config.cmake)

add_subdirectory(src)
add_subdirectory(tools)

include(cmake/packaging.cmake)
