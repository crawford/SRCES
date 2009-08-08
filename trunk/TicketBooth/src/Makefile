ticketbooth: main.o 1wire.o serial.o delay.o servo.o
	sdcc -mpic14 -p16f688 -V main.o 1wire.o serial.o delay.o servo.o
	cp main.hex ../release/ticketbooth.hex

main.o: main.c
	sdcc -c -mpic14 -p16f688 -V main.c

1wire.o: 1wire.c 1wire.h
	sdcc -c -mpic14 -p16f688 -V 1wire.c

serial.o: serial.c serial.h
	sdcc -c -mpic14 -p16f688 -V serial.c

delay.o: delay.c delay.h
	sdcc -c -mpic14 -p16f688 -V delay.c

servo.o: servo.c servo.h
	sdcc -c -mpic14 -p16f688 -V servo.c

burn:
	piklab-prog -c program -d 16f688 -p icd2 -t usb main.hex -f --target-self-powered true

full: ticketbooth burn

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
	rm -f delay.asm
	rm -f delay.cod
	rm -f delay.hex
	rm -f delay.lst
	rm -f delay.o
	rm -f servo.asm
	rm -f servo.cod
	rm -f servo.hex
	rm -f servo.lst
	rm -f servo.o

