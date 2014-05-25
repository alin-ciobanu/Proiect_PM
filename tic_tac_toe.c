/*
 * lab0.c
 *
 */


#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <stdlib.h>

#define N 3
#define LVERZI 11
#define LROSII 12
#define BUTOANE 13

#define PLAY_WITH_HUMAN 0
#define PLAY_WITH_BOT 2

#define X_MARK 11
#define ZERO_MARK 23
#define EMPTY_MARK 54

#define INFINITY 9999

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

	int i, j;
	/* setare butoane ca intrare */
	/* activare rezistenta de pull-up pentru butoane */
	/* setare led-uri ca iesire */
	/* oprire LED-uri */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			char ch = butoane_port[i][j];
			int pin = butoane[i][j];
			switch(ch) {
				case 'b': {
					DDRB &= ~(1 << pin);
					PORTB |= (1 << pin);
					break;
				}
				case 'c': {
					DDRC &= ~(1 << pin);
					PORTC |= (1 << pin);
					break;
				}
				case 'd': {
					DDRD &= ~(1 << pin);
					PORTD |= (1 << pin);
					break;
				}
			}
			
			ch = led_rosii_port[i][j];
			pin = led_rosii[i][j];
			
			switch(ch) {
				case 'a': {
					DDRA |= (1 << pin);
					PORTA &= ~(1 << pin);
					break;
				}
				case 'b': {
					DDRB |= (1 << pin);
					PORTB &= ~(1 << pin);
					break;
				}
				case 'c': {
					DDRC |= (1 << pin);
					PORTC &= ~(1 << pin);
					break;
				}
				case 'd': {
					DDRD |= (1 << pin);
					PORTD &= ~(1 << pin);
					break;
				}
			}
			
			ch = led_verzi_port[i][j];
			pin = led_verzi[i][j];
			
			switch(ch) {
				case 'a': {
					DDRA |= (1 << pin);
					PORTA &= ~(1 << pin);
					break;
				}
				case 'b': {
					DDRB |= (1 << pin);
					PORTB &= ~(1 << pin);
					break;
				}
				case 'c': {
					DDRC |= (1 << pin);
					PORTC &= ~(1 << pin);
					break;
				}
				case 'd': {
					DDRD |= (1 << pin);
					PORTD &= ~(1 << pin);
					break;
				}
			}
			
		}
	}

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

void getEmptyPositions (int board[N][N], Position* positions, int* n) {

	int i, j;
	*n = 0;
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j] == EMPTY_MARK) {
				positions[*n].line = i;
				positions[*n].col = j;
				(*n)++;
			}
		}
	}
}

int boardCount(int board[N][N]) {

	int i, j;
	int count = 0;
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j] == EMPTY_MARK) {
				count++;
			}
		}
	}
	
	return count;

}

int getScoreForLine (int board[N][N], int nrX, int nr0) {
	int score = 0;
	if (nr0 == 3) {
		return INFINITY;
	}
	if (nr0 == 2 && nrX == 0) {
		score += 0;
	}
	if (nr0 == 1 && nrX == 0) {
		score += 0;
	}
	if (nr0 == 0 && nrX == 1) {
		score += 0;
	}
	if (nrX == 2 && nr0 == 0) {
		score -= 1000;
	}
	if (nrX == 1 && nr0 == 1) {
		score += 0;
	}
	if (nrX == 2 && nr0 == 1) {
		score += 100;
	}
	if (nr0 == 2 && nrX == 1) {
		score -= 0;
	}
	return score;
}

int evaluateBoard(int board[N][N]) {

	int i, j;
	int nrX, nr0;
	int score = 0;

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
		int lineScore = getScoreForLine(board, nrX, nr0);
		if (lineScore == INFINITY) {
			return INFINITY;
		}
		score += lineScore;
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
		int lineScore = getScoreForLine(board, nrX, nr0);
		if (lineScore == INFINITY) {
			return INFINITY;
		}
		score += lineScore;
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
	int lineScore = getScoreForLine(board, nrX, nr0);
	if (lineScore == INFINITY) {
		return INFINITY;
	}
	score += lineScore;

	nrX = nr0 = 0;
	for (i = 0; i < N; i++) {
		if (board[i][i] == ZERO_MARK) {
			nr0++;
		}
		if (board[i][i] == X_MARK) {
			nrX++;
		}
	}
	lineScore = getScoreForLine(board, nrX, nr0);
	if (lineScore == INFINITY) {
		return INFINITY;
	}
	score += lineScore;

}

int isOpponent2OppositeCorners (int board[N][N], Position pos[], int n) {

	int i;
	int c = 0;
	Position corners2[2];
	int k = 0;
	for (i = 0; i < n; i++) {
		if (isCorner(board, pos[i])) {
			c++;
			corners2[k++] = pos[i];
		}
	}
	int isOpCorners = (corners2[0].line != corners2[0].line) && (corners2[0].col != corners2[0].col) && 
					(corners2[1].line != corners2[1].line) && (corners2[1].col != corners2[1].col);
	if (c == 2 && isOpCorners) {
		return 1;
	}
	return 0;

}

int isCorner (int board[N][N], Position pos) {
	int i = pos.line;
	int j = pos.col;

	if ((i == 0 && j == 0) || (i == 0 && j == N - 1) || 
		(i == N - 1 && j == 0) || (i == N - 1 && j == N - 1)) {
		return 1;
	}
	return 0;
}

Position getIntersectionOfOpponent(int board[N][N]) {

	int i, j;
	
	Position pos[2];
	int k = 0;
	Position zero;
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (board[i][j] == X_MARK) {
				pos[k].line = i;
				pos[k].col = j;
				k++;
			}
			if (board[i][j] == ZERO_MARK) {
				zero.line = i;
				zero.col = j;
			}
		}
	}
	
	Position pos1;
	pos1.line = pos[0].line;
	pos1.col = pos[1].col;
	
	Position pos2;
	pos2.line = pos[1].line;
	pos2.col = pos[1].col;

	if (pos1.line != zero.line && pos1.col != zero.col) {
		return pos1;
	}
	return pos2;

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
	Position opponent_pos[N * N];
	int n = 0;

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
				Position pos_op;
				pos_op.line = i;
				pos_op.col = j;
				opponent_pos[n++] = pos_op;
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

	if (count == 3) {
		// ugly hardcoded solution for stupid bot
		if (isOpponent2OppositeCorners(board, opponent_pos, n)) {
			pos.line = 0;
			pos.col = 1;
			return pos;
		}
		else {
			Position inters = getIntersectionOfOpponent(board);
			if (isEmpty(board, inters)) {
				pos.line = inters.line;
				pos.col = inters.col;
				return pos;
			}
		}
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

