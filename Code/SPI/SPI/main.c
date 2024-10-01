/*
 * SPI.c
 *
 * Created: 8/20/2024 3:58:02 PM
 * Author : ADMIN
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

void SPI_Master_Init() {
	DDRB |= ((1<<PB7) | (1<<PB5) | (PB4)); //set MOSI, SCK, SS output
	DDRB &= ~(1<<PB6); //set MISO input
	PORTB |= (1<<PB4); //SS high
	SPCR |= ((1<<SPE) | (1<<MSTR) | (1<<SPR0)); //enable SPI, master, fosc/16
}

void SPI_Master_Transmit(unsigned char data) {
	unsigned char xdata;
	SPDR = data; //write data to SPI shift register
	while(!(SPSR & (1<<SPIF))); //wait tranmission complete
	xdata = SPDR;
	return xdata;
}

/*
void SPI_Slave_Init() {
	DDRB &= ~((1<<PB7) | (1<<PB5) | (PB4)); 
	DDRB |= (1<<PB6); //MISO
	SPCR |= (1<<SPE); //slave mode
}

void SPI_Master_Transmit() {
	while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
	return(SPDR);			/* Return received data */
}
*/

int main(void)
{
    
    while (1) 
    {
    }
}

