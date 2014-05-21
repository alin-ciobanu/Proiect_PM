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

#define PLAY_WITH_HUMAN 0
#define PLAY_WITH_BOT 2

#define X_MARK 11
#define ZERO_MARK 23
#define EMPTY_MARK 54

int led_verzi[N][N] = {{PC4, PC6, PA7}, {PB2, PB0, PC2}, {PA0, PA2, PA4}};
char led_verzi_port[N][N] = {{'c', 'c', 'a'}, {'b', 'b', 'c'}, {'a', 'a', 'a'}};
int led_rosii[N][N] = {{PC5, PC7, PA6}, {PB1, PC1, PC3}, {PA1, PA3, PA5}};
char led_rosii_port[N][N] = {{'c', 'c', 'a'}, {'b', 'c', 'c'}, {'a', 'a', 'a'}};
int butoane[N][N] = {{PB3, PD1, PD6}, {PB4, PD4, PD5}, {PD0, PC0, PD7}};
char butoane_port[N][N] = {{'b', 'd', 'd'}, {'b', 'd', 'd'}, {'d', 'c', 'd'}};

typedef struct Position {
	int line;
	int col;
} Position;

void init () {

	/* setare butoane ca intrare */
	DDRD &= ~(1 << PD0);
	DDRD &= ~(1 << PD1);
	DDRC &= ~(1 << PC0);
	DDRD &= ~(1 << PD4);
	DDRD &= ~(1 << PD5);
	DDRD &= ~(1 << PD6);
	DDRD &= ~(1 << PD7);
	DDRB &= ~(1 << PB3);
	DDRB &= ~(1 << PB4);
	/* activare rezistenta de pull-up pentru butoane */
	PORTD |= (1 << PD0);
	PORTD |= (1 << PD1);
	PORTC |= (1 << PC0);
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
					case 'c': {
						if ((PINC & (1 << port)) == 0) {
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

void loadingWithLEDs (int color) {

	int i, j;
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			char port_led;
			int pin;
			
			if (color == LROSII) {
				port_led = led_rosii_port[i][j];
				pin = led_rosii[i][j];
			}
			else if (color == LVERZI) {
				port_led = led_verzi_port[i][j];
				pin = led_verzi[i][j];
			}
			
			switch (port_led) {
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
			_delay_ms(100);
		}
	}
	_delay_ms(500);
	init();

}

int isEmpty (int board[N][N], Position pos) {

	if (board[pos.line][pos.col] == EMPTY_MARK) {
		return 1;
	}
	
	return 0;

}

int isGameOver (int board[N][N]) {

	int i, j;
	int nrX, nr0;
		
	nrX = nr0 = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j] == ZERO_MARK) {
				nr0++;
			}
			if (board[i][j] == X_MARK) {
				nrX++;
			}
		}
		if (nr0 == N || nrX == N) {
			return 1;
		}
		nrX = nr0 = 0;
	}
	
	nrX = nr0 = 0;
	for (j = 0; j < N; j++) {
		for (i = 0; i < N; i++) {
			if (board[i][j] == ZERO_MARK) {
				nr0++;
			}
			if (board[i][j] == X_MARK) {
				nrX++;
			}
		}
		if (nr0 == N || nrX == N) {
			return 1;
		}
		nrX = nr0 = 0;
	}
	
	nrX = nr0 = 0;
	for (j = 0; j < N; j++) {
		for (i = 0; i < N; i++) {
			if (i + j == 2) {
				if (board[i][j] == ZERO_MARK) {
					nr0++;
				}
				if (board[i][j] == X_MARK) {
					nrX++;
				}
			}
		}		
	}
	if (nr0 == N || nrX == N) {
		return 1;
	}
	
	nrX = nr0 = 0;
	for (i = 0; i < N; i++) {
		if (board[i][i] == ZERO_MARK) {
			nr0++;
		}
		if (board[i][i] == X_MARK) {
			nrX++;
		}
	}
	if (nr0 == N || nrX == N) {
		return 1;
	}

	return 0;

}

