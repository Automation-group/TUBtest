project(qespta)

set(HEADERS
  MainWindow.h
  MessageWindow.h
  QespTest.h
  )

set(SOURCES
  main.cpp 
  MainWindow.cpp
  MessageWindow.cpp
  QespTest.cpp
  )

if(WIN32)
  add_definitions(-D_TTY_WIN_)
else(WIN32)
  add_definitions(-D_TTY_POSIX_)
endif(WIN32)

qt4_wrap_cpp(mocSOURCES ${HEADERS})

include_directories(${QT_QTGUI_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src)
add_executable(qespta ${SOURCES} ${mocSOURCES} ${HEADERS})
target_link_libraries(qespta ${QEXTSERIALPORT_LIB_NAME} ${QT_LIBRARIES} ${QT_QTGUI_LIBRARY})
