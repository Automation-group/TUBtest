cmake_minimum_required(VERSION 2.8)

project (QlfDevices)

find_package(Qt4 REQUIRED)

if(QT4_FOUND)
  include(${QT_USE_FILE})
  include(lfQT4Utils.cmake)
else(QT4_FOUND)
  message(FATAL_ERROR "Could not find Qt4")
endif(QT4_FOUND)

include_directories(include)

link_libraries(
  ${QT_QTCORE_LIBRARY}
  )

add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_KEYWORDS)

aux_source_directory (src SOURCES)

file(GLOB HEADERS src/*.h)

qt4_wrap_cpp(mocSOURCES ${HEADERS})

add_library(qlfdevices STATIC ${SOURCES} ${mocSOURCES})
