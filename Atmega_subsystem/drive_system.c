

#include <avr/io.h>
#include "timer.h"
#include "adc.h"
//#include "USART.h"
//state machine states
#define IDLE 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3
//USART variables
#define FOSC 1843200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

unsigned char set_speed(unsigned char speed_input, unsigned char curr_speed){
	unsigned char speed;
	if(speed_input == 0x10){
		speed = 1;
	}
	else if(speed_input == 0x20){
		speed = 2;
	}
	else if(speed_input == 0x40){
		speed = 3;
	}
	else{
		speed = curr_speed;
	}
	return speed;
}

int main(void)
{
	//Setup I/O
	//DDRA = 0x0F;
	//PORTA = 0xFF;
	
	DDRB = 0xFF;
	PORTB = 0xFF;
	
	DDRC = 0x00;
	//PORTC = 0xFF;
	
    unsigned char state = IDLE;
	unsigned char ctrl_in = 0;
	//unsigned char speed_cnt = 0;
	unsigned char speed_input = 0;
	unsigned char speed = 3;
	
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
		speed_input = PINC & 0xF0;
		speed = set_speed(speed_input, speed);
		
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
				PORTB = 0xFF;
				break;
			case LEFT:
				//PORTD = 0x03;
				//PORTA = 0x00;
				PORTB = 0x0F;
				break;
			case RIGHT:
				//PORTD = 0x04;
				//PORTA = 0xFF;
				PORTB = 0xF0;
				break;
			default:
				//PORTD = 0x07;
				//PORTA = 0x00;
				PORTB = 0x00;
				break;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
