#! /bin/sh

cd $(dirname $0)

VM=../../vm/vm
SOBIRASHKA=../../compiler/sobirashka.sh
TESTER=../tester.py
BINFILE=./bin.tmp

for i in *.test
do
    echo -n "Checking $i... "
    $SOBIRASHKA "$i.lang" > /dev/null
    mv -f -- *.bin "$BINFILE" 2>/dev/null
    $VM "$BINFILE" "$i" > /dev/null 2>&1
    for j in $i.gpio*
    do
        PORT=$(echo $j | sed -e 's/^.*gpio//')
        $TESTER "$j" "gpio_$PORT.dat" || exit 1
    done
    echo "OK"
done

rm -f -- "$BINFILE"
rm -f -- *.dat
