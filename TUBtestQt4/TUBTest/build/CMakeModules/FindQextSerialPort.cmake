find_path(QEXTSERIALPORT_INCLUDE_DIR qextserialport.h
if(UNIX)
../../qextserialport/src
endif(UNIX)
if(WIN32)
../../qextserialport_for_windows/src
endif(WIN32)
${QEXTSERIALPORT_INCLUDE_DIR}
${QEXTSERIALPORT_DIR}
)

if(NOT QEXTSERIALPORT_INCLUDE_DIR)
  message(FATAL_ERROR "QextSerialPort not found. Use cmake -DQEXTSERIALPORT_DIR=QExtSerialPortDir")
endif(NOT QEXTSERIALPORT_INCLUDE_DIR)

find_library(QEXTSERIALPORT_LIBRARY qextserialport
if(UNIX)
../../qextserialport/src/build
endif(UNIX)
if(WIN32)
../../qextserialport_for_windows/src/build
endif(WIN32)
${QEXTSERIALPORT_LIB_DIR}
${QEXTSERIALPORT_DIR}
)

if(NOT QEXTSERIALPORT_LIBRARY)
  message(FATAL_ERROR "QextSerialPort library not found. Use cmake -DQEXTSERIALPORT_DIR=QExtSerialPortDir")
endif(NOT QEXTSERIALPORT_LIBRARY)
