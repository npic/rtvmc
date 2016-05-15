#! /bin/sh

cd $(dirname $0)

COMPILER=../../compiler/compiler
TESTER=../tester.py
ANSWER=./answer.tmp

for i in *.lang
do
    echo -n "Checking $i... "
    $COMPILER < "$i" > "$ANSWER"
    $TESTER "$i.pat" "$ANSWER" || exit 1
    echo "OK"
done

rm -f -- "$ANSWER"
