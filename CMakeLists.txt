cmake_minimum_required(VERSION 3.5)

project(fssp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES translations/fssp_ru_RU.ts)

qt_create_translation(QM_FILES ${PROJECT_SOURCE_DIR} ${TS_FILES})

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/signaldata.cpp
        src/signaldata.h
        src/signalpage.cpp
        src/signalpage.h
        src/signalbuilder.cpp
        src/signalbuilder.h
        src/basedeserializer.h
        src/txtdeserializer.cpp
        src/txtdeserializer.h
        src/navigationdialog.cpp
        src/navigationdialog.h
        src/navigationwaveform.cpp
        src/navigationwaveform.h
        src/graphdialog.cpp
        src/graphdialog.h
        src/graphwaveform.cpp
        src/graphwaveform.h
        src/basemodel.cpp
        src/basemodel.h
        src/signalmodels.cpp
        src/signalmodels.h
        src/basewaveform.cpp
        src/basewaveform.h
        src/modelingwindow.cpp
        src/modelingwindow.h
        src/modelingwaveform.cpp
        src/modelingwaveform.h
        src/statisticwindow.cpp
        src/statisticwindow.h
        src/basespectrum.cpp
        src/basespectrum.h
        src/spectrumwindow.cpp
        src/spectrumwindow.h
        src/spectrumwaveform.cpp
        src/spectrumwaveform.h
        ${QM_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(fssp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET fssp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(fssp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(fssp
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(fssp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(fssp PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS fssp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(fssp)
endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
