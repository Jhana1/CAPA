#!/bin/bash

VAR=$($CXX -E -xc++ - -v < /dev/null 2>&1 >/dev/null | sed -n -e '/<...>/,$p' | sed -e '/End/,$d' | tail -n +2)
OUT=""

while read -r line; do
    OUT=" $OUT -I$line "
done <<< "$VAR"

echo "$OUT"

export VAR
