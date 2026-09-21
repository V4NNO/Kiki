cmake_minimum_required(VERSION 3.21)

project(PersonalScreenViewer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_add_executable(PersonalScreenViewer WIN32
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/monitorwidget.cpp
    src/monitorwidget.h
    src/viewerconnection.cpp
    src/viewerconnection.h
    src/frameprotocol.cpp
    src/frameprotocol.h
)

target_link_libraries(PersonalScreenViewer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

target_compile_definitions(PersonalScreenViewer PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
)

if(MSVC)
    target_compile_options(PersonalScreenViewer PRIVATE /W4 /permissive-)
else()
    target_compile_options(PersonalScreenViewer PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS PersonalScreenViewer
    RUNTIME DESTINATION .
    BUNDLE DESTINATION .
)

