
#include <avr/io.h>
#include "timer.h"
#include "adc.h"
//state machine states
#define IDLE 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3
#define SLOW 4


int main(void)
{
	//Setup I/O
	//DDRA = 0x0F;
	//PORTA = 0xFF;
	
	DDRB = 0xFF;
	
	DDRC = 0x00;
	//PORTC = 0xFF;
	
	unsigned char state = IDLE;
	unsigned char ctrl_in = 0;
	unsigned char speed_cnt = 0;
	
	//PORTA = 0x00;
	PORTB = 0x00; // upper nibble = left motors
	// lower nibble = right motors
	
	DDRD = 0xFF;
	PORTD = 0x00;
	
	TimerOn();
	TimerSet(25);
	
	setupADC();
	
	unsigned long left;
	unsigned long right;
	
	unsigned char eBrake = 0x00;
	
	while (1)
	{
		//Read input
		ctrl_in = PINC & 0x0F;
		
		getValueADC(&left, &right);

		if(left > 75 || right > 75){
			//PORTD |= 0x03;
			eBrake = 1;
		}
		else{
			//PORTD &= 0xFC;
			eBrake = 0;
		}
		///////////////////////////////////////////////////
		
		// state select switch statement
		switch(state){
			case IDLE:
				if( ctrl_in == 0x00 || ctrl_in == 0x01 || eBrake ){
					state = IDLE;
				}
				else if( ctrl_in == 0x02 && !eBrake ){
					state = FORWARD;
				}
				else if( ctrl_in == 0x04 && !eBrake ){
					state = LEFT;
				}
				else if( ctrl_in == 0x08 && !eBrake ){
					state = RIGHT;
				}
				else{
					state = IDLE;
				}
				break;
			
			case FORWARD:
				if( ctrl_in == 0x02 && !eBrake ){
					state = FORWARD;
				}
				else {
					state = IDLE;
				}
				break;
			
			case LEFT:
				if( !eBrake && ctrl_in == 0x04 ){
					state = LEFT;
				}
				else{
					state = IDLE;
				}
				break;
			
			case RIGHT:
				if( !eBrake && ctrl_in == 0x08 ){
					state = RIGHT;
				}
				else{
					state = IDLE;
				}
				break;
			
			case SLOW:
				if(ctrl_in == 0x10 && !eBrake){
					state = SLOW;
				}
				else{
					state = IDLE;
				}
				break;
			default:
				state = IDLE;
				break;
		}
		
		// action switch statement
		//upper nibble = left motors
		//lower nibble = right motors
		
		switch(state){
			case IDLE:
				//PORTD = 0x01;
				PORTB = 0x00;
				break;
			case FORWARD:
				//PORTD = 0x02;
				//PORTA = 0xFF;
				PORTB = ~PORTB;
				break;
			case LEFT:
				//PORTD = 0x03;
				//PORTA = 0x00;
				PORTB = ~PORTB & 0x0F;
				break;
			case RIGHT:
				//PORTD = 0x04;
				//PORTA = 0xFF;
				PORTB = ~PORTB & 0xF0;
				break;
			case SLOW:
				if(speed_cnt == 0){
					++speed_cnt;
				}
				else{
					PORTB = ~PORTB;
					speed_cnt = 0;
				}
			default:
				//PORTD = 0x07;
				//PORTA = 0x00;
				PORTB = 0x00;
				break;
		}
		//while(!TimerFlag);
		//TimerFlag = 0;
	}
}