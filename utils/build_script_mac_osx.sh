cd argoniumcore
if [ $? -ne 0 ]; then
    echo "Failed to cd argoniumcore"
    exit $?
fi

git pull
if [ $? -ne 0 ]; then
    echo "Failed to git pull"
    exit $?
fi

rm -rf build; mkdir -p build/release; cd build/release;

cmake -D BUILD_GUI=TRUE -D CMAKE_PREFIX_PATH=/Users/roky/Qt/5.3/clang_64 -D CMAKE_BUILD_TYPE=Release ../..
if [ $? -ne 0 ]; then
    echo "Failed to cmake"
    exit $?
fi



make qt-argoniumc
if [ $? -ne 0 ]; then
    echo "Failed to make qt-argoniumc"
    exit $?
fi


cd src/
if [ $? -ne 0 ]; then
    echo "Failed to cd src"
    exit $?
fi

/Users/roky/Qt/5.3/clang_64/bin/macdeployqt qt-argoniumc.app
if [ $? -ne 0 ]; then
    echo "Failed to macdeployqt qt-argoniumc.app"
    exit $?
fi

cp -R ../../../src/gui/qt-daemon/html qt-argoniumc.app/Contents/MacOS
if [ $? -ne 0 ]; then
    echo "Failed to cp html to MacOS"
    exit $?
fi

cp ../../../src/gui/qt-daemon/app.icns qt-argoniumc.app/Contents/Resources
if [ $? -ne 0 ]; then
    echo "Failed to cp app.icns to resources"
    exit $?
fi

zip -r -y "bbr-macos-x64-v0.2.0.zip" qt-argoniumc.app
if [ $? -ne 0 ]; then
    echo "Failed to zip app"
    exit $?
fi

cd ../..
echo "Build success"

