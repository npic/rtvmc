.PHONY: all clean

INJECTION?=../../vm-tests/test-rt/rt.bin

INCLUDE=-I../include -I./include -I./drivers/bsp/stm32f4xx-nucleo -I./drivers/cmsis/device/st/stm32f4xx/include -I./drivers/cmsis/include -I./drivers/hal/include

DRIVERSSRC=${wildcard ./drivers/hal/src/*.c} ${wildcard ./drivers/bsp/stm32f4xx-nucleo/*.c} ./drivers/cmsis/device/st/stm32f4xx/src/startup_stm32f401xe.s
DRIVERSOBJ=${patsubst %.c,%.o,${patsubst %.s,%.o,${DRIVERSSRC}}}

clean:
	${RM} *.o
	${RM} ${DRIVERSOBJ}

all: main.o vm.o vm_devices.o error.o stm32f4xx_it.o stm32f4xx_hal_msp.o system_stm32f4xx.o ${DRIVERSOBJ}

vm.o: vm.c
	sed -i -e '/INJECTION START/a'"$(shell xxd -i < ${INJECTION})" ./vm.c
	${CC} ${CFLAGS} ${INCLUDE} -c vm.c -o vm.o
	sed -i -e '/INJECTION START/,/INJECTION STOP/{//!d}' ./vm.c

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

%.o: %.s
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

