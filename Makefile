all: lab0.hex

lab0.elf: lab0.c
	avr-gcc -mmcu=atmega324a -Os -Wall -o lab0.elf lab0.c

lab0.hex: lab0.elf
	avr-objcopy  -j .text -j .data -O ihex lab0.elf lab0.hex
	avr-size lab0.elf

clean:
	rm -rf lab0.elf lab0.hex
