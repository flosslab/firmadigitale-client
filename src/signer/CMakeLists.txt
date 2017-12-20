set(FDOTOOL_SIGNER_SOURCES
        digisigner.cpp
        certificate.cpp)

set(FDOTOOL_SIGNER_HEADERS
        digisigner.hpp
        certificate.hpp)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Network)

include_directories(${CMAKE_BINARY_DIR})

add_library(signer STATIC
        ${FDOTOOL_SIGNER_SOURCES}
        ${FDOTOOL_SIGNER_HEADERS})

target_link_libraries(signer
        Qt5::Core
        Qt5::Network
        settings)

target_include_directories(signer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})
