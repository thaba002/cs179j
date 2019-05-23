

#include <avr/io.h>
//state machine states
#define IDLE 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3
//USART variables
#define FOSC 1843200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

unsigned char speed;
/*
void USART_Init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

void USART_Transmit(unsigned int data){
	while(!(UCSR0A & (1<<UDRE))) 
		;
	UCSR0B &= ~(1<<TXB8);
	if(data & 0x0100){
		UCSR0B |= (1<<TXB8);
	}
	UDR0 = data;
}

unsigned int USART_Recieve(void){
	unsigned char status, resh, res1;
	while(!(UCSR0A & (1<<RXC))){
		;
	}
	status = UCSR0A;
	resh = UCSR0B;
	res1 = UDR0;
	if(status & (1<<FE) | (1<<DOR) | (1<<UPE)){
		return -1;
	}
	resh = (resh >> 1) & 0x01;
	return ((resh << 8) | res1);
}

*/

int main(void)
{
	//Setup I/O
	DDRA = 0xFF;
	PORTA = 0x00;
	
	DDRB = 0xFF;
	PORTB = 0x00;
	
	DDRC = 0x00;
	PORTC = 0xFF;
	
    unsigned char state = IDLE;
	unsigned char ctrl_in = 0;
	speed = 1;
    while (1) 
    {
		ctrl_in = PINC & 0x0F;
		switch(state){
			case IDLE: 
				//state actions
				PORTA = 0x00;
				PORTB = 0x00;
				//state logic
				if(ctrl_in == 0x00 || ctrl_in == 0x01){
					state = IDLE;
				}
				else if(ctrl_in == 0x02){
					state = FORWARD;
				}
				else if(ctrl_in == 0x04){
					state = LEFT;
				}
				else if(ctrl_in == 0x08){
					state = RIGHT;
				}
				else{
					state = IDLE;
				}
				break;
			case FORWARD:
				//state actions
				PORTA = 0xFF;
				PORTB = 0xFF;
				//state logic
				if(ctrl_in == 0x01){
					state = IDLE;
				}
				else if(ctrl_in == 0x02 || ctrl_in == 0x00){
					state = FORWARD;
				}
				else if(ctrl_in == 0x04){
					state = LEFT;
				}
				else if(ctrl_in == 0x08){
					state = RIGHT;
				}
				else{
					state = IDLE;
				}
				break;
			case LEFT:
				//state actions
				PORTA = 0x00;
				PORTB = 0xFF;
				//state logic
				if(ctrl_in == 0x01 || ctrl_in == 0x00){
					state = IDLE;
				}
				else if(ctrl_in == 0x02){
					state = FORWARD;
				}
				else if(ctrl_in == 0x04){
					state = LEFT;
				}
				else if(ctrl_in == 0x08){
					state = RIGHT;
				}
				else{
					state = IDLE;
				}
				break;
			case RIGHT:
				//state actions
				PORTA = 0xFF;
				PORTB = 0x00;
				//state logic
				if(ctrl_in == 0x01 || ctrl_in == 0x00){
					state = IDLE;
				}
				else if(ctrl_in == 0x02){
					state = FORWARD;
				}
				else if(ctrl_in == 0x04){
					state = LEFT;
				}
				else if(ctrl_in == 0x08){
					state = RIGHT;
				}
				else{
					state = IDLE;
				}
				break;
			default:
				//state actions
				PORTA = 0x00;
				PORTB = 0x00;
				//state logic
				if(ctrl_in == 0x00 || 0x01){
					state = IDLE;
				}
				else if(ctrl_in == 0x02){
					state = FORWARD;
				}
				else if(ctrl_in == 0x04){
					state = LEFT;
				}
				else if(ctrl_in == 0x08){
					state = RIGHT;
				}
				else{
					state = IDLE;
				}
		}
    }
}
