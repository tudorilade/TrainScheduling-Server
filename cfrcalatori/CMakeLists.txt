cmake_minimum_required(VERSION 3.5)

project(qtcfrcalatori VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH /home/tudor/Qt/6.4.1/gcc_64/lib/cmake)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        main.h
        ClientController/client_controller.h
        ClientController/client_controller.cpp
        ClientThreadFactory/thread_factory.cpp
        ClientThreadFactory/thread_factory.h
        Requests/RequestsController/requests_controller.cpp
        Requests/RequestsController/requests_controller.h
        CommandControl/CommandControlManager/command_control_manager.cpp
        CommandControl/CommandControlManager/command_control_manager.h
        CommandControl/CommandControl/commands.cpp CommandControl/CommandControl/commands.h
        CommandControl/CommandControl/command_ABC.h
        CommandControl/CommandControl/command_factory.cpp
        CommandControl/CommandControl/command_factory.h
        XmlController/xmlcontrollerr.h
        XmlController/xmlcontrollerr.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qtcfrcalatori
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qtcfrcalatori APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qtcfrcalatori SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qtcfrcalatori
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)

set_target_properties(qtcfrcalatori PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

find_package(Threads REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Xml)
target_link_libraries(qtcfrcalatori PRIVATE Qt6::Xml)
target_link_libraries(qtcfrcalatori PRIVATE Threads::Threads)
target_link_libraries(qtcfrcalatori PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS qtcfrcalatori
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qtcfrcalatori)
endif()
