CC=gcc
CFLAGS=-Wall -L. -g
all : libs
	${CC} ${CFLAGS} liontamer.c -lhiwi -o liontamer
	${CC} ${CFLAGS} test.c -lhiwi -o test

libhiwi.o : 
	${CC} ${CFLAGS} -c hiwi.c -o libhiwi.o

libhiwi.a : libhiwi.o
	ar cr libhiwi.a libhiwi.o

libs : libhiwi.a

clean:
	rm *.o *.a liontamer test
