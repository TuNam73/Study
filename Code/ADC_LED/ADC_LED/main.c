/*
 * ADC_LED.c
 *
 * Created: 8/15/2024 10:45:40 AM
 * Author : ADMIN
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
    DDRB |= (1<<DDB3);
	PORTD &= ~(1<<PB3);
	TCCR0 |= ((1<<WGM01) | (1<<WGM00)); //fast pwm
	TCCR0 |= ((1<<COM01) | (1<<CS02) | (1<<CS00)); //
	TCNT0 = 0x00;
	
	ADMUX |= ((1<<REFS0) | (1<<MUX2) | (1<<MUX0)); //AVcc, ADC5
	ADCSRA |= ((1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); //PRESCALE = 128
	_delay_ms(10);
	
	//char adc_value;
	//char duty;
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA & (1<<ADIF)) == 0); //cho qua trinh chuyen doi hoan tat
		//adc_value = ADCW;
		OCR0 = ADCW/4;
		_delay_ms(1);
		
    }
}

