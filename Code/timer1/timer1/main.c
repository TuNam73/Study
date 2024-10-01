/*
 * timer1.c
 *
 * Created: 8/9/2024 5:10:00 PM
 * Author : ADMIN
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//-------------------------OVF-------------------------------------
/*
int main(void)
{
    DDRA = 0xFF;
	PORTA |= 0xFF;
	TCCR1B |= ((1<<CS10)|(1<<CS11)); //Prescale = 64
	//delay 1s -> T=1s 
	//n = Max - T*fIO/2/prescale; (max=2^16-1, T=1s, fIO=8MHz, prescale=64)
	//n = 3035 = 0BDB;
	TCNT1H = 0x0B;
	TCNT1L = 0xDB;
	TIMSK = (1<<TOIE1); //ngat tran timer1
	sei();
    while (1) 
    {
    }
	return 0;
}

ISR(TIMER1_OVF_vect) {
	TCNT1H = 0x0B;
	TCNT1L = 0xDB;
	PORTA ^= 0xFF;
}
*/
	
//---------------------------CTC----------------------------------------
// duoc su dung de dem cac su kien ben ngoai


int main(void) {
	DDRA |= 0xFF;//((1<<DDA0) |(1<<DDA1) | (1<<DDA2) | (1<<DDA3) | (1<<DDA4) |(1<<DDA5) | (1<<DDA6) | (1<<DDA7));
	PORTA |= 0xFF;//((0<<PA0) | (0<<PA1) | (0<<PA2) | (0<<PA3) | (0<<PA4) | (0<<PA5) | (0<<PA6) | (0<<PA7));
	TCCR1A = 0x00;
	
	//TCCR1B |= ((1<<WGM12) | (1<<CS11) | (1<<CS10)); //CTC mode 4, falling pulse
	//OCR1AL = 0x23;
	//OCR1AH = 0xF4;
	//TIMSK = (1<<OCIE1A);
	
	TCCR1B = ((1<<WGM13) |(1<<WGM12) | (1<<CS11) | (1<<CS10));
	ICR1H = 0xF4;
	ICR1L = 0x23;
	TIMSK = (1<<TICIE1);
	sei();
	while(1) {
		
	}
}

//ISR (TIMER1_COMPA_vect) {
//	OCR1AL = 0x23;
//	OCR1AH = 0xF4;
//	PORTA ^= 0xFF;
//}

ISR (TIMER1_CAPT_vect) {
	ICR1L = 0x23;
	ICR1H = 0xF4;
	PORTA ^= 0xFF;
}


