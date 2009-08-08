ticketbooth: main.o 1wire.o serial.o
	sdcc -mpic14 -p16f688 -V main.rel 1wire.rel serial.rel
	cp main.hex ../release/ticketbooth.hex

main.o: main.c
	sdcc -c -mpic14 -p16f688 -V main.c

1wire.o: 1wire.c 1wire.h
	sdcc -c -mpic14 -p16f688 -V 1wire.c

serial.o: serial.c serial.h
	sdcc -c -mpic14 -p16f688 -V serial.c

burn:
	piklab-prog -c program -d 16f688 -p icd2 -t usb main.hex -f --target-self-powered true

full: main burn

clean:
	rm -f main.asm
	rm -f main.cod
	rm -f main.hex
	rm -f main.lst
	rm -f main.o
	rm -f 1wire.asm
	rm -f 1wire.cod
	rm -f 1wire.hex
	rm -f 1wire.lst
	rm -f 1wire.o
	rm -f serial.asm
	rm -f serial.cod
	rm -f serial.hex
	rm -f serial.lst
	rm -f serial.o
