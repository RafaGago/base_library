#!/bin/bash

# check that we don't export symbols not starting by "bl_". Notice that this
# requires release builds.

staticlibs_prefix=$1
symbol_prefix=$2

ret=0
for f in $(ls "$staticlibs_prefix"*".a"); do
  echo -n "$f: "
  files=$(nm "$f" | grep " t " | grep -v " $symbol_prefix" | grep -v " \.")
  if [[ ! -z "$files" ]]; then
    echo "ERR: "
    echo "$files"
    ret=1
  else
    echo "OK"
  fi
done

exit $ret
