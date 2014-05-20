all: tic_tac_toe.hex

tic_tac_toe.elf: tic_tac_toe.c
	avr-gcc -mmcu=atmega324a -Os -Wall -o tic_tac_toe.elf tic_tac_toe.c

tic_tac_toe.hex: tic_tac_toe.elf
	avr-objcopy  -j .text -j .data -O ihex tic_tac_toe.elf tic_tac_toe.hex
	avr-size tic_tac_toe.elf

clean:
	rm -rf tic_tac_toe.elf tic_tac_toe.hex
