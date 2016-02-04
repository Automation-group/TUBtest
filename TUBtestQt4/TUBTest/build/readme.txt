# Сборка проекта
# 1. Необходимо получить и собрать QExtSerialPort
# linux
cd /tmp
hg clone http://bitbucket.org/lexaficus/qextserialport
cd qextserialport
qmake-qt4
make -j4
# windows
cd qextserialport\src
qmake
mingw32-make -j4

# 2. Необходимо получить и собрать QlfDevices
# linux
cd /tmp
hg clone http://bitbucket.org/lexaficus/qlfdevices
cd qlfdevices
mkdir build
cd build
cmake ..
make -j4
# windows
cd qlfdevices
cmake -G "MinGW Makefiles"
mingw32-make -j4

# 3. Собрать TUBTest
# linux
cd /tmp 
svn co https://srvlab5l/svn/aapavlyukov/Релаксация/RxStep/trunk/RxStep
cd TUBTest
mkdir build
cd build
cmake -DQLFDEVICES_DIR=../../qlfdevices/ -DQEXTSERIALPORT_DIR=../../qextserialport/ .
make -j4
# windows
cd TUBtest
cmake -G "MinGW Makefiles"
mingw32-make -j4
