/*
 * IR_LED.c
 *
 * Created: 5/23/2019 5:00:42 AM
 * Author : Jose
 */ 

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

unsigned long left;
unsigned long right;

void getValueADC(unsigned long *left, unsigned long *right)
{
	switch(ADMUX)
	{
		case 0x40: //used to 0x40
			*left = ADC;
			ADMUX = 0x41;
			break;
		case 0x41: //used to be 0x41
			*right = ADC;
			ADMUX = 0x40;
			break;
		
		default: break;
	}
	ADCSRA |= 1 << ADSC;	
}


void setupADC()
{
	ADCSRA |= 1 << ADPS0;
	ADMUX |= 1 << REFS0;
	ADCSRA |= 1 << ADEN;
	
	ADCSRA |= 1 << ADSC;
}
#endif //ADC_H
