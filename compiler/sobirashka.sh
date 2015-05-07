#!/bin/bash -e

SOURCELANG="$1"
SOURCEASM="${SOURCELANG%.lang}.vmasm"
BASEDIR=`dirname -- $0`
COMPILER="$BASEDIR/compiler"
ASM="$BASEDIR/asm.py"

printf -- "Compiling \"$SOURCELANG\"...\n"
$COMPILER < "$SOURCELANG" > "$SOURCEASM"
printf -- "Assembling \"$SOURCEASM\"...\n"
$ASM "$SOURCEASM"

if [[ "x$2" != "x--keep-asm" ]]
then
    printf -- "Removing \"$SOURCEASM\"...\n"
    rm -f -- "$SOURCEASM"
fi

printf -- "Sobirashka OK!\n"

