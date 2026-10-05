cmake_minimum_required(VERSION 3.21)

project(KikiHost VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Network Sql)

qt_add_executable(KikiHost
    src/main.cpp
    src/servicehost.cpp
    src/servicehost.h
    src/sessionmanager.cpp
    src/sessionmanager.h
    src/sessioningest.cpp
    src/sessioningest.h
    src/historyrecorder.cpp
    src/historyrecorder.h
    src/historyframereader.cpp
    src/historyframereader.h
    src/vp8codec.cpp
    src/vp8codec.h
    src/agentconnection.cpp
    src/agentconnection.h
    src/agentserver.cpp
    src/agentserver.h
    src/frameprotocol.cpp
    src/frameprotocol.h
)

target_link_libraries(KikiHost PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
)

# Vendored libvpx (VP8) for the History video storage -- see
# third_party/libvpx/README.md. Static lib built with this same MinGW GCC.
set(LIBVPX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libvpx")
target_include_directories(KikiHost PRIVATE "${LIBVPX_DIR}/include")
target_link_libraries(KikiHost PRIVATE "${LIBVPX_DIR}/lib/libvpx.a")
find_package(Threads REQUIRED)
target_link_libraries(KikiHost PRIVATE Threads::Threads)

if(WIN32)
    target_link_libraries(KikiHost PRIVATE wtsapi32 userenv advapi32)
endif()

target_compile_definitions(KikiHost PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
)

if(MSVC)
    target_compile_options(KikiHost PRIVATE /W4 /permissive-)
else()
    target_compile_options(KikiHost PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS KikiHost
    RUNTIME DESTINATION .
)
