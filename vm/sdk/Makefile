.PHONY: all clean

INCLUDE=-I../include -I./include

clean:
	${RM} *.asm
	${RM} *.lst 
	${RM} *.rel
	${RM} *.rst
	${RM} *.sym

all: main.rel vm.rel vm_devices.rel error.rel

%.rel: %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

