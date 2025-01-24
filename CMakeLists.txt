cmake_minimum_required(VERSION 3.21)

project(Auto_Sort LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE_FILES main.cpp)

find_package(pigpio REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
#find_package(OpenCV REQUIRED core imgproc highgui)
find_package(OpenCV REQUIRED)

include_directories(${pigpio_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(Auto_Sort main.cpp detection/detecting.h detection/ImageUtils.h detection/mainwindow.h
        detection/object_detect.h detection/detecting.cpp detection/ImageUtils.cpp
        detection/mainwindow.cpp detection/object_detect.cpp motor/motor_control.cpp
        motor/motor_control.h motor/motorDriver.cpp motor/motorDriver.h motor/thread_motor.cpp motor/thread_motor.h)

target_link_libraries(Auto_Sort pigpio)
target_link_libraries(Auto_Sort ${OpenCV_LIBS})
target_link_libraries(Auto_Sort
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )
# Add directory for unit test
#add_subdirectory(unit_tests)
include_directories(motor)
#add_subdirectory(motor)
#target_link_libraries(Auto_Sort motor Google_tests)


# Qt
if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

# Support for Google tests
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        unit_test
        unit_tests/unit_test.cpp
        detection/detecting.h detection/ImageUtils.h detection/mainwindow.h
        detection/object_detect.h detection/detecting.cpp detection/ImageUtils.cpp
        detection/mainwindow.cpp detection/object_detect.cpp motor/motor_control.cpp
        motor/motor_control.h motor/motorDriver.cpp motor/motorDriver.h motor/thread_motor.cpp motor/thread_motor.h
)
target_link_libraries(
        unit_test
        gtest_main
        ${OpenCV_LIBS}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        pigpio
)

include(GoogleTest)
gtest_discover_tests(unit_test)