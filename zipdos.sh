#! /bin/sh
cd $(readlink -f -- $(dirname -- "$0"))
zip -r vm.zip vm -x "vm/stm32/*" "vm/sdk/*"
