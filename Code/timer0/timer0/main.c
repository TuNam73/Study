/*
 * timer0.c
 *
 * Created: 8/9/2024 3:05:56 PM
 * Author : ADMIN
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

void timer0_delay() {
	TCNT0 = 0x82;        // load TCNT0
	TCCR0 |= ((1<<CS00) | (1<<CS00)); // prescale	
	/*	
		f=8MHz -> T=1/8M=0.125us
		count = 255-0 = 255 lan dem
		prescale = 1: cu sau 1 xung nhip (1*0.125us) thi TCNT0 tang 1 don vi, ma co 255 don vi
		->t(max) = 255*0.125us = 31.875us, tcnt0=0X00
		
		delay 2ms:
		t = 255*0.125=31.875us (ko du 2ms) -> tang prescale:
		prescale = 64 -> sau 64 xung nhip (64*0.125us = 8us) tang 1 don vi 
		-> t(max) = 8(us)*255= 2040us=2.04ms(t/m)
		prescale = 64 -> sau 1000/8=125 lan thi xong 1000us -> diem bat dau laf 255-125=130=0X82
		
		DELAY 1s: them count 
	*/
	while((TIFR & 0x01) == 0); //wait TOV0 to roll over 0=>1
	TCCR0 = 0;		//dung timer
	TIFR = 0x01;  //clear TOV0 flag
}


int main(void)
{
	DDRA = (1<<DDA0);
	PORTA = 0x00;
    while (1) 
    {
		
		
    }
}

