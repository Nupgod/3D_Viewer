cmake_minimum_required(VERSION 3.5)

project(viewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_compile_options(-Wall -Wextra -Werror)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
        main.cc
        mainwindow.cc
        mainwindow.h
        mainwindow.ui
        scene.h
        scene.cc
        model/Parcer.cc
        model/Parcer.h
        model/Transformator.cc
        model/Transformator.h
        controller/controller.cc
        controller/controller.h
        gif/gif.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        shader.qrc
    )
else()
    add_executable(viewer
        ${PROJECT_SOURCES}
        shader.qrc
    )
endif()

target_link_libraries(viewer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Gui
)

set_target_properties(viewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(viewer)
endif()
