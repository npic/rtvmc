#!/bin/sh

GNUPLOT="gnuplot --persist"
SCRIPT="set terminal qt size 1800,900\\nset multiplot layout $#,1"

for file in "$@"
do
    SCRIPT="${SCRIPT}\\nplot \"${file}\" with lines"
done

echo -e "$SCRIPT" | $GNUPLOT

