cmake_minimum_required(VERSION 3.16)
project(pushbullet_kde VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)
find_package(KF5Notifications)

add_executable(pushbullet_kde 
    LoginWindow.cpp
    RegisterDeviceDialog.cpp
    WebSocketListener.cpp
    loginwindow.ui
    main.cpp
    mainwindow.cpp mainwindow.ui
    newdevice.ui
    pushbulletcontroller.cpp
)
target_include_directories(pushbullet_kde PRIVATE
    /usr/include
    /usr/include/KF5
    /usr/include/KF5/KNotifications
    /usr/include/curl
    /usr/include/jsoncpp
    /usr/include/qt5/QtDBus
)

target_link_libraries(pushbullet_kde PRIVATE
    Qt::Core
    Qt::Gui
    KF5::Notifications
)

target_compile_options(pushbullet_kde PUBLIC
    -fpermissive
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(pushbullet_kde PRIVATE
        Qt::Widgets
    )
endif()

if(UNIX AND NOT MACOS)
    target_link_libraries(pushbullet_kde PRIVATE
        # Remove: L/usr/lib64/
        KF5Notifications
        Qt5DBus
        curl
        jsoncpp
        websockets
    )
endif()

install(TARGETS pushbullet_kde
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
