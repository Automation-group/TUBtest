if(UNIX)
find_path(QWT_INCLUDE_DIR qwt.h
../../qwt-5.2.3/src/
${QWT_INCLUDE_DIR}
${QWT_DIR}
)
endif(UNIX)

if(WIN32)
find_path(QWT_INCLUDE_DIR NAMES qwt.h PATHS
#/usr/include
#/usr/local/include
../../qwt-5.2.3/src/
"$ENV{LIB_DIR}/include"
"$ENV{INCLUDE}"
PATH_SUFFIXES qwt-qt4 qwt qwt5 qwt6
)
endif(WIN32)

if(NOT QWT_INCLUDE_DIR)
  message(FATAL_ERROR "Qwt not found. You may get it on http://qwt.sourceforge.net/. Use cmake -DQWT_DIR=QwtSrcDir")
endif(NOT QWT_INCLUDE_DIR)

if(WIN32)
if(ENABLE_QT5)
  set(QWT_LIBRARY_NAMES qwt-qt5 qwt6-qt5)
else(ENABLE_QT5)
  set(QWT_LIBRARY_NAMES qwt qwt6 qwt5 qwt-qt4 qwt6-qt4 qwt5-qt4)
endif(ENABLE_QT5)
endif(WIN32)

if(UNIX)
find_library(QWT_LIBRARY ${QWT_LIB_NAME}
../../qwt-6.1.0/
${QWT_LIB_DIR}/lib
${QWT_DIR}
)
endif(UNIX)

if(WIN32)
find_library(QWT_LIBRARY NAMES ${QWT_LIBRARY_NAMES}
PATHS
#/usr/lib
#/usr/local/lib
../../qwt-5.2.3/lib
"$ENV{LIB_DIR}/lib"
"$ENV{LIB}"
)
endif(WIN32)

if(NOT QWT_LIBRARY)
  message(FATAL_ERROR "Qwt library not found. You may get it on http://qwt.sourceforge.net/. Use cmake -DQWT_DIR=QwtSrcDir")
endif(NOT QWT_LIBRARY)
