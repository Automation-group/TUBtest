:: this commets in bat files
:: builds for windows

:: built qextserialport
cd ../../qextserialport/src/
qmake
mingw32-make -j8

:: build qlfDevise
cd ../../qlfdevices/
cmake -G "MinGW Makefiles"
mingw32-make -j8

:: build QWT
cd ../qwt-5.2.3/
qmake qwt.pro
mingw32-make -j8

:: build TUBtest
cd ../TUBTest/build/
cmake -G "MinGW Makefiles"
mingw32-make -j8