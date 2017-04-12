#!/bin/bash -e

USAGE="build_cmake_project <name> <desired.static.lib> [cmake-args]"

PROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"/..
DEP=$PROOT/dep
DEPINSTALL=$DEP/install
DEPBUILD=$DEP/build
SUBMODULES=$PROOT/gitmodules
TARGETPROJECT=$SUBMODULES/$1

if [[ -z $1 ]]; then
    >&2 echo "the project name is needed as first parameter"
    exit 1
fi

if [[ -z $2 ]]; then
    >&2 echo "the static lib to check for is needed as a second parameter"
    exit 1
fi

if [[ -f $DEPINSTALL/lib/$2 ]]; then
    exit 0
fi

git submodule update --init --recursive

if  [[ ! -d  $TARGETPROJECT ]]; then
    >&2 echo "non existing project: $PROJECTDIR"
    exit 1
fi

BUILDDIR=$DEPBUILD/$1
rm -rf $BUILDDIR
mkdir -p $BUILDDIR
cd $BUILDDIR
cmake -DCMAKE_INSTALL_PREFIX=/dep/install $3 $TARGETPROJECT
make
DESTDIR=$PROOT make install