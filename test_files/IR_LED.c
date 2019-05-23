/*
 * IR_LED.c
 *
 * Author : Jose
 */ 

#include <avr/io.h>

unsigned long front, left, right;

void getValueADC()
{
	switch(ADMUX)
	{
		case 0x40:
		front = ADC;
		ADMUX = 0x41;
		break;
		case 0x41:
		right = ADC;
		ADMUX = 0x42;
		break;
		case 0x42:
		left = ADC;
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

int main(void)
{
	DDRC = 0xFF;
	PORTC = 0xFF;
	setupADC();
    /* Replace with your application code */
    while (1) 
    {
		getValueADC();

		if(front >= 200)
		{
			PORTC |= 0x01;
		}
		else
		{
			PORTC &= 0xFE;
		}
		if(right >= 200)
		{
			PORTC |= 0x02;
		}
		else
		{
			PORTC &= 0xFD;
		}
		if(left >= 200)
		{
			PORTC |= 0x04;
		}
		else
		{
			PORTC &= 0xFB;
		}
    }
}

