.PHONY: all clean

INCLUDE=-I../../include -I../include

clean:
	${RM} *.o

all: main.o vm_devices.o

main.o: main.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o main.o main.c

vm_devices.o: vm_devices.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm_devices.o vm_devices.c

