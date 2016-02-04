cmake_minimum_required(VERSION 2.8)

project(qextserialport)

#set(qextserialport_VERSION_MAJOR 1)
#set(qextserialport_VERSION_MINOR 2)

find_package(Qt4 REQUIRED QtCore)
include(${QT_USE_FILE})

add_definitions(-DQEXTSERIALPORT_LIB)
set(QEXTSERIALPORT_LIB_NAME qextserialport)

add_subdirectory(src)
add_subdirectory(examples)
