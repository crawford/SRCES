CC=gcc
CFLAGS=-Wall -L. -g
all : libs
	${CC} ${CFLAGS} serial.c -lhiwi -o serial
	${CC} ${CFLAGS} network.c -lhiwi -o network
	${CC} ${CFLAGS} test.c -lhiwi -o test

libhiwi.o : 
	${CC} ${CFLAGS} -c hiwi.c -o libhiwi.o

libhiwi.a : libhiwi.o
	ar cr libhiwi.a libhiwi.o

libs : libhiwi.a

clean:
	rm *.o *.a network serial test
