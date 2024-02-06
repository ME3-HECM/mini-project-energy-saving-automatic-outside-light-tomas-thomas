#include <xc.h>
#include "interrupts.h"
#include "seconds.h"

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
    // turn on global interrupts, peripheral interrupts and the interrupt source 
	// It's a good idea to turn on global interrupts last, once all other interrupt configuration is done.

//    INTCONbits.GIEL = 1; // sets up interrupts for for the "peripheral interrupt enable bit" - bit confused on syntax pg 200
  INTCONbits.PEIE = 1; // this does the same thing as GIEL not sure why it has two names

// CLOCK SETup
// TMR0Ix
    PIE0bits.TMR0IE = 1; // E for enable: enables interrupt source timer
    PIR0bits.TMR0IF = 0; // F for flag: if 0 timer output hasn't overflowed, if 1 has reached max value - reset with software
    IPR0bits.TMR0IP = 1; // P for priority: set 1 to high priority
 
// Turning all interrupts stuff on 
    INTCONbits.GIE=1; 	//turn on interrupts globally (when this is off, all interrupts are deactivated) 
}

/************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR()
{

// Clock overflow
    if(PIR0bits.TMR0IF){ 		//check the interrupt source - if the TMR0IF (flag) goes to 1 then execute the code (overflows)
        LATHbits.LATH3 = !LATHbits.LATH3;    // if interrupt is trigger flip the LED light on/off
//        T0CON0bits.T0EN=0;                  // stops the clock
        TMR0H = 0b00001011;            //write High reg first, update happens when low reg is written to
        TMR0L = 0b11011100;
        secs++;
        if (secs >=60){
            secs = 0;
        }  
        PIR0bits.TMR0IF=0; 						//clear the interrupt flag!
	}
    
}