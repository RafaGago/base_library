#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "usage: list_files.sh <file-extension>"
fi
echo '['
find . -type f -name "*.$1" | sed "s|\./|    \'|g" | sed "s|\.$1$|.$1\',|g"
echo ']'