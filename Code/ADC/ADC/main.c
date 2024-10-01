/*
 * ADC.c
 *
 * Created: 8/14/2024 3:00:36 PM
 * Author : ADMIN
 * Note: https://startingelectronics.com/articles/atmel-AVR-8-bit/print-float-atmel-studio-7/
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

const unsigned char hexa_numbers[10] = {
	0x3F, //0
	0x06, //1
	0x5B, //2
	0x4F, //3
	0x66, //4
	0x6D, //5
	0x7D, //6
	0x07, //7
	0x7F, //8
	0x6F, //9
};

void display_segment(int number) {
	int donvi, chuc, tram, nghin;
	donvi = (number/1000)%10;
	chuc = (number/100)%10;
	tram = (number/10)%10;
	nghin = number%10;
	
	for (int i=0; i<100; i++) {
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

void LCD_Command(unsigned char cmd) {
	//--Send MSB-------
	PORTC = cmd;
	PORTA &= ~(1<<PINA2);					 //RS LOW, gui lenh
	_delay_ms(1);
	PORTD |= (1<<PINA6);						 //EN HIGH
	PORTD &= ~(1<<PINA6);					 //EN LOW
	_delay_ms(2);
	
	//--Send LSB--------
	PORTC = (cmd << 4); //send 4bits LSB
	_delay_ms(1);
	PORTD |= (1<<PINA6);						 //EN HIGH
	PORTD &= ~(1<<PINA6);					 //EN LOW
	_delay_ms(2);
}

void LCD_char(unsigned char data) {
	//--Send MSB-------
	PORTC = data;
	PORTA |= (1<<PINA2);					 //RS HIGH, GUI DU LIEU						 //EN HIGH
	_delay_ms(1);
	PORTD |= (1<<PINA6);
	PORTD &= ~(1<<PINA6);					 //EN LOW
	_delay_ms(2);
	
	//--Send LSB--------
	PORTC =  (data << 4);
	_delay_ms(1);
	PORTD |= (1<<PINA6);
	PORTD &= ~(1<<PINA6);					 //EN LOW
	_delay_ms(2);
}

void LCD_String(char *str){
	char data;
	int length = strlen(str);
	for (int i = 0; i < length; i++)
	{
		if (i == 16)
		LCD_Command(0xC0);
		data = str[i];
		if (data != '\0') {  
			LCD_char(data);
		}
	}
}

void set_Cursor(char row, char col){
	if ((row==0) && (col<16)) {
		LCD_Command(col  | (0x80));
	}
	else if ((row==1) && (col < 16)) {
		LCD_Command(col | (0xC0));
	}
}

void LCD_String_xy(char *str, char row, char col){
	set_Cursor(row, col);
	LCD_String(str);
}


void LCD_init() {
	LCD_Command(0xFF);
	_delay_ms(20);
	LCD_Command(0x33);
	_delay_ms(10);
	LCD_Command(0x32);
	_delay_ms(1);
	
	LCD_Command(0x28);      // Function Set: 4-bit, 2 Line, 5x8 Dots
	_delay_ms(10);
	LCD_Command(0x01);
	_delay_ms(200);
	LCD_Command(0x0C);  // Display on Cursor off
	_delay_ms(10);
	LCD_Command(0x06);  // Entry Mode
	_delay_ms(10);
}

void LCD_Clear()
{
	LCD_Command (0x01);		/* clear display */
	LCD_Command (0x80);		/* cursor at home position */
}

int main(void)
{
	//7_display_segment
	DDRA |= ((1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3));
	PORTA &= ~((1<<PA0) | (1<<PA1) | (1<<PA2) | (1<<PA3));
    DDRC = 0xFF;
	PORTC = 0x00;
	
	//LCD
	DDRA |= (1<<DDA2);
	PORTA &= ~(1<<PA2);
	DDRD |= (1<<DDD6);
	PORTD &= ~(1<<PD6);
	DDRC |= ((1<<DDC7) | (1<<DDC6) | (1<<DDC5) | (1<<DDC4));
	PORTC &= ~((1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4));
	
	//FAST PWM
	DDRB |= (1<<DDB3);
	PORTD &= ~(1<<PB3);
	TCCR0 |= ((1<<WGM01) | (1<<WGM00)); //fast pwm
	TCCR0 |= ((1<<COM01) | (1<<CS02) | (1<<CS00)); //
	TCNT0 = 0x00;
	
	//ADC
	//DDRA |= (1<<DDA5);
	//PORTA &= ~(1<<PA5);
	ADMUX |= ((1<<REFS0) | (1<<MUX2) | (1<<MUX0)); //AVcc, ADC5
	ADCSRA |= ((1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); //PRESCALE = 128 
	_delay_ms(10);
	
	int adc_value = 0;
	float voltage;
	char a[10];
	char b[10];
	
	LCD_init();
	LCD_Clear();
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA & (1<<ADIF)) == 0); //cho qua trinh chuyen doi hoan tat
		adc_value = ADCW;
		OCR0 = adc_value/4;
		voltage = adc_value*5.0/1023;
		
		sprintf(a, "Value: %4d", adc_value);
		LCD_String_xy(a, 0, 0);
		display_segment(adc_value);
		
		sprintf(b,"Voltage: %.2f", voltage);
		LCD_String_xy(b, 1, 0);
    }
}
