/*
 * TWI.c
 *
 * Created: 8/20/2024 10:03:35 PM
 * Author : ADMIN
 */ 

#define F_CPU 8000000UL
#define fosc 8000000

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

void TWI_Init(char slave_address, char sclock) {
	//slave_address take value in 1 - 127, TWGCE=1:enabel gcall,=0:disable gcall, sclock<=400000
	TWAR = (slave_address<<1)|TWGCE;
	TWSR &= 0x00; //prescale = 1
	TWBR |= (unsigned char) (((fosc/sclock)-16)/2);
	TWCR |= ((1<<TWEA) | (1<<TWEN) | (1<<TWIE)); //enable ACK, TWI, Interrupt
}

uint8_t TWI_Start() {
	TWCR |= ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
	while (!(TWCR & (1<<TWINT)));
	return (TWSR & 0xF8);
}

uint8_t TWI_ReatStart(char read_address) {
	int status;                                            /* Declare variable */
	TWCR |= ((1<<TWINT) | (1<<TWEN) | (1<<TWSTA));                    /* Enable TWI, generate start condition and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));                            /* Wait until TWI finish its current job (start condition) */
	status = TWSR & 0xF8;                                    /* Read TWI status register with masking lower three bits */
	if (status != 0x10)                                        /* Check weather repeated start condition transmitted successfully or not? */
	return 0;                                                /* If no then return 0 to indicate repeated start condition fail */
	TWDR = read_address;                                    /* If yes then write SLA+R in TWI data register */
	TWCR = 0b10000100;                            /* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));                            /* Wait until TWI finish its current job (Write operation) */
	status = TWSR & 0xF8;                                    /* Read TWI status register with masking lower three bits */
	if (status == 0x40)                                        /* Check weather SLA+R transmitted & ack received or not? */
	return 1;                                                /* If yes then return 1 to indicate ack received */
	if (status == 0x48)                                        /* Check weather SLA+R transmitted & nack received or not? */
	return 2;                                                /* If yes then return 2 to indicate nack received i.e. device is busy */
	else
	return 3;                                                /* Else return 3 to indicate SLA+W failed */
}

void TWI_Stop()		
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);/* Enable TWI, generate stop */
	while(TWCR&(1<<TWSTO));	/* Wait until stop condition execution */
}

unsigned char TWI_Read_Ack()		
{
	TWCR |= (1<<TWEN)|(1<<TWINT)|(1<<TWEA); /* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;			/* Return received data */
}

unsigned char TWI_Read_Nack()	
{
	TWCR |= (1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
	return TWDR;		/* Return received data */
}

uint8_t TWI_Write(char data)
{
	int status;		
	TWDR = data;			/* Copy data in TWI data register */
	TWCR |= (1<<TWEN) | (1<<TWINT);	/* Enable TWI and clear interrupt flag */
	while(!(TWCR & (1<<TWINT)));	/* Wait until TWI finish its current job */
	status = TWSR & 0xF8;		/* Read TWI status register */
	if(status==0x28)		/* Check for data transmitted &ack received */
	return 0;			/* Return 0 to indicate ack received */
	if(status==0x30)		/* Check for data transmitted &nack received */
	return 1;			/* Return 1 to indicate nack received */
	else
	return 2;			/* Else return 2 for data transmission failure */
}



int main(void)
{
    char array[10] = "test";	
    while (1) 
    {
    }
}

