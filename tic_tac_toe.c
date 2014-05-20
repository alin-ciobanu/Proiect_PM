/*
 * lab0.c
 *
 */


#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

void start() {
	
	/* setez pinul PB3 ca intrare */
	DDRD &= ~(1 << PD7);
	/* activez rezistenta de pull-up pentru pinul PB3*/
	PORTD |= (1 << PD7);

	/* setez pinul PC6 ca iesire */
	DDRC |= (1 << PC6);
	/* opresc LED-ul pe de pinul PC6 */
	PORTC &= ~(1 << PC6);
	
	while (1) {
		/* daca butonul este apasat */
		if ((PIND & (1 << PD7)) == 0) {
			/* ledul este aprins */
			PORTC |= (1 << PC6);
		} else {
			/* altfel ledul este stins */
			PORTC &= ~(1 << PC6);
		}
	}

}


/* TODO 1 - generare secvente aprindere LED-uri */
void task1() {
	/* Setare directii porturi folosite */
	
	while(1) {
		
		/*	Generare secvente pentru PB1 si PB0	*/
		
	}
}


/* TODO 2 - cod morse folosind speaker */
void task2() {
	/* Setare directii porturi folosite	*/
	
	while (1) {
		
		/*	Generare cod morse pentru PB1 si PB0 */
		
	}
}

/* TODO 2 - genereaza sunetul pentru tipul de cod folosit: lung sau scurt */
void speaker_morse(int tip_morse) {
	
	/* se va alterna starea portului PD5 la intervale de timp foarte mici */
	
}


/* TODO 3 - implementati afisarea secventei morse si pentru LED-uri
 * mai exact, codul scurt e un LED aprins, codul lung e de 3 LED-uri aprinse
 * iar pauza intre ele este mereu 1 LED
 * se poate face si analogia: un LED inseamna delay de 50 ms
 */
void task3() {
	/* Setare directii porturi folosite	*/
	
	while (1) {
		
		/*	Generare cod morse pentru PB1 si PB0 */
		
	}
}

/* TODO 3 - aprinde LED-urile corespunzatoare codului
 *	si shifteaza secventa anterioara la stanga
 */
void leduri(int tip_morse) {
	
}


int main(void) {
	start();
	//task1();
	//task2();
	//task3();
	
	return 0;
}

