#!/bin/bash -e
USAGE="build_cmake_project <project root> <project name> <desired lib> [cmake-args]"

PROOT=$1
DEP=$PROOT/dep
DEPINSTALL=$DEP/install
DEPBUILD=$DEP/build
SUBMODULES=$PROOT/gitmodules
TARGETPROJECT=$SUBMODULES/$2

if [[ -z $PROOT ]]; then
    >&2 echo "the project root is needed as the first parameter"
    exit 1
fi

if [[ -z $2 ]]; then
    >&2 echo "the project to build is needed as the second parameter"
    exit 1
fi

if [[ -z $3 ]]; then
    >&2 echo "the target lib to build is needed as the third parameter"
    exit 1
fi

if [[ ! -d $PROOT ]]; then
    >&2 echo "project root: can't access folder $PROOT"
    exit 1
fi

if [[ ! -d $TARGETPROJECT ]]; then
    >&2 echo "project name: can't access folder $TARGETPROJECT"
    exit 1
fi

if [[ -f $DEPINSTALL/lib/$3 ]]; then
    exit 0
fi

if [[ ! -f $TARGETPROJECT/CMakeLists.txt ]]; then
    git submodule update --init --recursive
fi

if [[ ! -f $TARGETPROJECT/CMakeLists.txt ]]; then
    >&2 echo "not found: $TARGETPROJECT/CMakeLists.txt"
fi

BUILDDIR=$DEPBUILD/$1
rm -rf $BUILDDIR
mkdir -p $BUILDDIR
cd $BUILDDIR
cmake -DCMAKE_INSTALL_PREFIX=/dep/install $4 $TARGETPROJECT
make
DESTDIR=$PROOT make install
