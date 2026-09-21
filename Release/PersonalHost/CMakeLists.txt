cmake_minimum_required(VERSION 3.21)

project(PersonalHost VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Network Sql)

qt_add_executable(PersonalHost
    src/main.cpp
    src/servicehost.cpp
    src/servicehost.h
    src/sessionmanager.cpp
    src/sessionmanager.h
    src/sessioningest.cpp
    src/sessioningest.h
    src/historyrecorder.cpp
    src/historyrecorder.h
    src/agentconnection.cpp
    src/agentconnection.h
    src/agentserver.cpp
    src/agentserver.h
    src/frameprotocol.cpp
    src/frameprotocol.h
)

target_link_libraries(PersonalHost PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Sql
)

if(WIN32)
    target_link_libraries(PersonalHost PRIVATE wtsapi32 userenv advapi32)
endif()

target_compile_definitions(PersonalHost PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
)

if(MSVC)
    target_compile_options(PersonalHost PRIVATE /W4 /permissive-)
else()
    target_compile_options(PersonalHost PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS PersonalHost
    RUNTIME DESTINATION .
)
