ticketbooth: main.o 1wire.o serial.o delay.o servo.o highwire.o
	sdcc -mpic14 -p16f688 -V main.o 1wire.o serial.o delay.o servo.o highwire.o
	cp main.hex ../release/ticketbooth.hex

main.o: main.c 1wire.h serial.h delay.h servo.h highwire.h
	sdcc -c -mpic14 -p16f688 -V main.c

1wire.o: 1wire.c 1wire.h
	sdcc -c -mpic14 -p16f688 -V 1wire.c

serial.o: serial.c serial.h
	sdcc -c -mpic14 -p16f688 -V serial.c

delay.o: delay.c delay.h
	sdcc -c -mpic14 -p16f688 -V delay.c

servo.o: servo.c servo.h
	sdcc -c -mpic14 -p16f688 -V servo.c

highwire.o: highwire.c highwire.h
	sdcc -c -mpic14 -p16f688 -V highwire.c

burn:
	piklab-prog -c program -d 16f688 -p icd2 -t usb main.hex -f --target-self-powered true

full: ticketbooth burn

clean:
	rm -f main.asm main.cod main.hex main.lst main.o
	rm -f 1wire.asm 1wire.cod 1wire.hex 1wire.lst 1wire.o
	rm -f serial.asm serial.cod serial.hex serial.lst serial.o
	rm -f delay.asm delay.cod delay.hex delay.lst delay.o
	rm -f servo.asm servo.cod servo.hex servo.lst servo.o
	rm -f highwire.asm highwire.cod highwire.hex highwire.lst highwire.o

