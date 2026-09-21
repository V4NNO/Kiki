cmake_minimum_required(VERSION 3.21)

project(PersonalScreenAgent VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_add_executable(PersonalScreenAgent WIN32
    src/main.cpp
    src/agentwindow.cpp
    src/agentwindow.h
    src/agentconnection.cpp
    src/agentconnection.h
    src/agentserver.cpp
    src/agentserver.h
    src/screencapture.cpp
    src/screencapture.h
    src/activityprobe.cpp
    src/activityprobe.h
    src/indicatoroverlay.cpp
    src/indicatoroverlay.h
    src/frameprotocol.cpp
    src/frameprotocol.h
)

target_link_libraries(PersonalScreenAgent PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

if(WIN32)
    target_link_libraries(PersonalScreenAgent PRIVATE user32)
endif()

target_compile_definitions(PersonalScreenAgent PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
)

if(MSVC)
    target_compile_options(PersonalScreenAgent PRIVATE /W4 /permissive-)
else()
    target_compile_options(PersonalScreenAgent PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS PersonalScreenAgent
    RUNTIME DESTINATION .
    BUNDLE DESTINATION .
)
