#!/bin/bash

# Generates a meson crossbuild file for Linux x64 from an enviroment that has
# sourced a Yocto SDK. Then this file is to be used as "meson --cross-file" from
# a clean environment.

echo "[binaries]"
echo "c = '"$(which $(echo $CC | cut -d ' ' -f 1))"'"
echo "cpp = '"$(which $(echo $CXX | cut -d ' ' -f 1))"'"
echo "strip = '"$(which $(echo $STRIP | cut -d ' ' -f 1))"'"
echo "ar = '"$(which $(echo $AR | cut -d ' ' -f 1))"'"

echo ""
echo "[properties]"
echo "has_function_printf = true"

echo -n "c_args = ["
for ARG in $(echo $CC | cut -d ' ' -f 1 --complement); do
    echo -n "'$ARG', "
done
echo "]"

echo -n "c_link = ["
for ARG in $(echo $LD | cut -d ' ' -f 1 --complement); do
    echo -n "'$ARG', "
done
echo "]"

CCOUT=$($CC -v 2>&1 | tr ' ' '\n')
CPU=$(echo "$CCOUT" | grep "with-arch" | cut -d '=' -f 2)
TARGETSTR=$(echo "$CCOUT" | grep '^\-\-target=' | cut -d '=' -f 2)
CPU_FAMILY=$(echo "$TARGETSTR" | cut -d '-' -f 1)

echo ""
echo "[target_machine]"
echo "system = 'linux'"
echo "endian = 'little'"
echo "cpu_family = '$CPU_FAMILY'"
echo "cpu = '$CPU'"

CCOUT=$(cc -v 2>&1 | tr ' ' '\n')
TARGETSTR=$(echo "$CCOUT" | grep '^\-\-target=' | cut -d '=' -f 2)
CPU_FAMILY=$(echo "$TARGETSTR" | cut -d '-' -f 1)

echo ""
echo "[host_machine]"
echo "system = 'linux'"
echo "endian = 'little'"
echo "cpu_family = '$CPU_FAMILY'"
echo "cpu = 'i686'"