/*
 * lab0.c
 *
 */


#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

#define N 3
#define LVERZI 11
#define LROSII 12
#define BUTOANE 13

int led_verzi[N][N] = {{PC4, PC6, PA7}, {PB2, PB0, PC2}, {PA0, PA2, PA4}};
char led_verzi_port[N][N] = {{'c', 'c', 'a'}, {'b', 'b', 'c'}, {'a', 'a', 'a'}};
int led_rosii[N][N] = {{PC5, PC7, PA6}, {PB1, PC1, PC3}, {PA1, PA3, PA5}};
char led_rosii_port[N][N] = {{'c', 'c', 'a'}, {'b', 'c', 'c'}, {'a', 'a', 'a'}};
int butoane[N][N] = {{PB3, PD1, PD6}, {PB4, PD4, PD5}, {PD0, PD3, PD7}};
char butoane_port[N][N] = {{'b', 'd', 'd'}, {'b', 'd', 'd'}, {'d', 'd', 'd'}};

typedef struct Position {
	int line;
	int col;
} Position;

Position getPressedButton() {

	int i, j;
	Position pos;

	while (1) {
	
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				char port_letter = butoane_port[i][j];
				int port = butoane[i][j];
				
				switch (port_letter) {
					case 'b': {
						if ((PINB & (1 << port)) == 0) {
							pos.line = i;
							pos.col = j;
							return pos;
						}
						break;
					}
					case 'd': {
						if ((PIND & (1 << port)) == 0) {
							pos.line = i;
							pos.col = j;
							return pos;
						}
						break;
					}
				}				
			}
		}
	}
	
	return pos;

}

/**
	Gaseste pozitia pin-ului pin in matricea specificata
*/
Position getPosition (int pin, int inWhatMatrix) {

	int i, j;
	Position pos;

	switch (inWhatMatrix) {
		
		case LVERZI: {
			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					if (led_verzi[i][j] == pin) {
						pos.line = i;
						pos.col = j;
						return pos;
					}
				}
			}
			break;
		}

		case LROSII: {
			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					if (led_rosii[i][j] == pin) {
						pos.line = i;
						pos.col = j;
						return pos;
					}
				}
			}
			break;
		}

		case BUTOANE: {
			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					if (butoane[i][j] == pin) {
						pos.line = i;
						pos.col = j;
						return pos;
					}
				}
			}
			break;
		}

	}
	
	return pos;

}

void init () {

	/* setare butoane ca intrare */
	DDRD &= ~(1 << PD0);
	DDRD &= ~(1 << PD1);
	DDRD &= ~(1 << PD3);
	DDRD &= ~(1 << PD4);
	DDRD &= ~(1 << PD5);
	DDRD &= ~(1 << PD6);
	DDRD &= ~(1 << PD7);
	DDRB &= ~(1 << PB3);
	DDRB &= ~(1 << PB4);
	/* activare rezistenta de pull-up pentru butoane */
	PORTD |= (1 << PD0);
	PORTD |= (1 << PD1);
	PORTD |= (1 << PD3);
	PORTD |= (1 << PD4);
	PORTD |= (1 << PD5);
	PORTD |= (1 << PD6);
	PORTD |= (1 << PD7);
	PORTB |= (1 << PB3);
	PORTB |= (1 << PB4);

	/* setare led-uri ca iesire */
	DDRC |= (1 << PC1);
	DDRC |= (1 << PC2);
	DDRC |= (1 << PC3);
	DDRC |= (1 << PC4);
	DDRC |= (1 << PC5);
	DDRC |= (1 << PC6);
	DDRC |= (1 << PC7);
	DDRA |= (1 << PA0);
	DDRA |= (1 << PA1);
	DDRA |= (1 << PA2);
	DDRA |= (1 << PA3);
	DDRA |= (1 << PA4);
	DDRA |= (1 << PA5);
	DDRA |= (1 << PA6);
	DDRA |= (1 << PA7);
	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	

	/* oprire LED-uri */
	PORTC &= ~(1 << PC1);
	PORTC &= ~(1 << PC2);
	PORTC &= ~(1 << PC3);
	PORTC &= ~(1 << PC4);
	PORTC &= ~(1 << PC5);
	PORTC &= ~(1 << PC6);
	PORTC &= ~(1 << PC7);
	PORTA &= ~(1 << PA0);
	PORTA &= ~(1 << PA1);
	PORTA &= ~(1 << PA2);
	PORTA &= ~(1 << PA3);
	PORTA &= ~(1 << PA4);
	PORTA &= ~(1 << PA5);
	PORTA &= ~(1 << PA6);
	PORTA &= ~(1 << PA7);
	PORTB &= ~(1 << PB0);
	PORTB &= ~(1 << PB1);
	PORTB &= ~(1 << PB2);

}

void start() {
	
	init();
	
	while (1) {
		Position pos = getPressedButton();
		
		char port_led_rosu = led_rosii_port[pos.line][pos.col];
		int pin = led_rosii[pos.line][pos.col];
		
		switch (port_led_rosu) {
			case 'a': {
				PORTA |= (1 << pin);
				break;
			}
			case 'b': {
				PORTB |= (1 << pin);
				break;
			}
			case 'c': {
				PORTC |= (1 << pin);
				break;
			}
		}

	}

}


int main(void) {
	start();	
	return 0;
}

