/*
* UART.c
*
* Created: 8/15/2024 3:44:27 PM
* Author : Tu Nam
*/

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void reverse(char* str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
	int ipart = (int)n;
	float fpart = n - (float)ipart;
	int i = intToStr(ipart, res, 0);
	if (afterpoint != 0) {
		res[i] = '.';  //add dot 
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}

void UART_Init(unsigned long BAUD, unsigned long fosc) {
	unsigned long ubrr = fosc/16/BAUD - 1;
	UCSRA &= 0x00;
	UCSRB |= ((1<<RXEN) | (1<<TXEN)); //enabale receive and transmit
	UCSRC |= ((1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1)); //UCSRC, size 8 bit
	UBRRL |= ubrr;
	UBRRH |= (ubrr>>8);
}

void UART_print_Char(unsigned char data){
	while (!(UCSRA & (1<<UDRE)));
	UDR |= data;
}

void UART_print_Str(char *str) {
	unsigned char i = 0;
	while (str[i] != 0) {
		UART_print_Char(str[i]);
		if (str[i] == '\n') UART_print_Char('\r');
		i++;
	}
}

void UART_print_Int(int value)
{
	char buf[10];
	intToStr(value, buf, 0);
	UART_print_Str(buf);
	
}

void UART_print_Float(float value, int afterpoint) {
	char buf[10];
	ftoa(value, buf, afterpoint);
	UART_print_Str(buf);
	
}

int main(void)
{
	UART_Init(9600, 8000000);
	while (1)
	{
		UART_print_Int(6789);
		UART_print_Float(34.533, 3);
		UART_print_Str("abcd");
		_delay_ms(1000);
	}
}

