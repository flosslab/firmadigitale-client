set(FDOTOOL_SOURCES
        fdotool.cpp
        action.cpp)

set(FDOTOOL_HEADERS
        version.hpp
        fdotool.hpp
        action.hpp)

set(FDOTOOL_RESOURCES
        fdotool.rc
        resources.qrc)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Svg
        Widgets
        Concurrent)

set(FDOTOOL_MAC_ICON_NAME icon.icns)
set(FDOTOOL_MAC_ICON_PATH ../icons/macos/icon.icns)

set_source_files_properties(${FDOTOOL_MAC_ICON_PATH}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION
        Resources)

set(MACOSX_BUNDLE_EXECUTABLE_NAME fdotool)
set(MACOSX_BUNDLE_BUNDLE_NAME FDOTool)
set(MACOSX_BUNDLE_ICON_FILE ${FDOTOOL_MAC_ICON_NAME})

configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/config.hpp)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(settings)
add_subdirectory(signer)
add_subdirectory(worker)
add_subdirectory(window)

add_executable(fdotool
        WIN32 MACOSX_BUNDLE
        ${FDOTOOL_SOURCES}
        ${FDOTOOL_HEADERS}
        ${FDOTOOL_RESOURCES}
        ${FDOTOOL_MAC_ICON_PATH})

target_link_libraries(fdotool
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::Concurrent
        settings
        signer
        worker
        window)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(FDOTOOL_DEPLOYQT_TYPE "--debug")
else ()
    set(FDOTOOL_DEPLOYQT_TYPE "--release")
endif ()

if (WIN32)
    add_custom_command(TARGET fdotool
            POST_BUILD
            COMMAND ${QT5_PREFIX}/bin/windeployqt.exe $<TARGET_FILE:fdotool> ${FDOTOOL_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR}/windeployqt)
elseif (APPLE)
    set_target_properties(fdotool
            PROPERTIES MACOSX_BUNDLE_ICON_FILE ${FDOTOOL_MAC_ICON_NAME})
    add_custom_command(TARGET fdotool
            POST_BUILD
            COMMAND macdeployqt $<TARGET_FILE:fdotool>)
endif ()

if (WIN32)
    install(TARGETS fdotool
            RUNTIME DESTINATION .
            COMPONENT fdotool)
elseif (APPLE)
    install(TARGETS fdotool
            BUNDLE DESTINATION .
            COMPONENT fdotool)
else ()
    install(TARGETS fdotool
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT fdotool)
endif ()

if (WIN32 OR APPLE)
    install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
            DESTINATION .
            COMPONENT qtlibs)
endif ()
