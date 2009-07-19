main:
	sdcc -mpic14 -p16f688 -V main.c
	cp main.hex ../release/ticketbooth.hex

burn:
	piklab-prog -c program -d 16f688 -p icd2 -t usb main.hex -f --target-self-powered true

full: main burn

clean:
	rm -f main.asm
	rm -f main.cod
	rm -f main.hex
	rm -f main.lst
	rm -f main.o
	
