/*
 * 7_display_segment.c
 *
 * Created: 8/8/2024 2:23:44 PM
 * Author : TuNam
 */ 

/*
Control(PA): PA3 PA2 PA1 PA0
led pin:     dot g f e d c b a
PC:          7   6 5 4 3 2 1 0
DECIMAL			BINARY			HEXA
0				00111111		3F
1				00000110		06
2				01011011		5B
				...
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "7_seg.h"

void display_numbers(int number){
	int donvi, chuc, tram, nghin,i;
	donvi = (number/1000)%10;
	chuc = (number/100)%10;
	tram = (number/10)%10;
	nghin = number%10;
	
	for (i = 0;i < 24; i++)
	{
		PORTA &= ~((1<<PA1) | (1<<PA2) | (1<<PA0));
		PORTA |= (1<<PA3);
		PORTC = hexa_numbers[donvi];
		_delay_ms(1);
		
		PORTA &= ~((1<<PA0)|(1<<PA1) | (1<<PA3));
		PORTA |= (1<<PA2);
		PORTC = hexa_numbers[chuc];
		_delay_ms(1);
		
		PORTA &= ~((1<<PA0)| (1<<PA2)| (1<<PA3));
		PORTA |= (1<<PA1);
		PORTC = hexa_numbers[tram];
		_delay_ms(1);
		
		PORTA &= ~((1<<PA3)| (1<<PA1)| (1<<PA2));
		PORTA |= (1<<PA0);
		PORTC = hexa_numbers[nghin];
		_delay_ms(1);
	}
}

void display_init() {
	DDRA |= (1<<DDA0)|(1<<DDA1)|(1<<DDA2)|(1<<DDA3);
	PORTA &= 0x00;
	DDRC = 0xFF;
	PORTC = 0x00;
}


