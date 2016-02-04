project(event)

set(HEADERS
  PortListener.h
  )

set(SOURCES
  main.cpp 
  PortListener.cpp
  )

qt4_wrap_cpp(mocSOURCES ${HEADERS})

include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(event ${SOURCES} ${mocSOURCES} ${HEADERS})
target_link_libraries(event ${QEXTSERIALPORT_LIB_NAME} ${QT_LIBRARIES})
