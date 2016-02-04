#!/bin/bash

# built qextserialport
cd ../../qextserialport/src/
qmake-qt4
make -j8

#build qlfDevise
cd ../../qlfdevices/
cmake .
make -j8

#build TUBtest
cd ../TUBTest/build/
cmake -DQLFDEVICES_DIR=../../qlfdevices/ -DQEXTSERIALPORT_DIR=../../qextserialport/
make -j8