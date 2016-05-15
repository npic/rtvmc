#! /bin/sh

cd $(dirname $0)

ASM=../../compiler/asm.py
ANSWER=./answer.tmp

for i in *.vmasm
do
    echo -n "Checking $i... "
    $ASM "$i" 2> /dev/null
    mv -f -- *.bin "$ANSWER" 2>/dev/null || echo -n "" > "$ANSWER"
    diff --brief "$i.pat" "$ANSWER" || exit 1
    echo "OK"
done

rm -f -- "$ANSWER"
