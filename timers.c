#include <xc.h>
#include "timers.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; //  Fosc/4 - setting the clock source to FOSC/4 - pg 334
    T0CON1bits.T0ASYNC=1;   // the input into the TMR0 counter is not synched to sys clocks //not sure what this is doing here????
                            //see datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1000; // 1:256 - prescaler effect pg 334 for details
        // want scaler to work at 244.4 - closest in data sheet is 
    T0CON0bits.T016BIT=1;	//8bit mode if 0 16bit mode if 1	
	
    // it's a good idea to initialise the timer registers so we know we are at 0
    TMR0H = 0b00001011;            //write High reg first, update happens when low reg is written to
    TMR0L = 0b11011100;  // 3036 in first 8bit of 16bit

    
    // to increase the accuracy we can change the starting point of the timer
    // internal clock is runing at 1 / (64 Mhz / 4) - tbe divide by 4 is since we are taking the FOSC and /4 per T0CS
    // this gives us 62.5 Ns per binary number
    // 16 bit mode 2^16
    // 2^16 * 62.5 * 10^-9 = total time per number w/out a scaler = 4.096e-3
    // start counting from 3037 instead of 1 and we'll have 62500 binary numbers 
          // actually 3036 since we count from 0 
    // 62500 * 62.5 * 10^-9 * 256 = 1
    // 0000 1011 1101 1101
    // split across coutner low and high bit
    // now make it do that in the clock but do the correct write sequence
    
    
    T0CON0bits.T0EN=1;	//start the timer - turn timer on
}

/************************************
 * Function to return the full 16bit timer value
 * Note TMR0L and TMR0H must be read in the correct order, or TMR0H will not contain the correct value
************************************/
unsigned int get16bitTMR0val(void)
{   
   // have to read low first before reading high, as high only updates when low is read
    unsigned int low = TMR0L ;
    unsigned int high = TMR0H ;
    return(high) ;
	//add your code to get the full 16 bit timer value here
}
