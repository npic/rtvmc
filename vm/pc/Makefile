.PHONY: all linux windows dos pc clean_linux clean_windows clean_dos clean_pc

INCLUDE=-I../include -I./include

clean_linux: clean_pc
	${MAKE} -C linux clean

clean_windows: clean_linux

clean_dos: clean_pc
	${MAKE} -C dos clean

clean_pc:
	${RM} *.o

all:
	${info You should NOT use this Makefile directly; use the toplevel one}

linux: pc
	${MAKE} -C linux all

windows: linux

dos: pc
	${MAKE} -C dos all

pc: vm.o error.o test.o

%.o: %.c
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

