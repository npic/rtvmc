.PHONY: all pc sdk clean_pc clean_sdk

INCLUDE=-I../include

all:
	${info You should NOT use this Makefile directly; use the toplevel one}

clean_pc:
	${RM} *.o

clean_sdk:
	${RM} *.asm
	${RM} *.lst 
	${RM} *.rel
	${RM} *.rst
	${RM} *.sym

pc: INCLUDE+=-I../pc/include
pc: vm.o vm_devices.o vm_instruction.o

vm.o: vm.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm.o vm.c

vm_devices.o: vm_devices.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm_devices.o vm_devices.c

vm_instruction.o: vm_instruction.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm_instruction.o vm_instruction.c

sdk: INCLUDE+=-I../sdk/include
sdk: vm.rel vm_devices.rel vm_instruction.rel

vm.rel: vm.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm.rel vm.c

vm_devices.rel: vm_devices.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm_devices.rel vm_devices.c

vm_instruction.rel: vm_instruction.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm_instruction.rel vm_instruction.c

