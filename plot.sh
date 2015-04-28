#!/bin/sh

GNUPLOT="gnuplot --persist"
SCRIPT=

SCRIPT="set multiplot layout $#,1"

for file in "$@"
do
    SCRIPT="${SCRIPT}\\nplot \"${file}\" with lines"
done

echo -e "$SCRIPT" | $GNUPLOT

