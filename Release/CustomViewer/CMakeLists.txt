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
    src/devicetilewidget.cpp
    src/devicetilewidget.h
    src/devicedetailview.cpp
    src/devicedetailview.h
    src/efficiencycategorybutton.cpp
    src/efficiencycategorybutton.h
    src/historyview.cpp
    src/historyview.h
    src/viewerconnection.cpp
    src/viewerconnection.h
    src/frameprotocol.cpp
    src/frameprotocol.h
)

# Original Kickidler History assets (qml_real/desktop/View/history) and the
# Roboto faces embedded in the real viewer.exe, so History renders with the
# exact same images/fonts instead of hand-drawn approximations.
qt_add_resources(PersonalScreenViewer "history_assets"
    PREFIX "/"
    BASE resources
    FILES
        resources/fonts/Roboto-Regular.ttf
        resources/fonts/Roboto-Medium.ttf
        resources/fonts/Roboto-Bold.ttf
        resources/history/sliderBar/bg_none.png
        resources/history/sliderBar/bg_loaded_cropped.png
        resources/history/sliderBar/bg_online_cropped.png
        resources/history/sliderBar/pick_normal.png
        resources/history/sliderBar/pick_hovered.png
        resources/history/sliderBar/pick_pressed.png
        resources/history/bg_play.png
        resources/history/bg_audio.png
        resources/history/play_normal.png
        resources/history/play_active.png
        resources/history/pause_normal.png
        resources/history/pause_active.png
        resources/history/volume_on.png
        resources/history/volume_off.png
        resources/history/avi_export_normal.png
        resources/history/avi_export_hovered.png
        resources/history/avi_export_pressed.png
        resources/history/speedButton/speed_bg.png
        resources/history/speedButton/speed_bg_hovered.png
        resources/history/speedButton/speed_bg_pressed.png
        resources/history/speedButton/triangle_normal.png
        resources/history/speedButton/triangle_hovered.png
        resources/history/speedButton/triangle_pressed.png
        resources/history/iconButtonViolation/arrow_up.png
        resources/history/iconButtonViolation/arrow_down.png
        resources/history/keylogger_bg.png
        resources/history/big_emptyStream.png
        resources/history/warning.png
        resources/history/info.png
        resources/history/spinner/spinner.png
        resources/history/video/buttonRunningApp/applications_arrow_left.png
        resources/history/video/buttonRunningApp/applications_arrow_right.png
        resources/sessionInfo/images/active_program.png
        resources/sessionInfo/categorizationButton/small_normal.png
        resources/sessionInfo/categorizationButton/small_hovered.png
        resources/sessionInfo/categorizationButton/small_pressed.png
        resources/sessionInfo/categorizationPanel/indent.png
        resources/sessionInfo/categorizationPanel/multiuser_on.png
        resources/sessionInfo/categorizationPanel/user_on.png
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

