.PHONY: all clean

INCLUDE=-I../include -I./include

clean:
	${RM} *.asm
	${RM} *.lst 
	${RM} *.rel
	${RM} *.rst
	${RM} *.sym

all: main.rel vm.rel vm_devices.rel error.rel

main.rel: main.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o main.rel main.c

vm.rel: vm.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm.rel vm.c

vm_devices.rel: vm_devices.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm_devices.rel vm_devices.c

error.rel: error.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o error.rel error.c

