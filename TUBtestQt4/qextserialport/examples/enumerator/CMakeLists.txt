project(enumerator)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(enumerator main.cpp)
target_link_libraries(enumerator ${QEXTSERIALPORT_LIB_NAME} ${QT_LIBRARIES})
