# Название проекта
set(PROJECT TUBTest)

# Требования по сборке
cmake_minimum_required(VERSION 2.8)

# Подключение модулей для CMake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

# Структура проекта
aux_source_directory(../src SOURCES)
file(GLOB HEADERS ../include/*.h)
file(GLOB QTUI ../ui/*.ui)
file(GLOB QTRESOURCES ../rc/*.qrc)
file(GLOB WIN_RESOURCES ../rc/*.rc)

# Поиск библиотек
find_package (Qt4 COMPONENTS QtCore QtGui)
find_package(Qwt REQUIRED)
find_package(lfDevices REQUIRED)
find_package(QextSerialPort REQUIRED)

# Подключение библиотек
include (${QT_USE_FILE})
include_directories(${QWT_INCLUDE_DIR} )
include_directories(${QLFDEVICES_INCLUDE_DIR} )
include_directories(${QEXTSERIALPORT_INCLUDE_DIR})

# Условия для сборки
if(WIN32)
  add_definitions(-D_TTY_WIN_)
else(WIN32)
  add_definitions(-D_TTY_POSIX_)
endif(WIN32)

# Сборка бислиотек
link_libraries(
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QWT_LIBRARY}
  ${QLFDEVICES_LIBRARY}
  ${QEXTSERIALPORT_LIBRARY}
)

# Генерация *.moc и *.h файлов
qt4_wrap_cpp(mocSOURCES ${HEADERS})
qt4_wrap_ui(UI ${QTUI})
qt4_add_resources(RESOURCES ${QTRESOURCES})

# Сборка проекта
add_executable(${PROJECT} ${SOURCES} ${mocSOURCES} ${TRANSLATIONS}
			  ${UI} ${RESOURCES} ${WIN_RESOURCES})
			   
# Линковка проекта
target_link_libraries(${PROJECT} ${QT_LIBRARIES})

# Cборка пакета .deb (нужно написать)
#ADD_EXECUTABLE(TUBTest ${SOURCES} ${mocSOURCES} ${RESOURCES} ${UI})


#INSTALL(TARGETS TUBTest
#    RUNTIME DESTINATION bin
#    PERMISSIONS OWNER_EXECUTE OWNER_READ
#                GROUP_EXECUTE GROUP_READ
#                WORLD_EXECUTE WORLD_READ)

#FIND_PACKAGE(Subversion)
#IF(Subversion_FOUND)
#    Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
#    MESSAGE("Current revision is ${Project_WC_REVISION}")
#ENDIF(Subversion_FOUND)

#CPack section

#IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
#    SET(CPACK_GENERATOR "DEB")
#    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Lab5 TUB test program.")
#    SET(CPACK_PACKAGE_VENDOR "lab5")
#    SET(CPACK_PACKAGE_VERSION_MAJOR "0")
#    SET(CPACK_PACKAGE_VERSION_MINOR "1")
#    SET(CPACK_PACKAGE_VERSION_PATCH "${Project_WC_REVISION}")
#    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "TUBTest ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
#    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4-gui (>= 4.4.0-4)")
#    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "lexaficus@list.ru")
    # List executables
#    INCLUDE(CPack)
#ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
