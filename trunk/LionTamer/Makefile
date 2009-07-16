CC=gcc
CFLAGS=-Wall -L.
all : libserial.a
	${CC} ${CFLAGS} serial.c -lserial -o serial
	${CC} ${CFLAGS} network.c -lserial -o network

libserial.o : 
	${CC} ${CFLAGS} -c libserial.c

libserial.a : libserial.o
	ar cr libserial.a libserial.o

libs : libserial.a

clean:
	rm *.o *.a network serial
