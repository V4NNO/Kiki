cmake_minimum_required(VERSION 3.21)

project(KikiViewer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_add_executable(KikiViewer WIN32
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/monitorwidget.cpp
    src/monitorwidget.h
    src/devicetilewidget.cpp
    src/devicetilewidget.h
    src/devicedetailview.cpp
    src/devicedetailview.h
    src/historyview.cpp
    src/historyview.h
    src/historysegmentstore.cpp
    src/historysegmentstore.h
    src/vp8decoder.cpp
    src/vp8decoder.h
    src/viewerconnection.cpp
    src/viewerconnection.h
    src/frameprotocol.cpp
    src/frameprotocol.h
)

# Original Kickidler assets (copied from qml_real) and the Roboto faces
# embedded in the real viewer.exe, so the UI renders with the exact same
# images/fonts instead of hand-drawn approximations.
file(GLOB_RECURSE VIEWER_ASSETS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.ttf")
qt_add_resources(KikiViewer "viewer_assets"
    PREFIX "/"
    BASE resources
    FILES ${VIEWER_ASSETS}
)

target_link_libraries(KikiViewer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

# Vendored libvpx (VP8), the same static lib KikiHost encodes with. History
# video arrives as VP8 runs and is decoded here, which is how the original
# Kickidler viewer works too (viewer.exe carries libvpx's VP8 decoder).
set(LIBVPX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/libvpx")
target_include_directories(KikiViewer PRIVATE "${LIBVPX_DIR}/include")
target_link_libraries(KikiViewer PRIVATE "${LIBVPX_DIR}/lib/libvpx.a")
find_package(Threads REQUIRED)
target_link_libraries(KikiViewer PRIVATE Threads::Threads)

target_compile_definitions(KikiViewer PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
)

if(MSVC)
    target_compile_options(KikiViewer PRIVATE /W4 /permissive-)
else()
    target_compile_options(KikiViewer PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS KikiViewer
    RUNTIME DESTINATION .
    BUNDLE DESTINATION .
)

