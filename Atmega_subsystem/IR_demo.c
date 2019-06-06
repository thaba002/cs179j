#include <avr/io.h>
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
	
	DDRB = 0xFF;
	
	DDRC = 0x00;
	
	unsigned char state = IDLE;
	unsigned char ctrl_in = 0;
	unsigned char speed_cnt = 0;
	
	PORTB = 0x00; // upper nibble = left motors
	// lower nibble = right motors
	
	DDRD = 0xFF;
	PORTD = 0x00;
	
	//TimerOn();
	//TimerSet(25);
	
	setupADC();
	
	unsigned long left;
	unsigned long right;
	
	unsigned char eBrake = 0x00;
	
	while (1)
	{
		//Read input
		ctrl_in = 0x02//PINC & 0x0F;
		
		getValueADC(&left, &right);

		if(left > 75 || right > 75){
			eBrake = 1;
		}
		else{
			eBrake = 0;
		}
		
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
				if(ctrl_in == 0x02 && !eBrake){
					state = FORWARD;
				}
				else{
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
				PORTB = 0x00;
				break;
			case FORWARD:
				PORTB = ~PORTB;
				break;
			case LEFT:
				PORTB = ~PORTB & 0x0F;
				break;
			case RIGHT:
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
				PORTB = 0x00;
				break;
		}
	}
}
