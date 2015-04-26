.PHONY: all linux windows pc clean_linux clean_windows clean_pc

INCLUDE=-I../include -I./include

clean_linux: clean_pc
	${MAKE} -C linux clean

clean_windows: clean_pc
	${MAKE} -C windows clean

clean_pc:
	${RM} *.o

all:
	${info You should NOT use this Makefile directly; use the toplevel one}

linux: pc
	${MAKE} -C linux all

windows: pc
	${MAKE} -C windows all

pc: vm.o error.o test.o

vm.o: vm.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o vm.o vm.c

error.o: error.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o error.o error.c

test.o: test.c
	${CC} ${CFLAGS} ${INCLUDE} -c -o test.o test.c

