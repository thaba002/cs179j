/*
 * ultrasonic_sensor.c
 *
 * Author : Jose
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


// Global variables used in busy timer code
volatile unsigned int TimerOverflowCnt = 0;

ISR(TIMER1_OVF_vect)
{
    TimerOverflowCnt += 1;
}


int main(void)
{
    
    //unsigned long count = 0;
    unsigned long t1 = 0;
    unsigned long t2 = 0;
    unsigned long pulseWidth = 0;
    unsigned long numTicks = 0;
    
    unsigned char distance;
    // Initialize output ports
    DDRA = 0xFF;
    PORTA = 0xFF;
    
    DDRB = 0xFF;
    PORTB = 0xFF;
    
    PORTD = 0xFF;
    
    sei()   ;               //enable global interrupt
    TIMSK1 = (1 << TOIE1);  // Enable Timer1 overflow interrupt
    
    //Initialize timer registers
    TCCR1A = 0x00; //For normal operation
    OCR1A = 88; //1us per 8 ticks (therefore 88/8 = 11 us delay)
    
    while(1){
        
        //prepare for trigger pulse delay
        TCCR1B = 0x01; //no prescaler
        // when TCNT1 == OCR1A event occurs and flag is set
        TCNT1 = 0; //initialize timer 1 to 0
        
        //provide 10us trigger pulse
        PORTB = 0x01;
        while(!(TIFR1 & 0x02));//wait for match to happen  //page 186 in datasheet
        TIFR1 = 1 << OCF1A; //clear OCF1A flag
        PORTB = 0x00;
        TCNT1 = 0;
        
        //prepare for rising edge input capture
        TCCR1B = 0x41;      //Capture on rising edge,no prescalar
        TIFR1 = 1 << ICF1; //clear ICP flag and timer overflow flag
        //TIFR1 = 0x01;
        
        //wait for rising edge
        while(!(TIFR1 & 0x20));
        TIFR1 = 1 << ICF1; //clear ICP flag and timer overflow flag
        ///TIFR1 = 0x01;
        //record timer value
        t1 = ICR1 + (65535 * TimerOverflowCnt); //TCNT1 is written here when correct edge is detected
        TimerOverflowCnt = 0;
        
        //prepare for falling edge input capture
        TCCR1B = 0x01; //capture falling edge, no prescalar
        
        
        //wait for falling edge
        while(!(TIFR1 & 0x20));
        TIFR1  = 1 << ICF1; //clear ICP flag and timer overflow flag
        
        //record timer value
        t2 = ICR1 + (65535 * TimerOverflowCnt);
        TimerOverflowCnt = 0;
        
        //reset timer
        TCNT1 = 0;
        //ICR1 = 0;
        
        //find number of ticks recorded
        numTicks = t2 - t1; 
        
        //convert that to us
        pulseWidth = numTicks * 0.125; //pulse width in microseconds
        
        //convert to centimeters
        distance = pulseWidth /58; // or pulseWidth / 148 for inches
        
        if(distance < 255)
        {
            PORTA = distance;
        }
        else
        {
            PORTA = 0xAA;
        }
    }
}


