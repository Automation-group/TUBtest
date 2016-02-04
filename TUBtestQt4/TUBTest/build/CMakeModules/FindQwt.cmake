find_path(QWT_INCLUDE_DIR qwt.h
/usr/include/qwt-qt4
${QWT_INCLUDE_DIR}
${QWT_DIR}
)

if(NOT QWT_INCLUDE_DIR)
  message(FATAL_ERROR "Qwt not found. You may get it on http://qwt.sourceforge.net/. Use cmake -DQWT_DIR=QwtSrcDir")
endif(NOT QWT_INCLUDE_DIR)

if(WIN32)
  set(QWT_LIB_NAME qwt5)
else(WIN32)
  set(QWT_LIB_NAME qwt-qt4)
endif(WIN32)

find_library(QWT_LIBRARY ${QWT_LIB_NAME}
${QWT_LIB_DIR}
${QWT_DIR}/build
${QWT_DIR}/lib
)

if(NOT QWT_LIBRARY)
  message(FATAL_ERROR "Qwt library not found. You may get it on http://qwt.sourceforge.net/. Use cmake -DQWT_DIR=QwtSrcDir")
endif(NOT QWT_LIBRARY)
