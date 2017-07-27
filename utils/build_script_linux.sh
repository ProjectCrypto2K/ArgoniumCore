#!/bin/bash

QT_PREFIX_PATH="$HOME/Qt/5.3/gcc_64"
QT_BINARIES_PATH="$HOME/bbr_binaries"


cd argoniumcore
prj_root=$(pwd)

git pull
if [ $? -ne 0 ]; then
    echo "Failed to pull"
    exit $?
fi

echo "---------------- BUILDING PROJECT ----------------"
echo "--------------------------------------------------"

echo "Backupping wallet files(on the off-chance)"
cp -v build/release/src/*.bin build/release/src/*.bin.keys build/release/src/*.bbr build/release/src/*.bbr.keys build/release/src/*.bbr.address.txt build/release/src/*.bin.address.txt ..

echo "Building...." 
rm -rf build; mkdir -p build/release; cd build/release; 
cmake -D STATIC=true -D BUILD_GUI=TRUE -D CMAKE_PREFIX_PATH="$QT_PREFIX_PATH" -D CMAKE_BUILD_TYPE=Release ../..
if [ $? -ne 0 ]; then
    echo "Failed to run cmake"
    exit 1
fi

make daemon qt-argoniumc simplewallet simpleminer connectivity_tool;
if [ $? -ne 0 ]; then
    echo "Failed to make!"
    exit 1
fi

 
read version_str <<< $(./src/argoniumcd --version | awk '/^Argoniumcore / { print $2 }')
echo $version_str


mkdir -p argoniumcore;

cp -Rv ../../src/gui/qt-daemon/html ./argoniumcore
cp -Rv ../../utils/qt-argoniumc.sh ./argoniumcore
cp -Rv $QT_BINARIES_PATH/libs ./argoniumcore
cp -Rv $QT_BINARIES_PATH/libs ./argoniumcore
cp -Rv src/argoniumcd src/qt-argoniumc src/simplewallet src/simpleminer src/connectivity_tool ./argoniumcore


tar -cjvf bbr-linux-x64-$version_str.tar.bz2 argoniumcore
if [ $? -ne 0 ]; then
    echo "Failed to pack"
    exit 1
fi



echo "Build success"

exit 0
