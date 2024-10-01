#ifndef _7_SEG_H
#define _7_SEG_H

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

const unsigned char hexa_alphabet[10] = {
	0x77, //A
	0x7C, //B
	0x39, //C
	0x5E, //D
	0x79, //E
	0x71, //F
	0x76, //H
};

void display_numbers(int number);
void display_init();


#endif
