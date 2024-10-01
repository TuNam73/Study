/*
 * LCD.c
 *
 * Created: 8/13/2024 2:47:32 PM
 * Author : ADMIN
 */ 
//-----------------LCD MODE 4 BITS---------------------
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

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
		data = str[i];
		LCD_char(data);
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

void LCD_number(int number, char row, char col) {
	char a[100];
	itoa(number, a, 10);
	LCD_String_xy(a, row, col);
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
	DDRA |= (1 << DDA2);		//	A2 output - RS
	PORTA &= ~(1 << PINA2);		//	A2 LOW
	DDRD |= (1 << DDD6);		//	D6 output - EN
	PORTD &= ~(1 << PIND6);		//	D6 LOW
	DDRC  |= 0xFF;
	PORTC  &= 0x00;
	LCD_init();
	LCD_String_xy("Hello", 0, 3);
	set_Cursor(1, 6);
	LCD_number(3, 1, 2);
    while (1)
    {
	    
    }
}

