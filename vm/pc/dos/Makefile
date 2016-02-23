.PHONY: all clean

INCLUDE=-I../../include -I../include

clean:
	${RM} *.o

all: main.o vm_devices.o

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

