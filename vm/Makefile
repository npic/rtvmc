.PHONY: all linux windows sdk load_sdk clean_linux clean_windows clean_sdk

CC=gcc
CFLAGS=-g -O0 -Wall -Wextra -Werror -pedantic -std=c89
RM=
M3PDIR=sdk/m3p
M3P=${M3PDIR}/m3p.exe
COMPORT=com1
COMLOG=${COMPORT}_log.txt
BAUD=9600
export

all:
	${info Valid targets are: linux windows sdk load_sdk clean_linux clean_windows clean_sdk}

linux:
	${MAKE} -C common pc
	${MAKE} -C pc linux
	${CC} -o vm common/vm.o common/vm_devices.o common/vm_instruction.o pc/vm.o pc/error.o pc/test.o pc/linux/main.o pc/linux/vm_devices.o

windows:
	${MAKE} -C common pc
	${MAKE} -C pc windows
	${CC} -o vm.exe common/vm.o common/vm_devices.o common/vm_instruction.o pc/vm.o pc/error.o pc/test.o pc/windows/main.o pc/windows/vm_devices.o

sdk: CC=C:/sdcc320/bin/sdcc
sdk: CFLAGS=--Werror --stack-auto --code-loc 0x2100 --xram-loc 0x6D00 --stack-loc 0x80
sdk:
	${MAKE} -C common sdk
	${MAKE} -C sdk all
	${CC} ${CFLAGS} -o vm.hex common/vm.rel common/vm_devices.rel common/vm_instruction.rel sdk/main.rel sdk/vm.rel sdk/vm_devices.rel sdk/error.rel
	${M3P} hb166 vm.hex vm.bin bye

load_sdk:
	${M3P} lfile ${M3PDIR}/load.m3p

clean_linux: RM=rm -f
clean_linux:
	${MAKE} -C common clean_pc
	${MAKE} -C pc clean_linux
	${RM} vm

clean_windows: RM=del
clean_windows:
	${MAKE} -C common clean_pc
	${MAKE} -C pc clean_windows
	${RM} vm.exe

clean_sdk: RM=del
clean_sdk:
	${MAKE} -C common clean_sdk
	${MAKE} -C sdk clean
	${RM} vm.hex
	${RM} vm.bin
	${RM} vm.map
	${RM} vm.mem
	${RM} vm.lk
	${RM} pm3p_*.txt

