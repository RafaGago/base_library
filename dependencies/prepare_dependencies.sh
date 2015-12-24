#!/bin/sh

cwd=$(pwd)

premake=$cwd/code/premake-5.0.0-alpha6
premake_dest=$cwd/../build/premake
cd code
unzip premake-5.0.0-alpha6-src.zip
cd $premake/build/gmake.unix
make -j4 config=release all
rm -rf $cwd/../build/premake/premake5
mv $premake/bin/release/premake5 $cwd/../build/premake
rm -rf $premake
cd $cwd

cmocka=$cwd/code/cmocka-1.0.1
cmocka_dest=$cwd/cmocka
cd code
tar -xvf cmocka-1.0.1.tar.xz
cd $cmocka
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$cmocka_dest -DCMAKE_BUILD_TYPE=Debug -DWITH_STATIC_LIB=On ..
make -j 4
rm -rf $cmocka_dest
make install
rm -rf $cmocka
rm -rf $cmocka_dest/lib/*.so*
cd $cwd

if [ -f $premake_dest/premake5 ] && [ -f $cmocka_dest/lib/libcmocka.a ]; then
  echo "\ndependencies built correctly"
else
  echo "\none or both of the dependencies failed to build"
fi

