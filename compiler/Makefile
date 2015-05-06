.PHONY: all clean

CC=gcc
LEX=flex
YACC=bison

all: compiler

compiler: y.tab.o lex.yy.o calc_codegen.o
	${CC} -o compiler y.tab.o lex.yy.o calc_codegen.o

y.tab.o: y.tab.c
	${CC} -c y.tab.c

y.tab.c: calc.y
	${YACC} -y -d calc.y

lex.yy.o: lex.yy.c
	${CC} -c lex.yy.c

lex.yy.c: calc.l
	${LEX} calc.l

calc_codegen.o: calc_codegen.c
	${CC} -c calc_codegen.c


clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h compiler