Position getBotMove (int board[N][N]) {
	/*
		It is supposed that bot plays with 0
	*/
	Position pos;
	int i, j;
	int nrX, nr0;
	int critical; // critical means that there are 2 X on a line
	int potential; // it means that there is one 0 on a line and no X
	int leastChoice;
	critical = potential = leastChoice = 0;
	Position emptyPos;
	int count = 0;

	nrX = nr0 = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j] == ZERO_MARK) {
				nr0++;
				count++;
			}
			if (board[i][j] == X_MARK) {
				nrX++;
				count++;
			}
			if (board[i][j] == EMPTY_MARK) {
				emptyPos.line = i;
				emptyPos.col = j;
			}
		}
		if (nr0 == 2 && nrX == 0) {
			// winning position, must move here
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			return pos;
		}
		if (nrX == 2 && nr0 == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			critical = 1;
		}
		if (nrX == 0 && nr0 == 1 && critical == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			potential = 1;
		}
		if (nrX == 1 && nr0 == 0 && critical == 0 && potential == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			leastChoice = 1;
		}
		if (critical == 0 && potential == 0 && leastChoice == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
		}
		nrX = nr0 = 0;
	}
	
	if (count == 1 && board[1][1] == EMPTY_MARK) {
		pos.line = 1;
		pos.col = 1;
		return pos;
	}
	
	nrX = nr0 = 0;
	for (j = 0; j < N; j++) {
		for (i = 0; i < N; i++) {
			if (board[i][j] == ZERO_MARK) {
				nr0++;
			}
			if (board[i][j] == X_MARK) {
				nrX++;
			}
			if (board[i][j] == EMPTY_MARK) {
				emptyPos.line = i;
				emptyPos.col = j;
			}
		}
		if (nr0 == 2 && nrX == 0) {
			// winning position, must move here
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			return pos;
		}
		if (nrX == 2 && nr0 == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			critical = 1;
		}
		if (nrX == 0 && nr0 == 1 && critical == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			potential = 1;
		}
		if (nrX == 1 && nr0 == 0 && critical == 0 && potential == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
			leastChoice = 1;
		}
		if (critical == 0 && potential == 0 && leastChoice == 0) {
			pos.line = emptyPos.line;
			pos.col = emptyPos.col;
		}
		nrX = nr0 = 0;
	}
	
	nrX = nr0 = 0;
	for (j = 0; j < N; j++) {
		for (i = 0; i < N; i++) {
			if (i + j == 2) {
				if (board[i][j] == ZERO_MARK) {
					nr0++;
				}
				if (board[i][j] == X_MARK) {
					nrX++;
				}
				if (board[i][j] == EMPTY_MARK) {
					emptyPos.line = i;
					emptyPos.col = j;
				}
			}
		}		
	}
	if (nr0 == 2 && nrX == 0) {
		// winning position, must move here
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		return pos;
	}
	if (nrX == 2 && nr0 == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		critical = 1;
	}
	if (nrX == 0 && nr0 == 1 && critical == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		potential = 1;
	}
	if (nrX == 1 && nr0 == 0 && critical == 0 && potential == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		leastChoice = 1;
	}
	if (critical == 0 && potential == 0 && leastChoice == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
	}
	
	nrX = nr0 = 0;
	for (i = 0; i < N; i++) {
		if (board[i][i] == ZERO_MARK) {
			nr0++;
		}
		if (board[i][i] == X_MARK) {
			nrX++;
		}
		if (board[i][i] == EMPTY_MARK) {
			emptyPos.line = i;
			emptyPos.col = i;
		}
	}
	if (nr0 == 2 && nrX == 0) {
		// winning position, must move here
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		return pos;
	}
	if (nrX == 2 && nr0 == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		critical = 1;
	}
	if (nrX == 0 && nr0 == 1 && critical == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		potential = 1;
	}
	if (nrX == 1 && nr0 == 0 && critical == 0 && potential == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
		leastChoice = 1;
	}
	if (critical == 0 && potential == 0 && leastChoice == 0) {
		pos.line = emptyPos.line;
		pos.col = emptyPos.col;
	}
	
	return pos;

}

void playWithHuman () {

	int board[N][N];
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			board[i][j] = EMPTY_MARK;
		}
	}
	Position pos;
	char port_led;
	int pin;

	while (1) {
		pos = getPressedButton();
		while (!isEmpty(board, pos)) {
			pos = getPressedButton();
		}
		
		port_led = led_rosii_port[pos.line][pos.col];
		pin = led_rosii[pos.line][pos.col];
		
		switch (port_led) {
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
		
		board[pos.line][pos.col] = X_MARK;
		
		if (isGameOver(board)) {
			return;
		}
		
		pos = getPressedButton();
		while (!isEmpty(board, pos)) {
			pos = getPressedButton();
		}
		
		port_led = led_verzi_port[pos.line][pos.col];
		pin = led_verzi[pos.line][pos.col];
		
		switch (port_led) {
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
		
		board[pos.line][pos.col] = ZERO_MARK;
		
		if (isGameOver(board)) {
			return;
		}
		
	}

}

void playWithBot () {

	int board[N][N];
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			board[i][j] = EMPTY_MARK;
		}
	}
	Position pos;
	char port_led;
	int pin;

	while (1) {
		pos = getPressedButton();
		while (!isEmpty(board, pos)) {
			pos = getPressedButton();
		}
		
		port_led = led_rosii_port[pos.line][pos.col];
		pin = led_rosii[pos.line][pos.col];
		
		switch (port_led) {
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
		
		board[pos.line][pos.col] = X_MARK;
		
		if (isGameOver(board)) {
			return;
		}
		
		_delay_ms(444);
		
		pos = getBotMove(board);
		
		port_led = led_verzi_port[pos.line][pos.col];
		pin = led_verzi[pos.line][pos.col];
		
		switch (port_led) {
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
		
		board[pos.line][pos.col] = ZERO_MARK;
		
		if (isGameOver(board)) {
			return;
		}
		
	}

}

void start() {

	init();

	Position pos = getPressedButton();
	if (pos.col == PLAY_WITH_HUMAN) {
		loadingWithLEDs(LVERZI);
		playWithHuman();
	}
	else if (pos.col == PLAY_WITH_BOT) {
		loadingWithLEDs(LROSII);
		playWithBot();
	}
	else {
		loadingWithLEDs(LVERZI);
		loadingWithLEDs(LROSII);

		int port_led = led_rosii_port[pos.line][pos.col];
		int pin = led_rosii[pos.line][pos.col];
		switch (port_led) {
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

