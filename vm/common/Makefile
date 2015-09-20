.PHONY: all pc stm32 sdk clean_pc clean_stm32 clean_sdk

INCLUDE=-I../include

all:
	${info You should NOT use this Makefile directly; use the toplevel one}

clean_pc:
	${RM} *.o

clean_stm32:
	${RM} *.o

#clean_sdk:
#	${RM} *.asm
#	${RM} *.lst 
#	${RM} *.rel
#	${RM} *.rst
#	${RM} *.sym

pc: INCLUDE+=-I../pc/include
pc: vm.o vm_devices.o vm_instruction.o

stm32: INCLUDE+=-I../stm32/include
stm32: vm.o vm_devices.o vm_instruction.o

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

#sdk: INCLUDE+=-I../sdk/include
#sdk: vm.rel vm_devices.rel vm_instruction.rel
#
#%.rel: %.c
#	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

