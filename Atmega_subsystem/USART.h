void USART_Init(unsigned int ubrr){
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

void USART_Transmit(unsigned int data){
	//UDRE = UCSR0A bit 5
	while(!(UCSR0A & (1<<((UCSR0A & 0x20))>>5)))
	;
	//TXB8 = UCSR0B bit 0
	UCSR0B &= ~(1<<(UCSR0B & 0x01));
	if(data & 0x0100){
		UCSR0B |= (1<<(UCSR0B & 0x01));
	}
	UDR0 = data;
}

unsigned int USART_Recieve(void){
	unsigned char status, resh, res1;
	//RXC = UCSR0A bit 7
	while(!(UCSR0A & (1<<((UCSR0A & 0x80)>>7)))){
		;
	}
	status = UCSR0A;
	resh = UCSR0B;
	res1 = UDR0;
	//FE = UCSR0A bit 4
	//DOR = UCSR0A bit 3
	//UPE = UCSR0A bit 2
	if(status & (1<<((UCSR0A & 0x10)>>4)) | (1<<((UCSR0A & 0x08)>>3)) | (1<<((UCSR0A & 0x04))>>2)){
		return -1;
	}
	resh = (resh >> 1) & 0x01;
	return ((resh << 8) | res1);
}