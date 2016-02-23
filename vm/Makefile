.PHONY: all linux stm32 windows dos sdk load_stm32 load_sdk clean_linux clean_stm32 clean_windows clean_dos clean_sdk

CC=gcc
CFLAGS=-g -O0 -Wall -Wextra -Werror
LDFLAGS=
RM=
#M3PDIR=sdk/m3p
#M3P=${M3PDIR}/m3p.exe
#COMPORT=com1
#COMLOG=${COMPORT}_log.txt
#BAUD=9600
export

all:
	${info Valid targets are: linux stm32 windows dos sdk load_stm32 load_sdk clean_linux clean_stm32 clean_windows clean_dos clean_sdk}

linux: CFLAGS+=-pedantic -std=c89
linux:
	${MAKE} -C common pc
	${MAKE} -C pc linux
	${CC} ${LDFLAGS} -o vm common/vm.o common/vm_devices.o common/vm_instruction.o pc/vm.o pc/error.o pc/test.o pc/linux/main.o pc/linux/vm_devices.o

stm32: CC=arm-none-eabi-gcc
stm32: OBJCOPY=arm-none-eabi-objcopy
stm32: CFLAGS+=-mcpu=cortex-m4 -mlittle-endian -mthumb -DSTM32F401xE -Wno-unused-parameter -Wno-sign-compare
stm32: LDFLAGS+=-g -mcpu=cortex-m4 -mlittle-endian -mthumb -DSTM32F401xE -Tstm32/stm32f401ce_flash.ld -Wl,--gc-sections
stm32:
	${MAKE} -C common stm32
	${MAKE} -C stm32 all
	${CC} ${LDFLAGS} -o vm.elf common/vm.o common/vm_devices.o common/vm_instruction.o stm32/*.o stm32/drivers/hal/src/*.o stm32/drivers/bsp/stm32f4xx-nucleo/*.o stm32/drivers/cmsis/device/st/stm32f4xx/src/startup_stm32f401xe.o
	${OBJCOPY} -Oihex vm.elf vm.hex

windows: CFLAGS+=-pedantic -std=c89
windows:
	${MAKE} -C common pc
	${MAKE} -C pc windows
	${CC} -o vm.exe common/vm.o common/vm_devices.o common/vm_instruction.o pc/vm.o pc/error.o pc/test.o pc/linux/main.o pc/linux/vm_devices.o

dos: CFLAGS+=-pedantic -std=gnu89
dos:
	${MAKE} -C common pc
	${MAKE} -C pc dos
	${CC} -o vm.exe common/vm.o common/vm_devices.o common/vm_instruction.o pc/vm.o pc/error.o pc/dos/main.o pc/dos/vm_devices.o

sdk load_sdk clean_sdk:
	${info SDK 1.1 port is not implemented yet}

#sdk: CC=C:/sdcc320/bin/sdcc
#sdk: CFLAGS=--Werror --stack-auto --code-loc 0x2100 --xram-loc 0x6D00 --stack-loc 0x80
#sdk:
#	${MAKE} -C common sdk
#	${MAKE} -C sdk all
#	${CC} ${CFLAGS} -o vm.hex common/vm.rel common/vm_devices.rel common/vm_instruction.rel sdk/main.rel sdk/vm.rel sdk/vm_devices.rel sdk/error.rel
#	${M3P} hb166 vm.hex vm.bin bye
#
#load_sdk:
#	${M3P} lfile ${M3PDIR}/load.m3p

load_stm32: GDB=arm-none-eabi-gdb
load_stm32:
	${GDB} -x stm32/loader.gdb

clean_linux: RM=rm -f --
clean_linux:
	${MAKE} -C common clean_pc
	${MAKE} -C pc clean_linux
	${RM} vm

clean_stm32: RM=rm -f --
clean_stm32:
	${MAKE} -C common clean_stm32
	${MAKE} -C stm32 clean
	${RM} vm.elf
	${RM} vm.hex

clean_windows: RM=del
clean_windows:
	${MAKE} -C common clean_pc
	${MAKE} -C pc clean_windows
	${RM} vm.exe

clean_dos: RM=del
clean_dos:
	${MAKE} -C common clean_pc
	${MAKE} -C pc clean_dos
	${RM} vm.exe

#clean_sdk: RM=del
#clean_sdk:
#	${MAKE} -C common clean_sdk
#	${MAKE} -C sdk clean
#	${RM} vm.hex
#	${RM} vm.bin
#	${RM} vm.map
#	${RM} vm.mem
#	${RM} vm.lk
#	${RM} pm3p_*.txt

