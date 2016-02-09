find_path(QEXTSERIALPORT_INCLUDE_DIR qextserialport.h
../../qextserialport/src
${QEXTSERIALPORT_INCLUDE_DIR}
${QEXTSERIALPORT_DIR}
)

if(NOT QEXTSERIALPORT_INCLUDE_DIR)
  message(FATAL_ERROR "QextSerialPort not found. Use cmake -DQEXTSERIALPORT_DIR=QExtSerialPortDir")
endif(NOT QEXTSERIALPORT_INCLUDE_DIR)

find_library(QEXTSERIALPORT_LIBRARY qextserialport
../../qextserialport/src/build
${QEXTSERIALPORT_LIB_DIR}
${QEXTSERIALPORT_DIR}
)

if(NOT QEXTSERIALPORT_LIBRARY)
  message(FATAL_ERROR "QextSerialPort library not found. Use cmake -DQEXTSERIALPORT_DIR=QExtSerialPortDir")
endif(NOT QEXTSERIALPORT_LIBRARY)
