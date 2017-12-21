include(ExternalProject)

if (WIN32)
    if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(TOOLS_OPENSSL_CONFIGURE_ARCH "VC-WIN64A")
        set(TOOLS_OPENSSL_CONFIGURE_DO "ms\\\\do_win64a.bat")
    else ()
        set(TOOLS_OPENSSL_CONFIGURE_ARCH "VC-WIN32")
        set(TOOLS_OPENSSL_CONFIGURE_DO "ms\\\\do_ms.bat")
    endif ()

    set(TOOLS_OPENSSL_CONFIGURE_COMMAND perl Configure --prefix=${CMAKE_BINARY_DIR}/openssl_patched no-asm no-shared ${TOOLS_OPENSSL_CONFIGURE_ARCH})
    set(TOOLS_OPENSSL_BUILD_COMMAND nmake -f .\\ms\\nt.mak)
    set(TOOLS_OPENSSL_INSTALL_COMMAND nmake -f .\\ms\\nt.mak install)

    set(TOOLS_OPENSSL_BINARY openssl.exe)
elseif (APPLE)
    if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(TOOLS_OPENSSL_CONFIGURE_ARCH "linux-x86_64")
    else ()
        set(TOOLS_OPENSSL_CONFIGURE_ARCH "linux-i386")
    endif ()

    set(TOOLS_OPENSSL_CONFIGURE_COMMAND ./Configure --prefix=${CMAKE_BINARY_DIR}/openssl_patched no-asm no-shared ${TOOLS_OPENSSL_CONFIGURE_ARCH})
    set(TOOLS_OPENSSL_BUILD_COMMAND make)
    set(TOOLS_OPENSSL_INSTALL_COMMAND make install_sw)

    set(TOOLS_OPENSSL_BINARY openssl)
else ()
    if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(TOOLS_OPENSSL_CONFIGURE_ARCH "linux-x86_64")
    else ()
        set(TOOLS_OPENSSL_CONFIGURE_ARCH "linux-i386")
    endif ()

    set(TOOLS_OPENSSL_CONFIGURE_COMMAND ./Configure --prefix=${CMAKE_BINARY_DIR}/openssl_patched no-asm no-shared ${TOOLS_OPENSSL_CONFIGURE_ARCH})
    set(TOOLS_OPENSSL_BUILD_COMMAND make)
    set(TOOLS_OPENSSL_INSTALL_COMMAND make install_sw)

    set(TOOLS_OPENSSL_BINARY openssl)
endif ()

set(TOOLS_OPENSSL_BUILD_DIRECTORY ${CMAKE_BINARY_DIR}/build_openssl_patched)

ExternalProject_Add(
        OpenSSLPatched
        URL https://www.openssl.org/source/openssl-1.0.2n.tar.gz
        URL_HASH SHA256=370babb75f278c39e0c50e8c4e7493bc0f18db6867478341a832a982fd15a8fe
        SOURCE_DIR ${TOOLS_OPENSSL_BUILD_DIRECTORY}
        PATCH_COMMAND patch -Np1 -i ${CMAKE_SOURCE_DIR}/tools/openssl-1.0.2n-cades.patch
        CONFIGURE_COMMAND ${TOOLS_OPENSSL_CONFIGURE_COMMAND}
        BUILD_COMMAND ${TOOLS_OPENSSL_BUILD_COMMAND}
        INSTALL_COMMAND ${TOOLS_OPENSSL_INSTALL_COMMAND}
        BUILD_IN_SOURCE 1
)

if (WIN32)
    ExternalProject_Add_Step(
            OpenSSLPatched do
            COMMAND ${TOOLS_OPENSSL_CONFIGURE_DO}
            DEPENDEES configure
            DEPENDERS build
            WORKING_DIRECTORY ${TOOLS_OPENSSL_BUILD_DIRECTORY}
            LOG 1
    )
endif ()

install(PROGRAMS ${CMAKE_BINARY_DIR}/openssl_patched/bin/${TOOLS_OPENSSL_BINARY}
        DESTINATION ${FDOTOOL_TOOLS_INSTALL_DIR}
        COMPONENT OpenSSLPatched)
